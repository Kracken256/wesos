/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#define WESOS_MEM_INITIALIZE_WITH_MALLOC
#include <wesos-mem/Memory.hh>
#include <wesos-smartptr/Box.hh>

#include "Helper.hh"

using namespace wesos;
using namespace wesos::smartptr;

TEST(wesos_smartptr, Box) {
  auto init_status = mem::initialize_with_malloc();
  ASSERT_TRUE(init_status);
  auto _ = defer([&]() { std::free(init_status->unwrap()); });

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  auto box = Box<SemanticCounter>::create()(constructed, moved, copied, destructed);
  ASSERT_TRUE(box);

  ASSERT_EQ(constructed, 1);
  ASSERT_EQ(moved, 0);
  ASSERT_EQ(copied, 0);
  ASSERT_EQ(destructed, 0);

  {
    auto box2 = std::move(box.value());

    ASSERT_EQ(constructed, 1);
    ASSERT_EQ(moved, 0);
    ASSERT_EQ(copied, 0);
    ASSERT_EQ(destructed, 0);
  }

  ASSERT_EQ(constructed, 1);
  ASSERT_EQ(moved, 0);
  ASSERT_EQ(copied, 0);
  ASSERT_EQ(destructed, 1);
}
