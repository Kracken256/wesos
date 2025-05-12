/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/Atomic.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::sync {
  class SpinLock final {
    Atomic<bool> m_locked;

  public:
    constexpr SpinLock() : m_locked(false) {}
    constexpr SpinLock(const SpinLock&) = delete;
    constexpr SpinLock(SpinLock&&) = delete;
    constexpr auto operator=(const SpinLock&) -> SpinLock& = delete;
    constexpr auto operator=(SpinLock&&) -> SpinLock& = delete;
    constexpr ~SpinLock() = default;

    auto lock() -> void;
    auto unlock() -> void;
    auto try_lock() -> bool;

    W_LOCK_PROTOCOL_IMPLEMENT(SpinLock);
  };
}  // namespace wesos::sync
