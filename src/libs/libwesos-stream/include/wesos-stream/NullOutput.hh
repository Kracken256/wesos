/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/OutputStreamProtocol.hh>

namespace wesos::stream {
  class NullOutput final : public OutputStreamProtocol {
    usize m_offset = 0, m_cache_size = 1;

  protected:
    [[nodiscard]] auto virt_write_some(View<u8> someof) -> WriteResult override;
    [[nodiscard]] auto virt_write_byte(u8 b) -> bool override;
    [[nodiscard]] auto virt_write_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_flush() -> bool override;
    [[nodiscard]] auto virt_set_cache(usize size) -> usize override;
    [[nodiscard]] auto virt_write_pos() const -> Nullable<usize> override;
    [[nodiscard]] auto virt_cache_size() const -> usize override;

  public:
    constexpr NullOutput() = default;
    constexpr NullOutput(const NullOutput&) = delete;
    constexpr NullOutput(NullOutput&&) = delete;
    constexpr auto operator=(const NullOutput&) -> NullOutput& = delete;
    constexpr auto operator=(NullOutput&&) -> NullOutput& = delete;
    constexpr ~NullOutput() override = default;
  };
}  // namespace wesos::stream
