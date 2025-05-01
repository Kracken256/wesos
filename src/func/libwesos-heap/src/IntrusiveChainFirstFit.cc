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

using Chunk = IntrusiveChainFirstFit::Chunk;

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(View<u8> pool) : m_initial_pool(pool) {
  virt_utilize(pool);
}

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(IntrusiveChainFirstFit&& o)
    : m_some(o.m_some) {
  // Any allocations from the source will fail after a move.

  o.m_some = nullptr;
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o)
    -> IntrusiveChainFirstFit& {
  // Any allocations from the source will fail after a move.

  m_some = o.m_some;
  o.m_some = nullptr;

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

static auto as_unaligned_range(NullableRefPtr<Chunk> node) -> View<u8> {
  W_DEBUG();

  auto* unaligned_ptr = reinterpret_cast<u8*>(node.unwrap());
  auto range = View<u8>(unaligned_ptr, node->m_size);
  assert_invariant(!range.empty());

  W_DEBUG();

  return range;
}

static auto as_aligned_range(View<u8> unaligned_range, usize align) -> View<u8> {
  W_DEBUG();

  auto padding = unaligned_range.into_ptr().align_pow2(align).into_uptr() -
                 unaligned_range.into_ptr().into_uptr();

  if (unaligned_range.size() < padding) {
    W_DEBUG();
    return View<u8>::create_empty();
  }

  W_DEBUG();

  auto range = unaligned_range.subview_unchecked(padding);
  assert_invariant(range.into_ptr().is_aligned(align));

  W_DEBUG();

  return range;
}

static auto seperate_range(View<u8> aligned_range, usize size) -> NullableRefPtr<Chunk> {
  W_DEBUG();

  auto range = aligned_range.subview_unchecked(size);
  auto padding = range.into_ptr().align_pow2(alignof(Chunk)).into_uptr() -  //
                 range.into_ptr().into_uptr();

  if (range.size() < padding + sizeof(Chunk)) {
    W_DEBUG();
    return nullptr;
  }

  W_DEBUG();

  range = range.subview_unchecked(padding);
  assert_invariant(range.into_ptr().is_aligned(alignof(Chunk)));

  W_DEBUG();

  auto* chunk = reinterpret_cast<Chunk*>(range.into_ptr().unwrap());

  chunk->m_size = aligned_range.size();
  chunk->m_next = nullptr;
  chunk->m_prev = nullptr;

  W_DEBUG();

  return chunk;
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_allocate(Least<usize, 0> size, PowerOfTwo<usize> align)
    -> Nullable<View<u8>> {
  W_DEBUG();

  for (NullableRefPtr<Chunk> node = m_some; node.isset(); node = node->m_next) {
    const auto unaligned_range = as_unaligned_range(node);
    const auto aligned_range = as_aligned_range(unaligned_range, align);
    if (size > aligned_range.size()) {
      W_DEBUG();
      continue;
    }

    const auto adjecent_range = seperate_range(aligned_range, size.unwrap());

    if (adjecent_range.isset()) {
      W_DEBUG();

      if (node->m_prev.isset()) {
        W_DEBUG();
        printf("node->m_prev = %p\n", (void*)node->m_prev.unwrap());
        node->m_prev->m_next = adjecent_range;
      }

      if (node->m_next.isset()) {
        W_DEBUG();
        node->m_next->m_prev = adjecent_range;
      }

      adjecent_range->m_prev = node->m_prev;
      adjecent_range->m_next = node->m_next;

      W_DEBUG();
    }

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

    auto object_range = aligned_range.subview_unchecked(0, size.unwrap());
    assert_invariant(object_range.into_ptr().is_aligned(align) && object_range.size() >= size);

    W_DEBUG();

    return object_range;
  }

  W_DEBUG();

  return nullptr;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_deallocate(View<u8> ptr) {
  W_DEBUG();

  auto chunk_range = ptr.subview_unchecked(0, sizeof(Chunk));
  assert_invariant(ptr.into_ptr().is_aligned(alignof(Chunk)));

  auto* chunk = reinterpret_cast<Chunk*>(chunk_range.into_ptr().unwrap());

  if (m_some.isset()) {
    W_DEBUG();

    always_assert(false && "Not implemented");

    /// TODO: Insert node
  } else {
    W_DEBUG();

    chunk->m_size = ptr.size();
    chunk->m_prev = nullptr;
    chunk->m_next = nullptr;

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
  m_some = nullptr;
  virt_utilize(m_initial_pool);
}
