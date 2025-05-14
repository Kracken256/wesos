/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-builtin/SourceLocation.hh>

using namespace wesos;

TEST(wesos_builtin, SourceLocation_Consruct) {
  auto loc = SourceLocation();
  EXPECT_EQ(loc.file_name(), "");
  EXPECT_EQ(loc.function_name(), "");
  EXPECT_EQ(loc.line_number(), 0);
  EXPECT_EQ(loc.column_number(), 0);
}

TEST(wesos_builtin, SourceLocation_Move) {
  const char* file = "test_file.cc";
  const char* function = "test_function";
  const int line = 10;
  const int column = 30;

  auto loc = SourceLocation(file, function, line, column);
  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);

  auto loc2 = std::move(loc);
  EXPECT_EQ(loc2.file_name(), file);
  EXPECT_EQ(loc2.function_name(), function);
  EXPECT_EQ(loc2.line_number(), line);
  EXPECT_EQ(loc2.column_number(), column);

  // The original loc should be cleared after move
  EXPECT_EQ(loc.file_name(), "");
  EXPECT_EQ(loc.function_name(), "");
  EXPECT_EQ(loc.line_number(), 0);
  EXPECT_EQ(loc.column_number(), 0);
}

TEST(wesos_builtin, SourceLocation_MoveAssign) {
  const char* file = "test_file.cc";
  const char* function = "test_function";
  const int line = 10;
  const int column = 30;

  auto loc = SourceLocation(file, function, line, column);
  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);

  auto loc2 = SourceLocation();
  loc2 = std::move(loc);

  EXPECT_EQ(loc2.file_name(), file);
  EXPECT_EQ(loc2.function_name(), function);
  EXPECT_EQ(loc2.line_number(), line);
  EXPECT_EQ(loc2.column_number(), column);

  // The original loc should be cleared after move
  EXPECT_EQ(loc.file_name(), "");
  EXPECT_EQ(loc.function_name(), "");
  EXPECT_EQ(loc.line_number(), 0);
  EXPECT_EQ(loc.column_number(), 0);
}

TEST(wesos_builtin, SourceLocation_MoveAssign_Self) {
  const char* file = "test_file.cc";
  const char* function = "test_function";
  const int line = 10;
  const int column = 30;

  auto loc = SourceLocation(file, function, line, column);
  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
  loc = std::move(loc);
#pragma GCC diagnostic pop

  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);
}

TEST(wesos_builtin, SourceLocation_Copy) {
  const char* file = "test_file.cc";
  const char* function = "test_function";
  const int line = 10;
  const int column = 30;

  auto loc = SourceLocation(file, function, line, column);
  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);

  auto loc2 = loc;
  EXPECT_EQ(loc2.file_name(), file);
  EXPECT_EQ(loc2.function_name(), function);
  EXPECT_EQ(loc2.line_number(), line);
  EXPECT_EQ(loc2.column_number(), column);

  // The original loc should remain unchanged after copy
  EXPECT_EQ(loc.file_name(), file);
  EXPECT_EQ(loc.function_name(), function);
  EXPECT_EQ(loc.line_number(), line);
  EXPECT_EQ(loc.column_number(), column);
}

TEST(wesos_builtin, SourceLocation_Current) {
  auto loc = SourceLocation::current();
  EXPECT_EQ(loc.file_name(), __builtin_FILE());
  EXPECT_EQ(loc.function_name(), __builtin_FUNCTION());
  EXPECT_EQ(loc.line_number(), __builtin_LINE() - 3);
  EXPECT_EQ(loc.column_number(), 14);
}