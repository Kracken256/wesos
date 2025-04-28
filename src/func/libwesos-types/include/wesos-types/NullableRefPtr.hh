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
  class NullableRefPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr NullableRefPtr() : m_ptr(nullptr) {}
    constexpr NullableRefPtr(PointeeGeneric* ptr) : m_ptr(ptr) {}
    constexpr NullableRefPtr(const NullableRefPtr&) = default;
    constexpr NullableRefPtr(NullableRefPtr&& other) = default;
    constexpr auto operator=(const NullableRefPtr&) -> NullableRefPtr& = default;
    constexpr auto operator=(NullableRefPtr&& other) -> NullableRefPtr& = default;
    constexpr auto operator<=>(const NullableRefPtr& other) const = default;
    constexpr ~NullableRefPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize i) const {
      return NullableRefPtr(unwrap() + i.unwrap());
    }
    [[nodiscard]] constexpr auto sub(usize i) const {
      return NullableRefPtr(unwrap() - i.unwrap());
    }

    void reflect(void* m, auto cb, auto&) const {
      cb(m, reinterpret_cast<const void*>(&m_ptr), sizeof(m_ptr));
    }
  };

  static_assert(sizeof(NullableRefPtr<void*>) == sizeof(void*),
                "Size of NullableRefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
