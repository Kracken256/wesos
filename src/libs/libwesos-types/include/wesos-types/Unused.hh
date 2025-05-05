/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>

namespace wesos::types {
  template <class T>
  class Unused {
    T m_value;

  public:
    constexpr Unused() = default;
    constexpr Unused(T x) : m_value(move(x)){};
    constexpr Unused(const Unused&) = default;
    constexpr Unused(Unused&&) = default;
    constexpr auto operator=(const Unused&) -> Unused& = default;
    constexpr auto operator=(Unused&&) -> Unused& = default;
    constexpr ~Unused() = default;

    [[nodiscard]] constexpr auto operator<=>(const Unused& o) const = default;
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }
  };
}  // namespace wesos::types
