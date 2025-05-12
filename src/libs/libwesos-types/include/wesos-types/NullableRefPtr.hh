/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Bitcast.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/OwnPtr.hh>
#include <wesos-types/PowerOfTwo.hh>
#include <wesos-types/RefPtr.hh>

namespace wesos::types {
  template <class Pointee>
  class NullableOwnPtr;

  template <class Pointee>
  class NullableRefPtr {
    Pointee* m_ptr = nullptr;

  public:
    constexpr NullableRefPtr() = default;
    constexpr NullableRefPtr(Null) {}
    constexpr NullableRefPtr(nullptr_t) {}
    constexpr NullableRefPtr(Pointee* ptr) : m_ptr(ptr) {}
    constexpr NullableRefPtr(RefPtr<Pointee> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr NullableRefPtr(OwnPtr<Pointee> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr NullableRefPtr(const NullableRefPtr&) = default;
    constexpr NullableRefPtr(NullableRefPtr&&) = default;
    constexpr auto operator=(const NullableRefPtr&) -> NullableRefPtr& = default;
    constexpr auto operator=(NullableRefPtr&&) -> NullableRefPtr& = default;
    constexpr ~NullableRefPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const NullableRefPtr&) const = default;
    [[nodiscard]] constexpr operator Pointee*() const { return unwrap(); }
    [[nodiscard]] constexpr operator bool() const { return isset(); }

    [[nodiscard]] constexpr auto isset() const -> bool { return unwrap() != nullptr; }
    [[nodiscard]] constexpr auto is_null() const -> bool { return !isset(); }
    [[nodiscard]] constexpr auto unwrap() const -> Pointee* { return m_ptr; }
    [[nodiscard]] constexpr auto as_uptr() const -> uptr { return bit_cast<uptr>(unwrap()); }

    [[nodiscard]] constexpr auto get() const -> RefPtr<Pointee> {
      assert_always(isset());
      return unwrap();
    }

    [[nodiscard]] constexpr auto get_unchecked() const -> RefPtr<Pointee> {
      assert_invariant(isset());
      return unwrap();
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator->() const -> U* requires(!is_same_v<U, void>) {
      assert_invariant(isset());
      return unwrap();
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator*() const -> U& requires(!is_same_v<U, void>) {
      assert_invariant(isset());
      return *unwrap();
    }
  };

  static_assert(sizeof(NullableRefPtr<void*>) == sizeof(void*),
                "Size of NullableRefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
