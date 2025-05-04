/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Bitcast.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/PowerOfTwo.hh>
#include <wesos-types/Template.hh>

namespace wesos::types {
  template <class PointerTypeGeneric>
  [[nodiscard]] static constexpr auto is_aligned(PointerTypeGeneric ptr, usize x) -> bool {
    if constexpr (is_pointer_v<PointerTypeGeneric>) {
      return bit_cast<uptr>(ptr) % x == 0;
    } else {
      return bit_cast<uptr>(ptr.into_uptr()) % x == 0;
    }
  }

  template <class PointerTypeGeneric>
  [[nodiscard]] static constexpr auto is_aligned_pow2(PointerTypeGeneric ptr,
                                                      PowerOfTwo<usize> x) -> bool {
    if constexpr (is_pointer_v<PointerTypeGeneric>) {
      return (bit_cast<uptr>(ptr) & (x.unwrap() - 1)) == 0;
    } else {
      return (bit_cast<uptr>(ptr.into_uptr()) & (x.unwrap() - 1)) == 0;
    }
  }

  // [[nodiscard]] constexpr auto align_pow2(PowerOfTwo<usize> x) -> NullableOwnPtr {
  //   const auto ptr = into_uptr();
  //   const auto align_ptr = (ptr + x - 1) & -x;
  //   return bit_cast<PointeeGeneric*>(align_ptr);
  // }

  // [[nodiscard]] constexpr auto align(usize x) -> NullableOwnPtr {
  //   assert_invariant(x != 0);
  //   const auto ptr = into_uptr();
  //   const auto align_ptr = ptr + ((x - (ptr % x)) % x);
  //   return bit_cast<PointeeGeneric*>(align_ptr);
  // }
}  // namespace wesos::types
