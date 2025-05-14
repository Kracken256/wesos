/*
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

    // constexpr Rc(OwnPtr<Object> ptr, OwnPtr<State> state) : m_ptr(ptr), m_state(state) {}

  public:
    constexpr Rc() = delete;

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

    constexpr ~Rc() {
      if (m_state.isset()) {
        if (m_ptr.isset() && --m_state->m_data_rc == 0) [[unlikely]] {
          auto& mm = m_state->m_mm;
          unwrap()->~Object();
          mm.deallocate_bytes(unwrap(), sizeof(Object), alignof(Object));
        }

        if (--m_state->m_state_rc == 0) [[unlikely]] {
          auto& mm = m_state->m_mm;
          m_state.unwrap()->~State();
          mm.deallocate_bytes(m_state.unwrap(), sizeof(State), alignof(State));
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
    [[nodiscard]] static constexpr auto create(mem::MemoryResourceProtocol& mm, Args&&... args) -> Nullable<Rc> {
      const auto object_storage = mm.allocate_bytes(sizeof(Object), alignof(Object));
      if (object_storage.is_null()) [[unlikely]] {
        return null;
      }

      const auto state_storage = mm.allocate_bytes(sizeof(State), alignof(State));
      if (state_storage.is_null()) [[unlikely]] {
        return null;
      }

      ::new (object_storage.unwrap()) Object(forward<Args>(args)...);
      const auto object_ptr = OwnPtr(static_cast<Object*>(object_storage.unwrap()));

      ::new (state_storage.unwrap()) State(mm);
      const auto state_ptr = OwnPtr(static_cast<State*>(state_storage.unwrap()));

      return Rc(object_ptr, state_ptr);
    }

    //=========================================================================================
    // LIFETIME MANAGEMENT
    //=========================================================================================

    [[nodiscard]] constexpr auto ref_count() const -> usize { return m_state.isset() ? m_state->m_data_rc : 0; }

    //=========================================================================================
    // POINTER ACCESS
    //=========================================================================================

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
