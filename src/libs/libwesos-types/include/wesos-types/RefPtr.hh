/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Move.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/OwnPtr.hh>
#include <wesos-types/PowerOfTwo.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class OwnPtr;

  template <class PointeeGeneric>
  class RefPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr RefPtr(PointeeGeneric* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr RefPtr(OwnPtr<PointeeGeneric> ptr) : m_ptr(ptr.unwrap()) {}
    constexpr RefPtr(const RefPtr&) = default;
    constexpr RefPtr(RefPtr&&) = default;
    constexpr auto operator=(const RefPtr&) -> RefPtr& = default;
    constexpr auto operator=(RefPtr&&) -> RefPtr& = default;
    constexpr ~RefPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const RefPtr&) const = default;
    [[nodiscard]] constexpr operator PointeeGeneric*() { return unwrap(); }

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto into_uptr() const -> uptr {
      return reinterpret_cast<uptr>(unwrap());
    }

    [[nodiscard]] constexpr auto is_aligned(usize x) const -> bool { return into_uptr() % x == 0; }
    [[nodiscard]] constexpr auto is_aligned_pow2(PowerOfTwo<usize> x) const -> bool {
      return (into_uptr() & (x.unwrap() - 1)) == 0;
    }

    [[nodiscard]] constexpr auto align_pow2(PowerOfTwo<usize> x) const -> RefPtr {
      const auto ptr = into_uptr();
      const auto align_ptr = (ptr + x - 1) & -x;
      return reinterpret_cast<PointeeGeneric*>(align_ptr);
    }

    [[nodiscard]] constexpr auto align(usize x) const -> RefPtr {
      assert_invariant(x != 0);
      const auto ptr = into_uptr();
      const auto align_ptr = ptr + ((x - (ptr % x)) % x);
      return reinterpret_cast<PointeeGeneric*>(align_ptr);
    }

    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return unwrap(); }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *unwrap(); }

    [[nodiscard]] constexpr auto add(usize i) const -> RefPtr { return unwrap() + i; }
    [[nodiscard]] constexpr auto sub(usize i) const -> RefPtr { return unwrap() - i; }

    [[nodiscard]] constexpr auto load() const -> PointeeGeneric& { return *unwrap(); }
    constexpr void store(PointeeGeneric x) const { *unwrap() = move(x); }

    [[nodiscard]] constexpr auto operator++() const -> RefPtr { return unwrap() + 1; }
    [[nodiscard]] constexpr auto operator++(int) const -> RefPtr { return unwrap() + 1; }
    [[nodiscard]] constexpr auto operator--() const -> RefPtr { return unwrap() - 1; }
    [[nodiscard]] constexpr auto operator--(int) const -> RefPtr { return unwrap() - 1; }
  };

  static_assert(sizeof(RefPtr<void*>) == sizeof(void*),
                "Size of RefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
