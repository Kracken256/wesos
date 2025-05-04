/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Template.hh>

namespace wesos {
  template <typename ToGeneric, typename FromGeneric>
  [[nodiscard]] constexpr auto bit_cast(const FromGeneric& from) -> ToGeneric
    requires(sizeof(ToGeneric) == sizeof(FromGeneric)) && __is_trivially_copyable
  (ToGeneric) && __is_trivially_copyable(FromGeneric) {
    return __builtin_bit_cast(ToGeneric, from);
  }
}  // namespace wesos