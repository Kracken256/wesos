/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Types.hh>

using namespace wesos::types::detail;

TEST(Primitive, Empty) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::Empty), 0); }
TEST(Primitive, U8) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U8), 1); }
TEST(Primitive, U16) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U16), 2); }
TEST(Primitive, U32) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U32), 4); }
TEST(Primitive, U64) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::U64), 8); }
TEST(Primitive, I8) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I8), 1); }
TEST(Primitive, I16) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I16), 2); }
TEST(Primitive, I32) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I32), 4); }
TEST(Primitive, I64) { EXPECT_EQ(static_cast<int>(PrimitiveTypeSize::I64), 8); }
