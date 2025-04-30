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

  protected:
    /**
     * @param align doesn't need to be a power of 2.
     * @warning This function returns uninitialized memory.
     */
    [[nodiscard, gnu::pure]] virtual auto virt_allocate(Least<usize, 0> size, Least<usize, 1> align)
        -> Nullable<View<u8>> = 0;

    virtual void virt_deallocate(View<u8> ptr) = 0;

  public:
    virtual ~HeapProtocol() = 0;

    ///===========================================================================================
    /// UNLOCKED THREAD-UNSAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate_nosync(Least<usize, 0> size,
                                                  Least<usize, 1> align = DEFAULT_ALIGNMENT_GLOBAL,
                                                  bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate_nosync(Nullable<View<u8>> ptr);

    ///===========================================================================================
    /// INTERNALLY LOCKED THREAD-SAFE ACCESS
    ///===========================================================================================

    [[nodiscard, gnu::pure]] auto allocate(Least<usize, 0> size,
                                           Least<usize, 1> align = DEFAULT_ALIGNMENT_GLOBAL,
                                           bool zero_memory = true) -> Nullable<View<u8>>;

    void deallocate(Nullable<View<u8>> ptr);

    static constexpr usize DEFAULT_ALIGNMENT_GLOBAL = 16;
  };
}  // namespace wesos::heap
