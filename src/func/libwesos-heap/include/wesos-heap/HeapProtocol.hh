/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-sync/SpinLock.hh>
#include <wesos-types/Types.hh>

namespace wesos::heap {
  class HeapProtocol {
    sync::SpinLock m_mutex;

  public:
    using LeftoverMemoryView = Nullable<Unused<View<u8>>>;
    using LeftoverMemory = Pair<LeftoverMemoryView, LeftoverMemoryView>;

    [[nodiscard]] constexpr auto operator<=>(const HeapProtocol&) const = default;

    virtual ~HeapProtocol() = 0;

    ///===========================================================================================
    /// UNLOCKED THREAD-UNSAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate_nosync(Least<usize, 0> size, PowerOfTwo<usize> align,
                                                  bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate_nosync(Nullable<View<u8>> ptr);

    auto utilize_nosync(View<u8> extra_memory) -> LeftoverMemory;

    ///===========================================================================================
    /// INTERNALLY LOCKED THREAD-SAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate(Least<usize, 0> size, PowerOfTwo<usize> align,
                                           bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate(Nullable<View<u8>> ptr);

    auto utilize(View<u8> extra_memory) -> LeftoverMemory;

  protected:
    [[nodiscard, gnu::pure]] virtual auto virt_allocate(
        Least<usize, 0> size, PowerOfTwo<usize> align) -> Nullable<View<u8>> = 0;

    virtual void virt_deallocate(View<u8> ptr) = 0;

    virtual auto virt_utilize(View<u8> extra_memory) -> LeftoverMemory = 0;
  };
}  // namespace wesos::heap
