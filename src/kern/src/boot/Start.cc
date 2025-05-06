/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-types/Types.hh>

namespace wesos::kern {
  extern "C" [[noreturn]] void cxx_start();

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
        "jmp cxx_start\n\t"            // Jump to C++ runtime setup
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
        "jmp cxx_start\n\t"            // Jump to C++ runtime setup
        :
        : [stack_top] "m"(KERNEL_STACK_GLOBAL[KERNEL_STACK_SIZE - 1])
        : "esp", "ebp");
  }

#elif ARCH_ARM_64

  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]  /// TODO: Test code
  void
  _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "mov x29, xzr\n\t"                            // Clear frame pointer
        "ldr x0, =KERNEL_STACK_GLOBAL + %[size]\n\t"  // Load address into x0
        "bic sp, x0, #15\n\t"                         // Align to 16 bytes
        "b cxx_start\n\t"                             // Jump to C++ runtime setup
        :
        : [size] "I"(KERNEL_STACK_SIZE - 1)
        : "sp", "x0", "x29");
  }

#elif ARCH_ARM_32

  // Note: This version assumes KERNEL_STACK_GLOBAL is linked in a way that makes address arithmetic legal in ldr
  // pseudo-instruction.
  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]  /// TODO: Test code
  void
  _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "mov r7, #0\n\t"                              // Clear frame pointer
        "ldr r0, =KERNEL_STACK_GLOBAL + %[size]\n\t"  // Load address into r0
        "bic sp, r0, #15\n\t"                         // Align to 16 bytes
        "b cxx_start\n\t"                             // Jump to C++ runtime setup
        :
        : [size] "I"(KERNEL_STACK_SIZE - 1)
        : "sp", "r0", "r7");
  }

#elif ARCH_RISCV_64

  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]
  void _start() {  /// TODO: Test code
    asm volatile(
        "li s0, 0\n\t"             // Clear frame pointer
        "la sp, %[stack_top]\n\t"  // Set sp to stack top
        "andi sp, sp, -16\n\t"     // Align stack to 16 bytes
        "j cxx_start\n\t"          // Jump to C++ runtime setup
        :
        : [stack_top] "i"(KERNEL_STACK_GLOBAL + KERNEL_STACK_SIZE - 1)
        : "sp", "s0");
  }

#elif ARCH_RISCV_32

  SYM_EXPORT extern "C" [[gnu::section(".text._start"), noreturn, gnu::naked]]  /// TODO: Test code
  void
  _start() {
    asm volatile(
        "li s0, 0\n\t"             // Clear frame pointer
        "la sp, %[stack_top]\n\t"  // Set sp to stack top
        "andi sp, sp, -16\n\t"     // Align stack to 16 bytes
        "j cxx_start\n\t"          // Jump to C++ runtime setup
        :
        : [stack_top] "i"(KERNEL_STACK_GLOBAL + KERNEL_STACK_SIZE - 1)
        : "sp", "s0");
  }

#else
#error "Architecture not supported"
#endif
}  // namespace wesos::kern
