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
  template <class ValueGeneric, auto LeastValue>
  class ClampLeast {
    ValueGeneric m_value;

    constexpr ClampLeast() = default;

  public:
    constexpr ClampLeast(ValueGeneric x) {
      if (x <= LeastValue) {
        m_value = LeastValue;
      } else {
        m_value = move(x);
      }
    };
    constexpr ClampLeast(const ClampLeast&) = default;
    constexpr ClampLeast(ClampLeast&&) = default;
    constexpr auto operator=(const ClampLeast&) -> ClampLeast& = default;
    constexpr auto operator=(ClampLeast&&) -> ClampLeast& = default;
    constexpr ~ClampLeast() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> ClampLeast {
      assert_invariant(x >= LeastValue);
      ClampLeast obj;
      obj.m_value = move(x);
      return obj;
    }
  };
}  // namespace wesos::types
