/**
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
  template <typename PointeeGeneric>
  class NullableOwnPtr;

  template <class PointeeGeneric>
  class NullableRefPtr {
    PointeeGeneric* m_ptr = nullptr;

  public:
    constexpr NullableRefPtr() = default;
    constexpr NullableRefPtr(Null) {}
    constexpr NullableRefPtr(nullptr_t) {}
    constexpr NullableRefPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableRefPtr(RefPtr<PointeeGeneric> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr NullableRefPtr(OwnPtr<PointeeGeneric> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr NullableRefPtr(const NullableRefPtr&) = default;
    constexpr NullableRefPtr(NullableRefPtr&&) = default;
    constexpr auto operator=(const NullableRefPtr&) -> NullableRefPtr& = default;
    constexpr auto operator=(NullableRefPtr&&) -> NullableRefPtr& = default;
    constexpr ~NullableRefPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const NullableRefPtr&) const = default;
    [[nodiscard]] constexpr operator PointeeGeneric*() { return unwrap(); }

    [[nodiscard]] constexpr auto isset() const -> bool { return unwrap() != nullptr; }
    [[nodiscard]] constexpr auto is_null() const -> bool { return !isset(); }
    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto into_uptr() const -> uptr { return bit_cast<uptr>(unwrap()); }

    [[nodiscard]] constexpr auto align_pow2(PowerOfTwo<usize> x) const -> NullableRefPtr {
      const auto ptr = into_uptr();
      const auto align_ptr = (ptr + x - 1) & -x;
      return bit_cast<PointeeGeneric*>(align_ptr);
    }

    [[nodiscard]] constexpr auto align(usize x) const -> NullableRefPtr {
      assert_invariant(x != 0);
      const auto ptr = into_uptr();
      const auto align_ptr = ptr + ((x - (ptr % x)) % x);
      return bit_cast<PointeeGeneric*>(align_ptr);
    }

    [[nodiscard]] constexpr auto get() const -> RefPtr<PointeeGeneric> {
      always_assert(isset());
      return unwrap();
    }

    [[nodiscard]] constexpr auto get_unchecked() const -> RefPtr<PointeeGeneric> {
      assert_invariant(isset());
      return unwrap();
    }

    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* {
      assert_invariant(isset());
      return unwrap();
    }

    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& {
      assert_invariant(isset());
      return *unwrap();
    }

    [[nodiscard]] constexpr auto add(usize i) const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() + i;
    }

    [[nodiscard]] constexpr auto sub(usize i) const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() - i;
    }

    [[nodiscard]] constexpr auto operator++() const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() + 1;
    }

    [[nodiscard]] constexpr auto operator++(int) const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() + 1;
    }

    [[nodiscard]] constexpr auto operator--() const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() - 1;
    }

    [[nodiscard]] constexpr auto operator--(int) const -> NullableRefPtr {
      assert_invariant(isset());
      return unwrap() - 1;
    }
  };

  static_assert(sizeof(NullableRefPtr<void*>) == sizeof(void*),
                "Size of NullableRefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
