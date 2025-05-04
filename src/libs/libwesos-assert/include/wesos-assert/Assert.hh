/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-builtin/SourceLocation.hh>

namespace wesos::assert {
  using OutputCallback = void (*)(void* m, const char* message, SourceLocation source);
  using AbortCallback = void (*)(void* m);

  void register_output_callback(void* m, OutputCallback cb);
  void register_abort_callback(void* m, AbortCallback cb);

  void assert_failure(const char* message, SourceLocation source);
}  // namespace wesos::assert

// NOLINTBEGIN(readability-identifier-naming)

#define always_assert(...)                                                                                \
  [](bool eval, const char* eval_code, wesos::SourceLocation source = wesos::SourceLocation::current()) { \
    if (!eval) [[unlikely]] {                                                                             \
      wesos::assert::assert_failure(eval_code, source);                                                   \
    }                                                                                                     \
  }((__VA_ARGS__), #__VA_ARGS__)

#ifdef NDEBUG
#define assert_invariant(...)
#else
#define assert_invariant(...) always_assert(__VA_ARGS__)
#endif

// NOLINTEND(readability-identifier-naming)
