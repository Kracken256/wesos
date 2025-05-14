/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#undef NDEBUG
#define NDEBUG

#include <gtest/gtest.h>

#include <wesos-assert/Assert.hh>

static void output_callback(void*, const char* message, wesos::SourceLocation source) {
  std::cerr << "\n==========================================================================="
               "===========\n"
            << "| Assertion Failed: \"" << message << "\";\n"
            << "| Function: [" << source.function_name() << "]: " << source.line_number() << ";\n"
            << "| File: \"" << source.file_name() << "\";\n"
            << "============================================================================="
               "=========\n"
            << std::endl;
}

static void abort_callback(void*) {
  std::cerr << "Aborting...\n";

  // This will flush the code coverage buffer
  std::exit(1);  // NOLINT(concurrency-mt-unsafe)
}

TEST(wesos_assert, nodebug_assert_always) {
  wesos::assert::register_output_callback(nullptr, output_callback);
  wesos::assert::register_abort_callback(nullptr, abort_callback);

  EXPECT_NO_FATAL_FAILURE(assert_always(true && "This should not fail"));
  EXPECT_NO_FATAL_FAILURE(assert_always(10 && "This should not fail"));
  EXPECT_DEATH(assert_always(false && "This should fail"), "This should fail");
}

TEST(wesos_assert, nodebug_assert_invariant_debug) {
  wesos::assert::register_output_callback(nullptr, output_callback);
  wesos::assert::register_abort_callback(nullptr, abort_callback);

  EXPECT_NO_FATAL_FAILURE(assert_invariant(true && "This should not fail"));
  EXPECT_NO_FATAL_FAILURE(assert_invariant(false && "This should fail"));
  EXPECT_DEATH(assert_always(false && "This should fail"), "This should fail");
}

TEST(wesos_assert, nodebug_null_output_callback) {
  wesos::assert::register_output_callback(nullptr, nullptr);
  wesos::assert::register_abort_callback(nullptr, abort_callback);

  EXPECT_NO_FATAL_FAILURE(assert_invariant(true && "This should not fail"));
  EXPECT_NO_FATAL_FAILURE(assert_invariant(false && "This should fail"));
  EXPECT_DEATH(assert_always(false && "This should fail"), "");
}

TEST(wesos_assert, nodebug_null_abort_callback) {
  wesos::assert::register_output_callback(nullptr, output_callback);
  wesos::assert::register_abort_callback(nullptr, nullptr);

  EXPECT_NO_FATAL_FAILURE(assert_invariant(true && "This should not fail"));
  EXPECT_NO_FATAL_FAILURE(assert_invariant(false && "This should fail"));
  EXPECT_DEATH(assert_always(false && "This should fail"), "This should fail");
}
