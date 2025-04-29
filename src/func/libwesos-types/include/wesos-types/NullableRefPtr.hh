/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/RefPtr.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class NullableOwnPtr;

  template <typename PointeeGeneric>
  class NullableRefPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr NullableRefPtr() : m_ptr(nullptr) {}
    constexpr NullableRefPtr(Null) : m_ptr(nullptr) {}
    constexpr NullableRefPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableRefPtr(const NullableRefPtr&) = default;
    constexpr NullableRefPtr(NullableRefPtr&& other) = default;
    constexpr auto operator=(const NullableRefPtr&) -> NullableRefPtr& = default;
    constexpr auto operator=(NullableRefPtr&& other) -> NullableRefPtr& = default;
    constexpr auto operator<=>(const NullableRefPtr& other) const = default;
    constexpr ~NullableRefPtr() = default;

    [[nodiscard]] constexpr auto isset() const -> bool { return m_ptr != nullptr; }

    [[nodiscard]] constexpr auto into_raw() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto get_unchecked() const -> RefPtr<PointeeGeneric> { return m_ptr; }
    [[nodiscard]] constexpr auto get() const -> RefPtr<PointeeGeneric> {
      always_assert(isset(), "Unwrapping a null pointer");
      return m_ptr;
    }

    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* {
      assert_invariant(isset(), "Dereferencing a null pointer");
      return m_ptr;
    }

    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& {
      assert_invariant(isset(), "Dereferencing a null pointer");
      return *m_ptr;
    }

    [[nodiscard]] constexpr auto add(usize i) const {
      assert_invariant(isset(), "Adding to a null pointer");
      return NullableRefPtr(into_raw() + i.unwrap());
    }

    [[nodiscard]] constexpr auto sub(usize i) const {
      assert_invariant(isset(), "Subtracting from a null pointer");
      return NullableRefPtr(into_raw() - i.unwrap());
    }

    [[nodiscard]] constexpr auto take_own() const { return NullableOwnPtr(into_raw()); }
  };

  static_assert(sizeof(NullableRefPtr<void*>) == sizeof(void*),
                "Size of NullableRefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
