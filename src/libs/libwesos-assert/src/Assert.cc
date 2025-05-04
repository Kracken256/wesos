/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Export.hh>
#include <wesos-sync/SpinLock.hh>

namespace wesos::assert {
  static void default_output_callback(void*, const char* message, SourceLocation source) {
    // This will run on bare-metal, so we can't use printf
    // or any other standard library functions. Just discard the message.

    (void)message;
    (void)source;
  }

  static void default_abort_callback(void*) { __builtin_trap(); }

  static struct OutputConfig {
    OutputCallback m_func = default_output_callback;
    void* m_data = nullptr;
  } OUTPUT_GLOBAL;

  static struct AbortConfig {
    AbortCallback m_func = default_abort_callback;
    void* m_data = nullptr;
  } ABORT_GLOBAL;

  static sync::SpinLock OUTPUT_LOCK_GLOBAL;
  static sync::SpinLock ABORT_LOCK_GLOBAL;
}  // namespace wesos::assert

SYM_EXPORT void wesos::assert::register_output_callback(void* m, OutputCallback cb) {
  assert_invariant(cb != nullptr);

  OUTPUT_LOCK_GLOBAL.critical_section([&] {
    OUTPUT_GLOBAL.m_func = cb;
    OUTPUT_GLOBAL.m_data = m;
  });
}

SYM_EXPORT void wesos::assert::register_abort_callback(void* m, AbortCallback cb) {
  assert_invariant(cb != nullptr);

  ABORT_LOCK_GLOBAL.critical_section([&] {
    ABORT_GLOBAL.m_func = cb;
    ABORT_GLOBAL.m_data = m;
  });
}

SYM_EXPORT void wesos::assert::assert_failure(const char* message, SourceLocation source) {
  using namespace assert;

  auto output_copy = OUTPUT_LOCK_GLOBAL.critical_section([] { return OUTPUT_GLOBAL; });
  auto abort_copy = ABORT_LOCK_GLOBAL.critical_section([] { return ABORT_GLOBAL; });

  output_copy.m_func(output_copy.m_data, message, source);
  abort_copy.m_func(abort_copy.m_data);
}
