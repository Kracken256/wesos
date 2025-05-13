/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <boot/EFI.hh>
#include <boot/Logger.hh>
#include <boot/MemoryMap.hh>
#include <boot/Storage.hh>
#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Memory.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-types/Types.hh>

using namespace wesos;
using namespace wesos::boot::efi;

namespace wesos::assert {
  void assert_failure(const char *, SourceLocation) { __builtin_trap(); }
}  // namespace wesos::assert

namespace wesos::boot::efi {
  SystemTable *SYSTEM_TABLE_GLOBAL = nullptr;
  Handle EFI_IMAGE_HANDLE_GLOBAL = nullptr;

  //====================================================================

  [[nodiscard]] static auto configure_uefi_globals(Handle image_handle, SystemTable *system_table) -> bool {
    EFI_IMAGE_HANDLE_GLOBAL = image_handle;
    SYSTEM_TABLE_GLOBAL = system_table;

    return SYSTEM_TABLE_GLOBAL->validate();
  }

  static void print_ownership() {
    NOTE << u"WesOS EFI Bootloader" << ENDL;
    NOTE << u"Copyright (C) 2023 WesOS Project" << ENDL;
    NOTE << u"License: Unlicense (https://unlicense.org/)" << ENDL;
  }

  static void print_uefi_info() {
    NOTE << u"UEFI Firmware Vendor: " << SYSTEM_TABLE_GLOBAL->firmware_vendor() << ENDL;
    NOTE << u"UEFI Firmware Revision: " << SYSTEM_TABLE_GLOBAL->firmware_revision() << ENDL;
  }

  SYM_EXPORT extern "C" auto efi_main(Handle image_handle, SystemTable *system_table) -> Status {
    if (!configure_uefi_globals(image_handle, system_table)) [[unlikely]] {
      FAIL << u"Failed to configure UEFI globals." << ENDL;
      return EFI_FAILURE;
    }

    (void)SYSTEM_TABLE_GLOBAL->con_out().clear_screen();

    print_ownership();
    print_uefi_info();

    const auto memory_map = MemoryMap::current();
    if (memory_map.is_null()) [[unlikely]] {
      FAIL << u"Failed to retrieve the UEFI memory map." << ENDL;
      FAIL << u"This is a critical boot error. Unable to proceed with booting WesOS." << ENDL;
      return EFI_FAILURE;
    }

    memory_map->dump(NOTE);

    auto fs = Storage::create(image_handle);
    if (fs.is_null()) [[unlikely]] {
      FAIL << u"Failed to create the storage object." << ENDL;
      FAIL << u"This is a critical boot error. Unable to proceed with booting WesOS." << ENDL;
      return EFI_FAILURE;
    }

    {
      auto file = fs->open_file(u"\\WESOS\\PROTO.ELF"_u16);
      if (file.is_null()) [[unlikely]] {
        FAIL << u"Failed to open the proto-kernel file." << ENDL;
        FAIL << u"This is a critical boot error. Unable to proceed with booting WesOS." << ENDL;
        return EFI_FAILURE;
      }
    }

    /** TODO: Outline of steps to load the proto-kernel:
     *  1. [✅] Get the current memory map.
     *  2. [🚧] Create the proto-kernel config file.
     *  3. [🚧] Parse the WESOS/PROTO.ELF binary.
     *  4. [🚧] Setup the GDT.
     *  5. [🚧] Exit the boot services.
     *  6. [🚧] Jump to the proto-kernel entry point.
     */

    while (true) {
      wesos::cpu::ephemeral_pause();
    };

    return EFI_SUCCESS;
  }
}  // namespace wesos::boot::efi
