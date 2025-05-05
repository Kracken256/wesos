/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/AtomicMemoryEconomy.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>

using namespace wesos::mem;

namespace wesos::mem {
  static AtomicMemoryEconomy ECONOMY_GLOBAL;
}  // namespace wesos::mem

SYM_EXPORT MemoryResourceProtocol::MemoryResourceProtocol() { ECONOMY_GLOBAL.add_resource(*this); };
SYM_EXPORT MemoryResourceProtocol::~MemoryResourceProtocol() { ECONOMY_GLOBAL.remove_resource(*this); };

SYM_EXPORT auto MemoryResourceProtocol::allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  return virt_allocate(size, align);
}

SYM_EXPORT auto MemoryResourceProtocol::deallocate_bytes(NullableOwnPtr<void> ptr, usize size,
                                                         PowerOfTwo<usize> align) -> void {
  if (ptr.isset()) [[likely]] {
    virt_deallocate(ptr.get_unchecked(), size, align);
  }
}

SYM_EXPORT auto MemoryResourceProtocol::utilize_bytes(View<u8> pool) -> void { virt_utilize(pool); }

SYM_EXPORT auto MemoryResourceProtocol::virt_embezzle(usize) -> View<u8> { return View<u8>::create_empty(); }
SYM_EXPORT auto MemoryResourceProtocol::virt_allocate(usize, PowerOfTwo<usize>) -> NullableOwnPtr<void> { return null; }
SYM_EXPORT auto MemoryResourceProtocol::virt_deallocate(OwnPtr<void>, usize, PowerOfTwo<usize>) -> void {}
SYM_EXPORT auto MemoryResourceProtocol::virt_utilize(View<u8>) -> void {}
