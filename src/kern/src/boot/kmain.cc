/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-kern/boot/Options.hh>

namespace wesos::kern {
  [[gnu::section(".text._start"), noreturn]] void start(Untrusted<boot::Options> boot_settings) {
    const auto& settings = boot_settings.unwrap();
    (void)settings;

    /// TODO: Implement the kernel

    while (true) {
      asm volatile("nop");
    }
  }
}  // namespace wesos::kern
