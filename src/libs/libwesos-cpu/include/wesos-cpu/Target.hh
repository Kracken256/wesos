/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#if defined(__x86_64__)
#define ARCH_X86_64 1
#elif defined(__i386__)
#define ARCH_X86_32 1
#elif defined(__aarch64__)
#define ARCH_ARM_64 1
#elif defined(__arm__)
#define ARCH_ARM_32 1
#elif defined(__riscv) && (__riscv_xlen == 64)
#define ARCH_RISCV_64 1
#elif defined(__riscv) && (__riscv_xlen == 32)
#define ARCH_RISCV_32 1
#else
#error "Sorry, WesOS does not currently support your target architecture"
#endif
