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

    constexpr Least() = default;

  public:
    constexpr Least(ValueGeneric x) : m_value(move(x)) { assert_invariant(x >= MinimumValue); };
    constexpr Least(const Least&) = default;
    constexpr Least(Least&&) = default;
    constexpr auto operator=(const Least&) -> Least& = default;
    constexpr auto operator=(Least&&) -> Least& = default;
    constexpr ~Least() = default;

    template <class RGeneric, auto RMinimum>
    [[nodiscard]] constexpr auto operator<=>(const Least<RGeneric, RMinimum>& o) const {
      return unwrap() <=> o.unwrap();
    }

    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }

    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> Least {
      assert_invariant(x >= MinimumValue);
      Least obj;
      obj.m_value = move(x);
      return obj;
    }
  };
}  // namespace wesos::types
