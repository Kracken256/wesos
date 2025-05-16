/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/MemoryEconomy.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>

using namespace wesos::mem;

SYM_EXPORT MemoryResourceProtocol::MemoryResourceProtocol() { global_memory_economy().add_resource(*this); }

SYM_EXPORT MemoryResourceProtocol::~MemoryResourceProtocol() { global_memory_economy().remove_resource(*this); };

SYM_EXPORT auto MemoryResourceProtocol::allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  eco_yield();

  return virt_allocate(size, align);
}

SYM_EXPORT auto MemoryResourceProtocol::deallocate_bytes(NullableOwnPtr<void> ptr, usize size,
                                                         PowerOfTwo<usize> align) -> void {
  eco_yield();

  if (ptr.isset()) [[likely]] {
    virt_deallocate(ptr.get_unchecked(), size, align);
  }
}

SYM_EXPORT auto MemoryResourceProtocol::utilize_bytes(View<u8> pool) -> void {
  eco_yield();

  virt_utilize(pool);
}

SYM_EXPORT auto MemoryResourceProtocol::embezzle_bytes(usize max_size) -> View<u8> {
  eco_yield();

  return virt_embezzle(max_size);
}

SYM_EXPORT void MemoryResourceProtocol::eco_yield() {
  if (const auto should_yield = m_eco_should_yield.load(sync::memory_order_acquire)) [[unlikely]] {
    m_eco_should_yield.store(false, sync::memory_order_relaxed);

    auto desired_size = m_eco_request_size.exchange(0, sync::memory_order_acq_rel);

    do {
      const auto relinquished_memory = virt_embezzle(desired_size);
      assert_invariant(relinquished_memory.size() <= desired_size);

      if (relinquished_memory.empty()) {
        return;
      }

      global_memory_economy().utilize(relinquished_memory);
      desired_size -= relinquished_memory.size();
    } while (desired_size > 0);
  }
}

SYM_EXPORT auto MemoryResourceProtocol::virt_embezzle(usize) -> View<u8> { return View<u8>::create_empty(); }
SYM_EXPORT auto MemoryResourceProtocol::virt_allocate(usize, PowerOfTwo<usize>) -> NullableOwnPtr<void> { return null; }
SYM_EXPORT auto MemoryResourceProtocol::virt_deallocate(OwnPtr<void>, usize, PowerOfTwo<usize>) -> void {}
SYM_EXPORT auto MemoryResourceProtocol::virt_utilize(View<u8>) -> void {}
