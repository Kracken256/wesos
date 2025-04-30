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
  template <class ValueGeneric, auto MostValue>
  class ClampMost {
    ValueGeneric m_value;

    constexpr ClampMost(ValueGeneric x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr ClampMost(ValueGeneric x) {
      if (x >= MostValue) {
        m_value = MostValue;
      } else {
        m_value = move(x);
      }
    };
    constexpr ClampMost(const ClampMost&) = default;
    constexpr ClampMost(ClampMost&&) = default;
    constexpr auto operator=(const ClampMost&) -> ClampMost& = default;
    constexpr auto operator=(ClampMost&&) -> ClampMost& = default;
    constexpr ~ClampMost() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> ClampMost {
      assert_invariant(x <= MostValue);
      return ClampMost(move(x), true);
    }
  };
}  // namespace wesos::types
