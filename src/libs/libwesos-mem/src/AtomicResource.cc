/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/AtomicResource.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT AtomicResource::AtomicResource(MemoryResourceProtocol& inner) : m_inner(inner) {}

SYM_EXPORT auto AtomicResource::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  return m_lock.critical_section([&] { return m_inner.allocate_bytes(size, align); });
}

SYM_EXPORT void AtomicResource::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  return m_lock.critical_section([&] { m_inner.deallocate_bytes(move(ptr), size, align); });
}

SYM_EXPORT auto AtomicResource::virt_utilize(View<u8> pool) -> void {
  return m_lock.critical_section([&] { m_inner.utilize_bytes(pool); });
}

SYM_EXPORT auto AtomicResource::virt_embezzle(usize max_size) -> View<u8> {
  return m_lock.critical_section([&] { return m_inner.embezzle_bytes(max_size); });
}
