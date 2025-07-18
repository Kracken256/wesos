/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/Atomic.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class MemoryEconomy;

  class MemoryResourceProtocol {
    friend class MemoryEconomy;

    sync::Atomic<usize> m_eco_request_size;
    sync::Atomic<bool> m_eco_should_yield;
    NullableRefPtr<MemoryResourceProtocol> m_eco_chain_next;

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
    virtual ~MemoryResourceProtocol();

    [[nodiscard]] auto embezzle_bytes(usize max_size) -> View<u8>;
    [[nodiscard]] auto allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void>;
    auto deallocate_bytes(NullableOwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void;
    auto utilize_bytes(View<u8> pool) -> void;

    void eco_yield();
  };
}  // namespace wesos::mem
