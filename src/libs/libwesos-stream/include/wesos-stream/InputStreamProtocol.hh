/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::stream {
  class ReadResult final {
    usize m_value;

  public:
    constexpr ReadResult(usize x) : m_value(x){};
    constexpr ReadResult(const ReadResult&) = default;
    constexpr ReadResult(ReadResult&&) = default;
    constexpr auto operator=(const ReadResult&) -> ReadResult& = default;
    constexpr auto operator=(ReadResult&&) -> ReadResult& = default;
    constexpr ~ReadResult() = default;

    [[nodiscard]] constexpr auto operator<=>(const auto& o) const { return m_value <=> o; }
    [[nodiscard]] constexpr auto operator->() const -> const usize* { return &m_value; }

    [[nodiscard]] constexpr auto is_okay() const -> bool { return m_value != 0; }
    [[nodiscard]] constexpr auto failed() const -> bool { return !is_okay(); }
    [[nodiscard]] constexpr auto count() const -> usize { return m_value; }

    [[nodiscard]] static constexpr auto null() -> ReadResult { return 0; };
  };

  class InputStreamProtocol {
  protected:
    [[nodiscard]] virtual auto virt_read_some(View<u8> someof) -> ReadResult = 0;
    [[nodiscard]] virtual auto virt_read_byte() -> Nullable<u8>;
    [[nodiscard]] virtual auto virt_read_seek(isize pos) -> bool;
    [[nodiscard]] virtual auto virt_read_pos() const -> Nullable<usize>;

  public:
    constexpr InputStreamProtocol() = default;
    constexpr InputStreamProtocol(const InputStreamProtocol&) = delete;
    constexpr InputStreamProtocol(InputStreamProtocol&&) = delete;
    constexpr auto operator=(const InputStreamProtocol&) -> InputStreamProtocol& = delete;
    constexpr auto operator=(InputStreamProtocol&&) -> InputStreamProtocol& = delete;
    constexpr virtual ~InputStreamProtocol() = default;

    [[nodiscard]] auto read(View<u8> allof) -> bool;
    [[nodiscard]] auto read_some(View<u8> someof) -> ReadResult;
    [[nodiscard]] auto read_byte() -> Nullable<u8>;
    [[nodiscard]] auto read_seek(isize pos) -> bool;
    [[nodiscard]] auto read_pos() const -> Nullable<usize>;
  };

}  // namespace wesos::stream
