/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Move.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/RefPtr.hh>
#include <wesos-types/View.hh>

namespace wesos::types {
  template <class ElementGeneric, auto CountGeneric>
  class Array {
    ElementGeneric m_data[CountGeneric];  // NOLINT(modernize-avoid-c-arrays)

  public:
    constexpr Array() { clear(); };
    constexpr Array(const Array&) = default;
    constexpr Array(Array&&) = default;
    constexpr auto operator=(const Array&) -> Array& = default;
    constexpr auto operator=(Array&&) -> Array& = default;
    constexpr ~Array() = default;

    [[nodiscard]] constexpr auto operator<=>(const Array&) const = default;
    [[nodiscard]] constexpr auto length() const -> usize { return CountGeneric; }
    [[nodiscard]] constexpr auto empty() const -> bool { return length() == 0; }

    [[nodiscard]] constexpr auto get(usize i) const -> const ElementGeneric& {
      always_assert(i < length(), "out of bounds");
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) const -> const ElementGeneric& {
      assert_invariant(i < length(), "out of bounds");
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get(usize i) -> ElementGeneric& {
      always_assert(i < length(), "out of bounds");
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) -> ElementGeneric& {
      assert_invariant(i < length(), "out of bounds");
      return m_data[i];
    }

    [[nodiscard]] constexpr auto front() const -> const ElementGeneric& {
      static_assert(length() > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    [[nodiscard]] constexpr auto front() -> ElementGeneric& {
      static_assert(length() > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    [[nodiscard]] constexpr auto back() const -> const ElementGeneric& {
      static_assert(length() > 0, "Cannot access back of an empty array");
      return m_data[length() - 1];
    }

    [[nodiscard]] constexpr auto back() -> ElementGeneric& {
      static_assert(length() > 0, "Cannot access back of an empty array");
      return m_data[length() - 1];
    }

    [[nodiscard]] constexpr auto cbegin() const -> const ElementGeneric* { return m_data; }
    [[nodiscard]] constexpr auto cend() const -> const ElementGeneric* { return m_data + length(); }
    [[nodiscard]] constexpr auto begin() -> ElementGeneric* { return m_data; }
    [[nodiscard]] constexpr auto end() -> ElementGeneric* { return m_data + length(); }

    [[nodiscard]] constexpr auto into_ptr() -> RefPtr<ElementGeneric> { return m_data; }
    [[nodiscard]] constexpr auto as_view() { return View<ElementGeneric>(into_ptr(), length()); }

    ///=========================================================================

    constexpr void set(usize i, ElementGeneric value) {
      always_assert(i < length(), "out of bounds");
      m_data[i] = move(value);
    }

    constexpr void set_unchecked(usize i, ElementGeneric value) {
      assert_invariant(i < length(), "out of bounds");
      m_data[i] = move(value);
    }

    constexpr void clear() {
      for (auto& element : m_data) {
        element = ElementGeneric{};
      }
    }

    constexpr void fill(const ElementGeneric& value) {
      for (auto& element : m_data) {
        element = value;
      }
    }
  };
}  // namespace wesos::types
