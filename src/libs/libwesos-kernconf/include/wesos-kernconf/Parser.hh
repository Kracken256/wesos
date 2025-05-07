/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-kernconf/Config.hh>

namespace wesos::kernconf {
  auto parse_kernel_config(View<const u8> config_text) -> Nullable<KernelConfig>;
}  // namespace wesos::kernconf
