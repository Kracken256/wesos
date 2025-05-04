/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/TracingResource.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT TracingResource::TracingResource(MemoryResourceProtocol& debugee, PrintCallback print)
    : m_debugee(&debugee), m_print(print) {}

SYM_EXPORT auto TracingResource::virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  auto ptr = m_debugee->allocate_bytes(size, align);
  m_print("do_allocate(%zu, %zu) -> %p\n", size, align.unwrap(), ptr.unwrap());

  return ptr;
}

SYM_EXPORT void TracingResource::virt_do_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  m_print("do_deallocate(%p, %zu, %zu)\n", ptr.unwrap(), size, align.unwrap());

  return m_debugee->deallocate_bytes(ptr, size, align);
}

SYM_EXPORT auto TracingResource::virt_do_utilize(View<u8> pool) -> void {
  m_debugee->utilize_bytes(pool);

  m_print("do_utilize(%p, %zu)\n", pool.into_ptr().unwrap(), pool.size());
}
