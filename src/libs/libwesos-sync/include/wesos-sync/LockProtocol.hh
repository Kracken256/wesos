/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::sync {
#define W_LOCK_PROTOCOL_IMPLEMENT(LockClassName)           \
  class LockLease final {                                  \
    friend class LockClassName;                            \
    LockClassName& m_parent;                               \
                                                           \
    LockLease(LockClassName& parent) : m_parent(parent) {} \
                                                           \
  public:                                                  \
    LockLease(const LockLease&) = delete;                  \
    LockLease(LockLease&&) = delete;                       \
                                                           \
    ~LockLease() { m_parent.unlock(); }                    \
  };                                                       \
                                                           \
  [[nodiscard]] auto lease() -> LockLease {                \
    lock();                                                \
    return {*this};                                        \
  }                                                        \
                                                           \
  [[nodiscard]] auto critical_section(auto code) {         \
    auto critical_scope = lease();                         \
    return code();                                         \
  }
}  // namespace wesos::sync
