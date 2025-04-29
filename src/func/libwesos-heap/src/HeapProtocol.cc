/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <cstring>
#include <wesos-assert/Export.hh>
#include <wesos-builtin/Memory.hh>
#include <wesos-heap/HeapProtocol.hh>
#include <wesos-types/Null.hh>

using namespace wesos::heap;

SYM_EXPORT auto HeapProtocol::allocate(usize n_bytes, usize align,
                                       bool zero_memory) -> NullableOwnPtr<u8> {
  if (n_bytes == 0 || align == 0) [[unlikely]] {
    return null;
  }

  auto ptr = virt_allocate(n_bytes, align);

  if (ptr != nullptr && zero_memory) {
    memset(ptr.into_raw(), 0, n_bytes.unwrap());
  }

  return ptr;
}

SYM_EXPORT void HeapProtocol::deallocate(NullableOwnPtr<u8> ptr) {
  if (ptr != nullptr) [[likely]] {
    virt_deallocate(ptr);
  }
}
