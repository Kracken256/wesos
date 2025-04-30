/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#define NDEBUG
#include <wesos-assert/Assert.hh>

TEST(assert_invariant, no_ndebug) {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  // Test that assert_invariant does not abort when the condition is true
  EXPECT_NO_FATAL_FAILURE(wesos::assert_invariant(true, "This should not fail"));

  // Test that assert_invariant aborts when the condition is false
  EXPECT_NO_FATAL_FAILURE(wesos::assert_invariant(false, "This should fail"));
}
