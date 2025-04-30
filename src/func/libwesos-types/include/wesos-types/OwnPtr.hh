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
  class OwnPtr;

  template <typename PointeeGeneric>
  class OwnPtr : public PtrBase<PointeeGeneric, OwnPtr<PointeeGeneric>> {
  public:
    constexpr OwnPtr(PointeeGeneric* ptr) : PtrBase<PointeeGeneric, OwnPtr<PointeeGeneric>>(ptr) {}
    constexpr OwnPtr(const OwnPtr&) = default;
    constexpr OwnPtr(OwnPtr&&) = default;
    constexpr auto operator=(const OwnPtr&) -> OwnPtr& = default;
    constexpr auto operator=(OwnPtr&&) -> OwnPtr& = default;
    constexpr ~OwnPtr() = default;

    [[nodiscard]] constexpr auto operator<=>(const OwnPtr&) const = default;
  };

  static_assert(sizeof(OwnPtr<void*>) == sizeof(void*),
                "Size of OwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
