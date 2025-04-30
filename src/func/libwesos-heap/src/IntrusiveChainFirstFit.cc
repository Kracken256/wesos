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
    const auto chunk_size = node->m_size;
    const auto chunk_ptr = RefPtr(reinterpret_cast<u8*>(node.unwrap()));
    const auto aligned_chunk_ptr = chunk_ptr.align_pow2(align);
    const auto unused_amount = aligned_chunk_ptr.sub(chunk_ptr.into_uptr()).into_uptr();

    if (chunk_size < size + unused_amount) {
      continue;
    }

    auto chunk_subview = View<u8>(aligned_chunk_ptr.unwrap(), chunk_size - unused_amount);
    auto object_subview = chunk_subview.subview_unchecked(0, size.unwrap());
    auto partition_subview = chunk_subview.subview_unchecked(size.unwrap());

    if (partition_subview.size() >= sizeof(Chunk)) {
      const RefPtr partition_ptr = reinterpret_cast<Chunk*>(partition_subview.into_ptr().unwrap());

      partition_ptr->m_next = node->m_next;
      partition_ptr->m_size = partition_subview.size();

      node->m_next = partition_ptr;

      /// TODO: Handle chunk splitting
    }

    prev->m_next = node->m_next;

    return object_subview;
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
