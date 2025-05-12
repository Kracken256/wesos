
/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-builtin/SourceLocation.hh>

/**
 * @brief Provides assertion utilities for the WesOS project.
 *
 * This header defines functions and macros for handling assertions, including
 * registering custom callbacks for output and abort behavior, and macros for
 * invariant and always-assert checks.
 *
 * @namespace wesos::assert
 * Contains assertion-related utilities and callback registration functions.
 */
namespace wesos::assert {
  /**
   * @brief A callback function type for emitting assertion output.
   *
   * @param m A user-defined context pointer.
   * @param message The assertion failure message.
   * @param source The source location where the assertion failed.
   * @note The function must be thread-safe and should return.
   */
  using OutputCallback = void (*)(void* m, const char* message, SourceLocation source);

  /**
   * @brief A callback function type for doing assertion abortion.
   *
   * @param m A user-defined context pointer.
   * @note The function is expected to be marked as `noreturn`.
   * @note The function must be thread-safe and must NOT return.
   */
  using AbortCallback = /* noreturn */ void (*)(void* m);

  /**
   * @brief Registers a callback for emitting assertion output.
   *
   * @param m A user-defined context pointer.
   * @param cb The callback function to handle assertion output.
   * @details This function is thread-safe
   */
  void register_output_callback(void* m, OutputCallback cb);

  /**
   * @brief Registers a callback for doing assertion abortion.
   *
   * @param m A user-defined context pointer.
   * @param cb The callback function to handle assertion abortion.
   * @details This function is thread-safe
   */
  void register_abort_callback(void* m, AbortCallback cb);

  /**
   * @brief Handles an assertion failure.
   *
   * This function is called when an assertion fails. It uses the registered
   * output and abort callbacks to handle the failure.
   *
   * @param message The assertion failure message.
   * @param source The source location where the assertion failed.
   * @details This function is thread-safe
   * @warning This function does NOT return.
   */
  [[noreturn]] void assert_failure(const char* message, SourceLocation source);
}  // namespace wesos::assert

// NOLINTBEGIN(readability-identifier-naming)

/**
 * @brief A macro to assert a condition unconditionally.
 *
 * Evaluates the given condition and triggers an assertion failure if the
 * condition evaluates to false. This macro is always active, regardless of
 * the build configuration.
 *
 * @param ... The condition to evaluate.
 */
#define assert_always(...)                                                                                \
  [](bool eval, const char* eval_code, wesos::SourceLocation source = wesos::SourceLocation::current()) { \
    if (!eval) [[unlikely]] {                                                                             \
      wesos::assert::assert_failure(eval_code, source);                                                   \
    }                                                                                                     \
  }((__VA_ARGS__), #__VA_ARGS__)

#ifdef NDEBUG
#define assert_invariant_impl(...) (void)(__VA_ARGS__)
#else
#define assert_invariant_impl(...) assert_always(__VA_ARGS__)
#endif

/**
 * @brief A macro to assert a condition in debug builds.
 *
 * Evaluates the given condition and triggers an assertion failure if the
 * condition evaluates to false. This macro is active only in debug builds
 * (when `NDEBUG` is not defined).
 *
 * @param ... The condition to evaluate.
 */
#define assert_invariant(...) assert_invariant_impl(__VA_ARGS__)

// NOLINTEND(readability-identifier-naming)
