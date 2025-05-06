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
  class AtomicMemoryEconomy final {
    friend class MemoryResourceProtocol;

    sync::SpinLock m_lock;

    [[nodiscard]] auto allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void>;
    void deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align);
    auto utilize(View<u8> pool) -> void;

    auto add_resource(MemoryResourceProtocol& child) -> void;
    auto remove_resource(MemoryResourceProtocol& child) -> void;

  public:
    AtomicMemoryEconomy() = default;
    AtomicMemoryEconomy(const AtomicMemoryEconomy&) = delete;
    AtomicMemoryEconomy(AtomicMemoryEconomy&&) = delete;
    auto operator=(const AtomicMemoryEconomy&) -> AtomicMemoryEconomy& = delete;
    auto operator=(AtomicMemoryEconomy&&) -> AtomicMemoryEconomy& = delete;
    ~AtomicMemoryEconomy() = default;
  };
}  // namespace wesos::mem
