/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <cstring>
#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Memory.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Null.hh>

using namespace wesos::mem;

SYM_EXPORT MemoryResourceProtocol::~MemoryResourceProtocol() = default;

SYM_EXPORT auto MemoryResourceProtocol::allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<u8> {
  return virt_do_allocate(size, align);
}

SYM_EXPORT auto MemoryResourceProtocol::deallocate_bytes(NullableOwnPtr<u8> ptr, usize size,
                                                         PowerOfTwo<usize> align) -> void {
  if (ptr.isset()) [[likely]] {
    virt_do_deallocate(ptr.get_unchecked(), size, align);
  }
}

SYM_EXPORT auto MemoryResourceProtocol::utilize_bytes(View<u8> pool) -> void { virt_do_utilize(pool); }
