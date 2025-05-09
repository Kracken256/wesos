/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <boot/EFI.hh>
#include <wesos-types/Types.hh>

namespace wesos::boot::efi {
  namespace detail {
    enum class LoggerLevel {
      NOTE,
      INFO,
      WARN,
      FAIL,
    };
  }

  class LoggerTarget {
    detail::LoggerLevel m_level;

  public:
    constexpr LoggerTarget(detail::LoggerLevel level) : m_level(level) {}

    [[nodiscard]] constexpr auto get_level() const { return m_level; }
  };

  struct LoggerChannel {
    LoggerChannel(detail::LoggerLevel level) {
      const u8 help_color = MAGENTA | BACKGROUND_BLACK;
      const u8 info_color = GREEN | BACKGROUND_BLACK;
      const u8 warn_color = YELLOW | BACKGROUND_BLACK;
      const u8 fail_color = RED | BACKGROUND_BLACK;
      const u8 default_color = LIGHTGRAY | BACKGROUND_BLACK;

      (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(default_color);
      (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"[ "_u16);

      switch (level) {
        case detail::LoggerLevel::NOTE: {
          (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(help_color);
          (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"NOTE"_u16);
          break;
        }

        case detail::LoggerLevel::INFO: {
          (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(info_color);
          (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"INFO"_u16);
          break;
        }

        case detail::LoggerLevel::WARN: {
          (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(warn_color);
          (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"WARN"_u16);
          break;
        }

        case detail::LoggerLevel::FAIL: {
          (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(fail_color);
          (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"FAIL"_u16);
          break;
        }
      }

      (void)SYSTEM_TABLE_GLOBAL->con_out().set_attribute(default_color);
      (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u" ]: "_u16);
    }
  };

  template <class T>
  static inline auto operator<<(const LoggerTarget &log, const T &value) -> LoggerChannel {
    auto channel = LoggerChannel(log.get_level());
    channel << value;
    return channel;
  }

  static inline auto operator<<(LoggerChannel log, const u16 *str) -> LoggerChannel {
    (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(str);
    return log;
  }

  static inline auto operator<<(LoggerChannel log, const char16_t *str) -> LoggerChannel {
    return log << bit_cast<const u16 *>(str);
  }

  template <typename T>
  static inline auto operator<<(LoggerChannel log, const T &value) -> LoggerChannel
    requires(types::is_integral_v<T>)
  {
    if constexpr (types::is_integral_v<T>) {
      const auto base_10 = 10;

      auto [num, is_negative] = [value]() -> Pair<u64, bool> {
        if constexpr (types::is_signed_v<T>) {
          if (value < 0) {
            return {static_cast<u64>(-value), true};
          }
        }

        return {static_cast<u64>(value), false};
      }();

      if (num == 0) {
        log << u"0";
      } else {
        if (is_negative) {
          (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(u"-"_u16);
        }

        const usize max_digits = 20;    // Maximum digits for u64 in base 10
        u16 str[max_digits + 1] = {0};  // +1 for null terminator
        usize index = max_digits;
        while (num > 0) {
          str[--index] = static_cast<u16>((num % base_10) + '0');
          num /= base_10;
        }

        (void)SYSTEM_TABLE_GLOBAL->con_out().output_string(str + index);
      }
    } else {
      static_assert(!types::is_same_v<T, T>, "Unsupported type for LoggerChannel");
    }

    return log;
  }

  static constexpr auto NOTE = LoggerTarget(detail::LoggerLevel::NOTE);
  static constexpr auto INFO = LoggerTarget(detail::LoggerLevel::INFO);
  static constexpr auto WARN = LoggerTarget(detail::LoggerLevel::WARN);
  static constexpr auto FAIL = LoggerTarget(detail::LoggerLevel::FAIL);

  static inline constexpr auto ENDL = u"\r\n";
}  // namespace wesos::boot::efi
