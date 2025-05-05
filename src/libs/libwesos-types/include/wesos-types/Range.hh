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
  template <class T, auto Minimum, auto Maximum>
  class Range {
    T m_value;

    constexpr Range(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr Range(T x) : m_value(move(x)) { assert_invariant(x >= Minimum && x <= Maximum); };
    constexpr Range(const Range&) = default;
    constexpr Range(Range&&) = default;
    constexpr auto operator=(const Range&) -> Range& = default;
    constexpr auto operator=(Range&&) -> Range& = default;
    constexpr ~Range() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> Range {
      assert_invariant(x >= Minimum && x <= Maximum);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
