/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Target.hh>
#include <wesos-types/Types.hh>

namespace wesos::kern {
  extern "C" [[noreturn]] void cxx_genesis(const u8 *configuration, usize configuration_len);

  static const auto KERNEL_STACK_SIZE = 0xffff;
  static const auto KERNEL_STACK_ALIGNMENT = 16;
  alignas(KERNEL_STACK_ALIGNMENT) static u8 KERNEL_STACK_GLOBAL[KERNEL_STACK_SIZE];

#if ARCH_X86_64

  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]
  void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "xor %%rbp, %%rbp\n\t"         // Clear base pointer
        "lea %[stack_top], %%rsp\n\t"  // Set rsp to stack top
        "and $-16, %%rsp\n\t"          // Align rsp to 16 bytes
        "call cxx_genesis\n\t"         // Call to C++ runtime setup
        :
        : [stack_top] "m"(KERNEL_STACK_GLOBAL[KERNEL_STACK_SIZE - 1])
        : "rsp", "rbp");
  }

#elif ARCH_X86_32

  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]
  void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "xor %%ebp, %%ebp\n\t"         // Clear base pointer
        "lea %[stack_top], %%esp\n\t"  // Set esp to stack top
        "and $-16, %%esp\n\t"          // Align esp to 16 bytes
        "call cxx_genesis\n\t"         // Call to C++ runtime setup
        :
        : [stack_top] "m"(KERNEL_STACK_GLOBAL[KERNEL_STACK_SIZE - 1])
        : "esp", "ebp");
  }

#else
#error "Architecture not supported"
#endif
}  // namespace wesos::kern
