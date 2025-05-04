/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Array.hh>
#include <wesos-types/ClampLeast.hh>
#include <wesos-types/ClampMost.hh>
#include <wesos-types/Group.hh>
#include <wesos-types/Least.hh>
#include <wesos-types/Most.hh>
#include <wesos-types/Null.hh>
#include <wesos-types/Nullable.hh>
#include <wesos-types/NullableOwnPtr.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Numeric.hh>
#include <wesos-types/OwnPtr.hh>
#include <wesos-types/PowerOfTwo.hh>
#include <wesos-types/Primitive.hh>
#include <wesos-types/Ptr.hh>
#include <wesos-types/Range.hh>
#include <wesos-types/RefPtr.hh>
#include <wesos-types/StrongOrdering.hh>
#include <wesos-types/Template.hh>
#include <wesos-types/ThreadSafe.hh>
#include <wesos-types/Untrusted.hh>
#include <wesos-types/Unused.hh>
#include <wesos-types/View.hh>

namespace wesos {
  using types::f32;
  using types::f64;
  using types::i16;
  using types::i32;
  using types::i64;
  using types::i8;
  using types::isize;
  using types::u16;
  using types::u32;
  using types::u64;
  using types::u8;
  using types::uptr;
  using types::usize;

  using types::numeric_limit_max;
  using types::numeric_limit_min;

  using types::null;

  using types::Array;
  using types::Nullable;
  using types::Pair;
  using types::Quadlet;
  using types::ThreadSafe;
  using types::Triplet;
  using types::Untrusted;
  using types::Unused;

  using types::NullableOwnPtr;
  using types::NullableRefPtr;
  using types::OwnPtr;
  using types::RefPtr;
  using types::View;

  using types::bytes_until_next_aligned_pow2;
  using types::is_aligned_pow2;
  using types::next_aligned_pow2;

  using types::ClampLeast;
  using types::ClampMost;
  using types::Least;
  using types::Most;
  using types::PowerOfTwo;
  using types::Range;

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