/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos {
  // NOLINTBEGIN(readability-identifier-naming)

  extern "C" auto memcpy(void *dest, const void *src, usize n) -> void *;
  extern "C" auto memset(void *dest, int value, usize n) -> void *;
  extern "C" auto memmove(void *dest, const void *src, usize n) -> void *;
  extern "C" auto memcmp(const void *ptr1, const void *ptr2, usize n) -> int;

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos