/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#if __SANITIZE_ADDRESS__ || (defined(__has_feature) && __has_feature(address_sanitizer))
#include <sanitizer/asan_interface.h>
#elif !defined(__SANITIZE_ADDRESS__)
// #warning "Building memory allocator without address -fsanitize=address enabled"

#define ASAN_POISON_MEMORY_REGION(addr, size) ((void)(addr), (void)(size))
#define ASAN_UNPOISON_MEMORY_REGION(addr, size) ((void)(addr), (void)(size))
#endif

#include <wesos-alloc/FreeList.hh>
#include <wesos-builtin/Export.hh>

using namespace wesos;
using namespace wesos::mem;
using namespace wesos::alloc;

SYM_EXPORT FreeList::FreeList(View<u8> pool) { FreeList::virt_utilize(pool); }
SYM_EXPORT FreeList::~FreeList() = default;

SYM_EXPORT auto FreeList::virt_embezzle(usize max_size) -> View<u8> {
  // TODO: Implement memory embezzelment
  (void)max_size;
  return View<u8>::create_empty();
}

SYM_EXPORT auto FreeList::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  size = max(size, sizeof(Chunk));

  if (!m_front.isset() || align > alignof(Chunk)) [[unlikely]] {
    return nullptr;
  }

  assert_invariant(max(align.unwrap(), alignof(Chunk)) == alignof(Chunk));

  struct Best {
    NullableRefPtr<Chunk> m_node;
    NullableRefPtr<Chunk> m_prev;
    usize m_size = 0;
  };

  Nullable<Best> bestfit;

  for (NullableRefPtr<Chunk> current = m_front, prev = null; current.isset();
       prev = current, current = current->m_next) {
    if (current->m_size < size || current->m_size < align.unwrap() || !is_aligned_pow2(current, align)) {
      continue;
    }

    if (!bestfit.isset() || current->m_size < bestfit->m_size) {
      bestfit = Best{current, prev, current->m_size};
    }
  }

  if (!bestfit.isset()) [[unlikely]] {
    return nullptr;
  }

  const auto freenode = bestfit->m_node;
  const auto prevnode = bestfit->m_prev;
  const auto freesize = bestfit->m_size;

  return null;

  // const auto freenode = m_front;
  // ASAN_UNPOISON_MEMORY_REGION(freenode.unwrap(), size);

  // m_front = freenode->m_next;

  // const auto result = OwnPtr(bit_cast<u8*>(freenode.unwrap()));
  // assert_invariant(is_aligned_pow2(result, align));

  // return result.unwrap();
}

SYM_EXPORT void FreeList::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  const auto ptr_u8 = OwnPtr(static_cast<u8*>(ptr.unwrap()));
  const auto chunk_size_ptr = OwnPtr(bit_cast<usize*>(ptr_u8.sub(sizeof(usize))));
  assert_invariant(is_aligned_pow2(chunk_size_ptr, alignof(usize)));

  const auto chunk_size = *chunk_size_ptr;
  const auto chunk_start = ptr_u8.sub(sizeof(usize));
  const auto chunk_end = chunk_start.add(chunk_size);

  //=========================================================================================
  // TODO: Reason about this code
  //=========================================================================================

  for (NullableRefPtr<Chunk> current = m_front, prev = null; current.isset();
       prev = current, current = current->m_next) {
    u8* current_start = bit_cast<u8*>(current.unwrap());
    u8* current_end = current_start + current->m_size;

    // Check if `current` ends at the start of the chunk we are deallocating
    if (current_end == chunk_start.unwrap() - 1) {
      current->m_size += chunk_size;
    } else if (current_start == chunk_end.unwrap() + 1) {
      // TODO: Handle merging with the next chunk
      return;
    }
  }

  // If we reach here, it means we need to create a new chunk
  auto* freenode_vp = bit_cast<void*>(chunk_start.unwrap());
  const auto freenode = RefPtr<Chunk>(new (freenode_vp) Chunk());

  freenode->m_size = chunk_size;
  freenode->m_next = m_front;

  m_front = freenode;

  //=========================================================================================
}

SYM_EXPORT auto FreeList::virt_utilize(View<u8> pool) -> void {
  static_assert(sizeof(Chunk) > sizeof(usize), "Chunk size must be at least the size of a pointer");
  static_assert(alignof(Chunk) == alignof(usize), "Chunk alignment must match pointer alignment");

  /// FIXME: Keep track of discarded leading bytes
  pool.align_or_truncate_to(alignof(Chunk));
  if (pool.size() < sizeof(Chunk)) [[unlikely]] {
    return;
  }

  const auto chunk_size = pool.size();
  const auto chunk_base = OwnPtr(pool.into_ptr().get().cast_to<u8>().unwrap());
  const auto region_size = chunk_size - sizeof(usize);
  const auto region_ptr = chunk_base.add(sizeof(usize)).cast_to<void>();

  assert_invariant(is_aligned_pow2(chunk_base, alignof(Chunk)));
  assert_invariant(is_aligned_pow2(region_ptr, alignof(usize)));

  new (chunk_base.unwrap()) usize(chunk_size);

  deallocate_bytes(region_ptr, region_size, alignof(Chunk));
}
