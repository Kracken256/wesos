/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <cstdarg>
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

using namespace wesos;
using namespace wesos::mem;

TEST(wesos_mem, MemoryResourceProtocolAllocate) {
  MemoryResourceProtocol resource;
  EXPECT_FALSE(resource.allocate_bytes(1, 1));
}

TEST(wesos_mem, MemoryResourceProtocolDeallocate) {
  MemoryResourceProtocol resource;
  resource.deallocate_bytes(nullptr, 1, 1);

  const uptr randome_address = 0xdeadbeef;
  u8* random_invalid_ptr = bit_cast<u8*>(randome_address);
  OwnPtr<void> invalid_ptr(random_invalid_ptr);

  // This would be undefined behavior in a real memory resource, but we are testing
  // the MemoryResourceProtocol default implementation. It is akin to the NullResource
  // allocator.
  resource.deallocate_bytes(invalid_ptr, 1, 1);
}

TEST(wesos_mem, MemoryResourceProtocolUtilize) {
  MemoryResourceProtocol resource;
  resource.utilize_bytes(View<u8>::create_empty());
}
