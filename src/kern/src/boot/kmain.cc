/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-kern/boot/Options.hh>
#include <wesos-mem/AtomicMemoryEconomy.hh>
#include <wesos-types/Types.hh>

using namespace wesos;
using namespace wesos::mem;

namespace wesos::kern {
  void main(boot::Options settings) {
    (void)settings;

    /// TODO: Implement the kernel

    while (true) {
      asm volatile("nop");
    }
  }
}  // namespace wesos::kern
