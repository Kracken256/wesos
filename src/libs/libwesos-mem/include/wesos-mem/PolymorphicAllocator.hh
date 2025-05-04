/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-mem/New.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  template <typename Object>
  class PolymorphicAllocator {
  public:
    using ValueType = Object;

    constexpr explicit PolymorphicAllocator(MemoryResourceProtocol& r) : m_resource(r) {}

    template <typename U>
    constexpr PolymorphicAllocator(const PolymorphicAllocator<U>& o) : m_resource(o.resource()) {}

    constexpr PolymorphicAllocator(const PolymorphicAllocator&) = default;
    constexpr PolymorphicAllocator(PolymorphicAllocator&&) = default;
    constexpr auto operator=(const PolymorphicAllocator&) -> PolymorphicAllocator& = delete;
    constexpr auto operator=(PolymorphicAllocator&&) -> PolymorphicAllocator& = default;
    constexpr ~PolymorphicAllocator() = default;

    [[nodiscard]] auto allocate_storage(usize n) -> NullableOwnPtr<Object> {
      const auto address = m_resource.allocate_bytes(n * sizeof(Object), alignof(Object));
      return static_cast<Object*>(address.unwrap());
    }

    void deallocate_storage(NullableOwnPtr<Object> p, usize n) {
      m_resource.deallocate_bytes(p.unwrap(), n * sizeof(Object), alignof(Object));
    }

    [[nodiscard]] auto resource() const -> MemoryResourceProtocol& { return m_resource; }

    template <typename... Args>
    void construct(Object& p, Args&&... args) {
      ::new (static_cast<void*>(&p)) Object(forward<Args>(args)...);
    }

    void destroy(Object& p) { p.~Object(); }

  private:
    MemoryResourceProtocol& m_resource;
  };
}  // namespace wesos::mem
