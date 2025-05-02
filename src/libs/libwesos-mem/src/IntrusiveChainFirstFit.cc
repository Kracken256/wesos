/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <sanitizer/asan_interface.h>

#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Range.hh>
#include <wesos-mem/IntrusiveChainFirstFit.hh>

using namespace wesos;
using namespace wesos::mem;

extern "C" {  /// TODO: Remove section
struct FILE;

auto printf(const char*, ...) -> int;
auto fflush(FILE*) -> int;

extern FILE* stdout;  // NOLINT

static void debug_s(const char* func, int line) {
  printf("[%s]: %d\n", func, line);
  fflush(stdout);
}

#define W_DEBUG() debug_s(__func__, __LINE__)
}

struct IntrusiveChainFirstFit::Chunk {
  Least<usize, 1> m_size;
  NullableRefPtr<Chunk> m_next;
};

using Chunk = IntrusiveChainFirstFit::Chunk;

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(View<u8> pool)
    : m_some(nullptr), m_initial_pool(pool) {
  virt_do_utilize(pool);
}

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(IntrusiveChainFirstFit&& o)
    : m_some(o.m_some), m_initial_pool(o.m_initial_pool) {
  o.m_some = nullptr;
  o.m_initial_pool.clear();
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o)
    -> IntrusiveChainFirstFit& {
  if (this != &o) {
    m_some = o.m_some;
    m_initial_pool = o.m_initial_pool;

    o.m_some = nullptr;
    o.m_initial_pool.clear();
  }

  return *this;
}

namespace wesos::mem::detail {
  class BitHeader {
    u8 m_before : 4;
    u8 m_after : 4;

    friend class ChunkHeaderFormat;
  } __attribute__((packed));

  static_assert(sizeof(BitHeader) < sizeof(Chunk) && alignof(BitHeader) < alignof(Chunk));

  class ChunkHeaderFormat final {
  public:
    static constexpr auto MAX_U4 = 15;
    static constexpr auto MAX_ALIGN = 16;

    static_assert(sizeof(Chunk) - 1 <= MAX_U4);
    static_assert(MAX_ALIGN - 1 <= MAX_U4);

    using BytesBeforeCount = Range<usize, 1, MAX_ALIGN>;
    using BytesAfterCount = Range<usize, 0, sizeof(Chunk) - 1>;

    ChunkHeaderFormat(BitHeader& header) : m_header(header) {
      ASAN_UNPOISON_MEMORY_REGION(&m_header, sizeof(BitHeader));
    }

    ~ChunkHeaderFormat() {
      //
      ASAN_POISON_MEMORY_REGION(&m_header, sizeof(BitHeader));
    }

    [[nodiscard]] constexpr auto get_bytes_before() -> BytesBeforeCount {
      return m_header.m_before + 1;
    }

    [[nodiscard]] constexpr auto get_bytes_after() -> BytesAfterCount { return m_header.m_after; }

    constexpr void set_bytes_before(BytesBeforeCount x) {
      m_header.m_before = static_cast<u8>(x.unwrap() - 1);
    }
    constexpr void set_bytes_after(BytesAfterCount x) {
      m_header.m_after = static_cast<u8>(x.unwrap());
    }

  private:
    BitHeader& m_header;
  };

  static auto get_dealigned_range(View<u8> zone) -> View<u8> {
    auto zone_begin = zone.into_ptr().get_unchecked();
    auto zone_end = zone_begin.add(zone.size());

    auto& bit_header = *reinterpret_cast<BitHeader*>(zone_begin.sub(sizeof(BitHeader)).unwrap());
    auto header = ChunkHeaderFormat(bit_header);

    zone_begin = zone_begin.sub(header.get_bytes_before());
    zone_end = zone_end.add(header.get_bytes_after());

    return {zone_begin, zone_end};
  }

