/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-kern/boot/Options.hh>

namespace wesos::kern {
  [[gnu::section(".text._start"), noreturn]] SYM_EXPORT void start(
      Untrusted<boot::Options> boot_settings) {
    const auto& settings = boot_settings.trust_and_unwrap();
    (void)settings;

    /// TODO: Implement the kernel

    while (true) {
      asm volatile("nop");
    }
  }
}  // namespace wesos::kern
