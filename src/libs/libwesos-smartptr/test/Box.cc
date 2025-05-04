/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-smartptr/Box.hh>

using namespace wesos;
using namespace wesos::smartptr;

TEST(Box, Create) {
  const auto buf_size = 32;
  Array<u8, buf_size> bytes;
  auto pmr = mem::IntrusivePool(sizeof(int), alignof(int), bytes.as_view());

  auto int_box = Box<int>::create(0)(pmr);

  // EXPECT_NE(int_box, null);
}
