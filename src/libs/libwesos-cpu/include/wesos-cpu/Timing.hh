/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::cpu {
  static inline void ephemeral_pause() {
#if defined(__x86_64__)
    __builtin_ia32_pause();
#else
#error "This implementation of ephemeral_pause() does not support your architecure. Sorry.."
#endif
  }
}  // namespace wesos::cpu
