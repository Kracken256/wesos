/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-smartptr/Box.hh>
#include <wesos-stream/OutputStreamProtocol.hh>
#include <wesos-sync/SpinLock.hh>

namespace wesos::stream {
  class AtomicOutputStreamRef final : public OutputStreamProtocol {
    friend smartptr::Box<AtomicOutputStreamRef>;

    mutable smartptr::Box<sync::SpinLock> m_lock;
    OutputStreamProtocol& m_inner;

    AtomicOutputStreamRef(smartptr::Box<sync::SpinLock> lock, OutputStreamProtocol& parent);

  protected:
    [[nodiscard]] auto virt_write_some(View<u8> someof) -> WriteResult override;
    [[nodiscard]] auto virt_write_byte(u8 b) -> bool override;
    [[nodiscard]] auto virt_write_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_flush() -> bool override;
    [[nodiscard]] auto virt_set_cache(usize size) -> usize override;
    [[nodiscard]] auto virt_write_pos() const -> Nullable<usize> override;
    [[nodiscard]] auto virt_cache_size() const -> usize override;

  public:
    constexpr AtomicOutputStreamRef(const AtomicOutputStreamRef&) = delete;
    constexpr AtomicOutputStreamRef(AtomicOutputStreamRef&&) = delete;
    constexpr auto operator=(const AtomicOutputStreamRef&) -> AtomicOutputStreamRef& = delete;
    constexpr auto operator=(AtomicOutputStreamRef&&) -> AtomicOutputStreamRef& = delete;
    constexpr ~AtomicOutputStreamRef() override = default;

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm,
                                     OutputStreamProtocol& parent) -> Nullable<smartptr::Box<AtomicOutputStreamRef>>;
  };

  class AtomicOutputStream final : public OutputStreamProtocol {
    friend smartptr::Box<AtomicOutputStream>;

    mutable smartptr::Box<sync::SpinLock> m_lock;
    smartptr::Box<OutputStreamProtocol> m_owned;

    AtomicOutputStream(smartptr::Box<sync::SpinLock> lock, smartptr::Box<OutputStreamProtocol> parent);

  protected:
    [[nodiscard]] auto virt_write_some(View<u8> someof) -> WriteResult override;
    [[nodiscard]] auto virt_write_byte(u8 b) -> bool override;
    [[nodiscard]] auto virt_write_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_flush() -> bool override;
    [[nodiscard]] auto virt_set_cache(usize size) -> usize override;
    [[nodiscard]] auto virt_write_pos() const -> Nullable<usize> override;
    [[nodiscard]] auto virt_cache_size() const -> usize override;

  public:
    constexpr AtomicOutputStream(const AtomicOutputStream&) = delete;
    constexpr AtomicOutputStream(AtomicOutputStream&&) = delete;
    constexpr auto operator=(const AtomicOutputStream&) -> AtomicOutputStream& = delete;
    constexpr auto operator=(AtomicOutputStream&&) -> AtomicOutputStream& = delete;
    constexpr ~AtomicOutputStream() override = default;

    template <class OutputStream, typename... Args>
    [[nodiscard]] static auto create_from(mem::MemoryResourceProtocol& mm,
                                          Args... args) -> Nullable<smartptr::Box<AtomicOutputStream>> {
      if (auto stream = smartptr::Box<OutputStream>::create(mm, forward<Args>(args)...)) [[likely]] {
        auto base = smartptr::box_cast<OutputStreamProtocol>(move(stream.value()));
        return smartptr::Box<AtomicOutputStream>::create(mm, mm, move(base));
      }

      return null;
    }

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm, smartptr::Box<OutputStreamProtocol> parent)
        -> Nullable<smartptr::Box<AtomicOutputStream>>;
  };
}  // namespace wesos::stream
