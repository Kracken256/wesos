/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Move.hh>
#include <wesos-types/Bitcast.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/OwnPtr.hh>
#include <wesos-types/PowerOfTwo.hh>

namespace wesos::types {
  template <class Pointee>
  class RefPtr {
    Pointee* m_ptr;

  public:
    constexpr RefPtr(Pointee* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr RefPtr(OwnPtr<Pointee> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr RefPtr(const RefPtr&) = default;
    constexpr RefPtr(RefPtr&&) = default;
    constexpr auto operator=(const RefPtr&) -> RefPtr& = default;
    constexpr auto operator=(RefPtr&&) -> RefPtr& = default;
    constexpr ~RefPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const RefPtr&) const = default;
    [[nodiscard]] constexpr operator Pointee*() { return unwrap(); }

    [[nodiscard]] constexpr auto unwrap() const -> Pointee* { return m_ptr; }
    [[nodiscard]] constexpr auto as_uptr() const -> uptr { return bit_cast<uptr>(unwrap()); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator->() const -> U* requires(!is_same_v<U, void>) { return unwrap(); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator*() const -> U& requires(!is_same_v<U, void>) { return *unwrap(); }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto add(usize i) const -> RefPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + i;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto sub(usize i) const -> RefPtr
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
    [[nodiscard]] constexpr auto operator++() const -> RefPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator++(int) const -> RefPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() + 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator--() const -> RefPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() - 1;
    }

    template <class U = Pointee>
    [[nodiscard]] constexpr auto operator--(int) const -> RefPtr
      requires(!is_same_v<U, void>)
    {
      return unwrap() - 1;
    }
  };

  static_assert(sizeof(RefPtr<void*>) == sizeof(void*), "Size of RefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
