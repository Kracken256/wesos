/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class IntrusivePool final : public MemoryResourceProtocol {
    struct FreeNode {
      NullableRefPtr<FreeNode> m_next;
    };

  public:
    using ObjectSize = ClampLeast<usize, sizeof(FreeNode)>;

  private:
    NullableRefPtr<FreeNode> m_front;
    ObjectSize m_object_size;
    PowerOfTwo<usize> m_object_align;
    View<u8> m_initial_pool;

    [[nodiscard]] constexpr auto object_size() const { return m_object_size.unwrap(); }
    [[nodiscard]] constexpr auto object_align() const { return m_object_align.unwrap(); }

    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    auto virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void override;
    auto virt_utilize(View<u8> pool) -> void override;

  public:
    IntrusivePool(ObjectSize object_size, PowerOfTwo<usize> object_align, View<u8> pool = View<u8>::create_empty());
    IntrusivePool(const IntrusivePool&) = delete;
    IntrusivePool(IntrusivePool&&) = delete;
    auto operator=(const IntrusivePool&) -> IntrusivePool& = delete;
    auto operator=(IntrusivePool&&) -> IntrusivePool& = delete;
    ~IntrusivePool() override;

    [[nodiscard]] static constexpr auto minimum_size() { return sizeof(FreeNode); }
    [[nodiscard]] static constexpr auto minimum_alignment() { return alignof(FreeNode); }
  };
}  // namespace wesos::mem
