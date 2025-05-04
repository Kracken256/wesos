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

    ///=============================================================================================
    /// MANAGE OBJECT INITIALIZATION
    ///=============================================================================================

    template <class Object, class... Args>
    void construct(Object& p, Args&&... args) {
      ::new (static_cast<void*>(&p)) Object(forward<Args>(args)...);
    }

    template <class Object>
    void destroy(Object& p) {
      p.~Object();
    }

    template <class Object>
    [[nodiscard]] auto allocate_storage(usize n) -> NullableOwnPtr<Object> {
      const auto address = allocate_bytes(n * sizeof(Object), alignof(Object));
      return static_cast<Object*>(address.unwrap());
    }

    template <class Object>
    void deallocate_storage(NullableOwnPtr<Object> p, usize n) {
      deallocate_bytes(p.unwrap(), n * sizeof(Object), alignof(Object));
    }

    template <class Object, class... Args>
    [[nodiscard]] auto allocate_and_construct(usize n, Args&&... args) -> NullableOwnPtr<Object> {
      const auto p = allocate_storage<Object>(n);

      if (p) {
        for (usize i = 0; i < n; i++) {
          construct<Object>(p.unwrap()[i], forward<Args>(args)...);
        }
      }

      return p;
    }

    template <class Object>
    void destroy_and_deallocate(NullableOwnPtr<Object> p, usize n) {
      if (p) {
        for (usize i = 0; i < n; i++) {
          destroy<Object>(p.unwrap()[i]);
        }

        deallocate_storage<Object>(p.unwrap(), n);
      }
    }

  private:
    virtual auto virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> = 0;
    virtual auto virt_do_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) -> void = 0;
    virtual auto virt_do_utilize(View<u8> pool) -> void = 0;
  };
}  // namespace wesos::mem
