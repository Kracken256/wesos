/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <sanitizer/asan_interface.h>

#include <wesos-builtin/Export.hh>
#include <wesos-builtin/Range.hh>
#include <wesos-mem/IntrusiveChainFirstFit.hh>

using namespace wesos;
using namespace wesos::mem;

extern "C" {  /// TODO: Remove section
struct FILE;

auto printf(const char*, ...) -> int;
auto fflush(FILE*) -> int;

extern FILE* stdout;  // NOLINT

static void debug_s(const char* func, int line) {
  printf("[%s]: %d\n", func, line);
  fflush(stdout);
}

#define W_DEBUG() debug_s(__func__, __LINE__)
}

struct IntrusiveChainFirstFit::Chunk {
  Least<usize, 1> m_size;
  NullableRefPtr<Chunk> m_next;
  NullableRefPtr<Chunk> m_prev;
};

using Chunk = IntrusiveChainFirstFit::Chunk;

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(View<u8> pool)
    : m_some(nullptr), m_initial_pool(pool) {
  virt_do_utilize(pool);
}

SYM_EXPORT IntrusiveChainFirstFit::IntrusiveChainFirstFit(IntrusiveChainFirstFit&& o)
    : m_some(o.m_some), m_initial_pool(o.m_initial_pool) {
  o.m_some = nullptr;
  o.m_initial_pool.clear();
}

SYM_EXPORT auto IntrusiveChainFirstFit::operator=(IntrusiveChainFirstFit&& o)
    -> IntrusiveChainFirstFit& {
  m_some = o.m_some;
  m_initial_pool = o.m_initial_pool;

  o.m_some = nullptr;
  o.m_initial_pool.clear();

  return *this;
}

static auto create_aligned_view(View<u8> from, PowerOfTwo<usize> align) -> View<u8> {
  /// TODO: Impl
  always_assert(false);
  return {};
}

static auto get_dealigned_range(View<u8> from) -> View<u8> {
  /// TODO: Impl
  always_assert(false);
  return {};
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_allocate(usize size, PowerOfTwo<usize> align)
    -> NullableOwnPtr<u8> {
  /// TODO: Audit code

  W_DEBUG();

  for (NullableRefPtr<Chunk> node = m_some; node.isset(); node = node->m_next) {
    /// TODO: Implement allocator
  }

  W_DEBUG();

  return nullptr;
}

SYM_EXPORT void IntrusiveChainFirstFit::virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                                           PowerOfTwo<usize> align) {
  /// TODO: Audit code

  assert_invariant(ptr.is_aligned(align));

  const auto dealigned_range = get_dealigned_range(View<u8>(ptr.unwrap(), size));
  assert_invariant(dealigned_range.into_ptr().is_aligned(alignof(Chunk)));

  const RefPtr chunk = reinterpret_cast<Chunk*>(dealigned_range.into_ptr().unwrap());

  if (!m_some.isset()) {
    W_DEBUG();

    chunk->m_size = dealigned_range.size();
    chunk->m_next = chunk->m_prev = nullptr;

    m_some = chunk;

    W_DEBUG();
  } else {
    W_DEBUG();

    /// TODO: Implement reinsertion
  }

  W_DEBUG();

  ASAN_POISON_MEMORY_REGION(dealigned_range.into_ptr().unwrap(), dealigned_range.size());
}

SYM_EXPORT auto IntrusiveChainFirstFit::virt_do_utilize(View<u8> pool) -> LeftoverMemory {
  if (pool.size() < sizeof(Chunk)) [[unlikely]] {
    return {{pool}, {}};
  }

  virt_do_deallocate(pool.into_ptr().take_own().get_unchecked(), pool.size(), alignof(u8));

  return {};
}
