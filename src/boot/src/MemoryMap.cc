/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <boot/Logger.hh>
#include <boot/MemoryMap.hh>

using namespace wesos;
using namespace wesos::boot::efi;

/// SHOULD: Generate documentation for these APIs
/// SHOULD: Strictly verify this code is compliant with the UEFI spec
/// SHOULD: This code has been tested on limited real hardware (but some),
///         check it on more hardware.

auto MemoryMap::initialize_memory_map_details(MemoryMap &mmap) -> bool {
  usize mmap_size = 0;
  usize map_key = 0;
  usize desc_size = 0;
  u32 desc_ver = 0;

  NOTE << u"Requesting the size of the memory map..." << ENDL;

  const auto get_mmap_api = SYSTEM_TABLE_GLOBAL->boot_services().m_get_memory_map;
  const auto status = get_mmap_api(&mmap_size, nullptr, &map_key, &desc_size, &desc_ver);

  if (status != EFI_BUFFER_TOO_SMALL && status != EFI_SUCCESS) [[unlikely]] {
    FAIL << u"Unable to determine the size of the UEFI memory map." << ENDL;
    return false;
  }

  NOTE << u"The minimum buffer size needed to hold the UEFI memory map is " << mmap_size << u" bytes." << ENDL;

  { /**
     * Double the memory map size to reduce the likehood of a
     * UEFI TOCTOU bug manifesting.
     */

    mmap_size *= 2;
    NOTE << u"Doubled the UEFI memory map buffer size from " << mmap_size / 2 << u" bytes to " << mmap_size
         << u" bytes." << ENDL;
  }

  INFO << u"The UEFI memory map buffer size is " << mmap_size << u" bytes." << ENDL;

  mmap.m_mmap = nullptr;
  mmap.m_mmap_size = mmap_size;
  mmap.m_desc_count = 0;
  mmap.m_desc_size = desc_size;
  mmap.m_desc_version = desc_ver;

  return true;
}

auto MemoryMap::create_memory_map_buffer(MemoryMap &mmap) -> bool {
  const auto allocate_pool_api = SYSTEM_TABLE_GLOBAL->boot_services().m_allocate_pool;

  NOTE << u"Allocating " << mmap.m_mmap_size << u" bytes for the UEFI memory map..." << ENDL;

  void *desc_array = nullptr;
  if (allocate_pool_api(MemoryType::LoaderData, mmap.m_mmap_size, &desc_array) != EFI_SUCCESS) {
    FAIL << u"Unable to allocate " << mmap.m_mmap_size << u" bytes for the UEFI memory map." << ENDL;
    return false;
  }

  INFO << u"Allocated " << mmap.m_mmap_size << u" bytes for the UEFI memory map." << ENDL;

  mmap.m_mmap = static_cast<MemoryDescriptor *>(desc_array);

  return true;
}

auto MemoryMap::retrieve_memory_map(MemoryMap &mmap) -> bool {
  NOTE << u"Retrieving the UEFI memory map..." << ENDL;

  usize map_key = 0;
  const auto get_mmap_api = SYSTEM_TABLE_GLOBAL->boot_services().m_get_memory_map;
  const auto status = get_mmap_api(&mmap.m_mmap_size, mmap.m_mmap, &map_key, &mmap.m_desc_size, &mmap.m_desc_version);

  if (status != EFI_SUCCESS) {
    FAIL << u"Failed to retrive the UEFI memory map." << ENDL;
    return false;
  }

  mmap.m_desc_count = mmap.m_mmap_size / mmap.m_desc_size;

  NOTE << u"UEFI memory map descriptor size: " << mmap.m_desc_size << u" bytes." << ENDL;
  NOTE << u"UEFI memory map descriptor count: " << mmap.m_desc_count << u"." << ENDL;
  NOTE << u"UEFI memory map descriptor version: " << mmap.m_desc_version << u"." << ENDL;

  INFO << u"Successfully retrieved the UEFI memory map." << ENDL;

  return true;
}

MemoryMap::MemoryMap(MemoryMap &&o)
    : m_mmap(o.m_mmap), m_desc_count(o.m_desc_count), m_desc_size(o.m_desc_size), m_desc_version(o.m_desc_version) {
  o.m_mmap = nullptr;
  o.m_desc_count = 0;
  o.m_desc_size = 0;
  o.m_desc_version = 0;
}

auto MemoryMap::operator=(MemoryMap &&o) -> MemoryMap & {
  if (this != &o) [[likely]] {
    m_mmap = o.m_mmap;
    m_desc_count = o.m_desc_count;
    m_desc_size = o.m_desc_size;
    m_desc_version = o.m_desc_version;

    o.m_mmap = nullptr;
    o.m_desc_count = 0;
    o.m_desc_size = 0;
    o.m_desc_version = 0;
  }

  return *this;
}

MemoryMap::~MemoryMap() {
  if (m_mmap != nullptr) {
    SYSTEM_TABLE_GLOBAL->boot_services().m_free_pool(m_mmap);
  }
}

auto MemoryMap::current() -> Nullable<MemoryMap> {
  const auto max_failure_count = 10;

  usize failure_count = 0;

  while (failure_count < max_failure_count) {
    NOTE << u"Requesting the system memory map..." << ENDL;

    MemoryMap mmap;

    if (!initialize_memory_map_details(mmap)) [[unlikely]] {
      FAIL << u"Failed to get the buffer size for the memory map." << ENDL;
      failure_count++;
      continue;
    }

    if (!create_memory_map_buffer(mmap)) [[unlikely]] {
      FAIL << u"Failed to allocate memory for the memory map." << ENDL;
      failure_count++;
      continue;
    }

    if (!retrieve_memory_map(mmap)) [[unlikely]] {
      FAIL << u"Failed to populate the memory map." << ENDL;
      failure_count++;
      continue;
    }

    return mmap;
  }

  FAIL << u"Unable to get the UEFI memory map after " << failure_count << u" attempts." << ENDL;

  return nullptr;
}
