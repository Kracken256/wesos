/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class OwnPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr OwnPtr(PointeeGeneric* ptr) : m_ptr(ptr) { assert(ptr != nullptr); }
    constexpr OwnPtr(const OwnPtr&) = default;
    constexpr OwnPtr(OwnPtr&& other) = default;
    constexpr auto operator=(const OwnPtr&) -> OwnPtr& = default;
    constexpr auto operator=(OwnPtr&& other) -> OwnPtr& = default;
    constexpr auto operator<=>(const OwnPtr& other) const = default;
    constexpr ~OwnPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize offset) const { return OwnPtr(unwrap() + offset); }
    [[nodiscard]] constexpr auto sub(usize offset) const { return OwnPtr(unwrap() - offset); }

    void reflect(void* m, auto cb, auto&) const {
      cb(m, reinterpret_cast<const void*>(&m_ptr), sizeof(m_ptr));
    }
  };

  static_assert(sizeof(OwnPtr<void*>) == sizeof(void*),
                "Size of OwnPtr<void*> must be equal to size of void*");

  template <typename PointeeGeneric>
  class RefPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr RefPtr(PointeeGeneric* ptr) : m_ptr(ptr) { assert(ptr != nullptr); }
    constexpr RefPtr(const RefPtr&) = default;
    constexpr RefPtr(RefPtr&& other) = default;
    constexpr auto operator=(const RefPtr&) -> RefPtr& = default;
    constexpr auto operator=(RefPtr&& other) -> RefPtr& = default;
    constexpr auto operator<=>(const RefPtr& other) const = default;
    constexpr ~RefPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize offset) const { return OwnPtr(unwrap() + offset); }
    [[nodiscard]] constexpr auto sub(usize offset) const { return OwnPtr(unwrap() - offset); }

    void reflect(void* m, auto cb, auto&) const {
      cb(m, reinterpret_cast<const void*>(&m_ptr), sizeof(m_ptr));
    }
  };

  static_assert(sizeof(RefPtr<void*>) == sizeof(void*),
                "Size of RefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
