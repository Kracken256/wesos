/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/AtomicInputStream.hh>
#include <wesos-stream/AtomicOutputStream.hh>
#include <wesos-stream/StreamProtocol.hh>
#include <wesos-sync/SpinLock.hh>

namespace wesos::stream {
  class AtomicStreamRef : public StreamProtocol {
    friend smartptr::Box<AtomicStreamRef>;

    mutable smartptr::Box<sync::SpinLock> m_lock;
    StreamProtocol& m_inner;

    AtomicStreamRef(smartptr::Box<sync::SpinLock> lock, StreamProtocol& parent);

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
    constexpr AtomicStreamRef(const AtomicStreamRef&) = delete;
    constexpr AtomicStreamRef(AtomicStreamRef&&) = delete;
    constexpr auto operator=(const AtomicStreamRef&) -> AtomicStreamRef& = delete;
    constexpr auto operator=(AtomicStreamRef&&) -> AtomicStreamRef& = delete;
    constexpr ~AtomicStreamRef() override = default;

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm,
                                     StreamProtocol& parent) -> Nullable<smartptr::Box<AtomicStreamRef>>;
  };

  class AtomicStream final : public StreamProtocol {
    friend smartptr::Box<AtomicStream>;

    mutable smartptr::Box<sync::SpinLock> m_lock;
    smartptr::Box<StreamProtocol> m_owned;

    AtomicStream(smartptr::Box<sync::SpinLock> lock, smartptr::Box<StreamProtocol> parent);

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
    constexpr AtomicStream(const AtomicStream&) = delete;
    constexpr AtomicStream(AtomicStream&&) = delete;
    constexpr auto operator=(const AtomicStream&) -> AtomicStream& = delete;
    constexpr auto operator=(AtomicStream&&) -> AtomicStream& = delete;
    constexpr ~AtomicStream() override = default;

    template <class Stream, typename... Args>
    [[nodiscard]] static auto create_from(mem::MemoryResourceProtocol& mm,
                                          Args... args) -> Nullable<smartptr::Box<AtomicStream>> {
      if (auto stream = smartptr::Box<Stream>::create(mm, forward<Args>(args)...)) [[likely]] {
        auto base = smartptr::box_cast<StreamProtocol>(move(stream.value()));
        return smartptr::Box<AtomicStream>::create(mm, mm, move(base));
      }

      return null;
    }

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm,
                                     smartptr::Box<StreamProtocol> parent) -> Nullable<smartptr::Box<AtomicStream>>;
  };
}  // namespace wesos::stream
