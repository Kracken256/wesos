/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>

namespace wesos::types {
  template <class T>
  class ThreadSafe {
    T m_value;

  public:
    constexpr ThreadSafe() = default;
    constexpr ThreadSafe(T x) : m_value(move(x)){};
    constexpr ThreadSafe(const ThreadSafe&) = default;
    constexpr ThreadSafe(ThreadSafe&&) = default;
    constexpr auto operator=(const ThreadSafe&) -> ThreadSafe& = default;
    constexpr auto operator=(ThreadSafe&&) -> ThreadSafe& = default;
    constexpr ~ThreadSafe() = default;

    [[nodiscard]] constexpr auto operator<=>(const ThreadSafe& o) const = default;
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }
  };
}  // namespace wesos::types
