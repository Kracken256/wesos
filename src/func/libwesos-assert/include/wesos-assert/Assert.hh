/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  namespace assert {
    using OutputCallback = void (*)(void* m, const char*);
    using AbortCallback = void (*)(void* m);

    void register_output_callback(void* m, OutputCallback cb);
    void register_abort_callback(void* m, AbortCallback cb);
  }  // namespace assert

  void always_assert(bool condition, const char* message = nullptr);

  // NOLINTBEGIN(readability-identifier-naming)

#ifdef NDEBUG
#define assert_invariant(condition, message) (void)(condition)
#else
#define assert_invariant(...) wesos::always_assert(__VA_ARGS__)
#endif

  // NOLINTEND(readability-identifier-naming)

}  // namespace wesos
