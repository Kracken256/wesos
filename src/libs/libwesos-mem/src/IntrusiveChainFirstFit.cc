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
#include <wesos-types/Bitcast.hh>

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

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(View<u8> pool) : m_some(nullptr), m_initial_pool(pool) {
  virt_do_utilize(pool);
}

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(IntrusiveChainFirstFit&& o)
    : m_some(o.m_some), m_initial_pool(o.m_initial_pool) {
  o.m_some = nullptr;
  o.m_initial_pool.clear();
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o) -> IntrusiveChainFirstFit& {
  if (this != &o) {
    m_some = o.m_some;
    m_initial_pool = o.m_initial_pool;

    o.m_some = nullptr;
    o.m_initial_pool.clear();
  }

  return *this;
}

struct IntrusiveChainFirstFit::Chunk {
  Least<usize, 1> m_size;
  NullableRefPtr<Chunk> m_next;
};

namespace wesos::mem::detail {
  using Chunk = IntrusiveChainFirstFit::Chunk;

  class BitHeader final {
    u8 m_before;
    u8 m_after;

    friend class ChunkHeaderFormat;
  } __attribute__((packed));

  static_assert(sizeof(BitHeader) < sizeof(Chunk) && alignof(BitHeader) < alignof(Chunk));

  class ChunkHeaderFormat final {
  public:
    static constexpr auto MAX_U8 = 0xff;

    static_assert(sizeof(Chunk) - 1 <= MAX_U8);

    using BytesBeforeCount = Most<usize, MAX_U8>;
    using BytesAfterCount = Most<usize, MAX_U8>;

    ChunkHeaderFormat(BitHeader& header) : m_meta(header) { ASAN_UNPOISON_MEMORY_REGION(&m_meta, sizeof(BitHeader)); }

    ~ChunkHeaderFormat() { ASAN_POISON_MEMORY_REGION(&m_meta, sizeof(BitHeader)); }

    [[nodiscard]] constexpr auto get_bytes_before() -> BytesBeforeCount { return m_meta.m_before; }
    [[nodiscard]] constexpr auto get_bytes_after() -> BytesAfterCount { return m_meta.m_after; }

    void set_bytes_before(BytesBeforeCount x) { m_meta.m_before = static_cast<u8>(x.unwrap()); }
    void set_bytes_after(BytesAfterCount x) { m_meta.m_after = static_cast<u8>(x.unwrap()); }

  private:
    BitHeader& m_meta;
  };

