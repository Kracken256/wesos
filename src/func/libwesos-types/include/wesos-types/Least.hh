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
  template <class ValueGeneric, auto MinimumValue>
  class Least {
    ValueGeneric m_value;

  public:
    constexpr Least(ValueGeneric x) : m_value(move(x)) { assert_invariant(x >= MinimumValue); };
    constexpr Least(const Least&) = default;
    constexpr Least(Least&&) = default;
    constexpr auto operator=(const Least&) -> Least& = default;
    constexpr auto operator=(Least&&) -> Least& = default;
    constexpr ~Least() = default;

    [[nodiscard]] constexpr auto operator<=>(const Least&) const = default;
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }

    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> Least {
      assert_invariant(x >= MinimumValue);
      return Least{.m_value = move(x)};
    }
  };
}  // namespace wesos::types
