/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Array.hh>
#include <wesos-types/Group.hh>
#include <wesos-types/Lambda.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Nullable.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/Pointer.hh>
#include <wesos-types/Primitive.hh>
#include <wesos-types/StrongOrdering.hh>
#include <wesos-types/Template.hh>

namespace wesos {
  using types::f32;
  using types::f64;
  using types::i16;
  using types::i32;
  using types::i64;
  using types::i8;
  using types::u16;
  using types::u32;
  using types::u64;
  using types::u8;
  using types::usize;

  using types::Array;
  using types::Lambda;
  using types::Nullable;
  using types::Pair;
  using types::Quadlet;
  using types::Triplet;

  using types::OwnPtr;
  using types::RefPtr;

  using types::operator""_u8;
  using types::operator""_u16;
  using types::operator""_u32;
  using types::operator""_u64;
  using types::operator""_i8;
  using types::operator""_i16;
  using types::operator""_i32;
  using types::operator""_i64;
  using types::operator""_f32;
  using types::operator""_f64;
  using types::operator""_usize;
}  // namespace wesos