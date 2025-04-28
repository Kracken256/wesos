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
  class RefPtr {
    PointeeGeneric* m_ptr;

  public:
    constexpr RefPtr(PointeeGeneric* ptr) : m_ptr(ptr) { assert_invariant(ptr != nullptr); }
    constexpr RefPtr(const RefPtr&) = default;
    constexpr RefPtr(RefPtr&& other) = default;
    constexpr auto operator=(const RefPtr&) -> RefPtr& = default;
    constexpr auto operator=(RefPtr&& other) -> RefPtr& = default;
    constexpr auto operator<=>(const RefPtr& other) const = default;
    constexpr ~RefPtr() = default;

    [[nodiscard]] constexpr auto unwrap() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator->() const -> PointeeGeneric* { return m_ptr; }
    [[nodiscard]] constexpr auto operator*() const -> PointeeGeneric& { return *m_ptr; }

    [[nodiscard]] constexpr auto add(usize i) const { return RefPtr(unwrap() + i.unwrap()); }
    [[nodiscard]] constexpr auto sub(usize i) const { return RefPtr(unwrap() - i.unwrap()); }
  };

  static_assert(sizeof(RefPtr<void*>) == sizeof(void*),
                "Size of RefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