  static void print_freelist(NullableRefPtr<Chunk> root) {
    printf("\n======= BEG FREE LIST =======\n");
    for (NullableRefPtr<Chunk> node = root; node.isset(); node = node->m_next) {
      printf("(%p, %zu) -> \n", (void*)node.unwrap(), node->m_size.unwrap());
    }
    printf("nullptr\n");
    printf("======= END FREE LIST =======\n\n");
  }
}  // namespace wesos::mem::detail

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_allocate(usize size, PowerOfTwo<usize> align)
    -> NullableOwnPtr<u8> {
  using namespace detail;

  if (align > max_alignment()) [[unlikely]] {
    return nullptr;
  }

  print_freelist(m_some);

  for (NullableRefPtr<Chunk> node = m_some, prev = nullptr; node.isset();
       prev = node, node = node->m_next) {
    const auto next = node->m_next;

    const auto unaligned_chunk_start = RefPtr(reinterpret_cast<u8*>(node.unwrap()));
    const auto unaligned_chunk_end = RefPtr(unaligned_chunk_start.unwrap() + node->m_size);
    const auto aligned_range_start = RefPtr(unaligned_chunk_start  //
                                                .add(sizeof(BitHeader))
                                                .align_pow2(align));
    const auto aligned_range_end = RefPtr(aligned_range_start.unwrap() + size);

    if (auto enough = aligned_range_end <= unaligned_chunk_end; !enough) {
      continue;
    }

    assert_invariant(aligned_range_start.sub(sizeof(BitHeader)) >= unaligned_chunk_start);
    auto& bit_header =
        *reinterpret_cast<BitHeader*>(aligned_range_start.sub(sizeof(BitHeader)).unwrap());
    auto header = ChunkHeaderFormat(bit_header);

    {  // Don't forget what comes before us
      const auto space_before = aligned_range_start.into_uptr() - unaligned_chunk_start.into_uptr();
      header.set_bytes_before(space_before);
    }

    {  // Don't loose sight of what comes after us
      if (prev.isset()) {
        prev->m_next = next;
      } else {
        m_some = nullptr;
      }

      auto extra_range = View<u8>(aligned_range_end, unaligned_chunk_end);
      const auto extra_range_pad = extra_range.into_ptr().align_pow2(alignof(Chunk)).into_uptr() -
                                   extra_range.into_ptr().into_uptr();

      if (extra_range.size() < extra_range_pad + sizeof(Chunk)) {
        const auto space_after = extra_range.size();
        assert_invariant(space_after + extra_range_pad < sizeof(Chunk));
        header.set_bytes_after(space_after + extra_range_pad);
      } else {
        extra_range = extra_range.subview_unchecked(extra_range_pad);

        const auto new_chunk = OwnPtr(reinterpret_cast<Chunk*>(extra_range.into_ptr().unwrap()));
        const auto new_chunk_size = extra_range.size();

        ASAN_UNPOISON_MEMORY_REGION(new_chunk.unwrap(), new_chunk_size);

        // Seek to maintain the order of the freelist.
        auto seek = m_some;
        while (seek.isset() && seek->m_size <= new_chunk_size) {
          seek = seek->m_next;
        }

        if (seek.isset()) {
          new_chunk->m_size = new_chunk_size;
          new_chunk->m_next = seek->m_next;

          seek->m_next = new_chunk;
        } else {
          new_chunk->m_size = new_chunk_size;
          new_chunk->m_next = nullptr;
          m_some = new_chunk;
        }
      }

      print_freelist(m_some);
    }

    ASAN_UNPOISON_MEMORY_REGION(aligned_range_start.unwrap(), size);

    return aligned_range_start.unwrap();
  }

  return nullptr;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                           PowerOfTwo<usize> align) {
  using namespace detail;

  assert_invariant(ptr.is_aligned_pow2(align));

  const auto dealigned_range = get_dealigned_range(View<u8>(ptr.unwrap(), size));
  assert_invariant(dealigned_range.size() >= sizeof(Chunk) &&
                   dealigned_range.into_ptr().is_aligned_pow2(alignof(Chunk)));

  ASAN_POISON_MEMORY_REGION(dealigned_range.into_ptr().unwrap(), dealigned_range.size());

  const auto chunk = OwnPtr(reinterpret_cast<Chunk*>(dealigned_range.into_ptr().unwrap()));
  const auto chunk_size = dealigned_range.size();

  // Seek to maintain the order of the freelist.
  auto prev = m_some;
  while (prev.isset() && prev->m_size <= chunk_size) {
    prev = prev->m_next;
  }

  if (prev.isset()) {
    ASAN_UNPOISON_MEMORY_REGION(chunk.unwrap(), sizeof(Chunk));
    chunk->m_size = chunk_size;
    chunk->m_next = prev->m_next;

    prev->m_next = chunk;

    {  // Merge adjacent free chunks
      const auto end_of_chunk = OwnPtr(reinterpret_cast<u8*>(chunk.unwrap())).add(chunk->m_size);
      const auto begin_of_next = NullableOwnPtr(reinterpret_cast<u8*>(chunk->m_next.unwrap()));

      if (end_of_chunk == begin_of_next) {
        chunk->m_size = chunk->m_size + chunk->m_next->m_size;
        chunk->m_next = chunk->m_next->m_next;

        assert_invariant(chunk->m_size != 0);

        ASAN_POISON_MEMORY_REGION(begin_of_next.unwrap(), sizeof(Chunk));
      }
    }

  } else {
    ASAN_UNPOISON_MEMORY_REGION(chunk.unwrap(), sizeof(Chunk));

    chunk->m_size = chunk_size;
    chunk->m_next = nullptr;

    m_some = chunk;
  }

  detail::print_freelist(m_some);
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_utilize(View<u8> pool) -> LeftoverMemory {
  using namespace detail;

  auto window = pool;

  {  // Adjust alignment and install header
    const auto space_before_chunk =
        window.into_ptr().add(sizeof(BitHeader)).align_pow2(alignof(Chunk)).into_uptr() -
        window.into_ptr().into_uptr();

    if (window.size() < space_before_chunk + sizeof(Chunk)) {
      return {{pool}, {}};
    }

    window = window.subview_unchecked(space_before_chunk);
    assert_invariant(window.into_ptr().is_aligned_pow2(alignof(Chunk)));

    auto& bit_header =
        *reinterpret_cast<BitHeader*>(window.into_ptr().sub(sizeof(BitHeader)).unwrap());
    auto header = ChunkHeaderFormat(bit_header);

    header.set_bytes_before(space_before_chunk);
    header.set_bytes_after(0);
  }

  assert_invariant(window.size() >= sizeof(Chunk));
  assert_invariant(window.into_ptr().is_aligned_pow2(alignof(Chunk)));

  const OwnPtr owned_ptr = window.into_ptr().unwrap();
  virt_do_deallocate(owned_ptr, window.size(), alignof(Chunk));

  return {};
}
