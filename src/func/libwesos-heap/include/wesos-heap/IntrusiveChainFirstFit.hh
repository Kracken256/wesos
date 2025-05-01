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
  class IntrusiveChainFirstFit final : public HeapProtocol {
  public:
    struct Chunk {
      Least<usize, 1> m_size;
      NullableRefPtr<Chunk> m_next;
      NullableRefPtr<Chunk> m_prev;
    };

  private:
    NullableRefPtr<Chunk> m_some;
    View<u8> m_initial_pool;

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate(Least<usize, 0> size, PowerOfTwo<usize> align)
        -> Nullable<View<u8>> override;

    void virt_deallocate(View<u8> ptr) override;

    auto virt_utilize(View<u8> pool) -> LeftoverMemory override;

    void virt_anew() override;

  public:
    IntrusiveChainFirstFit(View<u8> pool = View<u8>::create_empty());
    IntrusiveChainFirstFit(const IntrusiveChainFirstFit&) = delete;
    IntrusiveChainFirstFit(IntrusiveChainFirstFit&&);
    auto operator=(const IntrusiveChainFirstFit&) -> IntrusiveChainFirstFit& = delete;
    auto operator=(IntrusiveChainFirstFit&&) -> IntrusiveChainFirstFit&;
    ~IntrusiveChainFirstFit() override = default;

    [[nodiscard]] constexpr auto operator<=>(const IntrusiveChainFirstFit&) const = default;
  };
}  // namespace wesos::heap
