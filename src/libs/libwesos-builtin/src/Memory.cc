/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Memory.hh>

using namespace wesos;

extern "C" auto memcpy(void *dest, const void *src, usize n) -> void * {
  auto *d = static_cast<u8 *>(dest);
  const auto *s = static_cast<const u8 *>(src);
  for (usize i = 0; i < n; ++i) {
    d[i] = s[i];
  }
  return dest;
}

extern "C" auto memset(void *dest, int value, usize n) -> void * {
  auto *d = static_cast<u8 *>(dest);
  for (usize i = 0; i < n; ++i) {
    d[i] = static_cast<u8>(value);
  }
  return dest;
}

extern "C" auto memmove(void *dest, const void *src, usize n) -> void * {
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

extern "C" auto memcmp(const void *ptr1, const void *ptr2, usize n) -> int {
  const auto *p1 = static_cast<const u8 *>(ptr1);
  const auto *p2 = static_cast<const u8 *>(ptr2);

  for (usize i = 0; i < n; ++i) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }
  return 0;
}
