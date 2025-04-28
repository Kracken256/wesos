/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  namespace assert {
    using MessageCallback = void (*)(void* m, const char*);
    using AbortCallback = void (*)(void* m);

    void register_message_callback(void* m, MessageCallback cb);
    void register_abort_callback(void* m, AbortCallback cb);
  }  // namespace assert

  void always_assert(bool condition, const char* message = nullptr);

  // NOLINTBEGIN(readability-identifier-naming)

#ifdef NDEBUG
#define assert_invariant(condition, message) (void)(condition)
#else
#define assert_invariant(condition, ...) wesos::always_assert(condition __VA_ARGS__)
#endif

  // NOLINTEND(readability-identifier-naming)

}  // namespace wesos
