/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/NullResource.hh>

using namespace wesos;
using namespace wesos::mem;

SYM_EXPORT NullResource::NullResource() = default;

SYM_EXPORT NullResource::~NullResource() = default;

SYM_EXPORT auto NullResource::virt_embezzle(usize max_size) -> View<u8> {
  (void)max_size;
  return View<u8>::create_empty();
}

SYM_EXPORT auto NullResource::virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> {
  (void)size;
  (void)align;
  return nullptr;
};

SYM_EXPORT void NullResource::virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
  (void)ptr;
  (void)size;
  (void)align;
};

SYM_EXPORT auto NullResource::virt_utilize(View<u8> pool) -> void { (void)pool; };
