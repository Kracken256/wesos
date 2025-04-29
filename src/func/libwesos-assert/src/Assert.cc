/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-assert/Assert.hh>
#include <wesos-assert/Export.hh>
#include <wesos-sync/SpinLock.hh>

namespace wesos::assert {
  static void default_message_callback(void*, const char* message) {
    // This will run on bare-metal, so we can't use printf
    // or any other standard library functions. Just discard the message.

    (void)message;
  }

  static void default_abort_callback(void*) { __builtin_trap(); }

  static MessageCallback MESSAGE_CALLBACK_GLOBAL = default_message_callback;
  static AbortCallback ABORT_CALLBACK_GLOBAL = default_abort_callback;
  static void* ABORT_CALLBACK_DATA_GLOBAL = nullptr;
  static void* MESSAGE_CALLBACK_DATA_GLOBAL = nullptr;

  /// FIXME: Use a mutex to protect the global variables
}  // namespace wesos::assert

SYM_EXPORT void wesos::assert::register_message_callback(void* m, MessageCallback cb) {
  always_assert(cb != nullptr, "Message callback cannot be null");

  MESSAGE_CALLBACK_DATA_GLOBAL = m;
  MESSAGE_CALLBACK_GLOBAL = cb;
}

SYM_EXPORT void wesos::assert::register_abort_callback(void* m, AbortCallback cb) {
  always_assert(cb != nullptr, "Abort callback cannot be null");

  ABORT_CALLBACK_DATA_GLOBAL = m;
  ABORT_CALLBACK_GLOBAL = cb;
}

SYM_EXPORT void wesos::always_assert(bool condition, const char* message) {
  if (!condition) [[unlikely]] {
    assert::MESSAGE_CALLBACK_GLOBAL(assert::MESSAGE_CALLBACK_DATA_GLOBAL, message);
    assert::ABORT_CALLBACK_GLOBAL(assert::ABORT_CALLBACK_DATA_GLOBAL);
  }
}
