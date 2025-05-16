/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::sync {
#define W_LOCK_PROTOCOL_IMPLEMENT(LockClassName)                                                \
  class AutoUnlock final {                                                                      \
    friend class LockClassName;                                                                 \
    LockClassName& m_parent;                                                                    \
                                                                                                \
    AutoUnlock(LockClassName& parent) : m_parent(parent) {}                                     \
                                                                                                \
  public:                                                                                       \
    AutoUnlock(const AutoUnlock&) = delete;                                                     \
    AutoUnlock(AutoUnlock&&) = default;                                                         \
                                                                                                \
    ~AutoUnlock() { m_parent.unlock(); }                                                        \
  };                                                                                            \
                                                                                                \
  [[nodiscard]] auto lease() -> AutoUnlock {                                                    \
    lock();                                                                                     \
    return {*this};                                                                             \
  }                                                                                             \
                                                                                                \
  [[nodiscard]] auto try_lease() -> Nullable<AutoUnlock> {                                      \
    if (!try_lock()) {                                                                          \
      return null;                                                                              \
    }                                                                                           \
                                                                                                \
    return AutoUnlock(*this);                                                                   \
  }                                                                                             \
                                                                                                \
  auto critical_section(const auto& code) {                                                     \
    lock();                                                                                     \
    auto _ = defer([&] { unlock(); });                                                          \
    return code();                                                                              \
  }                                                                                             \
                                                                                                \
  template <class Callable>                                                                     \
  [[nodiscard]] auto try_critical_section(const Callable& code) -> Nullable<decltype(code())> { \
    if (!try_lock()) {                                                                          \
      return null;                                                                              \
    }                                                                                           \
    auto _ = defer([&] { unlock(); });                                                          \
    return code();                                                                              \
  }
}  // namespace wesos::sync
