/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>

namespace wesos::types {
  template <class T>
  class Untrusted {
    T m_value;

  public:
    constexpr Untrusted() = default;
    constexpr Untrusted(T value) : m_value(move(value)) {}
    constexpr Untrusted(const Untrusted&) = default;
    constexpr Untrusted(Untrusted&&) = default;
    constexpr auto operator=(const Untrusted&) -> Untrusted& = default;
    constexpr auto operator=(Untrusted&&) -> Untrusted& = default;
    constexpr ~Untrusted() = default;

    [[nodiscard]] constexpr auto operator<=>(const Untrusted&) const = default;

    [[nodiscard]] constexpr auto trust_and_unwrap() const -> const T& { return m_value; }
    [[nodiscard]] constexpr auto trust_and_unwrap() -> T& { return m_value; }

    constexpr void ensure_safe(auto verify) const { assert_always(verify(m_value)); }
  };
}  // namespace wesos::types
