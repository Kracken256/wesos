/**
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
  class AtomicMemoryEconomy final : public MemoryResourceProtocol {
    friend class MemoryResourceProtocol;

    sync::SpinLock m_lock;

    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    void virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) override;
    auto virt_utilize(View<u8> pool) -> void override;

    auto add_resource(MemoryResourceProtocol& child) -> void;
    auto remove_resource(MemoryResourceProtocol& child) -> void;

  public:
    AtomicMemoryEconomy() = default;
    AtomicMemoryEconomy(const AtomicMemoryEconomy&) = delete;
    AtomicMemoryEconomy(AtomicMemoryEconomy&&) = delete;
    auto operator=(const AtomicMemoryEconomy&) -> AtomicMemoryEconomy& = delete;
    auto operator=(AtomicMemoryEconomy&&) -> AtomicMemoryEconomy& = delete;
    ~AtomicMemoryEconomy() override = default;
  };
}  // namespace wesos::mem
