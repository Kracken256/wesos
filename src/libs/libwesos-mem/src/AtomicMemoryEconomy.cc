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

SYM_EXPORT auto AtomicMemoryEconomy::allocate(usize size) -> NullableOwnPtr<void> {
  m_lock.critical_section([&] {
    /// TODO: allocate memory from any child
    (void)size;
  });

  return null;
}

SYM_EXPORT auto AtomicMemoryEconomy::utilize(View<u8> pool) -> void {
  m_lock.critical_section([&] {
    /// TODO: distribute memory across children (uniformly?)
    (void)pool;
  });
}

SYM_EXPORT auto AtomicMemoryEconomy::add_resource(MemoryResourceProtocol& child) -> void {
  m_lock.critical_section([&] {
    assert_invariant(child.m_eco_chain_next.is_null());

    child.m_eco_chain_next = m_front;
    m_front = &child;
  });
}

SYM_EXPORT auto AtomicMemoryEconomy::remove_resource(MemoryResourceProtocol& child) -> void {
  m_lock.critical_section([&] {
    NullableRefPtr<MemoryResourceProtocol> node = m_front;
    NullableRefPtr<MemoryResourceProtocol> prev = null;

    while (node.isset()) {
      if (node.unwrap() == &child) [[unlikely]] {
        const auto next = node->m_eco_chain_next;
        child.m_eco_chain_next = null;

        if (prev.isset()) {
          prev->m_eco_chain_next = next;
        } else {
          m_front = next;
        }

        return;
      }

      prev = node;
      node = node->m_eco_chain_next;
    }
  });
}

SYM_EXPORT auto wesos::mem::global_memory_economy() -> AtomicMemoryEconomy& {
  static AtomicMemoryEconomy INSTANCE_STATIC;
  return INSTANCE_STATIC;
}
