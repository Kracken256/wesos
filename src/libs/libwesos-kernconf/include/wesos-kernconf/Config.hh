/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::kernconf {
  class KernelConfig {
  public:
    constexpr KernelConfig() = default;
    constexpr KernelConfig(const KernelConfig&) = delete;
    constexpr KernelConfig(KernelConfig&&) = default;
    constexpr auto operator=(const KernelConfig&) -> KernelConfig& = delete;
    constexpr auto operator=(KernelConfig&&) -> KernelConfig& = default;
    constexpr ~KernelConfig() = default;

    void insert(View<const u8> key, View<const u8> value);
  };
}  // namespace wesos::kernconf
