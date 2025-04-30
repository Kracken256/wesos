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
  template <class ValueGeneric>
  class PowerOfTwo {
    ValueGeneric m_value;

    constexpr PowerOfTwo() = default;

    static constexpr auto is_power_of_two(auto x) { return x > 0 && ((x & (x - 1)) == 0); }

  public:
    constexpr PowerOfTwo(ValueGeneric x) : m_value(move(x)) {
      assert_invariant(is_power_of_two(x));
    };
    constexpr PowerOfTwo(const PowerOfTwo&) = default;
    constexpr PowerOfTwo(PowerOfTwo&&) = default;
    constexpr auto operator=(const PowerOfTwo&) -> PowerOfTwo& = default;
    constexpr auto operator=(PowerOfTwo&&) -> PowerOfTwo& = default;
    constexpr ~PowerOfTwo() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const {
      return unwrap() <=> o.unwrap();
    }

    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }

    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> PowerOfTwo {
      assert_invariant(is_power_of_two(x));

      PowerOfTwo obj;
      obj.m_value = move(x);
      return obj;
    }
  };
}  // namespace wesos::types
