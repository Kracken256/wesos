/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types {
  template <class ValueGeneric>
  class Untrusted {
    ValueGeneric m_value;

  public:
    constexpr Untrusted(const ValueGeneric& value) : m_value(value) {}
    constexpr Untrusted(const Untrusted&) = default;
    constexpr Untrusted(Untrusted&&) = default;
    constexpr auto operator=(const Untrusted&) -> Untrusted& = default;
    constexpr auto operator=(Untrusted&&) -> Untrusted& = default;
    constexpr auto operator<=>(const Untrusted&) const = default;
    constexpr ~Untrusted() = default;

    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr auto unwrap() -> ValueGeneric& { return m_value; }
  };
}  // namespace wesos::types
