/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::sync {
  class LockProtocol {
    bool m_locked = false;

  protected:
    virtual void virt_lock() = 0;
    virtual void virt_unlock() = 0;

  public:
    class LockLease final {
      friend class LockProtocol;
      LockProtocol& m_parent;

      LockLease(LockProtocol& parent) : m_parent(parent) {}

    public:
      LockLease(const LockLease&) = delete;
      LockLease(LockLease&&) = delete;

      ~LockLease() { m_parent.unlock(); }
    };

    void lock() {
      virt_lock();
      m_locked = true;
    }

    void unlock() {
      m_locked = false;
      virt_unlock();
    }

    [[nodiscard]] auto is_locked() const -> bool { return m_locked; }

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
