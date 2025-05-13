/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
  static constexpr auto strlen16(const u16* str) -> usize {
    usize len = 0;
    while (*str != 0) {
      ++len;
      ++str;
    }
    return len;
  }
}  // namespace wesos::boot::efi
