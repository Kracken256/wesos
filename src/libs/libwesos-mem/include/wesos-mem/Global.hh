/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::mem {
  class MemoryResourceProtocol;

  [[nodiscard]] auto get_default_resource() -> MemoryResourceProtocol&;
};  // namespace wesos::mem
