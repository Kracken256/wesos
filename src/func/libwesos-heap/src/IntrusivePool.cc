/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-heap/IntrusivePool.hh>

using namespace wesos;
using namespace wesos::heap;

SYM_EXPORT IntrusivePool::IntrusivePool(ClampLeast<usize, sizeof(FreeNode)> object_size,
                                        PowerOfTwo<usize> object_align, View<u8> initial_pool)
    : m_object_size(object_size), m_object_align(object_align) {
  utilize(initial_pool);
}

SYM_EXPORT IntrusivePool::IntrusivePool(IntrusivePool&& o)
    : m_freelist_head(o.m_freelist_head),
      m_object_size(o.m_object_size),
      m_object_align(o.m_object_align) {
  // Any allocations from the source will fail after a move.

  o.m_freelist_head = nullptr;
}

SYM_EXPORT auto IntrusivePool::operator=(IntrusivePool&& o) -> IntrusivePool& {
  m_object_size = o.m_object_size;
  m_object_align = o.m_object_align;
  m_freelist_head = o.m_freelist_head;

  // Any allocations from the source will fail after a move.
  o.m_freelist_head = nullptr;

  return *this;
}

SYM_EXPORT auto IntrusivePool::virt_allocate(Least<usize, 0> size,
                                             PowerOfTwo<usize> align) -> Nullable<View<u8>> {
  if (!m_freelist_head.isset() || size > object_size() || align > object_align()) [[unlikely]] {
    return null;
  }

  const auto freenode = m_freelist_head;
  m_freelist_head = freenode->m_next;

  auto* object_ptr = reinterpret_cast<u8*>(freenode.unwrap());
  auto object = View<u8>(object_ptr, object_size());

  return object;
}

SYM_EXPORT void IntrusivePool::virt_deallocate(View<u8> ptr) {
  assert_invariant(ptr.size() == object_size());

  auto free_node_view = ptr.subview_unchecked(0, sizeof(FreeNode));
  auto* free_node = reinterpret_cast<FreeNode*>(free_node_view.into_ptr().unwrap());

  free_node->m_next = m_freelist_head;
  m_freelist_head = free_node;
}

SYM_EXPORT auto IntrusivePool::virt_utilize(View<u8> extra_memory) -> LeftoverMemory {
  const auto prefix_unused = extra_memory.into_ptr().into_uptr() & (m_object_align.unwrap() - 1);

  if (extra_memory.size() < sizeof(FreeNode)) [[unlikely]] {
    return {};
  }

  /// TODO: Utilize supplied memory

  // Wierd, but it works..
  deallocate(extra_memory);

  return {};
}
