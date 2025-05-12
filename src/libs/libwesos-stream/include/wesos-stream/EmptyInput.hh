/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/InputStreamProtocol.hh>

namespace wesos::stream {
  class EmptyInput final : public InputStreamProtocol {
    usize m_offset = 0;

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    constexpr EmptyInput() = default;
    constexpr EmptyInput(const EmptyInput&) = delete;
    constexpr EmptyInput(EmptyInput&&) = delete;
    constexpr auto operator=(const EmptyInput&) -> EmptyInput& = delete;
    constexpr auto operator=(EmptyInput&&) -> EmptyInput& = delete;
    constexpr ~EmptyInput() override = default;
  };
}  // namespace wesos::stream
