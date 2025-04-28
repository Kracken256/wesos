/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Lambda.hh>
#include <wesos-types/Primitive.hh>

namespace wesos::types {
  using ReflectionCallback = Lambda<void(const void* base, detail::PrimitiveTypeSize)>;
}  // namespace wesos::types
