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
  static constexpr auto MAX_ALIGNMENT_LOG2 = (sizeof(usize) * 8) - 1;

  static auto alignment_log2(PowerOfTwo<usize> x) -> Most<u8, MAX_ALIGNMENT_LOG2> {
    usize value = x.unwrap();
    u8 result = 0;

    while ((value >>= 1) != 0) {
      ++result;
    }

    return result;
  }

  static auto alignment_exp2(Most<u8, MAX_ALIGNMENT_LOG2> x) -> usize {
    return usize{1} << x.unwrap();
  }

  static auto store_metadata(View<u8> chunk, usize size,
                             PowerOfTwo<usize> align) -> NullableRefPtr<u8> {
    const auto chunk_ptr = chunk.into_ptr().get_unchecked();

    if (chunk_ptr.is_aligned_pow2(align)) {
      ASAN_UNPOISON_MEMORY_REGION(chunk_ptr.unwrap(), chunk.size());
      return chunk_ptr;
    }

    const auto padding = chunk_ptr.align_pow2(align).into_uptr() - chunk_ptr.into_uptr();
    if (const auto max_padding_supported = 0xffff; padding > max_padding_supported) [[unlikely]] {
      return nullptr;
    }

    const auto aligned_ptr = chunk_ptr.add(padding);
    const auto metadata_ptr = aligned_ptr.sub(1);
    const auto usable_range = View<u8>(aligned_ptr.unwrap(), size);

    ASAN_UNPOISON_MEMORY_REGION(usable_range.into_ptr().unwrap(), usable_range.size());

    ASAN_UNPOISON_MEMORY_REGION(metadata_ptr.unwrap(), 1);
    metadata_ptr.store(alignment_log2(align));
    ASAN_POISON_MEMORY_REGION(metadata_ptr.unwrap(), 1);

    return aligned_ptr;
  }

  static auto get_dealigned_range(View<u8> zone, PowerOfTwo<usize> align) -> View<u8> {
    const auto zone_ptr = zone.into_ptr().get_unchecked();

    if (zone_ptr.is_aligned_pow2(align)) {
      return zone;
    }

    /// TODO: Impl
    always_assert(false);
    return {};
  }
}  // namespace wesos::mem::detail

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_allocate(usize size, PowerOfTwo<usize> align)
    -> NullableOwnPtr<u8> {
  if (align > max_alignment()) {
    return nullptr;
  }

  W_DEBUG();

  for (auto node = m_some; node.isset(); node = node->m_next) {
    const auto unaligned_chunk_start = RefPtr(reinterpret_cast<u8*>(node.unwrap()));
    const auto unaligned_chunk_end = RefPtr(unaligned_chunk_start.unwrap() + node->m_size);
    const auto aligned_range_start = RefPtr(unaligned_chunk_start.align_pow2(align));
    const auto aligned_range_end = RefPtr(aligned_range_start.unwrap() + size);

    if (auto is_large_enough = aligned_range_end <= unaligned_chunk_end; !is_large_enough) {
      W_DEBUG();
      continue;
    }

    const auto min_range = View<u8>(unaligned_chunk_start.unwrap(), aligned_range_end.unwrap());
    const auto result = detail::store_metadata(min_range, size, align);

    W_DEBUG();

    return result.unwrap();
  }

  W_DEBUG();

  return nullptr;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                           PowerOfTwo<usize> align) {
  assert_invariant(ptr.is_aligned(align));

  const auto dealigned_range = detail::get_dealigned_range(View<u8>(ptr.unwrap(), size), align);
  assert_invariant(dealigned_range.size() >= sizeof(Chunk) &&
                   dealigned_range.into_ptr().is_aligned(alignof(Chunk)));

  ASAN_POISON_MEMORY_REGION(dealigned_range.into_ptr().unwrap(), dealigned_range.size());

  const auto chunk = OwnPtr(reinterpret_cast<Chunk*>(dealigned_range.into_ptr().unwrap()));
  const auto chunk_size = dealigned_range.size();

  // Seek to maintain the order of the freelist.
  auto prev = m_some;
  while (prev.isset() && prev->m_size <= chunk_size) {
    prev = prev->m_next;
  }

  if (prev.isset()) {
    W_DEBUG();

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

        ASAN_POISON_MEMORY_REGION(begin_of_next.unwrap(), sizeof(Chunk));
      }
    }
  } else {
    W_DEBUG();

    ASAN_UNPOISON_MEMORY_REGION(chunk.unwrap(), sizeof(Chunk));

    chunk->m_size = chunk_size;
    chunk->m_next = nullptr;

    m_some = chunk;
  }

  W_DEBUG();
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.size() < sizeof(Chunk)) [[unlikely]] {
    return {{pool}, {}};
  }

  const OwnPtr owned_ptr = pool.into_ptr().get_unchecked().unwrap();
  virt_do_deallocate(owned_ptr, pool.size(), alignof(u8));

  return {};
}
