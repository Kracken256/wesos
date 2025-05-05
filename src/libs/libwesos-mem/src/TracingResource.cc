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

SYM_EXPORT auto TracingResource::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  auto ptr = m_debugee->allocate_bytes(size, align);
  m_print("allocate(%zu, %zu) -> %p\n", size, align.unwrap(), ptr.unwrap());

  return ptr;
}

SYM_EXPORT void TracingResource::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  m_print("deallocate(%p, %zu, %zu)\n", ptr.unwrap(), size, align.unwrap());

  return m_debugee->deallocate_bytes(ptr, size, align);
}

SYM_EXPORT auto TracingResource::virt_utilize(View<u8> pool) -> void {
  m_debugee->utilize_bytes(pool);

  m_print("utilize(%p, %zu)\n", pool.into_ptr().unwrap(), pool.size());
}

SYM_EXPORT auto TracingResource::virt_embezzle(usize max_size) -> View<u8> {
  auto embezzled = m_debugee->embezzle(max_size);

  m_print("embezzle(%zu) -> (%p, %zu)\n", max_size, static_cast<void*>(embezzled.into_ptr().unwrap()),
          embezzled.size());

  return embezzled;
}
