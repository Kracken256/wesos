/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Template.hh>

namespace wesos {
  template <class To, class From>
  [[nodiscard]] constexpr auto bit_cast(const From& from) -> To
    requires(sizeof(To) == sizeof(From)) && __is_trivially_copyable
  (To) && __is_trivially_copyable(From) {
    return __builtin_bit_cast(To, from);
  }
}  // namespace wesos