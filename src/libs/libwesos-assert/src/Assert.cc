/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Export.hh>
#include <wesos-sync/SpinLock.hh>

namespace wesos::assert {
  static struct OutputConfig {
    OutputCallback m_output_func = nullptr;
    void* m_output_data = nullptr;

    AbortCallback m_abort_func = nullptr;
    void* m_abort_data = nullptr;
  } SETUP_GLOBAL;

  static sync::SpinLock SETUP_LOCK_GLOBAL;
}  // namespace wesos::assert

SYM_EXPORT void wesos::assert::register_output_callback(void* m, OutputCallback cb) {
  SETUP_LOCK_GLOBAL.critical_section([&] {
    SETUP_GLOBAL.m_output_func = cb;
    SETUP_GLOBAL.m_output_data = m;
  });
}

SYM_EXPORT void wesos::assert::register_abort_callback(void* m, AbortCallback cb) {
  SETUP_LOCK_GLOBAL.critical_section([&] {
    SETUP_GLOBAL.m_abort_func = cb;
    SETUP_GLOBAL.m_abort_data = m;
  });
}

SYM_EXPORT void wesos::assert::assert_failure(const char* message, SourceLocation source) {
  using namespace assert;

  auto setup = SETUP_LOCK_GLOBAL.critical_section([] { return SETUP_GLOBAL; });

  if (setup.m_output_func != nullptr) {
    setup.m_output_func(setup.m_output_data, message, source);
  }

  if (setup.m_abort_func != nullptr) {
    setup.m_abort_func(setup.m_abort_data);
  }

  __builtin_trap();
}
