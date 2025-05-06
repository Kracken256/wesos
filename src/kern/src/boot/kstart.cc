/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-types/Types.hh>

void operator delete(void*) { assert_always(false && "Calling C++ operator delete is not allowed"); }

extern "C" void __cxa_pure_virtual() {  // NOLINT(readability-identifier-naming)
  assert_always(false && "Calling into C++ pure virtual functions is not allowed");
}

namespace wesos::kern {
  static const auto KERNEL_STACK_SIZE = 0xffff;
  static const auto KERNEL_STACK_ALIGNMENT = 16;
  alignas(KERNEL_STACK_ALIGNMENT) static Array<u8, KERNEL_STACK_SIZE> KERNEL_STACK_GLOBAL;

#if defined(__x86_64__)

  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "xor %%rbp, %%rbp\n\t"         // Clear base pointer
        "lea %[stack_top], %%rsp\n\t"  // Load address of stack top into rsp
        "and %%rsp, 16\n\t"            // Align stack to 16 bytes
        "jmp runtime_setup\n\t"
        :
        : [stack_top] "m"(KERNEL_STACK_GLOBAL.get(KERNEL_STACK_SIZE - KERNEL_STACK_ALIGNMENT))
        : "rsp", "rbp");
  }

#elif defined(__i386__)

  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "xor %%ebp, %%ebp\n\t"         // Clear base pointer
        "lea %[stack_top], %%esp\n\t"  // Load address of stack top into esp
        "and %%esp, $0xfffffff0\n\t"   // Align stack to 16 bytes
        "jmp runtime_setup\n\t"
        :
        : [stack_top] "m"(KERNEL_STACK_GLOBAL.get(KERNEL_STACK_SIZE - KERNEL_STACK_ALIGNMENT))
        : "esp", "ebp");
  }

#elif defined(__arm__)

  // Note: This version assumes KERNEL_STACK_GLOBAL is linked in a way that makes address arithmetic legal in ldr
  // pseudo-instruction.
  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "mov r7, #0\n\t"                              // Clear frame pointer
        "ldr r0, =KERNEL_STACK_GLOBAL + %[size]\n\t"  // Load address into r0
        "bic sp, r0, #15\n\t"                         // Align to 16 bytes
        "b runtime_setup\n\t"
        :
        : [size] "I"(KERNEL_STACK_SIZE)
        : "sp", "r0", "r7");
  }

#elif defined(__aarch64__)

  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {  // NOLINT(readability-identifier-naming)
    asm volatile(
        "mov x29, xzr\n\t"                            // Clear frame pointer
        "ldr x0, =KERNEL_STACK_GLOBAL + %[size]\n\t"  // Load address into x0
        "bic sp, x0, #15\n\t"                         // Align to 16 bytes
        "b runtime_setup\n\t"
        :
        : [size] "I"(KERNEL_STACK_SIZE)
        : "sp", "x0", "x29");
  }

#elif defined(__riscv) && defined(__riscv64__)

  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {
    asm volatile(
        "mov x29, xzr\n\t"                          // Clear frame pointer
        "la sp, KERNEL_STACK_GLOBAL + %[size]\n\t"  // Load stack pointer address
        "andi sp, sp, -16\n\t"                      // Align the stack to 16 bytes
        "b runtime_setup\n\t"
        :
        : [size] "I"(KERNEL_STACK_SIZE)
        : "sp", "x29");
  }

#elif defined(__riscv) && defined(__riscv32__)

  [[gnu::section(".text._start"), noreturn, gnu::naked]]
  SYM_EXPORT void _start() {
    asm volatile(
        "mov sp, %[stack_top]\n\t"  // Set stack pointer to top
        "andi sp, sp, -16\n\t"      // Align the stack to 16 bytes
        "b runtime_setup\n\t"
        :
        : [stack_top] "r"(KERNEL_STACK_GLOBAL.get(KERNEL_STACK_SIZE - KERNEL_STACK_ALIGNMENT))
        : "sp");
  }

#else
#error "Architecture not supported"
#endif

  extern "C" void runtime_setup() {
    /// TODO: Initialize global variables

    while (true) {
      asm volatile("syscall");
      asm volatile("nop");
    }
  }
}  // namespace wesos::kern
