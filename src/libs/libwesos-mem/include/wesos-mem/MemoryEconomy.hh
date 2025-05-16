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
  class MemoryEconomy final {
    friend class MemoryResourceProtocol;

    sync::SpinLock m_lock;
    NullableRefPtr<MemoryResourceProtocol> m_front;

    auto add_resource(MemoryResourceProtocol& child) -> void;
    auto remove_resource(MemoryResourceProtocol& child) -> void;

  public:
    MemoryEconomy();
    MemoryEconomy(const MemoryEconomy&) = delete;
    MemoryEconomy(MemoryEconomy&&) = delete;
    auto operator=(const MemoryEconomy&) -> MemoryEconomy& = delete;
    auto operator=(MemoryEconomy&&) -> MemoryEconomy& = delete;
    ~MemoryEconomy() = default;

    [[nodiscard]] auto allocate(usize size) -> NullableOwnPtr<void>;
    auto utilize(View<u8> pool) -> void;
  };

  [[nodiscard]] auto global_memory_economy() -> MemoryEconomy&;
}  // namespace wesos::mem
