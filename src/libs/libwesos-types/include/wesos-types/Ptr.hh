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
  [[nodiscard]] static constexpr auto is_aligned_pow2(PointerTypeGeneric addr,
                                                      PowerOfTwo<usize> x) -> bool {
    const auto address = [addr]() {
      if constexpr (is_pointer_v<PointerTypeGeneric>) {
        return bit_cast<uptr>(addr);
      } else {
        return bit_cast<uptr>(addr.unwrap());
      }
    }();

    return (address & (x.unwrap() - 1)) == 0;
  }

  template <class PointerTypeGeneric>
  [[nodiscard]] static constexpr auto next_aligned_pow2(PointerTypeGeneric addr,
                                                        PowerOfTwo<usize> x) -> PointerTypeGeneric {
    using PointeeType = remove_reference_t<decltype(*addr)>;

    const auto address = [addr]() {
      if constexpr (is_pointer_v<PointerTypeGeneric>) {
        return bit_cast<uptr>(addr);
      } else {
        return bit_cast<uptr>(addr.unwrap());
      }
    }();

    const uptr aligned = (address + x.unwrap() - 1) & ~(x.unwrap() - 1);

    return bit_cast<PointeeType*>(aligned);
  }
}  // namespace wesos::types
