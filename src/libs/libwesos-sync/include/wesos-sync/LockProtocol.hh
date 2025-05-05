/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/StrongOrdering.hh>

namespace wesos::sync {
  class LockProtocol {
  protected:
    virtual auto virt_lock() -> void = 0;
    virtual auto virt_try_lock() -> bool = 0;
    virtual auto virt_unlock() -> void = 0;

  public:
    constexpr LockProtocol() = default;
    constexpr LockProtocol(const LockProtocol&) = delete;
    constexpr LockProtocol(LockProtocol&&) = delete;
    constexpr auto operator=(const LockProtocol&) -> LockProtocol& = delete;
    constexpr auto operator=(LockProtocol&&) -> LockProtocol& = delete;
    constexpr virtual ~LockProtocol() = default;

    class LockLease final {
      friend class LockProtocol;
      LockProtocol& m_parent;

      LockLease(LockProtocol& parent) : m_parent(parent) {}

    public:
      LockLease(const LockLease&) = delete;
      LockLease(LockLease&&) = delete;

      ~LockLease() { m_parent.unlock(); }
    };

    void lock() { virt_lock(); }
    auto try_lock() -> bool { return virt_try_lock(); }
    void unlock() { virt_unlock(); }

    [[nodiscard]] auto lease() -> LockLease {
      lock();
      return {*this};
    }

    [[nodiscard]] auto critical_section(auto code) {
      auto critical_scope = lease();
      return code();
    }
  };
}  // namespace wesos::sync
