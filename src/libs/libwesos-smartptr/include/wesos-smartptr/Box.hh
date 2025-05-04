/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/IntrusivePool.hh>  /// TODO: Remove me
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-mem/PolymorphicAllocator.hh>
#include <wesos-types/NullableOwnPtr.hh>
#include <wesos-types/Template.hh>

namespace wesos::smartptr {
  template <class Pointee, class Allocator = mem::PolymorphicAllocator<Pointee>>
  class Box {
    NullableOwnPtr<Pointee> m_ptr;
    Allocator m_allocator;

    Box(OwnPtr<Pointee> ptr, Allocator allocator) : m_ptr(ptr), m_allocator(move(allocator)) {}

  public:
    constexpr Box(const Box& o) = delete;
    constexpr auto operator=(const Box& o) -> Box& = delete;
    constexpr Box(Box&& o) : m_ptr(o.m_ptr), m_allocator(move(o.m_allocator)) { o.m_ptr = nullptr; };

    constexpr auto operator=(Box&& o) -> Box& {
      if (this != &o) [[likely]] {
        m_ptr = o.m_ptr;
        o.m_ptr = nullptr;
      }

      return *this;
    };

    ~Box() {
      if (m_ptr.is_null()) {
        return;
      }

      auto raw_ptr = m_ptr.unwrap();

      m_allocator.destroy(*raw_ptr);
      m_allocator.deallocate_storage(raw_ptr, 1);

#ifndef NDEBUG
      m_ptr = nullptr;
#endif
    }

    template <class... Args>
    [[nodiscard]] static auto create(Args... args) {
      return [... args = forward<Args>(args)](mem::MemoryResourceProtocol& pmr) -> Nullable<Box> {
        auto pma = mem::PolymorphicAllocator<Pointee>(pmr);

        const auto storage = pma.allocate_storage(1);
        if (storage.is_null()) [[unlikely]] {
          return null;
        }

        pma.construct(*storage.unwrap(), args...);

        return Box(storage.unwrap(), move(pma));
      };
    }
  };
}  // namespace wesos::smartptr
