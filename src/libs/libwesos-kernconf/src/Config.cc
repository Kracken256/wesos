/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-kernconf/Config.hh>

using namespace wesos::kernconf;

SYM_EXPORT auto KernelConfig::contains(const char* key) const -> bool {
  // TODO: Check if the key exists in the config
  (void)key;
  return false;
}

SYM_EXPORT auto KernelConfig::get_uint64(const char* key) const -> Nullable<u64> {
  // TODO: Parse the value as a uint64
  (void)key;
  return null;
}

SYM_EXPORT auto KernelConfig::get_string(const char* key) const -> Nullable<View<const u8>> {
  // TODO: Parse the value as a string
  (void)key;
  return null;
}

SYM_EXPORT void KernelConfig::insert(View<const u8> key, View<const u8> value) {
  // TODO: Do dynamic allocation for storage
  (void)key;
  (void)value;
}
