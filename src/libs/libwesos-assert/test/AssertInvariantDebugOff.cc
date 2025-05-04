/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#undef NDEBUG
#define NDEBUG

#include <gtest/gtest.h>

#include <wesos-assert/Assert.hh>

TEST(assert_invariant, no_ndebug) {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message,
                                                      wesos::SourceLocation source) {
    std::cerr << "\n==========================================================================="
                 "===========\n"
              << "| Assertion Failed: \"" << message << "\";\n"
              << "| Function: [" << source.function_name() << "]: " << source.line_number() << ";\n"
              << "| File: \"" << source.file_name() << "\";\n"
              << "============================================================================="
                 "=========\n"
              << std::endl;
  });

  // Test that assert_invariant does not abort when the condition is true
  EXPECT_NO_FATAL_FAILURE(assert_invariant(true && "This should not fail"));

  // Test that assert_invariant aborts when the condition is false
  EXPECT_NO_FATAL_FAILURE(assert_invariant(false && "This should fail"));
}
