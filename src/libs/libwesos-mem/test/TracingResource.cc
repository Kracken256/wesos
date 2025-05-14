/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <cstdarg>
#include <wesos-mem/NullResource.hh>
#include <wesos-mem/TracingResource.hh>
#include <wesos-types/Types.hh>

using namespace wesos;
using namespace wesos::mem;

TEST(wesos_mem, TracingResourceAllocate) {
  auto null_resource = NullResource();
  auto tracing_resource = TracingResource(null_resource, printf);

  testing::internal::CaptureStdout();
  EXPECT_FALSE(tracing_resource.allocate_bytes(69, 16));
  auto output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "TracingResource::allocate_bytes(69, 16) -> (nil)\n");
}

TEST(wesos_mem, TracingResourceDeallocate) {
  auto null_resource = NullResource();
  auto tracing_resource = TracingResource(null_resource, printf);

  const uptr randome_address = 0xdeadbeef;
  u8* random_invalid_ptr = bit_cast<u8*>(randome_address);
  OwnPtr<void> invalid_ptr(random_invalid_ptr);

  testing::internal::CaptureStdout();
  tracing_resource.deallocate_bytes(invalid_ptr, 69, 16);
  auto output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "TracingResource::deallocate_bytes(0xdeadbeef, 69, 16)\n");
}

TEST(wesos_mem, TracingResourceUtilize) {
  auto null_resource = NullResource();
  auto tracing_resource = TracingResource(null_resource, printf);

  testing::internal::CaptureStdout();
  tracing_resource.utilize_bytes(View<u8>::create_empty());
  auto output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "TracingResource::utilize_bytes((nil), 0)\n");
}
