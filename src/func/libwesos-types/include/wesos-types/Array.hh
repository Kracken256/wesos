/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/View.hh>

namespace wesos::types {
  template <class ElementGeneric, auto CountGeneric>
  class Array {
    ElementGeneric m_data[CountGeneric];  // NOLINT(modernize-avoid-c-arrays)

  public:
    constexpr Array() { clear(); };
    constexpr Array(const Array& other) = default;
    constexpr Array(Array&& other) = default;
    constexpr auto operator=(const Array& other) -> Array& = default;
    constexpr auto operator=(Array&& other) -> Array& = default;
    constexpr auto operator<=>(const Array& other) const = default;
    constexpr ~Array() = default;

    [[nodiscard]] constexpr auto length() const -> usize { return CountGeneric; }
    [[nodiscard]] constexpr auto empty() const -> bool { return length() == 0; }

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

    [[nodiscard]] constexpr auto get(usize i) const -> const ElementGeneric& {
      always_assert(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) const -> const ElementGeneric& {
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get(usize i) -> ElementGeneric& {
      always_assert(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) -> ElementGeneric& { return m_data[i]; }

    constexpr void set(usize i, const ElementGeneric& value) {
      always_assert(i < length());
      m_data[i] = value;
    }

    constexpr void set_unchecked(usize i, const ElementGeneric& value) { m_data[i] = value; }

    constexpr auto front() const -> const ElementGeneric& {
      static_assert(CountGeneric > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    constexpr auto front() -> ElementGeneric& {
      static_assert(CountGeneric > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    constexpr auto back() const -> const ElementGeneric& {
      static_assert(CountGeneric > 0, "Cannot access back of an empty array");
      return m_data[CountGeneric - 1];
    }

    constexpr auto back() -> ElementGeneric& {
      static_assert(CountGeneric > 0, "Cannot access back of an empty array");
      return m_data[CountGeneric - 1];
    }

    constexpr auto cbegin() const -> const ElementGeneric* { return m_data; }
    constexpr auto cend() const -> const ElementGeneric* { return m_data + CountGeneric; }
    constexpr auto begin() -> ElementGeneric* { return m_data; }
    constexpr auto end() -> ElementGeneric* { return m_data + CountGeneric; }

    constexpr auto into_pointer() -> ElementGeneric* { return m_data; }

    constexpr auto as_view() { return View<ElementGeneric>(begin(), length()); }
  };
}  // namespace wesos::types
