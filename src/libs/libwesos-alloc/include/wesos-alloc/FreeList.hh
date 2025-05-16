/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::alloc {
  class FreeList final : public mem::MemoryResourceProtocol {
    struct Chunk {
      static constexpr usize CHUNK_SIZE = sizeof(void*) * 2;

      usize m_size = 0;
      NullableRefPtr<Chunk> m_next;
    };

    static_assert(sizeof(Chunk) == Chunk::CHUNK_SIZE, "Chunk size mismatch");

    NullableRefPtr<Chunk> m_front;

    struct Statistics {
      usize m_total_memory_managed_bytes = 0;
      usize m_total_memory_allocated_bytes = 0;
      usize m_accumulated_allocations_bytes = 0;
      usize m_accumulated_deallocations_bytes = 0;
    } m_statistics;

    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    auto virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void override;
    auto virt_utilize(View<u8> pool) -> void override;

  public:
    FreeList(View<u8> pool = View<u8>::create_empty());
    FreeList(const FreeList&) = delete;
    FreeList(FreeList&&) = delete;
    auto operator=(const FreeList&) -> FreeList& = delete;
    auto operator=(FreeList&&) -> FreeList& = delete;
    ~FreeList() override;
  };
}  // namespace wesos::alloc
