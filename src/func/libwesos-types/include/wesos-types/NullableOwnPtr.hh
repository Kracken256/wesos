/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Null.hh>
#include <wesos-types/NullablePtrBase.hh>
#include <wesos-types/OwnPtr.hh>

namespace wesos::types {
  template <class PointeeGeneric>
  class NullableRefPtr;

  template <class PointeeGeneric>
  class NullableOwnPtr : public NullablePtrBase<PointeeGeneric, NullableOwnPtr<PointeeGeneric>,
                                                OwnPtr<PointeeGeneric>> {
  public:
    constexpr NullableOwnPtr() = default;
    constexpr NullableOwnPtr(Null) {}
    constexpr NullableOwnPtr(PointeeGeneric* ptr)
        : NullablePtrBase<PointeeGeneric, NullableOwnPtr<PointeeGeneric>, OwnPtr<PointeeGeneric>>(
              ptr) {}
    constexpr NullableOwnPtr(OwnPtr<PointeeGeneric> ptr)
        : NullablePtrBase<PointeeGeneric, NullableOwnPtr<PointeeGeneric>, OwnPtr<PointeeGeneric>>(
              ptr.unwrap()) {}
    constexpr NullableOwnPtr(const NullableOwnPtr&) = default;
    constexpr NullableOwnPtr(NullableOwnPtr&&) = default;
    constexpr auto operator=(const NullableOwnPtr&) -> NullableOwnPtr& = default;
    constexpr auto operator=(NullableOwnPtr&&) -> NullableOwnPtr& = default;
    constexpr auto operator<=>(const NullableOwnPtr&) const = default;
    constexpr ~NullableOwnPtr() = default;

    [[nodiscard]] constexpr auto take_ref() const { return NullableRefPtr(this->unwrap()); }
  };

  static_assert(sizeof(NullableOwnPtr<void*>) == sizeof(void*),
                "Size of NullableOwnPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
