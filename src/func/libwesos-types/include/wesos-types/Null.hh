/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/StrongOrdering.hh>

namespace wesos::types {
  constexpr class Null final {
  public:
    constexpr Null() = default;
    constexpr Null(const Null&) = default;
    constexpr Null(Null&&) = default;
    constexpr auto operator=(const Null&) -> Null& = default;
    constexpr auto operator=(Null&&) -> Null& = default;
    constexpr ~Null() = default;

    constexpr auto operator<=>(const Null&) const = default;
  } null;  // NOLINT(readability-identifier-naming)
}  // namespace wesos::types
