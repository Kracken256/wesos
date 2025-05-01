/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos {
  [[nodiscard]] constexpr static auto min(auto a, auto b) { return a < b ? a : b; }
  [[nodiscard]] constexpr static auto max(auto a, auto b) { return a > b ? a : b; }
}  // namespace wesos