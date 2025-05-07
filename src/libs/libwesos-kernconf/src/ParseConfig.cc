/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-kernconf/Parser.hh>

using namespace wesos;
using namespace wesos::kernconf;

static auto peek_byte(const View<const u8> &ss) -> Nullable<u8> {
  if (ss.empty()) [[unlikely]] {
    return null;
  }

  return ss.get(0);
}

static auto next_byte(View<const u8> &ss) -> Nullable<u8> {
  if (ss.empty()) [[unlikely]] {
    return null;
  }

  const auto ch = ss.get(0);
  ss.remove_prefix(1);

  return ch;
}

static auto read_hex_literal(View<const u8> &ss) -> Nullable<View<const u8>> {
  constexpr auto hex_chars = []() {
    Array<bool, 256> tab;
    tab.fill(false);

    for (u8 c = '0'; c <= '9'; ++c) {
      tab.set(c, true);
    }

    for (u8 c = 'a'; c <= 'f'; ++c) {
      tab.set(c, true);
    }

    for (u8 c = 'A'; c <= 'F'; ++c) {
      tab.set(c, true);
    }

    return tab;
  }();

  const auto *start = ss.begin();

  while (true) {
    const auto ch = peek_byte(ss);
    if (ch.is_null()) {
      return null;
    }

    const auto c = ch.value();
    if (!hex_chars.get(c)) {
      break;
    }

    next_byte(ss);
  }

  if (start == ss.begin()) [[unlikely]] {
    return null;  // '0x' is not valid
  }

  return View<const u8>(start - 2, ss.begin());  // -2 to include 0x prefix
}

static auto read_string_literal(View<const u8> &ss) -> Nullable<View<const u8>> {
  const auto quote = next_byte(ss);
  if (quote.is_null() || quote.value() != '"') [[unlikely]] {
    return null;
  }

  const auto *start = ss.begin();

  while (true) {
    const auto ch = next_byte(ss);
    if (ch.is_null()) {
      return null;
    }

    if (ch.value() == '"') {
      break;
    }
  }

  return View<const u8>(start, ss.begin() - 1);  // -1 to exclude closing quote
}

static auto read_value(View<const u8> &ss) -> Nullable<View<const u8>> {
  const auto ch = peek_byte(ss);
  if (ch.is_null()) [[unlikely]] {
    return null;
  }

  Nullable<View<const u8>> literal;

  switch (ch.value()) {
    case '0': {
      if (ss.size() >= 2 && ss.get(1) == 'x') {
        ss.remove_prefix(2);
        literal = read_hex_literal(ss);
      }
      break;
    }

    case '"': {
      literal = read_string_literal(ss);
      break;
    }

    default: {
      break;
    }
  }

  return literal;
}

static auto skip_whitespace(View<const u8> &ss) -> void {
  constexpr auto whitespace = []() {
    Array<bool, 256> tab;
    tab.fill(false);

    tab.set(' ', true);
    tab.set('\t', true);
    tab.set('\r', true);
    tab.set('\n', true);

    return tab;
  }();

  while (true) {
    const auto ch = peek_byte(ss);
    if (ch.is_null()) [[unlikely]] {
      return;
    }

    const auto c = ch.value();
    if (!whitespace.get(c)) {
      break;
    }

    next_byte(ss);
  }
}

static void skip_comment(View<const u8> &ss) {
  if (const auto ch = peek_byte(ss); ch.is_null() || ch.value() != '#') {
    return;
  }

  next_byte(ss);  // Skip '#'

  while (true) {
    const auto ch = next_byte(ss);
    if (ch.is_null() || ch.value() == '\n') [[unlikely]] {
      return;
    }
  }
}

static auto read_config_pair(View<const u8> &ss) -> Nullable<Pair<View<const u8>, View<const u8>>> {
  const auto key = read_value(ss);
  if (key.is_null()) [[unlikely]] {
    return null;
  }

  skip_whitespace(ss);
  skip_comment(ss);
  skip_whitespace(ss);

  const auto ch = next_byte(ss);
  if (ch.is_null() || ch.value() != '=') [[unlikely]] {
    return null;
  }

  skip_whitespace(ss);
  skip_comment(ss);
  skip_whitespace(ss);

  const auto value = read_value(ss);
  if (value.is_null()) [[unlikely]] {
    return null;
  }

  return {{key.value(), value.value()}};
}

void KernelConfig::insert(View<const u8> key, View<const u8> value) {
  /// TODO: Do dynamic allocation for storage
}

auto kernconf::parse_kernel_config(View<const u8> config_text) -> Nullable<KernelConfig> {
  auto ss = config_text;

  KernelConfig settings;

  while (true) {
    while (true) {
      const auto *old_pos = ss.begin();

      skip_whitespace(ss);
      skip_comment(ss);

      if (ss.begin() == old_pos) {
        break;
      }
    }

    if (peek_byte(ss).is_null()) {
      break;  // EOF
    }

    const auto pair = read_config_pair(ss);
    if (pair.is_null()) {
      return null;
    }

    settings.insert(pair->first(), pair->second());
  }

  return settings;
}
