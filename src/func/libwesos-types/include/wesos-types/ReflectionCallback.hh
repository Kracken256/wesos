/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types::detail {
  using ReflectionDepth = unsigned int;
  using ReflectionSize = unsigned int;
  using ReflectionCallback = void(void* m, const void* base, ReflectionSize size);
}  // namespace wesos::types::detail
