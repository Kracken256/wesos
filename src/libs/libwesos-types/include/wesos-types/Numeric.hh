/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Primitive.hh>
#include <wesos-types/StrongOrdering.hh>
#include <wesos-types/Template.hh>

namespace wesos::types {
  // NOLINTBEGIN(readability-identifier-naming)

  using u8 = detail::__u8;
  using u16 = detail::__u16;
  using u32 = detail::__u32;
  using u64 = detail::__u64;
  using i8 = detail::__i8;
  using i16 = detail::__i16;
  using i32 = detail::__i32;
  using i64 = detail::__i64;
  using f32 = detail::__f32;
  using f64 = detail::__f64;
  using usize = decltype([]() {
    if constexpr (sizeof(void*) == sizeof(u8)) {
      return u8{};
    } else if constexpr (sizeof(void*) == sizeof(u16)) {
      return u16{};
    } else if constexpr (sizeof(void*) == sizeof(u32)) {
      return u32{};
    } else if constexpr (sizeof(void*) == sizeof(u64)) {
      return u64{};
    } else {
      static_assert(is_same_v<i8, i8>, "Pointer size is not supported");
    }
  }());
  using isize = decltype([]() {
    if constexpr (sizeof(void*) == sizeof(i8)) {
      return i8{};
    } else if constexpr (sizeof(void*) == sizeof(i16)) {
      return i16{};
    } else if constexpr (sizeof(void*) == sizeof(i32)) {
      return i32{};
    } else if constexpr (sizeof(void*) == sizeof(i64)) {
      return i64{};
    } else {
      static_assert(is_same_v<i8, i8>, "Pointer size is not supported");
    }
  }());
  using uptr = usize;

  // NOLINTEND(readability-identifier-naming)

  static_assert(sizeof(usize) == sizeof(void*), "Pointer size is not supported");

  constexpr auto operator"" _u8(unsigned long long x) -> u8 { return static_cast<detail::__u8>(x); }

  constexpr auto operator"" _u16(unsigned long long x) -> u16 {
    return static_cast<detail::__u16>(x);
  }

  constexpr auto operator"" _u32(unsigned long long x) -> u32 {
    return static_cast<detail::__u32>(x);
  }

  constexpr auto operator"" _u64(unsigned long long x) -> u64 {
    return static_cast<detail::__u64>(x);
  }

  constexpr auto operator"" _i8(unsigned long long x) -> i8 { return static_cast<detail::__i8>(x); }

  constexpr auto operator"" _i16(unsigned long long x) -> i16 {
    return static_cast<detail::__i16>(x);
  }

  constexpr auto operator"" _i32(unsigned long long x) -> i32 {
    return static_cast<detail::__i32>(x);
  }

  constexpr auto operator"" _i64(unsigned long long x) -> i64 {
    return static_cast<detail::__i64>(x);
  }

  constexpr auto operator"" _f32(long double x) -> f32 { return static_cast<detail::__f32>(x); }

  constexpr auto operator"" _f64(long double x) -> f64 { return static_cast<detail::__f64>(x); }

  constexpr auto operator"" _usize(unsigned long long x) -> usize { return static_cast<usize>(x); }

  template <class DstGeneric, class SrcGeneric>
  [[nodiscard]] static constexpr auto cast_to(SrcGeneric x) -> DstGeneric {
    static_assert(sizeof(DstGeneric) >= sizeof(SrcGeneric),
                  "Implicit conversion is disallowed because it loses numeric precision. Use "
                  "trunc_to() instead.");
    static_assert(!is_same_v<DstGeneric, SrcGeneric>,
                  "Explicit conversion is disallowed because the destination type is the same as "
                  "source type.");

    return static_cast<DstGeneric>(x);
  }

  template <class DstGeneric, class SrcGeneric>
  [[nodiscard]] static constexpr auto trunc_to(SrcGeneric x) -> DstGeneric {
    static_assert(
        sizeof(DstGeneric) < sizeof(SrcGeneric),
        "Explicit truncation is disallowed because it is redundant. Use cast_to() instead.");
    return static_cast<DstGeneric>(x);
  }

  template <class ValueGeneric>
  [[nodiscard]] static constexpr auto numeric_limit_max() -> u64 {
    using namespace detail;

    constexpr u64 u8_max = 0xff;
    constexpr u64 u16_max = 0xffff;
    constexpr u64 u32_max = 0xffffffff;
    constexpr u64 u64_max = 0xffffffffffffffff;
    constexpr u64 i8_max = 0x7f;
    constexpr u64 i16_max = 0x7fff;
    constexpr u64 i32_max = 0x7fffffff;
    constexpr u64 i64_max = 0x7fffffffffffffff;

    if constexpr (is_same_v<ValueGeneric, __u8>) {
      return u8_max;
    } else if constexpr (is_same_v<ValueGeneric, __u16>) {
      return u16_max;
    } else if constexpr (is_same_v<ValueGeneric, __u32>) {
      return u32_max;
    } else if constexpr (is_same_v<ValueGeneric, __u64>) {
      return u64_max;
    } else if constexpr (is_same_v<ValueGeneric, __i8>) {
      return i8_max;
    } else if constexpr (is_same_v<ValueGeneric, __i16>) {
      return i16_max;
    } else if constexpr (is_same_v<ValueGeneric, __i32>) {
      return i32_max;
    } else if constexpr (is_same_v<ValueGeneric, __i64>) {
      return i64_max;
    } else {
      static_assert(is_same_v<ValueGeneric, ValueGeneric>, "Unsupported type");
    }
  }

  template <class ValueGeneric>
  [[nodiscard]] static constexpr auto numeric_limit_min() -> i64 {
    using namespace detail;

    constexpr i64 u8_min = 0;
    constexpr i64 u16_min = 0;
    constexpr i64 u32_min = 0;
    constexpr i64 u64_min = 0;
    constexpr i64 i8_min = -0x80LL;                                   // -128
    constexpr i64 i16_min = -0x8000LL;                                // -32768
    constexpr i64 i32_min = -0x80000000LL;                            // -2147483648
    constexpr i64 i64_min = static_cast<i64>(0x8000000000000000ULL);  // -9223372036854775808

    if constexpr (is_same_v<ValueGeneric, __u8>) {
      return u8_min;
    } else if constexpr (is_same_v<ValueGeneric, __u16>) {
      return u16_min;
    } else if constexpr (is_same_v<ValueGeneric, __u32>) {
      return u32_min;
    } else if constexpr (is_same_v<ValueGeneric, __u64>) {
      return u64_min;
    } else if constexpr (is_same_v<ValueGeneric, __i8>) {
      return i8_min;
    } else if constexpr (is_same_v<ValueGeneric, __i16>) {
      return i16_min;
    } else if constexpr (is_same_v<ValueGeneric, __i32>) {
      return i32_min;
    } else if constexpr (is_same_v<ValueGeneric, __i64>) {
      return i64_min;
    } else {
      static_assert(is_same_v<ValueGeneric, ValueGeneric>, "Unsupported type");
    }
  }
}  // namespace wesos::types
