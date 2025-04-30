/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-heap/ElementaryIntrusiveHeap.hh>

using namespace wesos;
using namespace wesos::heap;

SYM_EXPORT ElementaryIntrusiveHeap::ElementaryIntrusiveHeap(View<u8> chunk) {
  utilize_memory(chunk);
}

SYM_EXPORT ElementaryIntrusiveHeap::ElementaryIntrusiveHeap(View<View<u8>> chunks) {
  for (const auto& chunk : chunks) {
    utilize_memory(chunk);
  }
}

static auto align_pointer(void* ptr, usize align) -> void* {
  /// TODO: Implement this function
  return ptr;
}

static auto calculate_minimum_allowable_size(void* base, usize n_bytes, usize align) -> usize {
  /// TODO: Implement this function
  return 0;
}

SYM_EXPORT auto ElementaryIntrusiveHeap::virt_allocate(usize n_bytes,
                                                       usize align) -> Nullable<View<u8>> {
  if (!m_freelist_head.isset()) [[unlikely]] {
    return null;
  }

  NullableRefPtr<FreeNode> head = m_freelist_head;

  while (head.isset()) {
    NullableRefPtr<FreeNode> node = head;
    head = head->m_next;

    const auto min_allowed = calculate_minimum_allowable_size(node.into_raw(), n_bytes, align);

    if (node->m_size >= min_allowed) {
      /// TODO: Implement heap allocate
    }
  }

  return null;
}

SYM_EXPORT void ElementaryIntrusiveHeap::virt_deallocate(View<u8> ptr) {
  /**
   * Yeah... So, there is basically no security checks because
   * there is nothing to assert over. Oh, well...
   */

  auto free_node_view = ptr.subview(0, sizeof(FreeNode));
  auto* free_node = reinterpret_cast<FreeNode*>(free_node_view.into_ptr().into_raw());

  free_node->m_size = ptr.size();
  free_node->m_next = m_freelist_head;
  m_freelist_head = free_node;
}

SYM_EXPORT void ElementaryIntrusiveHeap::utilize_memory(View<u8> chunk) {
  if (chunk.size() < sizeof(FreeNode)) [[unlikely]] {
    return;
  }

  // Wierd, but it works..
  deallocate(chunk);
}
