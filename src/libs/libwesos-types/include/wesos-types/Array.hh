/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Move.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/RefPtr.hh>
#include <wesos-types/View.hh>

namespace wesos::types {
  template <class T, auto Count>
  class Array {
    T m_data[Count];  // NOLINT(modernize-avoid-c-arrays)

  public:
    constexpr Array() { clear(); };
    constexpr Array(const Array&) = default;
    constexpr Array(Array&&) = default;
    constexpr auto operator=(const Array&) -> Array& = default;
    constexpr auto operator=(Array&&) -> Array& = default;
    constexpr ~Array() = default;

    [[nodiscard]] constexpr auto operator<=>(const Array&) const = default;
    [[nodiscard]] constexpr auto length() const -> usize { return Count; }
    [[nodiscard]] constexpr auto empty() const -> bool { return length() == 0; }

    [[nodiscard]] constexpr auto get(usize i) const -> const T& {
      assert_always(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) const -> const T& {
      assert_invariant(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get(usize i) -> T& {
      assert_always(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) -> T& {
      assert_invariant(i < length());
      return m_data[i];
    }

    [[nodiscard]] constexpr auto front() const -> const T& {
      static_assert(Count > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    [[nodiscard]] constexpr auto front() -> T& {
      static_assert(Count > 0, "Cannot access front of an empty array");
      return m_data[0];
    }

    [[nodiscard]] constexpr auto back() const -> const T& {
      static_assert(Count > 0, "Cannot access back of an empty array");
      return m_data[length() - 1];
    }

    [[nodiscard]] constexpr auto back() -> T& {
      static_assert(Count > 0, "Cannot access back of an empty array");
      return m_data[length() - 1];
    }

    [[nodiscard]] constexpr auto cbegin() const -> const T* { return m_data; }
    [[nodiscard]] constexpr auto cend() const -> const T* { return m_data + length(); }
    [[nodiscard]] constexpr auto begin() -> T* { return m_data; }
    [[nodiscard]] constexpr auto end() -> T* { return m_data + length(); }

    [[nodiscard]] constexpr auto into_ptr() -> RefPtr<T> { return m_data; }
    [[nodiscard]] constexpr auto as_view() { return View<T>(into_ptr().unwrap(), length()); }

    ///=========================================================================

    constexpr void set(usize i, T value) {
      assert_always(i < length());
      m_data[i] = move(value);
    }

    constexpr void set_unchecked(usize i, T value) {
      assert_invariant(i < length());
      m_data[i] = move(value);
    }

    constexpr void clear() {
      for (auto& element : m_data) {
        element = T{};
      }
    }

    constexpr void fill(const T& value) {
      for (auto& element : m_data) {
        element = value;
      }
    }
  };
}  // namespace wesos::types
