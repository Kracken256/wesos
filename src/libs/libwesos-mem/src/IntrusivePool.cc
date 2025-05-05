/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <sanitizer/asan_interface.h>

#include <wesos-builtin/Export.hh>
#include <wesos-mem/IntrusivePool.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT IntrusivePool::IntrusivePool(ObjectSize object_size, PowerOfTwo<usize> object_align, View<u8> pool)
    : m_front(nullptr),
      m_object_size(object_size),
      m_object_align(max(object_align.unwrap(), alignof(FreeNode))),
      m_initial_pool(pool) {
  virt_utilize(pool);
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

SYM_EXPORT auto IntrusivePool::virt_embezzle(usize max_size) -> View<u8> {
  /// TODO: Implement memory embezzelment
  return View<u8>::create_empty();
}

SYM_EXPORT auto IntrusivePool::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  size = max(size, sizeof(FreeNode));

  if (!m_front.isset() || size > object_size() || align > object_align()) [[unlikely]] {
    return nullptr;
  }

  const auto freenode = m_front;
  ASAN_UNPOISON_MEMORY_REGION(freenode.unwrap(), size);

  m_front = freenode->m_next;

  const auto result = OwnPtr(bit_cast<u8*>(freenode.unwrap()));
  assert_invariant(is_aligned_pow2(result, align));

  return result.unwrap();
}

SYM_EXPORT void IntrusivePool::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  assert_invariant(size <= object_size() && max(align.unwrap(), alignof(FreeNode)) == object_align());

  const auto node = OwnPtr(bit_cast<FreeNode*>(ptr.unwrap()));

  node->m_next = m_front;
  m_front = node;

  ASAN_POISON_MEMORY_REGION(ptr.unwrap(), object_size());
}

SYM_EXPORT auto IntrusivePool::virt_utilize(View<u8> pool) -> void {
  if (pool.empty()) [[unlikely]] {
    return;
  }

  for_each_chunk_aligned(pool, object_size(), object_align(), [&](auto object_range) {
    const auto object_ptr = OwnPtr(object_range.into_ptr().get_unchecked().unwrap());
    assert_invariant(object_range.size() == object_size() && is_aligned_pow2(object_ptr, object_align()));

    virt_deallocate(object_ptr.unwrap(), object_size(), object_align());
  });
}
