/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Null.hh>
#include <wesos-types/NullablePtrBase.hh>
#include <wesos-types/RefPtr.hh>

namespace wesos::types {
  template <typename PointeeGeneric>
  class NullableOwnPtr;

  template <typename PointeeGeneric>
  class NullableRefPtr : public NullablePtrBase<PointeeGeneric, NullableRefPtr<PointeeGeneric>,
                                                RefPtr<PointeeGeneric>> {
  public:
    constexpr NullableRefPtr() = default;
    constexpr NullableRefPtr(Null) {}
    constexpr NullableRefPtr(PointeeGeneric* ptr)
        : NullablePtrBase<PointeeGeneric, NullableRefPtr<PointeeGeneric>, RefPtr<PointeeGeneric>>(
              ptr) {}
    constexpr NullableRefPtr(const NullableRefPtr&) = default;
    constexpr NullableRefPtr(NullableRefPtr&&) = default;
    constexpr auto operator=(const NullableRefPtr&) -> NullableRefPtr& = default;
    constexpr auto operator=(NullableRefPtr&&) -> NullableRefPtr& = default;
    constexpr auto operator<=>(const NullableRefPtr&) const = default;
    constexpr ~NullableRefPtr() = default;

    [[nodiscard]] constexpr auto take_own() const { return NullableOwnPtr(this->into_raw()); }
  };

  static_assert(sizeof(NullableRefPtr<void*>) == sizeof(void*),
                "Size of NullableRefPtr<void*> must be equal to size of void*");
}  // namespace wesos::types
