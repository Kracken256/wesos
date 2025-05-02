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
  template <class ValueGeneric, auto MinimumValue, auto MaximumValue>
  class Range {
    ValueGeneric m_value;

    constexpr Range(ValueGeneric x, bool unsafe) : m_value(move(x)) { (void)unsafe; };

  public:
    constexpr Range(ValueGeneric x) : m_value(move(x)) {
      assert_invariant(x >= MinimumValue && x <= MaximumValue);
    };
    constexpr Range(const Range&) = default;
    constexpr Range(Range&&) = default;
    constexpr auto operator=(const Range&) -> Range& = default;
    constexpr auto operator=(Range&&) -> Range& = default;
    constexpr ~Range() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return unwrap() <=> o; }
    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& { return m_value; }
    [[nodiscard]] constexpr auto operator->() const -> const ValueGeneric* { return &m_value; }
    [[nodiscard]] constexpr operator ValueGeneric() const { return m_value; }

    [[nodiscard]] constexpr static auto create_unchecked(ValueGeneric x) -> Range {
      assert_invariant(x >= MinimumValue && x <= MaximumValue);
      return {move(x), true};
    }
  };
}  // namespace wesos::types
