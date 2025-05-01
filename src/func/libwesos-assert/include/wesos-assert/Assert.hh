/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  namespace assert {
    using OutputCallback = void (*)(void* m, const char* message, const char* func_name,
                                    const char* file_name, int line);
    using AbortCallback = void (*)(void* m);

    void register_output_callback(void* m, OutputCallback cb);
    void register_abort_callback(void* m, AbortCallback cb);
  }  // namespace assert

  void always_assert_impl(bool cond, const char* cond_str, int line, const char* file_name,
                          const char* func_name);

  // NOLINTBEGIN(readability-identifier-naming)

#define always_assert(...)                                                                         \
  do {                                                                                             \
    wesos::always_assert_impl(__VA_ARGS__, #__VA_ARGS__, __LINE__, __FILE__, __PRETTY_FUNCTION__); \
  } while (0)

#ifdef NDEBUG
#define assert_invariant(...) \
  do {                        \
    (void)(__VA_ARGS__);      \
  } while (0)
#else
#define assert_invariant(...)                                                                      \
  do {                                                                                             \
    wesos::always_assert_impl(__VA_ARGS__, #__VA_ARGS__, __LINE__, __FILE__, __PRETTY_FUNCTION__); \
  } while (0)
#endif

  // NOLINTEND(readability-identifier-naming)

}  // namespace wesos
