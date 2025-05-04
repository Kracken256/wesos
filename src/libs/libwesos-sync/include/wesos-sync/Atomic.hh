/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-builtin/Move.hh>
#include <wesos-sync/MemoryOrder.hh>
#include <wesos-sync/arch/AtomicBackend.hh>

#if defined(__x86_64__)
#include <wesos-sync/arch/x86-64/Atomic.hh>
#else
#error "This implementation of wesos::Atomic does not support your platform. Sorry.."
#endif

namespace wesos::sync {
  template <class Atom>
  class Atomic {
  private:
    Atom m_value;

  public:
    constexpr Atomic() = default;
    constexpr explicit Atomic(Atom value) : m_value(value) {}

    [[nodiscard]] constexpr auto operator<=>(const Atomic& o) const { return load() <=> o.load(); };

    void store(Atom desired, MemoryOrder order = memory_order_seq_cst) {
      detail::atomic::arch::store(&m_value, desired, order);
    }

    auto load(MemoryOrder order = memory_order_seq_cst) const -> Atom {
      return detail::atomic::arch::load(&m_value, order);
    }

    auto exchange(Atom desired, MemoryOrder order = memory_order_seq_cst) -> Atom {
      return detail::atomic::arch::exchange(&m_value, desired, order);
    }

    auto compare_exchange_strong(Atom& expected, Atom desired, MemoryOrder success, MemoryOrder failure) -> bool {
      return detail::atomic::arch::compare_exchange_strong(&m_value, &expected, desired, success, failure);
    }

    auto compare_exchange_strong(Atom& expected, Atom desired, MemoryOrder order = memory_order_seq_cst) -> bool {
      return compare_exchange_strong(expected, desired, order, order);
    }

    auto compare_exchange_weak(Atom& expected, Atom desired, MemoryOrder success, MemoryOrder failure) -> bool {
      return detail::atomic::arch::compare_exchange_weak(&m_value, &expected, desired, success, failure);
    }

    auto compare_exchange_weak(Atom& expected, Atom desired, MemoryOrder order = memory_order_seq_cst) -> bool {
      return compare_exchange_weak(expected, desired, order, order);
    }
  };
}  // namespace wesos::sync
