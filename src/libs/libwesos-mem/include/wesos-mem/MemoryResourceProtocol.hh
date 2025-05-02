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
    using LeftoverMemory = Pair<Unused<View<u8>>, Unused<View<u8>>>;

    virtual ~MemoryResourceProtocol() = 0;

    [[nodiscard]] constexpr auto operator<=>(const MemoryResourceProtocol&) const = default;

    [[nodiscard]] auto utilize_bytes(View<u8> pool) -> LeftoverMemory;
    [[nodiscard]] auto allocate_bytes(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<u8>;
    void deallocate_bytes(NullableOwnPtr<u8> ptr, usize size, PowerOfTwo<usize> align);

  private:
    virtual auto virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<u8> = 0;
    virtual auto virt_do_deallocate(OwnPtr<u8> ptr, usize size,
                                    PowerOfTwo<usize> align) -> void = 0;
    virtual auto virt_do_utilize(View<u8> pool) -> LeftoverMemory = 0;
  };
}  // namespace wesos::mem
