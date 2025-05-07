/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-kern/boot/KernSettings.hh>

using namespace wesos::kern::boot;

auto KernSettings::parse_config(View<const u8> config_text) -> Nullable<KernSettings> {
  /// TODO: Implement config parsing
  return null;
}
