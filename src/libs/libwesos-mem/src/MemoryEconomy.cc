/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/MemoryEconomy.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT MemoryEconomy::MemoryEconomy() {}

SYM_EXPORT auto MemoryEconomy::allocate(usize size) -> NullableOwnPtr<void> {
  m_lock.critical_section([&] {
    /* Request that all children yield some of their memory */
    for (auto node = m_front; node.isset(); node = node->m_eco_chain_next) {
      node->m_eco_request_size.fetch_add(size);
      node->m_eco_should_yield.store(true);
    }
  });

  return null;
}

SYM_EXPORT auto MemoryEconomy::utilize(View<u8> pool) -> void {
  m_lock.critical_section([&] {
    // TODO: distribute memory across children (uniformly?)
    (void)pool;
  });
}

SYM_EXPORT auto MemoryEconomy::add_resource(MemoryResourceProtocol& child) -> void {
  m_lock.critical_section([&] {
    assert_invariant(child.m_eco_chain_next.is_null());

    child.m_eco_chain_next = m_front;
    m_front = &child;
  });
}

SYM_EXPORT auto MemoryEconomy::remove_resource(MemoryResourceProtocol& child) -> void {
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

SYM_EXPORT auto wesos::mem::global_memory_economy() -> MemoryEconomy& {
  static MemoryEconomy INSTANCE_STATIC;
  return INSTANCE_STATIC;
}
