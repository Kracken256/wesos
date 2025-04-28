/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Types.hh>

TEST(Number, U8) {
  using namespace wesos;

  EXPECT_EQ(sizeof(u8), 1);
  EXPECT_EQ(u8(0x00), 0x00);
  EXPECT_EQ(u8(0xff), 0xff);
  EXPECT_EQ(u8(45).unwrap(), 45);
  EXPECT_EQ(u8(45).cast_to<u16>().unwrap(), 45);
  EXPECT_EQ(u8(2) + u8(3) * u16(4), 2 + 3 * 4);
}

TEST(Number, U16) {
  using namespace wesos;

  EXPECT_EQ(sizeof(u16), 2);
  EXPECT_EQ(u16(0x00), 0x00);
  EXPECT_EQ(u16(0xffff), 0xffff);
  EXPECT_EQ(u16(45).unwrap(), 45);
  EXPECT_EQ(u16(0xdead).trunc_to<u8>(), 0xad);
  EXPECT_EQ(u16(0xdead).cast_to<u32>().unwrap(), 0xdead);
  EXPECT_EQ(u16(2) + u16(3) * u32(4), 2 + 3 * 4);
}

TEST(Number, U32) {
  using namespace wesos;

  EXPECT_EQ(sizeof(u32), 4);
  EXPECT_EQ(u32(0x00), 0x00);
  EXPECT_EQ(u32(0xffffffff), 0xffffffff);
  EXPECT_EQ(u32(45).unwrap(), 45);
  EXPECT_EQ(u32(0xdeadbeef).trunc_to<u8>(), 0xef);
  EXPECT_EQ(u32(0xdeadbeef).cast_to<u64>().unwrap(), 0xdeadbeef);
  EXPECT_EQ(u32(2) + u32(3) * u64(4), 2 + 3 * 4);
}

TEST(Number, U64) {
  using namespace wesos;

  EXPECT_EQ(sizeof(u64), 8);
  EXPECT_EQ(u64(0x00), 0x00);
  EXPECT_EQ(u64(0xffffffffffffffff), 0xffffffffffffffff);
  EXPECT_EQ(u64(45).unwrap(), 45);
  EXPECT_EQ(u64(0xdeadbeefc0ffee69).trunc_to<u8>(), 0x69);
  EXPECT_EQ(u64(0xdeadbeefc0ffee69).trunc_to<u32>().unwrap(), 0xc0ffee69);
  EXPECT_EQ(u64(2) + u64(3) * u64(4), 2 + 3 * 4);
}

TEST(Number, I8) {
  using namespace wesos;

  EXPECT_EQ(sizeof(i8), 1);
  EXPECT_EQ(i8(-0x80), -0x80);
  EXPECT_EQ(i8(0x7f), 0x7f);
  EXPECT_EQ(i8(45).unwrap(), 45);
  EXPECT_EQ(i8(-45).cast_to<i16>().unwrap(), -45);
  EXPECT_EQ(i8(-45).unsafe_cast<__u8>(), 0xd3);
  EXPECT_EQ(i8(2) + i8(3) * i16(4), 2 + 3 * 4);
}

TEST(Number, I16) {
  using namespace wesos;

  EXPECT_EQ(sizeof(i16), 2);
  EXPECT_EQ(i16(-0x8000), -0x8000);
  EXPECT_EQ(i16(0x7fff), 0x7fff);
  EXPECT_EQ(i16(45).unwrap(), 45);
  EXPECT_EQ(i16(-0x1234).trunc_to<i8>().unwrap(), -52);
  EXPECT_EQ(i16(-4545).cast_to<i32>().unwrap(), -4545);
  EXPECT_EQ(i16(2) + i16(3) * i32(4), 2 + 3 * 4);
}

TEST(Number, I32) {
  using namespace wesos;

  EXPECT_EQ(sizeof(i32), 4);
  EXPECT_EQ(i32(-0x8000000), -0x8000000);
  EXPECT_EQ(i32(0x7fffffff), 0x7fffffff);
  EXPECT_EQ(i32(45).unwrap(), 45);
  EXPECT_EQ(i32(-0x12345678).trunc_to<i8>().unwrap(), -120);
  EXPECT_EQ(i32(-4545).cast_to<i64>().unwrap(), -4545);
  EXPECT_EQ(i32(2) + i32(3) * i64(4), 2 + 3 * 4);
}

TEST(Number, I64) {
  using namespace wesos;

  EXPECT_EQ(sizeof(i64), 8);
  EXPECT_EQ(i64(-0x800000000000000), -0x800000000000000);
  EXPECT_EQ(i64(0x7fffffffffffffff), 0x7fffffffffffffff);
  EXPECT_EQ(i64(45).unwrap(), 45);
  EXPECT_EQ(i64(-0x123456789abcdef0).trunc_to<i8>().unwrap(), 16);
  EXPECT_EQ(i64(-4545).trunc_to<i32>().unwrap(), -4545);
  EXPECT_EQ(i64(2) + i64(3) * i64(4), 2 + 3 * 4);
}
