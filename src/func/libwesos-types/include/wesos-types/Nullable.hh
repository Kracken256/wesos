/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Array.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <class ValueGeneric>
  class Nullable {
    union Payload {
      Array<u8, sizeof(ValueGeneric)> m_bytes;
      ValueGeneric m_obj;

      constexpr Payload() : m_bytes{} {}
    } m_value;
    bool m_isset = false;

  public:
    constexpr Nullable() = default;
    constexpr Nullable(Null) {}
    constexpr Nullable(ValueGeneric x) : m_isset(true) { m_value.m_obj = x; }
    constexpr Nullable(const Nullable& other) = default;
    constexpr Nullable(Nullable&& other) = default;
    constexpr auto operator=(const Nullable& other) -> Nullable& = default;
    constexpr auto operator=(Nullable&& other) -> Nullable& = default;
    constexpr auto operator<=>(const Nullable& other) const = default;
    constexpr auto operator=(ValueGeneric x) -> Nullable& {
      m_isset = true;
      m_value.m_obj = x;
      return *this;
    }

    constexpr ~Nullable() { unset(); }

    [[nodiscard]] constexpr auto isset() const -> bool { return m_isset; }

    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& {
      /// FIXME: Do bounds checking
      return m_value.m_obj;
    }

    [[nodiscard]] constexpr auto unwrap_unchecked() const -> const ValueGeneric& {
      return m_value.m_obj;
    }

    [[nodiscard]] constexpr auto unwrap() -> ValueGeneric& {
      /// FIXME: Do bounds checking
      return m_value.m_obj;
    }

    [[nodiscard]] constexpr auto unwrap_unchecked() -> ValueGeneric& { return m_value.m_obj; }

    void unset() {
      if (isset()) {
        m_value.m_obj.~ValueGeneric();
        m_isset = false;
      }
    }

    void anew() {
      unset();
      m_value.m_obj = ValueGeneric();
      m_isset = true;
    }

    void fill() {
      if (!isset()) {
        m_value.m_obj = ValueGeneric();
        m_isset = true;
      }
    }
  };
}  // namespace wesos::types
