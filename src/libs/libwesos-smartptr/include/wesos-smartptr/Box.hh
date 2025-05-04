/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/NullableOwnPtr.hh>
#include <wesos-types/Template.hh>

namespace wesos::smartptr {
  template <class Object>
  class Box {
    NullableOwnPtr<Object> m_ptr;
    mem::MemoryResourceProtocol& m_pmr;

    Box(OwnPtr<Object> ptr, mem::MemoryResourceProtocol& pmr) : m_ptr(ptr), m_pmr(pmr) {}

  public:
    constexpr Box(const Box& o) = delete;
    constexpr auto operator=(const Box& o) -> Box& = delete;
    constexpr Box(Box&& o) : m_ptr(o.m_ptr), m_pmr(o.m_pmr) { o.m_ptr = nullptr; };

    constexpr auto operator=(Box&& o) -> Box& {
      if (this != &o) [[likely]] {
        m_ptr = o.m_ptr;
        o.m_ptr = nullptr;
      }

      return *this;
    };

    ~Box() {
      if (!m_ptr.is_null()) {
        m_pmr.destroy_and_deallocate<Object>(m_ptr.unwrap(), 1);
#ifndef NDEBUG
        m_ptr = nullptr;
#endif
      }
    }

    template <class... Args>
    [[nodiscard]] static auto create(Args... args) {
      return [... args = forward<Args>(args)](mem::MemoryResourceProtocol& pmr) -> Nullable<Box> {
        if (auto ptr = pmr.allocate_and_construct<Object>(1, args...)) [[likely]] {
          return Box(ptr.unwrap(), pmr);
        }

        return null;
      };
    }

    template <class U = Object>
    [[nodiscard]] constexpr auto get() -> U& requires(!types::is_same_v<U, void>) { return *m_ptr.as_ref().get(); }

    template <class U = Object>
    [[nodiscard]] constexpr auto get() const -> const U& requires(!types::is_same_v<U, void>) {
      return *m_ptr.as_ref().get();
    }
  };

  static_assert(sizeof(Box<void>) == sizeof(void*) * 2);
}  // namespace wesos::smartptr
