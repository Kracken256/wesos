/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-types/Types.hh>

#include "EFI.hh"

using namespace wesos;
using namespace wesos::boot::efi;

// This is like int main() in a typical C program.
// In this case, we create an ImageHandle for the overall EFI interface,
// as well as a System Table pointer to the EFI_SYSTEM_TABLE struct.
// UEFI 2.9 Specs PDF Page 91
auto efi_main(Handle image_handle, SystemTable *system_table) -> Status {
  (void)image_handle;

  auto api = *system_table;
  if (!api.validate()) {
    return EFI_FAILURE;
  }

  (void)api.con_out().clear_screen();
  (void)api.con_out().output_string(u"Hello, WesOS! FROM UEFI API!!\r\n"_u16);

  while (true) {
    wesos::cpu::ephemeral_pause();
  };

  return EFI_SUCCESS;
}
