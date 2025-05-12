/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Bitcast.hh>
#include <wesos-types/Move.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/PowerOfTwo.hh>

namespace wesos::types {
  template <class Pointee>
  class RefPtr;

  template <class Pointee>
  class OwnPtr {
    Pointee* m_ptr;

  public:
    constexpr OwnPtr(Pointee* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr OwnPtr(const OwnPtr&) = default;
    constexpr OwnPtr(OwnPtr&&) = default;
    constexpr auto operator=(const OwnPtr&) -> OwnPtr& = default;
    constexpr auto operator=(OwnPtr&&) -> OwnPtr& = default;
    constexpr ~OwnPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const OwnPtr&) const = default;
    [[nodiscard]] constexpr operator Pointee*() { return unwrap(); }

    [[nodiscard]] constexpr auto unwrap() const -> Pointee* { return m_ptr; }
    [[nodiscard]] constexpr auto as_uptr() const -> uptr { return bit_cast<uptr>(unwrap()); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator->() const -> U* requires(!is_same_v<U, void>) { return unwrap(); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator*() const -> U& requires(!is_same_v<U, void>) { return *unwrap(); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto add(usize i) const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + i;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto sub(usize i) const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() - i;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto load() const -> U& requires(!is_same_v<U, void>) { return *unwrap(); }

    template <class U = Pointee>
    constexpr void store(U x) const
      requires(!is_same_v<U, void>)
    {
      *unwrap() = move(x);
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator++() const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator++(int) const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator--() const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() - 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator--(int) const -> OwnPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() - 1;
    }

    [[nodiscard]] constexpr auto as_ref() const -> RefPtr<Pointee> { return unwrap(); }
  };

  static_assert(sizeof(OwnPtr<void*>) == sizeof(void*), "Size of OwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
