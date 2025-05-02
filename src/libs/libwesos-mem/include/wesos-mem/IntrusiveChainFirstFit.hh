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
  class IntrusiveChainFirstFit final : public MemoryResourceProtocol {
  public:
    struct Chunk {
      Least<usize, 1> m_size;
      NullableRefPtr<Chunk> m_next;
      NullableRefPtr<Chunk> m_prev;
    };

  private:
    NullableRefPtr<Chunk> m_some;
    View<u8> m_initial_pool;

    auto virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<u8> override;
    auto virt_do_deallocate(OwnPtr<u8> ptr, usize size, PowerOfTwo<usize> align) -> void override;
    auto virt_do_utilize(View<u8> pool) -> LeftoverMemory override;

  public:
    IntrusiveChainFirstFit(View<u8> pool = View<u8>::create_empty());
    IntrusiveChainFirstFit(const IntrusiveChainFirstFit&) = delete;
    IntrusiveChainFirstFit(IntrusiveChainFirstFit&&);
    auto operator=(const IntrusiveChainFirstFit&) -> IntrusiveChainFirstFit& = delete;
    auto operator=(IntrusiveChainFirstFit&&) -> IntrusiveChainFirstFit&;
    ~IntrusiveChainFirstFit() override = default;

    [[nodiscard]] constexpr auto operator<=>(const IntrusiveChainFirstFit&) const = default;
  };
}  // namespace wesos::mem
