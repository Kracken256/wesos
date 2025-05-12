/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <boot/EFI.hh>
#include <boot/Logger.hh>
#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
  class MemoryMap {
    MemoryDescriptor *m_mmap = nullptr;
    usize m_mmap_size = 0;
    usize m_desc_count = 0;
    usize m_desc_size = 0;
    u32 m_desc_version = 0;

    MemoryMap() = default;

    [[nodiscard]] static auto initialize_memory_map_details(MemoryMap &mmap) -> bool;
    [[nodiscard]] static auto create_memory_map_buffer(MemoryMap &mmap) -> bool;
    [[nodiscard]] static auto retrieve_memory_map(MemoryMap &mmap) -> bool;

  public:
    MemoryMap(MemoryMap &&o);
    MemoryMap(const MemoryMap &) = delete;
    auto operator=(MemoryMap &&o) -> MemoryMap &;
    auto operator=(const MemoryMap &) -> MemoryMap & = delete;
    ~MemoryMap();

    [[nodiscard]] auto desc_count() const -> usize { return m_desc_count; }
    [[nodiscard]] auto desc_size() const -> usize { return m_desc_size; }
    [[nodiscard]] auto desc_version() const -> u32 { return m_desc_version; }
    [[nodiscard]] auto at(usize i) const -> MemoryDescriptor * {
      const auto *u8_ptr = bit_cast<u8 *>(m_mmap);
      const auto offset = i * m_desc_size;
      assert_invariant(offset < desc_count() * desc_size());

      return bit_cast<MemoryDescriptor *>(u8_ptr + offset);
    }

    [[nodiscard]] static auto current() -> Nullable<MemoryMap>;

    void dump(auto &os) const {
      auto ss = os << u"Memory Map Dump:" << ENDL;
      for (usize i = 0; i < desc_count(); i++) {
        const auto *desc = at(i);

        ss << u"          Desc " << i << u": Ty: " << desc->type() << u", PhysAddr: " << desc->physical_start()
           << u", N: " << desc->number_of_pages() << ENDL;
      }
    }
  };
}  // namespace wesos::boot::efi
