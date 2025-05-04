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
#include <wesos-types/PowerOfTwo.hh>
#include <wesos-types/RefPtr.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class NullableRefPtr;

  template <class PointeeGeneric>
  class NullableOwnPtr {
    PointeeGeneric* m_ptr = nullptr;

  public:
    constexpr NullableOwnPtr() = default;
    constexpr NullableOwnPtr(Null) {}
    constexpr NullableOwnPtr(nullptr_t) {}
    constexpr NullableOwnPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableOwnPtr(OwnPtr<PointeeGeneric> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr NullableOwnPtr(const NullableOwnPtr&) = default;
    constexpr NullableOwnPtr(NullableOwnPtr&&) = default;
    constexpr auto operator=(const NullableOwnPtr&) -> NullableOwnPtr& = default;
    constexpr auto operator=(NullableOwnPtr&&) -> NullableOwnPtr& = default;
    constexpr ~NullableOwnPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const NullableOwnPtr&) const = default;
    [[nodiscard]] constexpr operator PointeeGeneric*() { return unwrap(); }

    [[nodiscard]] constexpr auto isset() const -> bool { return unwrap() != nullptr; }
    [[nodiscard]] constexpr auto is_null() const -> bool { return !isset(); }
    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto as_uptr() const -> uptr { return bit_cast<uptr>(unwrap()); }

    [[nodiscard]] constexpr auto get() const -> OwnPtr<PointeeGeneric> {
      assert_always(isset());
      return unwrap();
    }

    [[nodiscard]] constexpr auto get_unchecked() const -> OwnPtr<PointeeGeneric> {
      assert_invariant(isset());
      return unwrap();
    }

    template <class U = PointeeGeneric>
    [[nodiscard]] constexpr auto operator->() const -> U* requires(!is_same_v<U, void>) {
      assert_invariant(isset());
      return unwrap();
    }

    template <class U = PointeeGeneric>
    [[nodiscard]] constexpr auto operator*() const -> U& requires(!is_same_v<U, void>) {
      assert_invariant(isset());
      return *unwrap();
    }

    [[nodiscard]] constexpr auto as_ref() const -> NullableRefPtr<PointeeGeneric> {
      return this->unwrap();
    }
  };

  static_assert(sizeof(NullableOwnPtr<void*>) == sizeof(void*),
                "Size of NullableOwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
