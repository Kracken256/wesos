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

#ifdef NDEBUG
  static constexpr void assert_invariant(bool condition, const char* message = nullptr) {
    (void)condition;
    (void)message;
  }
#else
  static constexpr void assert_invariant(bool condition, const char* message = nullptr) {
    always_assert(condition, message);
  }
#endif

}  // namespace wesos
