/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Target.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-kernconf/Parser.hh>
#include <wesos-mem/MemoryEconomy.hh>
#include <wesos-sync/MemoryOrder.hh>
#include <wesos-types/Types.hh>

void operator delete(void *) { assert_always(false && "Calling C++ operator delete is not allowed"); }

namespace wesos::kern {
  extern "C" [[noreturn]] void __stack_chk_fail() {  // NOLINT(readability-identifier-naming)
    assert_always(false && "Stack smashing detected");
    __builtin_unreachable();
  }

  struct AtexitFuncEntry {
    void (*m_destructor_func)(void *);
    void *m_obj_ptr;
    void *m_dso_handle;
  };

  static const auto W_ATEXIT_MAX_FUNCS = 128;
  static Array<AtexitFuncEntry, W_ATEXIT_MAX_FUNCS> ATEXIT_FUNCS_GLOBAL;
  static usize ATEXIT_FUNC_COUNT_GLOBAL = 0;

  extern "C" void *__dso_handle = nullptr;  // NOLINT(readability-identifier-naming)

  extern "C" auto __cxa_atexit(void (*f)(void *), void *objptr,  // NOLINT(readability-identifier-naming)
                               void *dso) -> int {
    if (ATEXIT_FUNC_COUNT_GLOBAL >= W_ATEXIT_MAX_FUNCS) [[unlikely]] {
      return -1;
    }

    auto &entry = ATEXIT_FUNCS_GLOBAL.get_unchecked(ATEXIT_FUNC_COUNT_GLOBAL);
    entry.m_destructor_func = f;
    entry.m_obj_ptr = objptr;
    entry.m_dso_handle = dso;

    ATEXIT_FUNC_COUNT_GLOBAL++;

    return 0;
  }

  extern "C" void __cxa_finalize(void *f) {  // NOLINT(readability-identifier-naming)
    usize i = ATEXIT_FUNC_COUNT_GLOBAL;

    if (f == nullptr) {
      /*
       * According to the Itanium C++ ABI, if __cxa_finalize is called without a
       * function ptr, then it means that we should destroy EVERYTHING MUAHAHAHA!!
       */

      while (i-- != 0) {
        if (auto &entry = ATEXIT_FUNCS_GLOBAL.get_unchecked(i); entry.m_destructor_func != nullptr) {
          /*
           * Avoid calling any entries that have already been called and unset
           * at runtime.
           */
          (*entry.m_destructor_func)(entry.m_obj_ptr);
        }
      }

      return;
    }

    while (i < ATEXIT_FUNC_COUNT_GLOBAL) {
      if (auto &entry = ATEXIT_FUNCS_GLOBAL.get(i); bit_cast<void *>(entry.m_destructor_func) == f) {
        (*entry.m_destructor_func)(entry.m_obj_ptr);
        entry.m_destructor_func = nullptr;

        usize remaining = ATEXIT_FUNC_COUNT_GLOBAL - i - 1;
        if (remaining > 0) {
          __builtin_memmove(&entry, &ATEXIT_FUNCS_GLOBAL.get(i + 1), remaining * sizeof(decltype(entry)));
        }

        --ATEXIT_FUNC_COUNT_GLOBAL;
      } else {
        ++i;
      }
    }
  }

  //============================================================================================
  using ConstructorFunc = void (*)();

  extern "C" ConstructorFunc BEG_CXX_CTORS_GLOBAL[];
  extern "C" ConstructorFunc END_CXX_CTORS_GLOBAL[];

  static void globals_init() {
    for (auto *i = BEG_CXX_CTORS_GLOBAL; i != END_CXX_CTORS_GLOBAL; i++) {
      (*i)();
    }
  }

  static void globals_deinit() { __cxa_finalize(nullptr); }

  //============================================================================================

  extern "C" void __cxa_pure_virtual() {  // NOLINT(readability-identifier-naming)
    assert_always(false && "Calling into C++ pure virtual functions is not allowed");
  }

  //============================================================================================

  auto main(const kernconf::KernelConfig &config) -> int;

  // #if ARCH_X86_64
  //   void stdio_output_handler(void *, const char *message, SourceLocation source) {
  //     usize len = 0;
  //     for (const auto *x = message; *x; len++, x++) {
  //     }

  //     asm volatile(
  //         "mov $1, %%rax\n"
  //         "mov $1, %%rdi\n"
  //         "mov %[buf_ptr], %%rsi\n"
  //         "movq %[buf_size], %%rdx\n"
  //         "syscall"  // Call sys_write()
  //         :
  //         : [buf_ptr] "r"(message), [buf_size] "r"(len)
  //         : "rax", "rdi", "rsi", "rdx");

  //     while (true) {
  //       cpu::ephemeral_pause();
  //     }
  //   }
  // #endif

  extern "C" [[noreturn, gnu::used]] void cxx_genesis(const u8 *configuration, usize configuration_len) {
    const auto cxx_runtime = [](const kernconf::KernelConfig &config) -> int {
      int rc;

      globals_init();
      rc = main(config);
      globals_deinit();

      return rc;
    };

    // #if ARCH_X86_64
    //     assert::register_output_callback(nullptr, stdio_output_handler);
    // #endif

    auto config_opt = kernconf::parse_kernel_config({configuration, configuration_len});
    assert_always(config_opt.isset() && "Failed to parse kernel config options");

    const auto config = move(config_opt.value());

    // TODO: Initialize usable memory regions

    (void)cxx_runtime(config);

    while (true) {
      cpu::ephemeral_pause();
    }

    assert_always(false && "Unreachable?");
  }
}  // namespace wesos::kern
