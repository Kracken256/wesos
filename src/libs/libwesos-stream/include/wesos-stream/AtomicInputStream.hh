/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-smartptr/Box.hh>
#include <wesos-stream/InputStreamProtocol.hh>
#include <wesos-sync/LockProtocol.hh>

namespace wesos::stream {
  class AtomicInputStreamRef : public InputStreamProtocol {
    friend smartptr::Box<AtomicInputStreamRef>;

    mutable smartptr::Box<sync::LockProtocol> m_lock;
    InputStreamProtocol& m_inner;

    AtomicInputStreamRef(smartptr::Box<sync::LockProtocol> lock, InputStreamProtocol& parent);

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    constexpr AtomicInputStreamRef(const AtomicInputStreamRef&) = delete;
    constexpr AtomicInputStreamRef(AtomicInputStreamRef&&) = delete;
    constexpr auto operator=(const AtomicInputStreamRef&) -> AtomicInputStreamRef& = delete;
    constexpr auto operator=(AtomicInputStreamRef&&) -> AtomicInputStreamRef& = delete;
    constexpr ~AtomicInputStreamRef() override = default;

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm,
                                     InputStreamProtocol& parent) -> Nullable<smartptr::Box<AtomicInputStreamRef>>;
  };

  class AtomicInputStream : public InputStreamProtocol {
    friend smartptr::Box<AtomicInputStream>;

    mutable smartptr::Box<sync::LockProtocol> m_lock;
    smartptr::Box<InputStreamProtocol> m_owned;

    AtomicInputStream(smartptr::Box<sync::LockProtocol> lock, smartptr::Box<InputStreamProtocol> parent);

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    constexpr AtomicInputStream(const AtomicInputStream&) = delete;
    constexpr AtomicInputStream(AtomicInputStream&&) = delete;
    constexpr auto operator=(const AtomicInputStream&) -> AtomicInputStream& = delete;
    constexpr auto operator=(AtomicInputStream&&) -> AtomicInputStream& = delete;
    constexpr ~AtomicInputStream() override = default;

    template <class InputStream, typename... Args>
    [[nodiscard]] static auto create_owned(mem::MemoryResourceProtocol& mm,
                                           Args... args) -> Nullable<smartptr::Box<AtomicInputStream>> {
      if (auto stream = smartptr::Box<InputStream>::create(mm, forward<Args>(args)...)) [[likely]] {
        auto base = smartptr::box_cast<InputStreamProtocol>(move(stream.value()));
        return smartptr::Box<AtomicInputStream>::create(mm, mm, move(base));
      }

      return null;
    }
  };
}  // namespace wesos::stream
