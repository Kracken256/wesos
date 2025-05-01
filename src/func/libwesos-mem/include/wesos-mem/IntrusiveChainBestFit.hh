/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-heap/HeapProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::heap {
  class IntrusiveChainBestFit final : public HeapProtocol {
    struct Chunk {
      Least<usize, 1> m_size;
      NullableRefPtr<Chunk> m_next;
    };

    NullableRefPtr<Chunk> m_front;
    View<u8> m_initial_pool;

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate(Least<usize, 0> size, PowerOfTwo<usize> align)
        -> Nullable<View<u8>> override;

    void virt_deallocate(View<u8> ptr) override;

    auto virt_utilize(View<u8> pool) -> LeftoverMemory override;

    void virt_anew() override;

  public:
    IntrusiveChainBestFit(View<u8> pool = View<u8>::create_empty());
    IntrusiveChainBestFit(const IntrusiveChainBestFit&) = delete;
    IntrusiveChainBestFit(IntrusiveChainBestFit&&);
    auto operator=(const IntrusiveChainBestFit&) -> IntrusiveChainBestFit& = delete;
    auto operator=(IntrusiveChainBestFit&&) -> IntrusiveChainBestFit&;
    ~IntrusiveChainBestFit() override = default;

    [[nodiscard]] constexpr auto operator<=>(const IntrusiveChainBestFit&) const = default;
  };
}  // namespace wesos::heap
