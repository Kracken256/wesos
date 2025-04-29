/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-assert/Assert.hh>

TEST(always_assert, call) {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  // Test that always_assert does not abort when the condition is true
  EXPECT_NO_FATAL_FAILURE(wesos::always_assert(true, "This should not fail"));

  // Test that always_assert aborts when the condition is false
  EXPECT_DEATH(wesos::always_assert(false, "This should fail"), "This should fail");
}
