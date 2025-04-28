/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

namespace wesos::kern {
  [[gnu::section(".text._start"), noreturn]] void start() {
    /// TODO: Implement the kernel

    while (true) {
      asm volatile("nop");
    }
  }
}  // namespace wesos::kern
