/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/MemoryOrder.hh>

namespace wesos::sync::detail::atomic::arch {
  template <typename AtomGeneric>
  void store(AtomGeneric* ptr, AtomGeneric value, MemoryOrder order);

  template <typename AtomGeneric>
  auto load(const AtomGeneric* ptr, MemoryOrder order) -> AtomGeneric;

  template <typename AtomGeneric>
  auto exchange(AtomGeneric* ptr, AtomGeneric value, MemoryOrder order) -> AtomGeneric;

  template <typename AtomGeneric>
  auto compare_exchange_strong(AtomGeneric* ptr, AtomGeneric* expected, AtomGeneric desired,
                               MemoryOrder success, MemoryOrder failure) -> bool;

  template <typename AtomGeneric>
  auto compare_exchange_weak(AtomGeneric* ptr, AtomGeneric* expected, AtomGeneric desired,
                             MemoryOrder success, MemoryOrder failure) -> bool;
}  // namespace wesos::sync::detail::atomic::arch
