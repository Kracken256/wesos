/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Move.hh>

namespace wesos::types {
  template <class T, auto Minimum>
  class Least {
    T m_value;

    constexpr Least(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr Least(T x) : m_value(move(x)) { assert_invariant(x >= Minimum); };
    constexpr Least(const Least&) = default;
    constexpr Least(Least&&) = default;
    constexpr auto operator=(const Least&) -> Least& = default;
    constexpr auto operator=(Least&&) -> Least& = default;
    constexpr ~Least() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> Least {
      assert_invariant(x >= Minimum);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
