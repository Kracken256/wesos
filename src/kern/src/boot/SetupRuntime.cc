/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-cpu/Target.hh>
#include <wesos-cpu/Timing.hh>
#include <wesos-kern/boot/Options.hh>
#include <wesos-sync/MemoryOrder.hh>
#include <wesos-types/Types.hh>

void operator delete(void *) { assert_always(false && "Calling C++ operator delete is not allowed"); }

namespace wesos::kern {
  extern void main(boot::Options settings);

  namespace detail {
    ///============================================================================================
    using ConstructorFunc = void (*)();

    extern "C" ConstructorFunc BEG_CXX_CTORS_GLOBAL[];
    extern "C" ConstructorFunc END_CXX_CTORS_GLOBAL[];

    void cxx_constructors() {
      for (auto *i = BEG_CXX_CTORS_GLOBAL; i != END_CXX_CTORS_GLOBAL; i++) {
        (*i)();
      }
    }

    ///============================================================================================

    static const auto W_ATEXIT_MAX_FUNCS = 128;

    extern "C" {
    struct AtexitFuncEntry {
      void (*m_destructor_func)(void *);
      void *m_obj_ptr;
      void *m_dso_handle;
    };

    static Array<AtexitFuncEntry, W_ATEXIT_MAX_FUNCS> ATEXIT_FUNCS_GLOBAL;
    static usize ATEXIT_FUNC_COUNT_GLOBAL = 0;

    void *__dso_handle = nullptr;  // NOLINT(readability-identifier-naming)

    auto __cxa_atexit(void (*f)(void *), void *objptr, void *dso) -> int {  // NOLINT(readability-identifier-naming)
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

    void __cxa_finalize(void *f) {  // NOLINT(readability-identifier-naming)
      usize i = ATEXIT_FUNC_COUNT_GLOBAL;

      if (f == nullptr) {
        /**
         * According to the Itanium C++ ABI, if __cxa_finalize is called without a
         * function ptr, then it means that we should destroy EVERYTHING MUAHAHAHA!!
         */

        while (i-- != 0) {
          if (auto &entry = ATEXIT_FUNCS_GLOBAL.get_unchecked(i); entry.m_destructor_func != nullptr) {
            /**
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
    }

    void cxx_destructors() {}

    ///============================================================================================

    extern "C" void __cxa_pure_virtual() {  // NOLINT(readability-identifier-naming)
      assert_always(false && "Calling into C++ pure virtual functions is not allowed");
    }

    ///============================================================================================
  }  // namespace detail

  extern "C" [[noreturn]] void cxx_start() {
    constexpr auto cxx_runtime = [] {
      detail::cxx_constructors();

      {
        /// TODO: Get kernel options from somewhere
        boot::Options settings;

        main(settings);
      }

      detail::cxx_destructors();
    };

    cxx_runtime();

    while (true) {
      cpu::ephemeral_pause();
    }

    assert_always(false && "Unreachable?");
  }
}  // namespace wesos::kern
