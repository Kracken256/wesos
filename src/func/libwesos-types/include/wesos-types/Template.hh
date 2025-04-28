/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types {
  // NOLINTBEGIN(readability-identifier-naming)

  template <typename TpGeneric, typename UpGeneric>
  inline constexpr bool is_same_v = __is_same(TpGeneric, UpGeneric);

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
