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
#include <wesos-types/OwnPtr.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class NullableOwnPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr NullableOwnPtr() : m_ptr(nullptr) {}
    constexpr NullableOwnPtr(Null) : m_ptr(nullptr) {}
    constexpr NullableOwnPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableOwnPtr(const NullableOwnPtr&) = default;
    constexpr NullableOwnPtr(NullableOwnPtr&& other) = default;
    constexpr auto operator=(const NullableOwnPtr&) -> NullableOwnPtr& = default;
    constexpr auto operator=(NullableOwnPtr&& other) -> NullableOwnPtr& = default;
    constexpr auto operator<=>(const NullableOwnPtr& other) const = default;
    constexpr ~NullableOwnPtr() = default;

    [[nodiscard]] constexpr auto isset() const -> bool { return m_ptr != nullptr; }

    [[nodiscard]] constexpr auto into_raw() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto get_unchecked() const -> OwnPtr<PointeeGeneric> { return m_ptr; }
    [[nodiscard]] constexpr auto get() const -> OwnPtr<PointeeGeneric> {
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
      return NullableOwnPtr(into_raw() + i.unwrap());
    }

    [[nodiscard]] constexpr auto sub(usize i) const {
      assert_invariant(isset(), "Subtracting from a null pointer");
      return NullableOwnPtr(into_raw() - i.unwrap());
    }
  };

  static_assert(sizeof(NullableOwnPtr<void*>) == sizeof(void*),
                "Size of NullableOwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
