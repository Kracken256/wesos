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
  class Most {
    T m_value;

    constexpr Most(T x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr Most(T x) : m_value(move(x)) { assert_invariant(x <= Maximum); };
    constexpr Most(const Most&) = default;
    constexpr Most(Most&&) = default;
    constexpr auto operator=(const Most&) -> Most& = default;
    constexpr auto operator=(Most&&) -> Most& = default;
    constexpr ~Most() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const T* { return &m_value; }
    [[nodiscard]] constexpr operator T() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(T x) -> Most {
      assert_invariant(x <= Maximum);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
