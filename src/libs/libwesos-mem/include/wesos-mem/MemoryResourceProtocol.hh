/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/SpinLock.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class MemoryResourceProtocol {
    sync::SpinLock m_mutex;

  public:
    using LeftoverMemoryView = Nullable<Unused<View<u8>>>;
    using LeftoverMemory = Pair<LeftoverMemoryView, LeftoverMemoryView>;

    [[nodiscard]] constexpr auto operator<=>(const MemoryResourceProtocol&) const = default;

    virtual ~MemoryResourceProtocol() = 0;

    ///===========================================================================================
    /// UNLOCKED THREAD-UNSAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate_bytes_nosync(
        Least<usize, 0> size, PowerOfTwo<usize> align,
        bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate_bytes_nosync(Nullable<View<u8>> ptr);

    auto utilize_bytes_nosync(View<u8> pool) -> LeftoverMemory;

    ///===========================================================================================
    /// INTERNALLY LOCKED THREAD-SAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate_bytes(Least<usize, 0> size, PowerOfTwo<usize> align,
                                                 bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate_bytes(Nullable<View<u8>> ptr);

    auto utilize_bytes(View<u8> pool) -> LeftoverMemory;

    void anew();

  protected:
    [[nodiscard, gnu::pure]] virtual auto virt_allocate_bytes(
        Least<usize, 0> size, PowerOfTwo<usize> align) -> Nullable<View<u8>> = 0;

    virtual void virt_deallocate_bytes(View<u8> ptr) = 0;

    virtual auto virt_utilize_bytes(View<u8> pool) -> LeftoverMemory = 0;

    virtual void virt_anew() = 0;
  };
}  // namespace wesos::mem
