/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Primitive.hh>
#include <wesos-types/ReflectionCallback.hh>
#include <wesos-types/StrongOrdering.hh>
#include <wesos-types/Template.hh>

namespace wesos::types {
  template <class ValueGeneric>
  class NumbericProtocol {
    ValueGeneric m_value;

  public:
    constexpr NumbericProtocol() : m_value(0){};
    constexpr NumbericProtocol(ValueGeneric x) : m_value(x) {}
    constexpr NumbericProtocol(const NumbericProtocol& other) = default;
    constexpr NumbericProtocol(NumbericProtocol&& other) = default;
    constexpr auto operator=(const NumbericProtocol& other) -> NumbericProtocol& = default;
    constexpr auto operator=(NumbericProtocol&& other) -> NumbericProtocol& = default;
    constexpr auto operator<=>(const NumbericProtocol& other) const = default;

    void reflect(const ReflectionCallback& cb) const {
      cb(reinterpret_cast<void*>(&m_value),
         static_cast<detail::PrimitiveTypeSize>(sizeof(ValueGeneric)));
    }

    [[nodiscard]] constexpr auto unwrap() const -> ValueGeneric { return m_value; }

    template <class DestGeneric>
    [[nodiscard]] constexpr auto cast_to() const -> DestGeneric {
      return static_cast<DestGeneric>(unwrap());
    }

#define W_DECLARE_NUMERIC_PROTOCOL_OP(op)                                             \
  constexpr auto operator op(const NumbericProtocol& other) const->NumbericProtocol { \
    static_assert(is_same_v<decltype(*this), decltype(other)>,                        \
                  "Implicit conversion is disallowed");                               \
    return {unwrap() op other.unwrap()};                                              \
  }

    W_DECLARE_NUMERIC_PROTOCOL_OP(+)
    W_DECLARE_NUMERIC_PROTOCOL_OP(-)
    W_DECLARE_NUMERIC_PROTOCOL_OP(*)
    W_DECLARE_NUMERIC_PROTOCOL_OP(/)
    W_DECLARE_NUMERIC_PROTOCOL_OP(%)
    W_DECLARE_NUMERIC_PROTOCOL_OP(&)
    W_DECLARE_NUMERIC_PROTOCOL_OP(|)
    W_DECLARE_NUMERIC_PROTOCOL_OP(^)
    W_DECLARE_NUMERIC_PROTOCOL_OP(<<)
    W_DECLARE_NUMERIC_PROTOCOL_OP(>>)
    W_DECLARE_NUMERIC_PROTOCOL_OP(==)
    W_DECLARE_NUMERIC_PROTOCOL_OP(!=)
    W_DECLARE_NUMERIC_PROTOCOL_OP(<=)
    W_DECLARE_NUMERIC_PROTOCOL_OP(>=)
    W_DECLARE_NUMERIC_PROTOCOL_OP(<)
    W_DECLARE_NUMERIC_PROTOCOL_OP(>)
    W_DECLARE_NUMERIC_PROTOCOL_OP(&&)
    W_DECLARE_NUMERIC_PROTOCOL_OP(||)

#undef W_DECLARE_NUMERIC_PROTOCOL_OP

    constexpr auto operator++() const -> NumbericProtocol { return {unwrap() + 1}; }
    constexpr auto operator--() const -> NumbericProtocol { return {unwrap() - 1}; }
    constexpr auto operator++(int) const -> NumbericProtocol { return {unwrap() + 1}; }
    constexpr auto operator--(int) const -> NumbericProtocol { return {unwrap() - 1}; }
  };

  // NOLINTBEGIN(readability-identifier-naming)

  using u8 = NumbericProtocol<detail::__u8>;
  using u16 = NumbericProtocol<detail::__u16>;
  using u32 = NumbericProtocol<detail::__u32>;
  using u64 = NumbericProtocol<detail::__u64>;
  using i8 = NumbericProtocol<detail::__i8>;
  using i16 = NumbericProtocol<detail::__i16>;
  using i32 = NumbericProtocol<detail::__i32>;
  using i64 = NumbericProtocol<detail::__i64>;

  using f32 = NumbericProtocol<detail::__f32>;
  using f64 = NumbericProtocol<detail::__f64>;

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

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
