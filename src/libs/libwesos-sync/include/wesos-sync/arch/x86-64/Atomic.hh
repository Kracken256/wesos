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
  template <class Atom>
  void store(Atom* ptr, Atom value, MemoryOrder order) {
    __atomic_store_n(ptr, value, order);
  }

  template <class Atom>
  auto load(const Atom* ptr, MemoryOrder order) -> Atom {
    return __atomic_load_n(ptr, order);
  }

  template <class Atom>
  auto exchange(Atom* ptr, Atom value, MemoryOrder order) -> Atom {
    return __atomic_exchange_n(ptr, value, order);
  }

  template <class Atom>
  auto compare_exchange_strong(Atom* ptr, Atom* expected, Atom desired, MemoryOrder success,
                               MemoryOrder failure) -> bool {
    return __atomic_compare_exchange_n(ptr, expected, desired, false, success, failure);
  }

  template <class Atom>
  auto compare_exchange_weak(Atom* ptr, Atom* expected, Atom desired, MemoryOrder success,
                             MemoryOrder failure) -> bool {
    return __atomic_compare_exchange_n(ptr, expected, desired, true, success, failure);
  }

  template <class Atom>
  auto fetch_add(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_add(ptr, val, order);
  }

  template <class Atom>
  auto fetch_sub(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_sub(ptr, val, order);
  }

  template <class Atom>
  auto fetch_and(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_and(ptr, val, order);
  }

  template <class Atom>
  auto fetch_or(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_or(ptr, val, order);
  }

  template <class Atom>
  auto fetch_xor(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_xor(ptr, val, order);
  }

  template <class Atom>
  auto fetch_nand(Atom* ptr, Atom val, MemoryOrder order) -> Atom {
    return __atomic_fetch_nand(ptr, val, order);
  }
}  // namespace wesos::sync::detail::atomic::arch
