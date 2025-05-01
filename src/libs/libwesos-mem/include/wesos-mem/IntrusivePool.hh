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

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate_bytes(Least<usize, 0> size, PowerOfTwo<usize> align)
        -> Nullable<View<u8>> override;

    void virt_deallocate_bytes(View<u8> ptr) override;

    auto virt_utilize_bytes(View<u8> pool) -> LeftoverMemory override;

    void virt_anew() override;

  public:
    IntrusivePool(ObjectSize object_size, PowerOfTwo<usize> object_align,
                  View<u8> pool = View<u8>::create_empty());
    IntrusivePool(const IntrusivePool&) = delete;
    IntrusivePool(IntrusivePool&&);
    auto operator=(const IntrusivePool&) -> IntrusivePool& = delete;
    auto operator=(IntrusivePool&&) -> IntrusivePool&;
    ~IntrusivePool() override = default;

    [[nodiscard]] constexpr auto operator<=>(const IntrusivePool&) const = default;

    [[nodiscard]] static constexpr auto minimum_size() { return sizeof(FreeNode); }
    [[nodiscard]] static constexpr auto minimum_alignment() { return alignof(FreeNode); }
  };
}  // namespace wesos::mem
