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
    : m_front(o.m_front) {
  // Any allocations from the source will fail after a move.

  o.m_front = nullptr;
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o)
    -> IntrusiveChainFirstFit& {
  // Any allocations from the source will fail after a move.

  m_front = o.m_front;
  o.m_front = nullptr;

  return *this;
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_allocate(Least<usize, 0> size, PowerOfTwo<usize> align)
    -> Nullable<View<u8>> {
  for (NullableRefPtr<Chunk> node = m_front, prev = m_front; node.isset();
       prev = node, node = node->m_next) {
    const auto whole_chunk_range = [&]() {
      auto* unaligned_ptr = reinterpret_cast<u8*>(node.unwrap());
      auto range = View<u8>(unaligned_ptr, node->m_size);
      assert_invariant(!range.empty());

      auto padding = range.into_ptr()
                         .align_pow2(align)  //
                         .sub(range.into_ptr().into_uptr())
                         .into_uptr();

      if (range.size() < padding) {
        return View<u8>::create_empty();
      }

      range = range.subview_unchecked(padding);
      assert_invariant(range.into_ptr().is_aligned(align));

      return range;
    }();

    if (size > whole_chunk_range.size()) {
      continue;
    }

    const auto new_adjacent_chunk = [&]() -> NullableRefPtr<Chunk> {
      auto range = whole_chunk_range.subview_unchecked(size.unwrap());
      auto padding = range.into_ptr()
                         .align_pow2(alignof(Chunk))  //
                         .sub(range.into_ptr().into_uptr())
                         .into_uptr();

      if (range.size() < padding + sizeof(Chunk)) {
        return nullptr;
      }

      auto chunk_range = range.subview_unchecked(padding);
      assert_invariant(chunk_range.into_ptr().is_aligned(alignof(Chunk)));

      return reinterpret_cast<Chunk*>(chunk_range.into_ptr().unwrap());
    }();

    if (new_adjacent_chunk.isset()) {
      // partition_ptr->m_next = node->m_next;
      // partition_ptr->m_size = partition_range.size();

      // node->m_next = partition_ptr;

      /// TODO: Handle chunk splitting
    }

    // prev->m_next = node->m_next;

    return whole_chunk_range.subview_unchecked(0, size.unwrap());
  }

  return null;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_deallocate(View<u8> ptr) {
  auto free_node_view = ptr.subview_unchecked(0, sizeof(Chunk));
  auto* free_node = reinterpret_cast<Chunk*>(free_node_view.into_ptr().unwrap());

  free_node->m_next = m_front;
  free_node->m_size = ptr.size();
  m_front = free_node;
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.size() < sizeof(Chunk)) [[unlikely]] {
    return {{pool}, {}};
  }

  virt_deallocate(pool);
  return {};
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_anew() {
  m_front = null;
  virt_utilize(m_initial_pool);
}
