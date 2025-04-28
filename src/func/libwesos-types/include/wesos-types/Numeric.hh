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

    void reflect(void* m, auto cb, auto&) const {
      cb(m, reinterpret_cast<const void*>(&m_value), sizeof(ValueGeneric));
    }

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
