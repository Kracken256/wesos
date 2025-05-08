/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-types/Types.hh>

// These are the same typedefs used in the official PDF specs
using CHAR16 = char16_t;

using UINT32 = wesos::u32;
using UINT64 = wesos::u64;

using BOOLEAN = wesos::u8;

using EfiHandle = void *;
using EfiStatus = UINT64;

// This struct is a placeholder and not usable at this time
// The code will not compile without it though.
using EfiSimpleTextInputProtocol = struct EfiSimpleTextInputProtocol {};

// We are forward declaring this struct so that the two function typedefs can operate.
struct EfiSimpleTextOutputProtocol;

// This function resets the string output.
using EfiTextReset = EfiStatus (*)(struct EfiSimpleTextOutputProtocol *, BOOLEAN);

// This function prints the string output to the screen.
using EfiTextString = EfiStatus (*)(struct EfiSimpleTextOutputProtocol *, const CHAR16 *);

// The struct for the EFI Text Output protocols.
// UEFI 2.9 Specs PDF Page 449
using EfiSimpleTextOutputProtocol = struct EfiSimpleTextOutputProtocol {
  EfiTextReset m_reset;
  EfiTextString m_output_string;
};

// This is the main EFI header for all of the EFI.
// UEFI 2.9 Specs PDF Page 93
using EfiTableHeader = struct EfiTableHeader {
  UINT64 m_signature;
  UINT32 m_revision;
  UINT32 m_header_size;
  UINT32 m_cr_c32;
  UINT32 m_reserved;
};

// EFI has a system and runtime. This system table is the first struct
// called from the main section.
// UEFI 2.9 Specs PDF Page 94
using EfiSystemTable = struct EfiSystemTable {
  EfiTableHeader m_hrd;
  CHAR16 *m_firmware_vendor;
  UINT32 m_firmware_version;
  EfiHandle m_console_in_handle;
  EfiSimpleTextInputProtocol *m_con_in;
  EfiHandle m_console_out_handle;
  EfiSimpleTextOutputProtocol *m_con_out;
};

// This is like int main() in a typical C program.
// In this case, we create an ImageHandle for the overall EFI interface,
// as well as a System Table pointer to the EFI_SYSTEM_TABLE struct.
// UEFI 2.9 Specs PDF Page 91
auto efi_main(EfiHandle image_handle, EfiSystemTable *system_table) -> EfiStatus {
  // This clears the screen and buffer.
  system_table->m_con_out->m_reset(system_table->m_con_out, 1);

  // This prints "Testing..." to the screen ( A.K.A. ConOut is Console Out )
  system_table->m_con_out->m_output_string(system_table->m_con_out, u"Testing...\r\n");

  // We use this while loop to hang. At this point, simple shut off computer.
  while (true) {
    wesos::cpu::ephemeral_pause();
  };

  // The EFI needs to have a 0 ( or EFI_SUCCESS ) in order to know everything is ok.
  return 0;
}
