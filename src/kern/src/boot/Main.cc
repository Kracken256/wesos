/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-kernconf/Config.hh>
#include <wesos-types/Types.hh>

using namespace wesos;

namespace wesos::kern {
  auto main(const kernconf::KernelConfig& config) -> int {
    (void)config;

    // TODO: Implement the kernel

    return 0;
  }
}  // namespace wesos::kern
