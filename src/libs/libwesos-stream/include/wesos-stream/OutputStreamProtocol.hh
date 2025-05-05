/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::stream {
  class WriteResult final {
    usize m_value;

  public:
    constexpr WriteResult(usize x) : m_value(x){};
    constexpr WriteResult(const WriteResult&) = default;
    constexpr WriteResult(WriteResult&&) = default;
    constexpr auto operator=(const WriteResult&) -> WriteResult& = default;
    constexpr auto operator=(WriteResult&&) -> WriteResult& = default;
    constexpr ~WriteResult() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return m_value <=> o; }
    [[nodiscard]] constexpr auto operator->() const -> const usize* { return &m_value; }

    [[nodiscard]] constexpr auto is_okay() const -> bool { return m_value != 0; }
    [[nodiscard]] constexpr auto failed() const -> bool { return !is_okay(); }
    [[nodiscard]] constexpr auto count() const -> usize { return m_value; }

    [[nodiscard]] static constexpr auto null() -> WriteResult { return 0; };
  };

  class OutputStreamProtocol {
  protected:
    [[nodiscard]] virtual auto virt_write_some(View<u8> someof) -> WriteResult = 0;
    [[nodiscard]] virtual auto virt_write_byte(u8 b) -> bool;
    [[nodiscard]] virtual auto virt_write_seek(isize pos) -> bool;
    [[nodiscard]] virtual auto virt_flush() -> bool;
    [[nodiscard]] virtual auto virt_set_cache(usize size) -> usize;
    [[nodiscard]] virtual auto virt_write_pos() const -> Nullable<usize>;
    [[nodiscard]] virtual auto virt_cache_size() const -> usize;

  public:
    constexpr OutputStreamProtocol() = default;
    constexpr OutputStreamProtocol(const OutputStreamProtocol&) = delete;
    constexpr OutputStreamProtocol(OutputStreamProtocol&&) = delete;
    constexpr auto operator=(const OutputStreamProtocol&) -> OutputStreamProtocol& = delete;
    constexpr auto operator=(OutputStreamProtocol&&) -> OutputStreamProtocol& = delete;
    constexpr virtual ~OutputStreamProtocol() = default;

    [[nodiscard]] auto write(View<u8> allof) -> bool;
    [[nodiscard]] auto write_some(View<u8> someof) -> WriteResult;
    [[nodiscard]] auto write_byte(u8 b) -> bool;
    [[nodiscard]] auto write_seek(isize pos) -> bool;
    [[nodiscard]] auto flush() -> bool;
    [[nodiscard]] auto set_cache(usize size) -> usize;
    [[nodiscard]] auto write_pos() const -> Nullable<usize>;
    [[nodiscard]] auto cache_size() const -> usize;
  };
}  // namespace wesos::stream
