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
  constexpr auto bit_cast(const FromGeneric& src) -> ToGeneric {
    static_assert(sizeof(ToGeneric) == sizeof(FromGeneric), "bit_cast requires equal sizes");
    /// TODO: Implement these checks

    // static_assert(std::is_trivially_copyable_v<FromGeneric>,
    //               "bit_cast requires trivially copyable From");
    // static_assert(std::is_trivially_copyable_v<ToGeneric>,
    //               "bit_cast requires trivially copyable ToGeneric");
    // static_assert(std::is_trivially_constructible_v<ToGeneric>,
    //               "bit_cast requires trivially constructible ToGeneric");

    ToGeneric dst;
    __builtin_memcpy(&dst, &src, sizeof(ToGeneric));  // safe type punning
    return dst;
  }
}  // namespace wesos