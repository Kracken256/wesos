/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/OutputStreamProtocol.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::stream {
  class AtomicOutput : public OutputStreamProtocol {
    sync::LockProtocol& m_lock;
    OutputStreamProtocol& m_inner;

  protected:
    [[nodiscard]] auto virt_write_some(View<u8> someof) -> WriteResult override;
    [[nodiscard]] auto virt_write_byte(u8 b) -> bool override;
    [[nodiscard]] auto virt_write_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_flush() -> bool override;
    [[nodiscard]] auto virt_set_cache(usize size) -> usize override;
    [[nodiscard]] auto virt_write_pos() const -> Nullable<usize> override;
    [[nodiscard]] auto virt_cache_size() const -> usize override;

  public:
    AtomicOutput(OutputStreamProtocol& parent);
    constexpr AtomicOutput(const AtomicOutput&) = delete;
    constexpr AtomicOutput(AtomicOutput&&) = delete;
    constexpr auto operator=(const AtomicOutput&) -> AtomicOutput& = delete;
    constexpr auto operator=(AtomicOutput&&) -> AtomicOutput& = delete;
    ~AtomicOutput() override;
  };
}  // namespace wesos::stream
