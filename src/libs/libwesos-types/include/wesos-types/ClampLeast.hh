/*
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
  class ClampLeast {
    T m_value;

    constexpr ClampLeast(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

    [[nodiscard]] static constexpr auto clamp(T x) -> T { return x < Minimum ? T(Minimum) : x; }

  public:
    constexpr ClampLeast(T x) : m_value(clamp(move(x))){};
    constexpr ClampLeast(const ClampLeast&) = default;
    constexpr ClampLeast(ClampLeast&&) = default;
    constexpr auto operator=(const ClampLeast&) -> ClampLeast& = default;
    constexpr auto operator=(ClampLeast&&) -> ClampLeast& = default;
    constexpr ~ClampLeast() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> ClampLeast {
      assert_invariant(x >= Minimum);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
