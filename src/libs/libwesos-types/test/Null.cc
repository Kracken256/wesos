/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Types.hh>

TEST(wesos_types, Null) {
  // Ensure proper type alias
  static_assert(wesos::types::is_same_v<decltype(wesos::types::null), const wesos::types::Null>);

  {  // Check instance identity properties
    const auto &obj = wesos::types::null;

    EXPECT_LE(sizeof(obj), 1);
    EXPECT_EQ(wesos::types::Null(), obj);
    EXPECT_FALSE(obj < obj);
    EXPECT_FALSE(obj > obj);
    EXPECT_TRUE(obj <= obj);
    EXPECT_TRUE(obj >= obj);
    EXPECT_EQ(obj, obj);
    EXPECT_FALSE(obj != obj);
  }
}
