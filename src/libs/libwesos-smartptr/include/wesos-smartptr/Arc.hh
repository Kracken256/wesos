/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-builtin/Range.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-sync/Atomic.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Template.hh>

namespace wesos::smartptr {
  template <class Object>
  class Arc {
    struct State {
      sync::Atomic<usize> m_state_rc = 1;
      sync::Atomic<usize> m_data_rc = 1;
      mem::MemoryResourceProtocol& m_pmr;

      constexpr State(mem::MemoryResourceProtocol& pmr) : m_pmr(pmr) {}
    };

    NullableRefPtr<Object> m_ptr;
    NullableRefPtr<State> m_state;

    constexpr Arc(OwnPtr<Object> ptr, OwnPtr<State> state) : m_ptr(ptr), m_state(state) {}

  public:
    constexpr Arc(const Arc& o) : m_ptr(o.m_ptr), m_state(o.m_state) {
      ++m_state->m_state_rc;
      ++m_state->m_data_rc;
    }

    constexpr auto operator=(const Arc& o) -> Arc& {
      m_ptr = o.m_ptr;
      m_state = o.m_state;
      ++m_state->m_state_rc;
      ++m_state->m_data_rc;
    }

    constexpr Arc(Arc&& o) : m_ptr(o.m_ptr), m_state(o.m_state) {
      o.m_ptr = null;
      o.m_state = null;
    }

    constexpr auto operator=(Arc&& o) -> Arc& {
      m_ptr = o.m_ptr;
      m_state = o.m_state;
      o.m_ptr = null;
      o.m_state = null;
    };

    constexpr ~Arc() {
      if (m_state.isset()) {
        if (m_ptr.isset() && --m_state->m_data_rc == 0) [[unlikely]] {
          m_state->m_pmr.template destroy_and_deallocate<Object>(m_ptr.unwrap(), 1);
        }

        if (--m_state->m_state_rc == 0) [[unlikely]] {
          m_state->m_pmr.template destroy_and_deallocate<State>(m_state.unwrap(), 1);
        }
      }
    }

    [[nodiscard]] constexpr auto operator<=>(const Arc& o) const { return unwrap() <=> o.unwrap(); }
    [[nodiscard]] constexpr auto operator==(types::Null) const { return is_null(); }
    [[nodiscard]] constexpr auto operator==(types::nullptr_t) const { return is_null(); }

    [[nodiscard]] static constexpr auto min_alloc_size() -> usize { return max(sizeof(State), sizeof(Object)); }

    [[nodiscard]] static constexpr auto min_alloc_alignment() -> PowerOfTwo<usize> {
      return max(alignof(State), alignof(Object));
    }

    template <class... Args>
    [[nodiscard]] static constexpr auto create(mem::MemoryResourceProtocol& pmr, Args&&... args) -> Nullable<Arc> {
      auto inner_object = pmr.allocate_storage<Object>(1);
      if (inner_object.is_null()) [[unlikely]] {
        return null;
      }

      auto state_object = pmr.allocate_storage<State>(1);
      if (state_object.is_null()) [[unlikely]] {
        return null;
      }

      ::new (static_cast<void*>(inner_object.unwrap())) Object(forward<Args>(args)...);
      ::new (static_cast<void*>(state_object.unwrap())) State(pmr);

      return Arc(inner_object.get_unchecked(), state_object.get_unchecked());
    }

    ///=========================================================================================
    /// LIFETIME MANAGEMENT
    ///=========================================================================================

    [[nodiscard]] constexpr auto ref_count() const -> usize { return m_state.isset() ? m_state->m_data_rc.load() : 0; }

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

  static_assert(sizeof(Arc<void>) == sizeof(void*) * 2);
}  // namespace wesos::smartptr
