/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/InputStreamProtocol.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::stream {
  class AtomicInput : public InputStreamProtocol {
    sync::LockProtocol& m_lock;
    InputStreamProtocol& m_inner;

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    AtomicInput(InputStreamProtocol& parent);
    constexpr AtomicInput(const AtomicInput&) = delete;
    constexpr AtomicInput(AtomicInput&&) = delete;
    constexpr auto operator=(const AtomicInput&) -> AtomicInput& = delete;
    constexpr auto operator=(AtomicInput&&) -> AtomicInput& = delete;
    ~AtomicInput() override;
  };
}  // namespace wesos::stream
