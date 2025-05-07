/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::kern::boot {
  class KernSettings {
    void insert(View<const u8> key, View<const u8> value);

  public:
    constexpr KernSettings() = default;
    constexpr KernSettings(const KernSettings&) = delete;
    constexpr KernSettings(KernSettings&&) = default;
    constexpr auto operator=(const KernSettings&) -> KernSettings& = delete;
    constexpr auto operator=(KernSettings&&) -> KernSettings& = default;
    constexpr ~KernSettings() = default;

    static auto parse_config(View<const u8> config_text) -> Nullable<KernSettings>;
  };
}  // namespace wesos::kern::boot
