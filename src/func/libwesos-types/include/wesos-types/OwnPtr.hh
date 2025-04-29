/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class OwnPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr OwnPtr(PointeeGeneric* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr OwnPtr(const OwnPtr&) = default;
    constexpr OwnPtr(OwnPtr&& other) = default;
    constexpr auto operator=(const OwnPtr&) -> OwnPtr& = default;
    constexpr auto operator=(OwnPtr&& other) -> OwnPtr& = default;
    constexpr auto operator<=>(const OwnPtr& other) const = default;
    constexpr ~OwnPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize i) const { return OwnPtr(unwrap() + i.unwrap()); }
    [[nodiscard]] constexpr auto sub(usize i) const { return OwnPtr(unwrap() - i.unwrap()); }

    constexpr auto operator++() const -> OwnPtr { return {unwrap() + 1}; }
    constexpr auto operator--() const -> OwnPtr { return {unwrap() - 1}; }
    constexpr auto operator++(int) const -> OwnPtr { return {unwrap() + 1}; }
    constexpr auto operator--(int) const -> OwnPtr { return {unwrap() - 1}; }
  };

  static_assert(sizeof(OwnPtr<void*>) == sizeof(void*),
                "Size of OwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
