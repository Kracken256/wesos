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
  class ContiguousIntrusiveHeap : public HeapProtocol {
    View<u8> m_bank;

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate(usize n_bytes,
                                                usize align) -> NullableOwnPtr<u8> override;

    void virt_deallocate(NullableOwnPtr<u8> ptr) override;

  public:
    constexpr ContiguousIntrusiveHeap(View<u8> mem) : m_bank(mem) {}
    constexpr ContiguousIntrusiveHeap(const ContiguousIntrusiveHeap&) = delete;
    constexpr ContiguousIntrusiveHeap(ContiguousIntrusiveHeap&&) = delete;
    constexpr auto operator=(const ContiguousIntrusiveHeap&) -> ContiguousIntrusiveHeap& = delete;
    constexpr auto operator=(ContiguousIntrusiveHeap&&) -> ContiguousIntrusiveHeap& = delete;
    constexpr auto operator<=>(const ContiguousIntrusiveHeap&) const = delete;
    ~ContiguousIntrusiveHeap() override = default;
  };
}  // namespace wesos::heap
