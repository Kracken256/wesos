/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/Atomic.hh>
#include <wesos-sync/SpinLock.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class AtomicMemoryEconomy;

  class MemoryResourceProtocol {
    friend class TracingResource;

    sync::Atomic<bool> m_yield_requested;
    sync::Atomic<usize> m_embezzlement_request;

  protected:
    [[nodiscard]] virtual auto virt_embezzle(usize max_size) -> View<u8>;
    [[nodiscard]] virtual auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void>;
    virtual auto virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void;
    virtual auto virt_utilize(View<u8> pool) -> void;

  public:
    MemoryResourceProtocol();
    MemoryResourceProtocol(const MemoryResourceProtocol&) = delete;
    MemoryResourceProtocol(MemoryResourceProtocol&&) = delete;
    auto operator=(const MemoryResourceProtocol&) -> MemoryResourceProtocol& = delete;
    auto operator=(MemoryResourceProtocol&&) -> MemoryResourceProtocol& = delete;
    virtual ~MemoryResourceProtocol() = 0;

    ///=============================================================================================
    /// RAW MEMORY ALLOCATION
    ///=============================================================================================

    [[nodiscard]] auto allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void>;
    auto deallocate_bytes(NullableOwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void;
    auto utilize_bytes(View<u8> pool) -> void;

    ///=============================================================================================
    /// MEMORY ECONOMY MANAGEMENT
    ///=============================================================================================

    void eco_yield();
  };
}  // namespace wesos::mem
