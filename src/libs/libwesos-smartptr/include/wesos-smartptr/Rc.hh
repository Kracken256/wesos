/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Template.hh>

namespace wesos::smartptr {
  template <class Object>
  class Rc {
    struct State {
      usize m_state_rc = 1;
      usize m_data_rc = 1;
      mem::MemoryResourceProtocol& m_mm;

      constexpr State(mem::MemoryResourceProtocol& mm) : m_mm(mm) {}
    };

    NullableRefPtr<Object> m_ptr;
    NullableRefPtr<State> m_state;

    constexpr Rc(OwnPtr<Object> ptr, OwnPtr<State> state) : m_ptr(ptr), m_state(state) {}

  public:
    constexpr Rc(const Rc& o) : m_ptr(o.m_ptr), m_state(o.m_state) {
      assert_invariant(m_state.isset());

      ++m_state->m_state_rc;
      ++m_state->m_data_rc;
    }

    constexpr auto operator=(const Rc& o) -> Rc& {
      assert_invariant(m_state.isset());

      m_ptr = o.m_ptr;
      m_state = o.m_state;
      ++m_state->m_state_rc;
      ++m_state->m_data_rc;
    }

    constexpr Rc(Rc&& o) : m_ptr(o.m_ptr), m_state(o.m_state) {
      o.m_ptr = null;
      o.m_state = null;
    }

    constexpr auto operator=(Rc&& o) -> Rc& {
      m_ptr = o.m_ptr;
      m_state = o.m_state;
      o.m_ptr = null;
      o.m_state = null;
    };

    ~Rc() {
      if (m_state.isset()) {
        mem::MemoryResourceProtocol& mm = m_state->m_mm;

        if (m_ptr.isset() && --m_state->m_data_rc == 0) [[unlikely]] {
          mm.template destroy_and_deallocate<Object>(m_ptr.unwrap(), 1);
        }

        if (--m_state->m_state_rc == 0) [[unlikely]] {
          mm.template destroy_and_deallocate<State>(m_state.unwrap(), 1);
        }
      }
    }

    [[nodiscard]] constexpr auto operator<=>(const Rc& o) const { return unwrap() <=> o.unwrap(); }
    [[nodiscard]] constexpr auto operator==(types::Null) const { return is_null(); }
    [[nodiscard]] constexpr auto operator==(types::nullptr_t) const { return is_null(); }

    [[nodiscard]] static constexpr auto min_alloc_size() -> usize { return max(sizeof(State), sizeof(Object)); }

    [[nodiscard]] static constexpr auto min_alloc_alignment() -> PowerOfTwo<usize> {
      return max(alignof(State), alignof(Object));
    }

    template <class... Args>
    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm, Args&&... args) -> Nullable<Rc> {
      auto inner_object = mm.allocate_storage<Object>(1);
      if (inner_object.is_null()) [[unlikely]] {
        return null;
      }

      auto state_object = mm.allocate_storage<State>(1);
      if (state_object.is_null()) [[unlikely]] {
        return null;
      }

      ::new (static_cast<void*>(inner_object.unwrap())) Object(forward<Args>(args)...);
      ::new (static_cast<void*>(state_object.unwrap())) State(mm);

      return Rc(inner_object.get_unchecked(), state_object.get_unchecked());
    }

    ///=========================================================================================
    /// LIFETIME MANAGEMENT
    ///=========================================================================================

    [[nodiscard]] constexpr auto ref_count() const -> usize { return m_state.isset() ? m_state->m_data_rc : 0; }

    ///=========================================================================================
    /// POINTER ACCESS
    ///=========================================================================================

    [[nodiscard]] constexpr auto unwrap() -> Object* { return m_ptr.unwrap(); }
    [[nodiscard]] constexpr auto unwrap() const -> const Object* { return m_ptr.unwrap(); }

    [[nodiscard]] constexpr auto isset() const -> bool { return m_ptr.isset(); }
    [[nodiscard]] constexpr auto is_null() const -> bool { return m_ptr.is_null(); }

    template <class U = Object>
    [[nodiscard]] constexpr auto operator->() -> U* requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto operator->() const -> const U* requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto operator*() -> U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto operator*() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto get() -> U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto get() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto get_unchecked() -> U& requires(!types::is_same_v<U, void>) {
      assert_invariant(m_ptr.isset());
      return *unwrap();
    };

    template <class U = Object>
    [[nodiscard]] constexpr auto get_unchecked() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_invariant(m_ptr.isset());
      return *unwrap();
    };
  };

  static_assert(sizeof(Rc<void>) == sizeof(void*) * 2);
}  // namespace wesos::smartptr
