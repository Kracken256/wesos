/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-assert/Assert.hh>

TEST(always_assert, call) {
  wesos::assert::register_output_callback(
      nullptr,
      [](void*, const char* message, const char* func_name, const char* file_name, int line) {
        std::cerr << "\n==========================================================================="
                     "===========\n"
                  << "| Assertion Failed: \"" << message << "\";\n"
                  << "| Function: [" << func_name << "]: " << line << ";\n"
                  << "| File: \"" << file_name << "\";\n"
                  << "============================================================================="
                     "=========\n"
                  << std::endl;
      });

  // Test that always_assert does not abort when the condition is true
  EXPECT_NO_FATAL_FAILURE(always_assert(true && "This should not fail"));

  // Test that always_assert aborts when the condition is false
  EXPECT_DEATH(always_assert(false && "This should fail"), "This should fail");
}
