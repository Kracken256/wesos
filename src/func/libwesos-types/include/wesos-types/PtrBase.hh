/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/PowerOfTwo.hh>

namespace wesos::types {
  template <class PointeeGeneric, class DerivedGeneric>
  class PtrBase {
    PointeeGeneric* m_ptr;

  public:
    constexpr PtrBase(PointeeGeneric* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr PtrBase(const PtrBase&) = default;
    constexpr PtrBase(PtrBase&&) = default;
    constexpr auto operator=(const PtrBase&) -> PtrBase& = default;
    constexpr auto operator=(PtrBase&&) -> PtrBase& = default;
    constexpr ~PtrBase() = default;

    [[nodiscard]] constexpr auto operator<=>(const PtrBase&) const = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }
    [[nodiscard]] constexpr auto into_uptr() const -> uptr {
      return reinterpret_cast<uptr>(unwrap());
    }

    [[nodiscard]] constexpr auto is_aligned(usize x) -> bool { return into_uptr() % x == 0; }

    [[nodiscard]] constexpr auto next_aligned_pow2(PowerOfTwo<usize> x) -> DerivedGeneric {
      const auto ptr = into_uptr();
      const auto aligned_ptr = (ptr + x - 1) & ~(x - 1);
      return reinterpret_cast<PointeeGeneric*>(aligned_ptr);
    }

    [[nodiscard]] constexpr auto next_aligned(usize x) -> DerivedGeneric {
      assert_invariant(x != 0);

      const auto ptr = into_uptr();
      const auto aligned_ptr = ptr + ((x - (ptr % x)) % x);
      return reinterpret_cast<PointeeGeneric*>(aligned_ptr);
    }

    [[nodiscard]] constexpr auto add(usize i) const -> DerivedGeneric { return unwrap() + i; }
    [[nodiscard]] constexpr auto sub(usize i) const -> DerivedGeneric { return unwrap() - i; }

    [[nodiscard]] constexpr auto operator++() const -> DerivedGeneric { return unwrap() + 1; }
    [[nodiscard]] constexpr auto operator--() const -> DerivedGeneric { return unwrap() - 1; }
    [[nodiscard]] constexpr auto operator++(int) const -> DerivedGeneric { return unwrap() + 1; }
    [[nodiscard]] constexpr auto operator--(int) const -> DerivedGeneric { return unwrap() - 1; }
  };
}  // namespace wesos::types
