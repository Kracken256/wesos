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
  class ElementaryIntrusiveHeap : public HeapProtocol {
    struct FreeNode {
      usize m_size;
      NullableRefPtr<FreeNode> m_next;
    };

    NullableRefPtr<FreeNode> m_freelist_head;

  protected:
    [[nodiscard, gnu::pure]] auto virt_allocate(usize n_bytes,
                                                usize align) -> Nullable<View<u8>> override;

    void virt_deallocate(View<u8> ptr) override;

  public:
    ElementaryIntrusiveHeap(View<u8> chunk);
    ElementaryIntrusiveHeap(View<View<u8>> chunks);
    ElementaryIntrusiveHeap(const ElementaryIntrusiveHeap&) = delete;
    ElementaryIntrusiveHeap(ElementaryIntrusiveHeap&&) = delete;
    auto operator=(const ElementaryIntrusiveHeap&) -> ElementaryIntrusiveHeap& = delete;
    auto operator=(ElementaryIntrusiveHeap&&) -> ElementaryIntrusiveHeap& = delete;
    auto operator<=>(const ElementaryIntrusiveHeap&) const = delete;
    ~ElementaryIntrusiveHeap() override = default;

    void utilize_memory(View<u8> chunk);
  };
}  // namespace wesos::heap
