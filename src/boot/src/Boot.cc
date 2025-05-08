/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Timing.hh>

SYM_EXPORT extern "C" auto efi_main(void* image_handle, void* system_table) -> int {
  while (true) {
    /// TODO: Implement bootloader
    wesos::cpu::ephemeral_pause();
  }
}
