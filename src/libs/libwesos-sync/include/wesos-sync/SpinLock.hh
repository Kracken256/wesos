/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/Atomic.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::sync {
  class SpinLock final : public LockProtocol {
    Atomic<bool> m_locked;

  protected:
    auto virt_lock() -> void override;
    auto virt_unlock() -> void override;
    auto virt_try_lock() -> bool override;

  public:
    constexpr SpinLock() : m_locked(false) {}
    constexpr SpinLock(const SpinLock&) = delete;
    constexpr SpinLock(SpinLock&&) = delete;
    constexpr auto operator=(const SpinLock&) -> SpinLock& = delete;
    constexpr auto operator=(SpinLock&&) -> SpinLock& = delete;
    constexpr ~SpinLock() override = default;
  };
}  // namespace wesos::sync
