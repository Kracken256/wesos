/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-sync/SpinLock.hh>

using namespace wesos::sync;

SYM_EXPORT void SpinLock::lock() {
  bool expected;

  while (true) {
    expected = false;
    if (m_locked.compare_exchange_weak(expected, true, memory_order_acquire, memory_order_relaxed)) [[likely]] {
      return;
    }

    while (m_locked.load(memory_order_relaxed)) {
      cpu::ephemeral_pause();
    }
  }
}

SYM_EXPORT void SpinLock::unlock() { m_locked.store(false, memory_order_release); }

SYM_EXPORT auto SpinLock::try_lock() -> bool {
  bool expected = false;
  return m_locked.compare_exchange_strong(expected, true, memory_order_acquire, memory_order_relaxed);
}
