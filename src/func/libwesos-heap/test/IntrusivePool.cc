/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-heap/IntrusivePool.hh>

/// TODO:

TEST(IntrusivePool, CreatePool) {
  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> initial_pool(buf.data(), buf.size());

  auto heap = heap::IntrusivePool(sizeof(int), alignof(int), initial_pool);
}

TEST(IntrusivePool, Allocate) {
  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> initial_pool(buf.data(), buf.size());

  auto heap = heap::IntrusivePool(sizeof(int), alignof(int), initial_pool);

  EXPECT_EQ(heap.allocate(sizeof(int)), null);
}
