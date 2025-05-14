/*
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
    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    void virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) override;
    auto virt_utilize(View<u8> pool) -> void override;

  public:
    NullResource();
    NullResource(const NullResource&) = delete;
    NullResource(NullResource&&) = delete;
    auto operator=(const NullResource&) -> NullResource& = delete;
    auto operator=(NullResource&&) -> NullResource& = delete;
    ~NullResource() override;
  };
}  // namespace wesos::mem
