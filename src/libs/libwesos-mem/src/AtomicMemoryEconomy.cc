/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/AtomicMemoryEconomy.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT auto AtomicMemoryEconomy::virt_embezzle(usize) -> View<u8> {
  // Don't embezzel from children, otherwise it cause a recursive explosion.
  return View<u8>::create_empty();
}

SYM_EXPORT auto AtomicMemoryEconomy::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  m_lock.critical_section([&] {
    /// TODO: allocate from a child
  });

  return null;
}

SYM_EXPORT void AtomicMemoryEconomy::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  m_lock.critical_section([&] {
    /// TODO: deallocate from a child
  });
}

SYM_EXPORT auto AtomicMemoryEconomy::virt_utilize(View<u8> pool) -> void {
  /// TODO: supply more memory
}

SYM_EXPORT auto AtomicMemoryEconomy::add_resource(MemoryResourceProtocol& child) -> void {
  m_lock.critical_section([&] {
    /// TODO: add child node
  });
}

SYM_EXPORT auto AtomicMemoryEconomy::remove_resource(MemoryResourceProtocol& child) -> void {
  m_lock.critical_section([&] {
    /// TODO: remove child node
  });
}
