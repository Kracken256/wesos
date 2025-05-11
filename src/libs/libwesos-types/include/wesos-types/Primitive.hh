/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Template.hh>

namespace wesos::types::detail {
  enum class PrimitiveTypeSize : unsigned char {
    Empty = 0,

    U8 = 1,
    U16 = 2,
    U32 = 4,
    U64 = 8,

    I8 = 1,
    I16 = 2,
    I32 = 4,
    I64 = 8,

    F32 = 4,
    F64 = 8,
  };

  // NOLINTBEGIN(readability-identifier-naming)

  using __u8 = unsigned char;
  static_assert(sizeof(__u8) == static_cast<int>(PrimitiveTypeSize::U8), "__u8 is not 1 byte");

  using __u16 = unsigned short;
  static_assert(sizeof(__u16) == static_cast<int>(PrimitiveTypeSize::U16), "__u16 is not 2 bytes");

  using __u32 = unsigned int;
  static_assert(sizeof(__u32) == static_cast<int>(PrimitiveTypeSize::U32), "__u32 is not 4 bytes");

  using __u64 = conditional_t<sizeof(unsigned long) == static_cast<int>(PrimitiveTypeSize::U64), unsigned long,
                              unsigned long long>;

  static_assert(sizeof(__u64) == static_cast<int>(PrimitiveTypeSize::U64), "__u64 is not 8 bytes");

  using __i8 = signed char;
  static_assert(sizeof(__i8) == static_cast<int>(PrimitiveTypeSize::I8), "__i8 is not 1 byte");

  using __i16 = signed short;
  static_assert(sizeof(__i16) == static_cast<int>(PrimitiveTypeSize::I16), "__i16 is not 2 bytes");

  using __i32 = signed int;
  static_assert(sizeof(__i32) == static_cast<int>(PrimitiveTypeSize::I32), "__i32 is not 4 bytes");

  using __i64 = conditional_t<sizeof(long) == static_cast<int>(PrimitiveTypeSize::I64), long, long long>;

  static_assert(sizeof(__i64) == static_cast<int>(PrimitiveTypeSize::I64), "__i64 is not 8 bytes");

  using __f32 = float;
  static_assert(sizeof(__f32) == static_cast<int>(PrimitiveTypeSize::F32), "__f32 is not 4 bytes");

  using __f64 = double;
  static_assert(sizeof(__f64) == static_cast<int>(PrimitiveTypeSize::F64), "__f64 is not 8 bytes");

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types::detail
