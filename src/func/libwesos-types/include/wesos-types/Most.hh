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
  template <class ValueGeneric, auto MaximumValue>
  class Most {
    ValueGeneric m_value;

    constexpr Most(ValueGeneric x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr Most(ValueGeneric x) : m_value(move(x)) { assert_invariant(x <= MaximumValue); };
    constexpr Most(const Most&) = default;
    constexpr Most(Most&&) = default;
    constexpr auto operator=(const Most&) -> Most& = default;
    constexpr auto operator=(Most&&) -> Most& = default;
    constexpr ~Most() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const ValueGeneric* { return &m_value; }
    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> Most {
      assert_invariant(x <= MaximumValue);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
