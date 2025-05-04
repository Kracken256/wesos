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
  template <class T, auto Maximum>
  class ClampMost {
    T m_value;

    constexpr ClampMost(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

    [[nodiscard]] static constexpr auto clamp(T x) -> T { return x > Maximum ? T(Maximum) : x; }

  public:
    constexpr ClampMost(T x) : m_value(clamp(move(x))){};
    constexpr ClampMost(const ClampMost&) = default;
    constexpr ClampMost(ClampMost&&) = default;
    constexpr auto operator=(const ClampMost&) -> ClampMost& = default;
    constexpr auto operator=(ClampMost&&) -> ClampMost& = default;
    constexpr ~ClampMost() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> ClampMost {
      assert_invariant(x <= Maximum);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
