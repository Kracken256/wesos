/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-assert/Export.hh>
#include <wesos-heap/ContiguousIntrusiveHeap.hh>

using namespace wesos::heap;

SYM_EXPORT auto ContiguousIntrusiveHeap::virt_allocate(usize n_bytes,
                                                       usize align) -> NullableOwnPtr<u8> {
  /// TODO: Implement heap allocate
  return null;
}

SYM_EXPORT void ContiguousIntrusiveHeap::virt_deallocate(NullableOwnPtr<u8> ptr) {
  /// TODO: Implement heap deallocate
}
