/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <cstring>
#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Memory.hh>
#include <wesos-heap/HeapProtocol.hh>
#include <wesos-types/Null.hh>

using namespace wesos::heap;

SYM_EXPORT auto HeapProtocol::allocate(usize n_bytes, usize align,
                                       bool zero_memory) -> Nullable<View<u8>> {
  if (n_bytes == 0 || align == 0) [[unlikely]] {
    return null;
  }

  auto slice_opt = virt_allocate(n_bytes, align);

  if (slice_opt.isset() && zero_memory) {
    memset(slice_opt.unwrap().into_ptr().into_raw(), 0, n_bytes.unwrap());
  }

  return slice_opt;
}

SYM_EXPORT void HeapProtocol::deallocate(Nullable<View<u8>> ptr) {
  if (ptr.isset()) [[likely]] {
    virt_deallocate(ptr.unwrap());
  }
}
