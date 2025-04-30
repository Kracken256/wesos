/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/PtrBase.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class RefPtr;

  template <typename PointeeGeneric>
  class RefPtr : public PtrBase<PointeeGeneric, RefPtr<PointeeGeneric>> {
  public:
    constexpr RefPtr(PointeeGeneric* ptr) : PtrBase<PointeeGeneric, RefPtr<PointeeGeneric>>(ptr) {}
    constexpr RefPtr(const RefPtr&) = default;
    constexpr RefPtr(RefPtr&&) = default;
    constexpr auto operator=(const RefPtr&) -> RefPtr& = default;
    constexpr auto operator=(RefPtr&&) -> RefPtr& = default;
    constexpr ~RefPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const RefPtr&) const = default;
  };

  static_assert(sizeof(RefPtr<void*>) == sizeof(void*),
                "Size of RefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
