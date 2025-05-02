/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <sanitizer/asan_interface.h>

#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Range.hh>
#include <wesos-mem/IntrusivePool.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT IntrusivePool::IntrusivePool(ObjectSize object_size, PowerOfTwo<usize> object_align,
                                        View<u8> pool)
    : m_front(nullptr),
      m_object_size(object_size),
      m_object_align(max(object_align.unwrap(), alignof(FreeNode))),
      m_initial_pool(pool) {
  virt_do_utilize(pool);
}

SYM_EXPORT IntrusivePool::IntrusivePool(IntrusivePool&& o)
    : m_front(o.m_front),
      m_object_size(o.m_object_size),
      m_object_align(o.m_object_align),
      m_initial_pool(o.m_initial_pool) {
  o.m_front = nullptr;
  o.m_initial_pool.clear();
}

SYM_EXPORT auto IntrusivePool::operator=(IntrusivePool&& o) -> IntrusivePool& {
  if (this != &o) {
    m_object_size = o.m_object_size;
    m_object_align = o.m_object_align;
    m_front = o.m_front;
    m_initial_pool = o.m_initial_pool;

    o.m_front = nullptr;
    o.m_initial_pool.clear();
  }

  return *this;
}

SYM_EXPORT IntrusivePool::~IntrusivePool() {
  auto node = m_front;
  while (node.isset()) {
    ASAN_UNPOISON_MEMORY_REGION(node, object_size());
    node = node->m_next;
  }
}

SYM_EXPORT auto IntrusivePool::virt_do_allocate(usize size,
                                                PowerOfTwo<usize> align) -> NullableOwnPtr<u8> {
  if (!m_front.isset() || size > object_size() || align > object_align()) [[unlikely]] {
    return nullptr;
  }

  const auto freenode = m_front;
  ASAN_UNPOISON_MEMORY_REGION(freenode.unwrap(), size);

  m_front = freenode->m_next;

  OwnPtr object_ptr = reinterpret_cast<u8*>(freenode.unwrap());
  assert_invariant(object_ptr.is_aligned(align));

  return object_ptr;
}

SYM_EXPORT void IntrusivePool::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                  PowerOfTwo<usize> align) {
  align = max(align.unwrap(), alignof(FreeNode));
  assert_invariant(size <= object_size() && align >= object_align());

  RefPtr node = reinterpret_cast<FreeNode*>(ptr.unwrap());

  node->m_next = m_front;
  m_front = node;

  ASAN_POISON_MEMORY_REGION(ptr.unwrap(), size);
}

SYM_EXPORT auto IntrusivePool::virt_do_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.empty()) [[unlikely]] {
    return {{pool}, {}};
  }

  View<u8> remaining = pool;

  {
    while (true) {
      const auto padding = remaining.into_ptr().align_pow2(object_align()).into_uptr() -  //
                           remaining.into_ptr().into_uptr();
      if (remaining.size() < padding + object_size()) {
        break;
      }

      remaining = remaining.subview_unchecked(padding);
      assert_invariant(remaining.into_ptr().is_aligned(object_align()) &&
                       remaining.size() >= object_size());

      const auto object_range = remaining.subview_unchecked(0, object_size());

      // Wierd, but it works..
      const OwnPtr owned_ptr = object_range.into_ptr().get_unchecked().unwrap();
      virt_do_deallocate(owned_ptr, object_range.size(), object_align());

      remaining = remaining.subview_unchecked(object_size());
    };
  }

  const auto front_padding = pool.into_ptr().align_pow2(object_align()).into_uptr() -  //
                             pool.into_ptr().into_uptr();
  const auto beginning_unused = pool.subview_unchecked(0, front_padding);
  const auto end_unused = remaining;

  return {{beginning_unused}, {end_unused}};
}
