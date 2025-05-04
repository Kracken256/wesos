/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/MemoryOrder.hh>

namespace wesos::sync::detail::atomic::arch {
  template <typename Atom>
  void store(Atom* ptr, Atom value, MemoryOrder order);

  template <typename Atom>
  auto load(const Atom* ptr, MemoryOrder order) -> Atom;

  template <typename Atom>
  auto exchange(Atom* ptr, Atom value, MemoryOrder order) -> Atom;

  template <typename Atom>
  auto compare_exchange_strong(Atom* ptr, Atom* expected, Atom desired, MemoryOrder success,
                               MemoryOrder failure) -> bool;

  template <typename Atom>
  auto compare_exchange_weak(Atom* ptr, Atom* expected, Atom desired, MemoryOrder success, MemoryOrder failure) -> bool;
}  // namespace wesos::sync::detail::atomic::arch
