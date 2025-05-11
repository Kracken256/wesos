/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos {
  /// NOLINTBEGIN(readability-identifier-naming)

  extern "C" inline auto memcpy(void *dest, const void *src, usize n) -> void * {
    auto *d = static_cast<u8 *>(dest);
    const auto *s = static_cast<const u8 *>(src);
    for (usize i = 0; i < n; ++i) {
      d[i] = s[i];
    }
    return dest;
  }

  extern "C" inline auto memset(void *dest, int value, usize n) -> void * {
    auto *d = static_cast<u8 *>(dest);
    for (usize i = 0; i < n; ++i) {
      d[i] = static_cast<u8>(value);
    }
    return dest;
  }

  extern "C" inline auto memmove(void *dest, const void *src, usize n) -> void * {
    auto *d = static_cast<u8 *>(dest);
    const auto *s = static_cast<const u8 *>(src);

    if (d < s) {
      for (usize i = 0; i < n; ++i) {
        d[i] = s[i];
      }
    } else if (d > s) {
      for (usize i = n; i != 0; --i) {
        d[i - 1] = s[i - 1];
      }
    }

    return dest;
  }

  /// NOLINTEND(readability-identifier-naming)
}  // namespace wesos