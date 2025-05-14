/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-builtin/Memory.hh>

TEST(wesos_builtin, memcpy) {
  constexpr std::string_view data = "Hello, World!";

  std::string str1(data);
  std::string str2(data.size(), '\0');

  auto result = wesos::memcpy(str2.data(), str1.data(), str1.size());
  EXPECT_EQ(result, str2.data());
  EXPECT_EQ(str2, str1);
  EXPECT_EQ(str2.size(), str1.size());
  EXPECT_EQ(str2.capacity(), str1.capacity());
}

TEST(wesos_builtin, memmove) {
  constexpr std::string_view data = "Hello, World!";

  std::string str1(data);
  std::string str2(data.size(), '\0');

  auto result = wesos::memmove(str2.data(), str1.data(), str1.size());
  EXPECT_EQ(result, str2.data());
  EXPECT_EQ(str2, str1);
  EXPECT_EQ(str2.size(), str1.size());
  EXPECT_EQ(str2.capacity(), str1.capacity());
}

TEST(wesos_builtin, memset) {
  constexpr size_t length = 14;

  std::string str;
  str.resize(length);

  auto *result = wesos::memset(str.data(), 'A', length);
  EXPECT_EQ(result, str.data());
  EXPECT_EQ(str, std::string(length, 'A'));
  EXPECT_EQ(str.size(), length);
}

TEST(wesos_builtin, memcmp) {
  constexpr std::string_view data1 = "Hello, World!";
  constexpr std::string_view data2 = "Hello, World!";

  std::string str1(data1);
  std::string str2(data2);

  auto result = wesos::memcmp(str1.data(), str2.data(), str1.size());
  EXPECT_EQ(result, 0);
  EXPECT_EQ(str1, str2);
  EXPECT_EQ(str1.size(), str2.size());
}

TEST(wesos_builtin, memcmp2) {
  constexpr std::string_view data1 = "Hello, World!";
  constexpr std::string_view data2 = "Aello, World!";

  std::string str1(data1);
  std::string str2(data2);

  auto result = wesos::memcmp(str1.data(), str2.data(), str1.size());
  EXPECT_GE(result, 1);
  EXPECT_NE(str1, str2);
  EXPECT_EQ(str1.size(), str2.size());
}
