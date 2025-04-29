/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/MemoryOrder.hh>
#include <wesos-sync/arch/AtomicBackend.hh>

namespace wesos::sync::detail::atomic::arch {
  template <typename AtomGeneric>
  void store(AtomGeneric* ptr, AtomGeneric value, MemoryOrder order) {
    __atomic_store_n(ptr, value, order);
  }

  template <typename AtomGeneric>
  auto load(const AtomGeneric* ptr, MemoryOrder order) -> AtomGeneric {
    return __atomic_load_n(ptr, order);
  }

  template <typename AtomGeneric>
  auto exchange(AtomGeneric* ptr, AtomGeneric value, MemoryOrder order) -> AtomGeneric {
    return __atomic_exchange_n(ptr, value, order);
  }

  template <typename AtomGeneric>
  auto compare_exchange_strong(AtomGeneric* ptr, AtomGeneric* expected, AtomGeneric desired,
                               MemoryOrder success, MemoryOrder failure) -> bool {
    return __atomic_compare_exchange_n(ptr, expected, desired, false, success, failure);
  }

  template <typename AtomGeneric>
  auto compare_exchange_weak(AtomGeneric* ptr, AtomGeneric* expected, AtomGeneric desired,
                             MemoryOrder success, MemoryOrder failure) -> bool {
    return __atomic_compare_exchange_n(ptr, expected, desired, true, success, failure);
  }
}  // namespace wesos::sync::detail::atomic::arch
