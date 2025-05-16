/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>
#include <wesos-types/Nullable.hh>

namespace wesos::types {
  template <typename Callable>
  class DeferBomb {
    Nullable<Callable> m_func;

  public:
    constexpr DeferBomb(Callable code) : m_func(move(code)) {}
    constexpr ~DeferBomb() {
      if (m_func.isset()) {
        m_func.value()();
      }
    }
    constexpr DeferBomb(DeferBomb&& o) : m_func(move(o.m_func)) { o.m_func.unset(); };
    constexpr DeferBomb(const DeferBomb&) = delete;
    constexpr auto operator=(DeferBomb&& o) -> DeferBomb& {
      if (this != &o) {
        m_func = move(o.m_func);
        o.m_func.unset();
      }

      return *this;
    }
    constexpr auto operator=(const DeferBomb&) -> DeferBomb& = delete;
  };

  template <typename Callable>
  [[nodiscard(
      "The result structure must be attached to a variable. Ensure the deferred code as a lifetime.")]] constexpr auto
  defer(Callable code) -> DeferBomb<Callable> {
    return DeferBomb<Callable>{code};
  }
}  // namespace wesos::types
