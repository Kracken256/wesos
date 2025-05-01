/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-heap/IntrusiveChainFirstFit.hh>

using namespace wesos;
using namespace wesos::heap;

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(View<u8> pool) : m_initial_pool(pool) {
  virt_utilize(pool);
}

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(IntrusiveChainFirstFit&& o)
    : m_some(o.m_some) {
  // Any allocations from the source will fail after a move.

  o.m_some = null;
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o)
    -> IntrusiveChainFirstFit& {
  // Any allocations from the source will fail after a move.

  m_some = o.m_some;
  o.m_some = null;

  return *this;
}

extern "C" {
struct FILE;

auto printf(const char*, ...) -> int;
auto fflush(FILE*) -> int;

extern FILE* stdout;
}

static void debug_s(const char* func, int line) {
  printf("[%s]: %d\n", func, line);
  fflush(stdout);
}

#define W_DEBUG() debug_s(__func__, __LINE__)

SYM_EXPORT auto IntrusiveChainFirstFit::virt_allocate(Least<usize, 0> size, PowerOfTwo<usize> align)
    -> Nullable<View<u8>> {
  W_DEBUG();

  for (NullableRefPtr<Chunk> node = m_some; node.isset(); node = node->m_next) {
    const auto whole_chunk_range = [&]() {
      W_DEBUG();

      auto* unaligned_ptr = reinterpret_cast<u8*>(node.unwrap());
      auto range = View<u8>(unaligned_ptr, node->m_size);
      assert_invariant(!range.empty());

      W_DEBUG();

      return range;
    }();

    const auto chunk_range = [&]() {
      W_DEBUG();

      auto padding = whole_chunk_range.into_ptr()
                         .align_pow2(align)  //
                         .sub(whole_chunk_range.into_ptr().into_uptr())
                         .into_uptr();

      if (whole_chunk_range.size() < padding) {
        W_DEBUG();
        return View<u8>::create_empty();
      }

      W_DEBUG();

      auto range = whole_chunk_range.subview_unchecked(padding);
      assert_invariant(range.into_ptr().is_aligned(align));

      W_DEBUG();

      return range;
    }();

    W_DEBUG();

    if (size > chunk_range.size()) {
      W_DEBUG();
      continue;
    }

    W_DEBUG();

    const auto split_chunk = [&]() -> NullableRefPtr<Chunk> {
      W_DEBUG();

      auto range = chunk_range.subview_unchecked(size.unwrap());
      auto padding = range.into_ptr()
                         .align_pow2(alignof(Chunk))  //
                         .sub(range.into_ptr().into_uptr())
                         .into_uptr();

      if (range.size() < padding + sizeof(Chunk)) {
        W_DEBUG();
        return null;
      }

      W_DEBUG();

      range = range.subview_unchecked(padding);
      assert_invariant(range.into_ptr().is_aligned(alignof(Chunk)));

      W_DEBUG();

      auto* chunk = reinterpret_cast<Chunk*>(range.into_ptr().unwrap());
      // chunk->m_size = chunk_range.size();
      // chunk->m_next = null;
      // chunk->m_prev = null;

      W_DEBUG();

      return chunk;
    }();

    W_DEBUG();

    // if (split_chunk.isset()) {
    //   W_DEBUG();

    //   if (node->m_prev.isset()) {
    //     W_DEBUG();
    //     node->m_prev->m_next = split_chunk;
    //   }

    //   if (node->m_next.isset()) {
    //     W_DEBUG();
    //     node->m_next->m_prev = split_chunk;
    //   }

    //   split_chunk->m_prev = node->m_prev;
    //   split_chunk->m_next = node->m_next;

    //   W_DEBUG();
    // }

    W_DEBUG();

    if (node->m_prev.isset()) {
      W_DEBUG();
      node->m_prev->m_next = node->m_next;
    }

    if (node->m_next.isset()) {
      W_DEBUG();
      node->m_next->m_prev = node->m_prev;
    }

    W_DEBUG();

    /// TODO: Handle chunk splitting

    auto object_range = chunk_range.subview_unchecked(0, size.unwrap());
    assert_invariant(object_range.into_ptr().is_aligned(align) && object_range.size() >= size);

    W_DEBUG();

    return object_range;
  }

  W_DEBUG();

  return null;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_deallocate(View<u8> ptr) {
  W_DEBUG();

  auto chunk_range = ptr.subview_unchecked(0, sizeof(Chunk));
  assert_invariant(ptr.into_ptr().is_aligned(alignof(Chunk)));
  W_DEBUG();

  auto* chunk = reinterpret_cast<Chunk*>(chunk_range.into_ptr().unwrap());

  W_DEBUG();

  if (m_some.isset()) {
    W_DEBUG();

    if (m_some->m_prev.isset()) {
      W_DEBUG();
      m_some->m_prev->m_next = chunk;
    }

    if (m_some->m_next.isset()) {
      W_DEBUG();
      m_some->m_next->m_prev = chunk;
    }

    W_DEBUG();

    chunk->m_size = ptr.size();
    chunk->m_prev = m_some;
  } else {
    W_DEBUG();

    chunk->m_size = ptr.size();
    chunk->m_prev = null;
    chunk->m_next = null;

    m_some = chunk;
  }

  W_DEBUG();
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.size() < sizeof(Chunk)) [[unlikely]] {
    return {{pool}, {}};
  }

  virt_deallocate(pool);

  return {};
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_anew() {
  m_some = null;
  virt_utilize(m_initial_pool);
}
