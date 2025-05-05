/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/AtomicInputStream.hh>
#include <wesos-stream/AtomicOutputStream.hh>
#include <wesos-stream/StreamProtocol.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::stream {
  class AtomicStream : public StreamProtocol {
    sync::LockProtocol& m_lock;
    StreamProtocol& m_inner;

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

    [[nodiscard]] auto virt_write_some(View<u8> someof) -> WriteResult override;
    [[nodiscard]] auto virt_write_byte(u8 b) -> bool override;
    [[nodiscard]] auto virt_write_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_flush() -> bool override;
    [[nodiscard]] auto virt_set_cache(usize size) -> usize override;
    [[nodiscard]] auto virt_write_pos() const -> Nullable<usize> override;
    [[nodiscard]] auto virt_cache_size() const -> usize override;

  public:
    AtomicStream(StreamProtocol& parent);
    AtomicStream(InputStreamProtocol& input, OutputStreamProtocol& output);
    constexpr AtomicStream(const AtomicStream&) = delete;
    constexpr AtomicStream(AtomicStream&&) = delete;
    constexpr auto operator=(const AtomicStream&) -> AtomicStream& = delete;
    constexpr auto operator=(AtomicStream&&) -> AtomicStream& = delete;
    ~AtomicStream() override;
  };
}  // namespace wesos::stream
