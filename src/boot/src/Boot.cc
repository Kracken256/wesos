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

  struct MemoryMap {
    MemoryDescriptor *m_mmap;
    usize m_desc_count;
    usize m_desc_size;
    u32 m_desc_version;
  };

  [[nodiscard]] static auto get_mmap_nature() -> Nullable<MemoryMap> {
    const auto get_mmap_api = SYSTEM_TABLE_GLOBAL->boot_services().m_get_memory_map;

    usize mmap_size = 0;
    usize desc_key = 0;
    usize desc_size = 0;
    u32 desc_ver = 0;

    switch (get_mmap_api(&mmap_size, nullptr, &desc_key, &desc_size, &desc_ver)) {
      case EFI_SUCCESS:
      case EFI_BUFFER_TOO_SMALL: {
        INFO << u"The minimum buffer size needed to hold the memory map is " << mmap_size << u" bytes." << ENDL;
        break;
      }

      default: {
        FAIL << u"The UEFI firmware was unable to determine the size of the memory map." << ENDL;
        return null;
      }
    }

    // Double the memory map size to minimize the chances
    // that the reported minimum buffer size becomes inadequate
    // due to changes in the memory map (by firmware threads?).
    mmap_size *= 2;
    NOTE << u"Doubled the expected memory map size from " << mmap_size / 2 << u" bytes to " << mmap_size << u" bytes."
         << ENDL;

    INFO << u"Successfully retrieved the memory map size of " << mmap_size << u" bytes." << ENDL;

    return MemoryMap{
        .m_mmap = nullptr,
        .m_desc_count = mmap_size / desc_size,
        .m_desc_size = desc_size,
        .m_desc_version = desc_ver,
    };
  }

  [[nodiscard]] static auto create_mmap_storage(usize desc_count, usize desc_size,
                                                u32 mmap_version) -> Nullable<MemoryMap> {
    const auto allocate_pool_api = SYSTEM_TABLE_GLOBAL->boot_services().m_allocate_pool;
    const auto mmap_size = desc_count * desc_size;

    void *desc_array = nullptr;
    if (allocate_pool_api(MemoryType::LoaderData, mmap_size, &desc_array) != EFI_SUCCESS) {
      FAIL << u"The UEFI firmware failed to allocate " << mmap_size << u" bytes to hold the memory map." << ENDL;
      return null;
    }

    INFO << u"Allocated " << mmap_size << u" bytes for the memory map." << ENDL;

    return MemoryMap{
        .m_mmap = static_cast<MemoryDescriptor *>(desc_array),
        .m_desc_count = desc_count,
        .m_desc_size = desc_size,
        .m_desc_version = mmap_version,
    };
  }

  [[nodiscard]] static auto get_mmap_filled(MemoryMap mmap) -> bool {
    const auto mmap_api = SYSTEM_TABLE_GLOBAL->boot_services().m_get_memory_map;

    usize mmap_size = mmap.m_desc_count * mmap.m_desc_size;
    if (mmap_api(&mmap_size, mmap.m_mmap, &mmap.m_desc_count, &mmap.m_desc_size, &mmap.m_desc_version) != EFI_SUCCESS) {
      FAIL << u"Failed to get the memory map." << ENDL;
      return false;
    }

    NOTE << u"Memory map array filled with " << mmap_size << u" bytes." << ENDL;
    NOTE << u"Memory map descriptor size: " << mmap.m_desc_size << u" bytes." << ENDL;
    NOTE << u"Memory map descriptor count: " << mmap.m_desc_count << u"." << ENDL;
    NOTE << u"Memory map descriptor version: " << mmap.m_desc_version << u"." << ENDL;

    INFO << u"Successfully retrieved the memory map." << ENDL;

    return true;
  }

  [[nodiscard]] static auto get_memory_map() -> Nullable<MemoryMap> {
    const auto max_failure_count = 10;
    usize failure_count = 0;

    while (failure_count < max_failure_count) {
      NOTE << u"Requesting the system memory map..." << ENDL;

      auto mmap = MemoryMap{nullptr, 0, 0, 0};

      if (const auto result = get_mmap_nature()) {
        mmap = result.value();
      } else {
        FAIL << u"Failed to get the buffer size for the memory map." << ENDL;
        failure_count++;
        continue;
      }

      if (const auto mmap_storage = create_mmap_storage(mmap.m_desc_count, mmap.m_desc_size, mmap.m_desc_version)) {
        mmap = mmap_storage.value();
      } else {
        FAIL << u"Failed to allocate memory for the memory map." << ENDL;
        failure_count++;
        continue;
      }

      if (!get_mmap_filled(mmap)) {
        failure_count++;

        {
          INFO << u"Freeing memory map array..." << ENDL;
          SYSTEM_TABLE_GLOBAL->boot_services().m_free_pool(mmap.m_mmap);
          INFO << u"Memory map array freed." << ENDL;
        }

        continue;
      }

      return mmap;

      //   {
      //     INFO << u"Freeing memory map array..." << ENDL;
      //     SYSTEM_TABLE_GLOBAL->boot_services().m_free_pool(memory_map_array);
      //     INFO << u"Memory map array freed." << ENDL;
      //   }

      //   failure_count++;
      //   continue;
      // }

      // NOTE << u"Memory map array filled with " << desc_array_size << u" bytes." << ENDL;
      // NOTE << u"Memory map descriptor size: " << desc_size << u" bytes." << ENDL;
      // NOTE << u"Memory map descriptor count: " << desc_array_size / desc_size << u"." << ENDL;
      // NOTE << u"Memory map descriptor version: " << desc_version << u"." << ENDL;
      // INFO << u"Successfully retrieved the memory map." << ENDL;

      // return MemoryMap{
      //     .m_first_descriptor = static_cast<MemoryDescriptor *>(memory_map_array),
      //     .m_desc_count = desc_array_size / desc_size,
      //     .m_desc_size = desc_size,
      // };
    }

    FAIL << u"Failed to get the memory map after " << failure_count << u" attempts." << ENDL;

    return nullptr;
  }

  SYM_EXPORT extern "C" auto efi_main(Handle image_handle, SystemTable *system_table) -> Status {
    if (!configure_uefi_globals(image_handle, system_table)) [[unlikely]] {
      FAIL << u"Failed to configure UEFI globals." << ENDL;
      return EFI_FAILURE;
    }

    (void)SYSTEM_TABLE_GLOBAL->con_out().clear_screen();

    print_ownership();
    print_uefi_info();

    const auto memory_map = get_memory_map();
    if (memory_map.is_null()) [[unlikely]] {
      return EFI_FAILURE;
    }

    // NOTE << u"Search for the proto-kernel in the EFI partition..." << ENDL;
    // INFO << u"Found the proto-kernel in the EFI partition at: " << u"/WESOS/PROTO.ELF" << ENDL;
    // NOTE << u"Loading the proto-kernel file into memory..." << ENDL;
    // NOTE << u"The proto-kernel file is 1.5MB in size." << ENDL;
    // NOTE << u"Reading the proto-kernel file into heap memory at: " << u"" << u"0x8A000000" << ENDL;
    // INFO << u"Proto-kernel file loaded into memory at: " << u"" << u"0x8A000000" << ENDL;
    // NOTE << u"Reading memory map..." << ENDL;
    // INFO << u"Memory map read successfully." << ENDL;
    // NOTE << u"Generating the ephemeral kernel config file..." << ENDL;
    // INFO << u"Kernel config file generated successfully." << ENDL;
    // NOTE << u"Exiting boot services..." << ENDL;
    // INFO << u"Boot services exited successfully." << ENDL;
    // NOTE << u"Jumping to the proto-kernel entry point..." << ENDL;

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
