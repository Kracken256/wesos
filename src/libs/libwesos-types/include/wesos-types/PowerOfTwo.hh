/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Move.hh>

namespace wesos::types {
  template <class T>
  class PowerOfTwo {
    T m_value;

    constexpr PowerOfTwo(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

    static constexpr auto is_power_of_two(auto x) {
      return x > static_cast<T>(0) && ((x & (x - static_cast<T>(1))) == static_cast<T>(0));
    }

  public:
    constexpr PowerOfTwo(T x) : m_value(move(x)) { assert_invariant(is_power_of_two(x)); };
    constexpr PowerOfTwo(const PowerOfTwo&) = default;
    constexpr PowerOfTwo(PowerOfTwo&&) = default;
    constexpr auto operator=(const PowerOfTwo&) -> PowerOfTwo& = default;
    constexpr auto operator=(PowerOfTwo&&) -> PowerOfTwo& = default;
    constexpr ~PowerOfTwo() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr auto next() const -> PowerOfTwo { return unwrap() * T(2); }
    [[nodiscard]] constexpr auto prev() const -> PowerOfTwo { return unwrap() / T(2); }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> PowerOfTwo {
      assert_invariant(is_power_of_two(x));
      return PowerOfTwo(move(x), true);
    }
  };
}  // namespace wesos::types
