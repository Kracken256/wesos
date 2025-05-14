/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>

namespace wesos::types {
  template <typename Callable>
  class DeferBomb {
    Callable m_code;

  public:
    constexpr DeferBomb(Callable code) : m_code(move(code)) {}
    constexpr ~DeferBomb() { m_code(); }
    constexpr DeferBomb(DeferBomb&&) = delete;
    constexpr DeferBomb(const DeferBomb&) = delete;
    constexpr auto operator=(DeferBomb&&) -> DeferBomb& = delete;
    constexpr auto operator=(const DeferBomb&) -> DeferBomb& = delete;
  };

  template <typename Callable>
  [[nodiscard(
      "The result structure must be attached to a variable. Ensure the deferred code as a lifetime.")]] constexpr auto
  defer(Callable code) -> DeferBomb<Callable> {
    return DeferBomb<Callable>{code};
  }
}  // namespace wesos::types
