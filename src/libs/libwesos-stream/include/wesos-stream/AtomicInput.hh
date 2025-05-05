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
  class AtomicInputRef : public InputStreamProtocol {
    friend smartptr::Box<AtomicInputRef>;

    smartptr::Box<sync::LockProtocol> m_lock;
    InputStreamProtocol& m_inner;

    AtomicInputRef(mem::MemoryResourceProtocol& mm, InputStreamProtocol& parent);

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    constexpr AtomicInputRef(const AtomicInputRef&) = delete;
    constexpr AtomicInputRef(AtomicInputRef&&) = delete;
    constexpr auto operator=(const AtomicInputRef&) -> AtomicInputRef& = delete;
    constexpr auto operator=(AtomicInputRef&&) -> AtomicInputRef& = delete;
    ~AtomicInputRef() override;

    [[nodiscard]] static auto create(mem::MemoryResourceProtocol& mm,
                                     InputStreamProtocol& parent) -> Nullable<smartptr::Box<AtomicInputRef>>;
  };

  class AtomicInput : public InputStreamProtocol {
    friend smartptr::Box<AtomicInput>;

    smartptr::Box<sync::LockProtocol> m_lock;
    smartptr::Box<InputStreamProtocol> m_owned;

    AtomicInput(mem::MemoryResourceProtocol& mm, smartptr::Box<InputStreamProtocol> parent);

  protected:
    [[nodiscard]] auto virt_read_some(View<u8> someof) -> ReadResult override;
    [[nodiscard]] auto virt_read_byte() -> Nullable<u8> override;
    [[nodiscard]] auto virt_read_seek(isize pos) -> bool override;
    [[nodiscard]] auto virt_read_pos() const -> Nullable<usize> override;

  public:
    constexpr AtomicInput(const AtomicInput&) = delete;
    constexpr AtomicInput(AtomicInput&&) = delete;
    constexpr auto operator=(const AtomicInput&) -> AtomicInput& = delete;
    constexpr auto operator=(AtomicInput&&) -> AtomicInput& = delete;
    ~AtomicInput() override;

    template <class InputStream, typename... Args>
    [[nodiscard]] static auto create_owned(mem::MemoryResourceProtocol& mm,
                                           Args... args) -> Nullable<smartptr::Box<AtomicInput>> {
      if (auto stream = smartptr::Box<InputStream>::create(mm, forward<Args>(args)...)) [[likely]] {
        auto base = smartptr::box_cast<InputStreamProtocol>(move(stream.value()));
        return smartptr::Box<AtomicInput>::create(mm, mm, move(base));
      }

      return null;
    }
  };
}  // namespace wesos::stream
