/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <boot/EFI.hh>

namespace wesos::boot::efi {
  struct LoggerChannel {};

  static inline constexpr auto operator<<(const LoggerChannel &log, const char16_t *str) -> const LoggerChannel & {
    /// TODO: Implement the logging function
    return log;
  }

  static inline constexpr auto operator<<(const LoggerChannel &log, const u16 *str) -> const LoggerChannel & {
    return log << bit_cast<const char16_t *>(str);
  }

  template <typename T>
  static inline constexpr auto operator<<(const LoggerChannel &log, const T &value) -> const LoggerChannel & {
    ///  TODO: Implement the logging function
    return log;
  }

  static inline constexpr LoggerChannel HELP;
  static inline constexpr LoggerChannel INFO;
  static inline constexpr LoggerChannel WARN;
  static inline constexpr LoggerChannel FAIL;
}  // namespace wesos::boot::efi
