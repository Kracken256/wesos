/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Types.hh>

TEST(Number, min) {
  using namespace wesos;

  constexpr i64 u8_min = 0;
  constexpr i64 u16_min = 0;
  constexpr i64 u32_min = 0;
  constexpr i64 u64_min = 0;
  constexpr i64 i8_min = -0x80LL;                                   // -128
  constexpr i64 i16_min = -0x8000LL;                                // -32768
  constexpr i64 i32_min = -0x80000000LL;                            // -2147483648
  constexpr i64 i64_min = static_cast<i64>(0x8000000000000000ULL);  // -9223372036854775808

  EXPECT_EQ(wesos::numeric_limit_min<u8>(), u8_min);
  EXPECT_EQ(wesos::numeric_limit_min<u16>(), u16_min);
  EXPECT_EQ(wesos::numeric_limit_min<u32>(), u32_min);
  EXPECT_EQ(wesos::numeric_limit_min<u64>(), u64_min);
  EXPECT_EQ(wesos::numeric_limit_min<i8>(), i8_min);
  EXPECT_EQ(wesos::numeric_limit_min<i16>(), i16_min);
  EXPECT_EQ(wesos::numeric_limit_min<i32>(), i32_min);
  EXPECT_EQ(wesos::numeric_limit_min<i64>(), i64_min);
}

TEST(Number, max) {
  using namespace wesos;

  constexpr u64 u8_max = 0xff;
  constexpr u64 u16_max = 0xffff;
  constexpr u64 u32_max = 0xffffffff;
  constexpr u64 u64_max = 0xffffffffffffffff;
  constexpr u64 i8_max = 0x7f;
  constexpr u64 i16_max = 0x7fff;
  constexpr u64 i32_max = 0x7fffffff;
  constexpr u64 i64_max = 0x7fffffffffffffff;

  EXPECT_EQ(wesos::numeric_limit_max<u8>(), u8_max);
  EXPECT_EQ(wesos::numeric_limit_max<u16>(), u16_max);
  EXPECT_EQ(wesos::numeric_limit_max<u32>(), u32_max);
  EXPECT_EQ(wesos::numeric_limit_max<u64>(), u64_max);
  EXPECT_EQ(wesos::numeric_limit_max<i8>(), i8_max);
  EXPECT_EQ(wesos::numeric_limit_max<i16>(), i16_max);
  EXPECT_EQ(wesos::numeric_limit_max<i32>(), i32_max);
  EXPECT_EQ(wesos::numeric_limit_max<i64>(), i64_max);
}
