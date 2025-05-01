/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-sync/SpinLock.hh>
#include <wesos-sync/Timing.hh>

using namespace wesos::sync;

SYM_EXPORT void SpinLock::virt_lock() {
  while (true) {
    bool expected = false;
    if (m_locked.compare_exchange_strong(expected, true)) {
      return;  // Successfully locked
    }

    // Pause for better performance on hyperthreaded CPUs
    ephemeral_pause();
  }
}

SYM_EXPORT void SpinLock::virt_unlock() { m_locked.store(false); }

SYM_EXPORT auto SpinLock::virt_try_lock() -> bool {
  bool expected = false;
  return m_locked.compare_exchange_strong(expected, true);
}
