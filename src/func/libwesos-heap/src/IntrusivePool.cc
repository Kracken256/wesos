/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Range.hh>
#include <wesos-heap/IntrusivePool.hh>

using namespace wesos;
using namespace wesos::heap;

SYM_EXPORT IntrusivePool::IntrusivePool(ObjectSize object_size, PowerOfTwo<usize> object_align,
                                        View<u8> pool)
    : m_object_size(object_size),
      m_object_align(max(object_align.unwrap(), alignof(FreeNode))),
      m_initial_pool(pool) {
  virt_utilize(pool);
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

SYM_EXPORT auto IntrusivePool::virt_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.empty()) [[unlikely]] {
    LeftoverMemory unused;
    unused.first() = pool;
    return unused;
  }

  View<u8> window = pool;

  {
    usize padding;

    while (true) {
      padding = window.into_ptr().align_pow2(m_object_align).into_uptr() -  //
                window.into_ptr().into_uptr();
      if (window.size() < padding + m_object_size) {
        break;
      }

      window = window.subview_unchecked(padding);
      assert_invariant(window.into_ptr().is_aligned(m_object_align));
      assert_invariant(window.size() >= m_object_size);

      auto object_mem = window.subview_unchecked(0, m_object_size);

      // Wierd, but it works..
      virt_deallocate(object_mem);

      window = window.subview_unchecked(m_object_size);
    };
  }

  const auto initial_padding = pool.into_ptr().align_pow2(m_object_align).into_uptr() -  //
                               pool.into_ptr().into_uptr();
  const auto beginning_unused = pool.subview_unchecked(0, initial_padding);
  const auto end_unused = window;

  LeftoverMemory unused;
  unused.first() = beginning_unused;
  unused.second() = end_unused;

  return unused;
}

SYM_EXPORT void IntrusivePool::virt_anew() {
  m_freelist_head = null;
  virt_utilize(m_initial_pool);
}
