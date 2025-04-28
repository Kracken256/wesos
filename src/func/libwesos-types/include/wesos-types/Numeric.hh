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
  template <class ValueGeneric>
  class GenericNumber {
    ValueGeneric m_value;

  public:
    using ValueType = ValueGeneric;

    constexpr GenericNumber() : m_value(0){};
    constexpr GenericNumber(ValueGeneric x) : m_value(x) {}
    constexpr GenericNumber(const GenericNumber& other) = default;
    constexpr GenericNumber(GenericNumber&& other) = default;
    constexpr auto operator=(const GenericNumber& other) -> GenericNumber& = default;
    constexpr auto operator=(GenericNumber&& other) -> GenericNumber& = default;
    constexpr auto operator<=>(const GenericNumber& other) const = default;

    [[nodiscard]] constexpr auto unwrap() const -> ValueGeneric { return m_value; }

    template <class DestGeneric>
    [[nodiscard]] constexpr auto cast_to() const -> DestGeneric {
      static_assert(sizeof(DestGeneric) >= sizeof(*this),
                    "Implicit conversion is disallowed because it loses numeric precision. Use "
                    "trunc_to() instead.");
      static_assert(!is_same_v<typename DestGeneric::ValueType, ValueGeneric>,
                    "Explicit conversion is disallowed because the destination type is the same as "
                    "source type.");

      return static_cast<DestGeneric>(unwrap());
    }

    template <class DestGeneric>
    [[nodiscard]] constexpr auto trunc_to() const -> DestGeneric {
      static_assert(
          sizeof(DestGeneric) < sizeof(*this),
          "Explicit truncation is disallowed because it is redundant. Use cast_to() instead.");
      return {static_cast<DestGeneric::ValueType>(unwrap())};
    }

    template <class DestGeneric>
    [[nodiscard]] constexpr auto unsafe_cast() const -> DestGeneric {
      return {static_cast<DestGeneric>(unwrap())};
    }

    constexpr auto operator++() const -> GenericNumber { return {unwrap() + 1}; }
    constexpr auto operator--() const -> GenericNumber { return {unwrap() - 1}; }
    constexpr auto operator++(int) const -> GenericNumber { return {unwrap() + 1}; }
    constexpr auto operator--(int) const -> GenericNumber { return {unwrap() - 1}; }

    [[nodiscard]] static constexpr auto max() -> GenericNumber {
      using namespace detail;

      enum {
        u8_max = 0xff,
        u16_max = 0xffff,
        u32_max = 0xffffffff,
        u64_max = 0xffffffffffffffff,
        i8_max = 0x7f,
        i16_max = 0x7fff,
        i32_max = 0x7fffffff,
        i64_max = 0x7fffffffffffffff,
      };

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

    [[nodiscard]] static constexpr auto min() -> GenericNumber {
      using namespace detail;

      enum {
        u8_min = 0,
        u16_min = 0,
        u32_min = 0,
        u64_min = 0,
        i8_min = -0x80,
        i16_min = -0x8000,
        i32_min = -0x80000000,
        i64_min = -0x8000000000000000,
      };

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
  };

  template <class LGeneric, class RGeneric>
  constexpr auto operator+(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() + rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator-(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() - rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator*(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() * rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator/(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() / rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator%(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() % rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator&(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() & rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator|(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() | rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator^(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() ^ rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator<<(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() << rhs.unwrap());
  }

  template <class LGeneric, class RGeneric>
  constexpr auto operator>>(GenericNumber<LGeneric> lhs, GenericNumber<RGeneric> rhs) {
    return GenericNumber(lhs.unwrap() >> rhs.unwrap());
  }

  // NOLINTBEGIN(readability-identifier-naming)

  using u8 = GenericNumber<detail::__u8>;
  using u16 = GenericNumber<detail::__u16>;
  using u32 = GenericNumber<detail::__u32>;
  using u64 = GenericNumber<detail::__u64>;
  using i8 = GenericNumber<detail::__i8>;
  using i16 = GenericNumber<detail::__i16>;
  using i32 = GenericNumber<detail::__i32>;
  using i64 = GenericNumber<detail::__i64>;
  using f32 = GenericNumber<detail::__f32>;
  using f64 = GenericNumber<detail::__f64>;
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

  static_assert(sizeof(usize) == sizeof(void*), "Pointer size is not supported");

  constexpr auto operator"" _u8(unsigned long long x) -> u8 {
    return {static_cast<detail::__u8>(x)};
  }

  constexpr auto operator"" _u16(unsigned long long x) -> u16 {
    return {static_cast<detail::__u16>(x)};
  }

  constexpr auto operator"" _u32(unsigned long long x) -> u32 {
    return {static_cast<detail::__u32>(x)};
  }

  constexpr auto operator"" _u64(unsigned long long x) -> u64 {
    return {static_cast<detail::__u64>(x)};
  }

  constexpr auto operator"" _i8(unsigned long long x) -> i8 {
    return {static_cast<detail::__i8>(x)};
  }

  constexpr auto operator"" _i16(unsigned long long x) -> i16 {
    return {static_cast<detail::__i16>(x)};
  }

  constexpr auto operator"" _i32(unsigned long long x) -> i32 {
    return {static_cast<detail::__i32>(x)};
  }

  constexpr auto operator"" _i64(unsigned long long x) -> i64 {
    return {static_cast<detail::__i64>(x)};
  }

  constexpr auto operator"" _f32(long double x) -> f32 { return {static_cast<detail::__f32>(x)}; }

  constexpr auto operator"" _f64(long double x) -> f64 { return {static_cast<detail::__f64>(x)}; }

  constexpr auto operator"" _usize(unsigned long long x) -> usize {
    return {static_cast<usize>(x)};
  }

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
