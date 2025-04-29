/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::heap {
  class HeapProtocol {
  protected:
    /**
     * @param align > 0;
     * @param n_bytes > 0;
     *
     * @warning Neither parameter needs to be a power of two.
     * @warning This function may return uninitialized memory.
     */
    [[nodiscard, gnu::pure]] virtual auto virt_allocate(usize n_bytes,
                                                        usize align) -> Nullable<View<u8>> = 0;

    /**
     * @warning This function might not be idempotent (double frees constitute UB)
     */
    virtual void virt_deallocate(View<u8> ptr) = 0;

  public:
    virtual ~HeapProtocol() = 0;

    /**
     * @brief Allocate a contiguous memory block
     * @param align >= 0;
     * @param n_bytes >= 0;
     *
     * @warning Neither parameter needs to be a power of two.
     * @warning Not all instances are thread-safe
     */
    [[nodiscard, gnu::pure]] auto allocate(usize n_bytes, usize align = DEFAULT_ALIGNMENT_GLOBAL,
                                           bool zero_memory = true) -> Nullable<View<u8>>;

    /**
     * @brief Deallocate a contiguous memory block
     * @warning Not all instances are idempotent
     * @warning Not all instances are thread-safe
     */
    void deallocate(Nullable<View<u8>> ptr);

    static constexpr usize DEFAULT_ALIGNMENT_GLOBAL = 16;
  };
}  // namespace wesos::heap
