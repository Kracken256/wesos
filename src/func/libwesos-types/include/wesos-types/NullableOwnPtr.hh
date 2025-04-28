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
  class NullableOwnPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr NullableOwnPtr() : m_ptr(nullptr) {}
    constexpr NullableOwnPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableOwnPtr(const NullableOwnPtr&) = default;
    constexpr NullableOwnPtr(NullableOwnPtr&& other) = default;
    constexpr auto operator=(const NullableOwnPtr&) -> NullableOwnPtr& = default;
    constexpr auto operator=(NullableOwnPtr&& other) -> NullableOwnPtr& = default;
    constexpr auto operator<=>(const NullableOwnPtr& other) const = default;
    constexpr ~NullableOwnPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize i) const {
      return NullableOwnPtr(unwrap() + i.unwrap());
    }
    [[nodiscard]] constexpr auto sub(usize i) const {
      return NullableOwnPtr(unwrap() - i.unwrap());
    }

    void reflect(void* m, auto cb, auto&) const {
      cb(m, reinterpret_cast<const void*>(&m_ptr), sizeof(m_ptr));
    }
  };

  static_assert(sizeof(NullableOwnPtr<void*>) == sizeof(void*),
                "Size of NullableOwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
