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

  public:
    constexpr Most(ValueGeneric x) : m_value(move(x)) { always_assert(x <= MaximumValue); };
    constexpr Most(const Most&) = default;
    constexpr Most(Most&&) = default;
    constexpr auto operator=(const Most&) -> Most& = default;
    constexpr auto operator=(Most&&) -> Most& = default;
    constexpr ~Most() = default;

    [[nodiscard]] constexpr auto operator<=>(const Most&) const = default;
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }

    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> Most {
      assert_invariant(x <= MaximumValue);
      return Most{.m_value = move(x)};
    }
  };
}  // namespace wesos::types
