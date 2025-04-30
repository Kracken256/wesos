/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <class PointeeGeneric, class ChildGeneric, class UnwrappedGeneric>
  class NullablePtrBase {
    PointeeGeneric* m_ptr = nullptr;

  public:
    constexpr NullablePtrBase() = default;
    constexpr NullablePtrBase(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullablePtrBase(const NullablePtrBase&) = default;
    constexpr NullablePtrBase(NullablePtrBase&&) = default;
    constexpr auto operator=(const NullablePtrBase&) -> NullablePtrBase& = default;
    constexpr auto operator=(NullablePtrBase&&) -> NullablePtrBase& = default;
    constexpr ~NullablePtrBase() = default;

    [[nodiscard]] constexpr auto operator<=>(const NullablePtrBase&) const = default;
    [[nodiscard]] constexpr auto isset() const -> bool { return m_ptr != nullptr; }
    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr operator PointeeGeneric*() const { return m_ptr; }
    [[nodiscard]] constexpr auto into_uptr() const -> uptr {
      return reinterpret_cast<uptr>(unwrap());
    }

    [[nodiscard]] constexpr auto is_aligned(usize x) -> bool { return into_uptr() % x == 0; }

    [[nodiscard]] constexpr auto next_aligned_pow2(usize x) -> ChildGeneric {
      const auto ptr = into_uptr();
      const auto aligned_ptr = (ptr + x - 1) & ~(x - 1);
      return reinterpret_cast<PointeeGeneric*>(aligned_ptr);
    }

    [[nodiscard]] constexpr auto next_align(usize x) -> ChildGeneric {
      const auto ptr = into_uptr();
      const auto aligned_ptr = ptr + ((x - (ptr % x)) % x);
      return reinterpret_cast<PointeeGeneric*>(aligned_ptr);
    }

    [[nodiscard]] constexpr auto get() const -> UnwrappedGeneric {
      always_assert(isset(), "Unwrapping a null pointer");
      return m_ptr;
    }

    [[nodiscard]] constexpr auto get_unchecked() const -> UnwrappedGeneric {
      assert_invariant(isset(), "Unwrapping a null pointer");
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

    [[nodiscard]] constexpr auto add(usize i) const -> ChildGeneric {
      assert_invariant(isset(), "Adding to a null pointer");
      return unwrap() + i;
    }

    [[nodiscard]] constexpr auto sub(usize i) const -> ChildGeneric {
      assert_invariant(isset(), "Subtracting from a null pointer");
      return unwrap() - i;
    }

    [[nodiscard]] constexpr auto operator++() const -> ChildGeneric {
      assert_invariant(isset(), "Incrementing a null pointer");
      return unwrap() + 1;
    }

    [[nodiscard]] constexpr auto operator--() const -> ChildGeneric {
      assert_invariant(isset(), "Decrementing a null pointer");
      return unwrap() - 1;
    }

    [[nodiscard]] constexpr auto operator++(int) const -> ChildGeneric {
      assert_invariant(isset(), "Incrementing a null pointer");
      return unwrap() + 1;
    }

    [[nodiscard]] constexpr auto operator--(int) const -> ChildGeneric {
      assert_invariant(isset(), "Decrementing a null pointer");
      return unwrap() - 1;
    }
  };
}  // namespace wesos::types
