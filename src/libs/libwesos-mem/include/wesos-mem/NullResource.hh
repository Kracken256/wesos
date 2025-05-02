/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class NullResource final : public MemoryResourceProtocol {
    [[nodiscard]] auto virt_do_utilize(View<u8> pool) -> LeftoverMemory override {
      return {{pool}, {}};
    };

    [[nodiscard]] auto virt_do_allocate(usize size,
                                        PowerOfTwo<usize> align) -> NullableOwnPtr<u8> override {
      (void)size;
      (void)align;
      return nullptr;
    };

    void virt_do_deallocate(OwnPtr<u8> ptr, usize size, PowerOfTwo<usize> align) override {
      (void)ptr;
      (void)size;
      (void)align;
    };

  public:
    NullResource() = default;
    NullResource(const NullResource&) = delete;
    NullResource(NullResource&&) = default;
    auto operator=(const NullResource&) -> NullResource& = delete;
    auto operator=(NullResource&&) -> NullResource& = default;
    ~NullResource() override = default;

    [[nodiscard]] constexpr auto operator<=>(const NullResource&) const = default;
  };

  extern ThreadSafe<mem::NullResource> NULL_RESOURCE_GLOBAL;
}  // namespace wesos::mem
