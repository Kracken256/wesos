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

namespace wesos::types {
  template <class ValueGeneric>
  class Nullable {
    union Payload {
      int m_placeholder;
      ValueGeneric m_obj;

      constexpr Payload() : m_placeholder{} {}
    } m_value;
    bool m_isset = false;

    [[nodiscard]] constexpr auto get() const -> const ValueGeneric& { return m_value.m_obj; }
    [[nodiscard]] constexpr auto get() -> ValueGeneric& { return m_value.m_obj; }

  public:
    constexpr Nullable() = default;
    constexpr Nullable(Null) {}
    constexpr Nullable(ValueGeneric x) { assign(move(x)); }
    constexpr Nullable(const Nullable&) = default;
    constexpr Nullable(Nullable&& o) {
      if (o.isset()) {
        assign(move(o.get()));
        o.unset();
      }
    }
    constexpr auto operator=(const Nullable&) -> Nullable& = default;
    constexpr auto operator=(Nullable&& o) -> Nullable& {
      if (o.isset()) {
        assign(move(o.get()));
        o.unset();
      } else {
        unset();
      }
    }
    constexpr ~Nullable() { unset(); }

    [[nodiscard]] constexpr auto operator<=>(const Nullable&) const = default;
    [[nodiscard]] constexpr auto isset() const -> bool { return m_isset; }
    [[nodiscard]] constexpr operator bool() { return isset(); }

    [[nodiscard]] constexpr auto unwrap() const -> const ValueGeneric& {
      always_assert(isset(), "nullable is null");
      return get();
    }

    [[nodiscard]] constexpr auto unwrap_unchecked() const -> const ValueGeneric& {
      assert_invariant(isset(), "nullable is null");
      return get();
    }

    [[nodiscard]] constexpr auto unwrap() -> ValueGeneric& {
      always_assert(isset(), "nullable is null");
      return get();
    }

    [[nodiscard]] constexpr auto unwrap_unchecked() -> ValueGeneric& {
      assert_invariant(isset(), "nullable is null");
      return get();
    }

    [[nodiscard]] constexpr auto value_or(ValueGeneric&& y) const -> ValueGeneric {
      return isset() ? get() : y;
    }

    constexpr auto operator=(ValueGeneric x) -> Nullable& {
      assign(move(x));
      return *this;
    }

    constexpr auto assign(ValueGeneric x) -> Nullable& {
      unset();
      m_value.m_obj = move(x);
      m_isset = true;

      return *this;
    }

    constexpr auto unset() -> Nullable& {
      if (isset()) {
        get().~ValueGeneric();
        m_isset = false;
      }

      return *this;
    }

    constexpr auto anew() -> ValueGeneric& {
      unset();
      return assign({});
    }

    constexpr auto ensure() -> ValueGeneric& {
      if (!isset()) {
        assign({});
      }

      return get();
    }
  };
}  // namespace wesos::types
