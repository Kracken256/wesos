/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-cpu/Target.hh>

namespace wesos::cpu {
  static inline void ephemeral_pause() {
#if ARCH_X86_64
    __builtin_ia32_pause();
#elif ARCH_X86_32
    __builtin_ia32_pause();
#elif ARCH_ARM_64
/// TODO: Suport target
#error "Not implement yet"
#elif ARCH_ARM_32
/// TODO: Suport target
#error "Not implement yet"
#elif ARCH_RISCV_64
/// TODO: Suport target
#error "Not implement yet"
#elif ARCH_RISCV_32
/// TODO: Suport target
#error "Not implement yet"
#else
#error "This implementation of ephemeral_pause() does not support your architecure. Sorry.."
#endif
  }
}  // namespace wesos::cpu