  static auto get_dealigned_range(View<u8> zone) -> View<u8> {  /// TODO: Struct code review
    auto zone_begin = zone.into_ptr().get_unchecked();
    auto zone_end = zone_begin.add(zone.size());

    auto& bit_header = *bit_cast<BitHeader*>(zone_begin.sub(sizeof(BitHeader)).unwrap());
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
    -> NullableOwnPtr<u8> {  /// TODO: Struct code review
  using namespace detail;

  if (align > max_alignment()) [[unlikely]] {
    return nullptr;
  }

  for (NullableRefPtr<Chunk> node = m_some, prev = nullptr; node.isset(); prev = node, node = node->m_next) {
    const auto unaligned_chunk_start = RefPtr(bit_cast<u8*>(node.unwrap()));
    const auto unaligned_chunk_end = RefPtr(unaligned_chunk_start.unwrap() + node->m_size);

    const auto aligned_range_start = RefPtr(next_aligned_pow2(unaligned_chunk_start.add(sizeof(BitHeader)), align));
    const auto aligned_range_end = aligned_range_start.add(size);

    if (aligned_range_end > unaligned_chunk_end) {
      continue;
    }

    assert_invariant(aligned_range_start.sub(sizeof(BitHeader)) >= unaligned_chunk_start);
    auto& bit_header = *bit_cast<BitHeader*>(aligned_range_start.sub(sizeof(BitHeader)).unwrap());
    auto header = ChunkHeaderFormat(bit_header);

    const auto space_before = aligned_range_start.as_uptr() - unaligned_chunk_start.as_uptr();
    header.set_bytes_before(space_before);

    const auto space_after = unaligned_chunk_end.as_uptr() - aligned_range_end.as_uptr();

    const auto new_chunk_ptr_raw = next_aligned_pow2(aligned_range_end, alignof(Chunk));
    const auto padding = new_chunk_ptr_raw.as_uptr() - aligned_range_end.as_uptr();

    if (space_after < padding + sizeof(Chunk)) {
      header.set_bytes_after(space_after);
      if (prev.isset()) {
        prev->m_next = node->m_next;
      } else {
        m_some = node->m_next;
      }
    } else {
      const auto new_chunk_ptr = new_chunk_ptr_raw;
      const auto new_chunk = OwnPtr(bit_cast<Chunk*>(new_chunk_ptr.unwrap()));
      const auto new_chunk_size = unaligned_chunk_end.as_uptr() - new_chunk_ptr.as_uptr();

      ASAN_UNPOISON_MEMORY_REGION(new_chunk.unwrap(), new_chunk_size);

      new_chunk->m_size = new_chunk_size;

      // Insert new_chunk into freelist in address order
      NullableRefPtr<Chunk> seek = m_some;
      NullableRefPtr<Chunk> seek_prev = nullptr;
      while (seek.isset() && seek.as_uptr() < new_chunk.as_uptr()) {
        seek_prev = seek;
        seek = seek->m_next;
      }

      new_chunk->m_next = seek;
      if (seek_prev.isset()) {
        seek_prev->m_next = new_chunk;
      } else {
        m_some = new_chunk;
      }

      if (prev.isset()) {
        prev->m_next = node->m_next;
      } else {
        m_some = node->m_next;
      }

      header.set_bytes_after(padding);
    }

    ASAN_UNPOISON_MEMORY_REGION(aligned_range_start.unwrap(), size);
    return aligned_range_start.unwrap();
  }

  return nullptr;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                           PowerOfTwo<usize> align) {  /// TODO: Struct code review
  using namespace detail;

  assert_invariant(is_aligned_pow2(ptr, align));

  const auto dealigned_range = get_dealigned_range(View<u8>(ptr.unwrap(), size));
  assert_invariant(dealigned_range.size() >= sizeof(Chunk) &&
                   is_aligned_pow2(dealigned_range.into_ptr(), alignof(Chunk)));

  ASAN_POISON_MEMORY_REGION(dealigned_range.into_ptr().unwrap(), dealigned_range.size());

  const auto chunk = OwnPtr(bit_cast<Chunk*>(dealigned_range.into_ptr().unwrap()));
  const auto chunk_size = dealigned_range.size();

  NullableRefPtr<Chunk> prev = nullptr;
  NullableRefPtr<Chunk> curr = m_some;
  while (curr.isset() && curr.as_uptr() < chunk.as_uptr()) {
    prev = curr;
    curr = curr->m_next;
  }

  // Seek to maintain the order of the freelist.
  // auto prev = m_some;
  // while (prev.isset() && prev->m_size <= chunk_size) {
  //   prev = prev->m_next;
  // }

  if (prev.isset()) {
    ASAN_UNPOISON_MEMORY_REGION(chunk.unwrap(), sizeof(Chunk));
    chunk->m_size = chunk_size;
    chunk->m_next = prev->m_next;

    prev->m_next = chunk;

    {  // Merge adjacent free chunks
      const auto end_of_chunk = OwnPtr(bit_cast<u8*>(chunk.unwrap())).add(chunk->m_size);
      const auto begin_of_next = NullableOwnPtr(bit_cast<u8*>(chunk->m_next.unwrap()));

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

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_utilize(View<u8> pool) -> LeftoverMemory {  /// TODO: Struct code review
  using namespace detail;

  auto window = pool;

  {  // Adjust alignment and install header
    const auto space_before_chunk =
        next_aligned_pow2(window.into_ptr().add(sizeof(BitHeader)), alignof(Chunk)).as_uptr() -
        window.into_ptr().as_uptr();

    if (window.size() < space_before_chunk + sizeof(Chunk)) {
      return {{pool}, {}};
    }

    window = window.subview_unchecked(space_before_chunk);
    assert_invariant(is_aligned_pow2(window.into_ptr(), alignof(Chunk)));

    auto& bit_header = *bit_cast<BitHeader*>(window.into_ptr().sub(sizeof(BitHeader)).unwrap());
    auto header = ChunkHeaderFormat(bit_header);

    header.set_bytes_before(space_before_chunk);
    header.set_bytes_after(0);
  }

  assert_invariant(window.size() >= sizeof(Chunk));
  assert_invariant(is_aligned_pow2(window.into_ptr(), alignof(Chunk)));

  const OwnPtr owned_ptr = window.into_ptr().unwrap();
  virt_do_deallocate(owned_ptr, window.size(), alignof(Chunk));

  return {};
}
