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

    constexpr ClampLeast(ValueGeneric x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

    [[nodiscard]] static constexpr auto clamp(ValueGeneric x) -> ValueGeneric {
      return x < LeastValue ? ValueGeneric(LeastValue) : x;
    }

  public:
    constexpr ClampLeast(ValueGeneric x) : m_value(clamp(move(x))){};
    constexpr ClampLeast(const ClampLeast&) = default;
    constexpr ClampLeast(ClampLeast&&) = default;
    constexpr auto operator=(const ClampLeast&) -> ClampLeast& = default;
    constexpr auto operator=(ClampLeast&&) -> ClampLeast& = default;
    constexpr ~ClampLeast() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const ValueGeneric* { return &m_value; }
    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> ClampLeast {
      assert_invariant(x >= LeastValue);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
