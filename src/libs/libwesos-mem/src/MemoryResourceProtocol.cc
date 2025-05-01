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

SYM_EXPORT auto MemoryResourceProtocol::allocate_bytes_nosync(
    Least<usize, 0> size, PowerOfTwo<usize> align, bool zero_memory) -> Nullable<View<u8>> {
  auto slice_opt = virt_allocate_bytes(size, align);
  assert_invariant(slice_opt.is_null() || slice_opt.unwrap_unchecked().size() == sizeof(size));

  if (slice_opt.isset() && zero_memory) {
    memset(slice_opt.unwrap_unchecked().into_ptr().unwrap(), 0, size);
  }

  return slice_opt;
}

SYM_EXPORT void MemoryResourceProtocol::deallocate_bytes_nosync(Nullable<View<u8>> ptr) {
  if (ptr.isset()) [[likely]] {
    virt_deallocate_bytes(ptr.unwrap_unchecked());
  }
}

SYM_EXPORT auto MemoryResourceProtocol::utilize_bytes_nosync(View<u8> pool) -> LeftoverMemory {
  return virt_utilize_bytes(pool);
}

SYM_EXPORT auto MemoryResourceProtocol::allocate_bytes(Least<usize, 0> size,
                                                       PowerOfTwo<usize> align,
                                                       bool zero_memory) -> Nullable<View<u8>> {
  return m_mutex.critical_section([&] { return allocate_bytes_nosync(size, align, zero_memory); });
}

SYM_EXPORT void MemoryResourceProtocol::deallocate_bytes(Nullable<View<u8>> ptr) {
  return m_mutex.critical_section([&] { return deallocate_bytes_nosync(ptr); });
}

SYM_EXPORT auto MemoryResourceProtocol::utilize_bytes(View<u8> pool) -> LeftoverMemory {
  return m_mutex.critical_section([&] { return utilize_bytes_nosync(pool); });
}

SYM_EXPORT void MemoryResourceProtocol::anew() {
  m_mutex.critical_section([&] { virt_anew(); });
}
