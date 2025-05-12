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
    mem::MemoryResourceProtocol& m_mm;

  public:
    constexpr Box(mem::MemoryResourceProtocol& mm, OwnPtr<Object> ptr) : m_ptr(ptr), m_mm(mm) {}
    constexpr Box(const Box& o) = delete;
    constexpr auto operator=(const Box& o) -> Box& = delete;
    constexpr Box(Box&& o) : m_ptr(o.m_ptr), m_mm(o.m_mm) { o.m_ptr = null; };

    constexpr auto operator=(Box&& o) -> Box& {
      if (this != &o) [[likely]] {
        m_ptr = o.m_ptr;
        o.m_ptr = null;
      }

      return *this;
    };

    constexpr ~Box() {
      if (!m_ptr.is_null()) {
        unwrap()->~Object();
        m_mm.deallocate_bytes(unwrap(), sizeof(Object), alignof(Object));
      }
    }

    [[nodiscard]] constexpr auto operator<=>(const Box& o) const { return unwrap() <=> o.unwrap(); }
    [[nodiscard]] constexpr auto operator==(types::Null) const { return is_null(); }
    [[nodiscard]] constexpr auto operator==(types::nullptr_t) const { return is_null(); }

    template <class... Args>
    [[nodiscard]] static constexpr auto create(mem::MemoryResourceProtocol& mm, Args&&... args) -> Nullable<Box> {
      const auto object_storage = mm.allocate_bytes(sizeof(Object), alignof(Object));
      if (!object_storage) {
        return null;
      }

      ::new (object_storage.unwrap()) Object(forward<Args>(args)...);
      const auto object_ptr = OwnPtr(static_cast<Object*>(object_storage.unwrap()));

      return Box(mm, object_ptr);
    }

    //=========================================================================================
    /// LIFETIME MANAGEMENT
    //=========================================================================================

    constexpr auto disown() -> void { m_ptr = null; }

    [[nodiscard]] constexpr auto memory_resource() -> mem::MemoryResourceProtocol& { return m_mm; }

    //=========================================================================================
    /// POINTER ACCESS
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

  template <class To, class From>
  [[nodiscard]] constexpr auto box_cast(Box<From> box) -> Box<To> {
    auto inner_ptr = box.unwrap();
    auto& mm = box.memory_resource();
    box.disown();

    return Box<To>(mm, inner_ptr);
  }

  static_assert(sizeof(Box<void>) == sizeof(void*) * 2);
}  // namespace wesos::smartptr
