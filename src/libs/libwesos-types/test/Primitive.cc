/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Types.hh>

using namespace wesos::types;
using namespace wesos::types::detail;

TEST(wesos_types, U8) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U8), 1);
  EXPECT_EQ(sizeof(u8), 1);
  EXPECT_EQ(alignof(u8), 1);
}

TEST(wesos_types, U16) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U16), 2);
  EXPECT_EQ(sizeof(u16), 2);
  EXPECT_EQ(alignof(u16), 2);
}

TEST(wesos_types, U32) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U32), 4);
  EXPECT_EQ(sizeof(u32), 4);
  EXPECT_EQ(alignof(u32), 4);
}

TEST(wesos_types, U64) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U64), 8);
  EXPECT_EQ(sizeof(u64), 8);
  EXPECT_EQ(alignof(u64), 8);
}

TEST(wesos_types, I8) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I8), 1);
  EXPECT_EQ(sizeof(i8), 1);
  EXPECT_EQ(alignof(i8), 1);
}

TEST(wesos_types, I16) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I16), 2);
  EXPECT_EQ(sizeof(i16), 2);
  EXPECT_EQ(alignof(i16), 2);
}

TEST(wesos_types, I32) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I32), 4);
  EXPECT_EQ(sizeof(i32), 4);
  EXPECT_EQ(alignof(i32), 4);
}

TEST(wesos_types, I64) {
  EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I64), 8);
  EXPECT_EQ(sizeof(i64), 8);
  EXPECT_EQ(alignof(i64), 8);
}
