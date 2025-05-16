/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-sync/SpinLock.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class AtomicResource final : public MemoryResourceProtocol {
    MemoryResourceProtocol& m_inner;
    sync::SpinLock m_lock;

    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    void virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) override;
    auto virt_utilize(View<u8> pool) -> void override;

  public:
    AtomicResource(MemoryResourceProtocol& inner);
    AtomicResource(const AtomicResource&) = delete;
    AtomicResource(AtomicResource&&) = delete;
    auto operator=(const AtomicResource&) -> AtomicResource& = delete;
    auto operator=(AtomicResource&&) -> AtomicResource& = delete;
    ~AtomicResource() override = default;
  };
}  // namespace wesos::mem
