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
  public:
    virtual ~MemoryResourceProtocol() = 0;

    [[nodiscard]] constexpr auto operator<=>(const MemoryResourceProtocol&) const = default;

    ///=============================================================================================
    /// RAW MEMORY ALLOCATION
    ///=============================================================================================

    [[nodiscard]] auto allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void>;
    auto deallocate_bytes(NullableOwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void;
    auto utilize_bytes(View<u8> pool) -> void;

  private:
    virtual auto virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> = 0;
    virtual auto virt_do_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void = 0;
    virtual auto virt_do_utilize(View<u8> pool) -> void = 0;
  };
}  // namespace wesos::mem
