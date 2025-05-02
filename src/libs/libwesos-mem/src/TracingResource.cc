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

SYM_EXPORT auto TracingResource::virt_do_allocate(usize size,
                                                  PowerOfTwo<usize> align) -> NullableOwnPtr<u8> {
  auto ptr = m_debugee->allocate_bytes(size, align);
  m_print("do_allocate(%zu, %zu) -> %p\n", size, align.unwrap(), (void*)ptr.unwrap());

  return ptr;
}

SYM_EXPORT void TracingResource::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                    PowerOfTwo<usize> align) {
  m_print("do_deallocate(%p, %zu, %zu)\n", (void*)ptr.unwrap(), size, align.unwrap());

  return m_debugee->deallocate_bytes(ptr, size, align);
}

SYM_EXPORT auto TracingResource::virt_do_utilize(View<u8> pool) -> LeftoverMemory {
  auto unused = m_debugee->utilize_bytes(pool);

  m_print("do_utilize(%p, %zu) -> ((%p, %zu), (%p, %zu))\n", (void*)pool.into_ptr().unwrap(),
          pool.size(), (void*)unused.first()->into_ptr().unwrap(), unused.first()->size(),
          (void*)unused.second()->into_ptr().unwrap(), unused.second()->size());

  return unused;
}
