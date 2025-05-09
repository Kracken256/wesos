/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <boot/EFI.hh>
#include <boot/Logger.hh>
#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-types/Types.hh>

using namespace wesos;
using namespace wesos::boot::efi;

namespace wesos::boot::efi {
  SystemTable *SYSTEM_TABLE_GLOBAL = nullptr;
  Handle EFI_IMAGE_HANDLE_GLOBAL = nullptr;

  ///====================================================================

  extern "C" auto efi_main(Handle image_handle, SystemTable *system_table) -> Status {
    EFI_IMAGE_HANDLE_GLOBAL = image_handle;
    SYSTEM_TABLE_GLOBAL = system_table;

    if (!SYSTEM_TABLE_GLOBAL->validate()) [[unlikely]] {
      return EFI_FAILURE;
    }

    (void)SYSTEM_TABLE_GLOBAL->con_out().clear_screen();

    INFO << u"WesOS EFI Bootloader" << ENDL;
    INFO << u"UEFI Firmware Vendor: " << SYSTEM_TABLE_GLOBAL->firmware_vendor() << ENDL;
    INFO << u"UEFI Firmware Revision: " << SYSTEM_TABLE_GLOBAL->firmware_revision() << ENDL;

    HELP << u"Loading KERNEL/PRELOAD.CONF from the EFI partition..." << ENDL;

    /** TODO: Outline the steps to load the kernel:
     *  1.  Load KERNEL/PRELOAD.CONF from the EFI partition.
     *  2.  Parse the config file.
     *  3.  Create an in memory file system.
     *  4.  Load each of the files listed in the config file storing them in the in memory file system.
     *  5.  Identity map all the memory pages.
     *  6.  Get the current memory map.
     *  7.  Create the kernel config file.
     *  8.  Parse the KERNEL ELF binary.
     *  9.  Exit the boot services.
     *  10. Jump to the kernel entry point.
     */

    while (true) {
      wesos::cpu::ephemeral_pause();
    };

    return EFI_SUCCESS;
  }
}  // namespace wesos::boot::efi
