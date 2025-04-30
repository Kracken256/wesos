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
  class IntrusivePool : public HeapProtocol {
    struct FreeNode {
      NullableRefPtr<FreeNode> m_next;
    };

    NullableRefPtr<FreeNode> m_freelist_head;
    Least<usize, 1> m_object_size;
    Least<usize, 1> m_object_align;

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate(Least<usize, 0> size, Least<usize, 1> align)
        -> Nullable<View<u8>> override;

    void virt_deallocate(View<u8> ptr) override;

  public:
    IntrusivePool(Least<usize, 1> object_size, Least<usize, 1> object_align,
                  View<u8> initial_pool = View<u8>::create_empty());
    IntrusivePool(const IntrusivePool&) = delete;
    IntrusivePool(IntrusivePool&&);
    auto operator=(const IntrusivePool&) -> IntrusivePool& = delete;
    auto operator=(IntrusivePool&&) -> IntrusivePool&;
    ~IntrusivePool() override = default;

    [[nodiscard]] constexpr auto operator<=>(const IntrusivePool&) const = default;

    void utilize_memory(View<u8> chunk);
  };
}  // namespace wesos::heap
