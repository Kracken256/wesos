/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  template <typename ObjectGeneric>
  class PolymorphicAllocator {
  public:
    using ValueType = ObjectGeneric;

    constexpr explicit PolymorphicAllocator(RefPtr<MemoryResourceProtocol> r) : m_resource(r) {}

    template <typename UGeneric>
    constexpr PolymorphicAllocator(const PolymorphicAllocator<UGeneric>& o) : m_resource(o.resource()) {}

    constexpr PolymorphicAllocator(const PolymorphicAllocator&) = default;
    constexpr PolymorphicAllocator(PolymorphicAllocator&&) = default;
    constexpr auto operator=(const PolymorphicAllocator&) -> PolymorphicAllocator& = delete;
    constexpr auto operator=(PolymorphicAllocator&&) -> PolymorphicAllocator& = default;
    constexpr ~PolymorphicAllocator() = default;

    [[nodiscard]] auto allocate_storage(usize n) -> NullableOwnPtr<ObjectGeneric> {
      const auto address = m_resource->allocate_bytes(n * sizeof(ObjectGeneric), alignof(ObjectGeneric));
      return static_cast<ObjectGeneric*>(address.unwrap());
    }

    void deallocate_storage(NullableOwnPtr<ObjectGeneric> p, usize n) {
      m_resource->deallocate_bytes(p, n * sizeof(ObjectGeneric), alignof(ObjectGeneric));
    }

    [[nodiscard]] auto resource() const -> RefPtr<MemoryResourceProtocol> { return m_resource; }

    template <typename UninitializedObjectGeneric, typename... ArgsGeneric>
    void construct(RefPtr<UninitializedObjectGeneric> p, ArgsGeneric&&... args) {
      ::new (static_cast<void*>(p)) UninitializedObjectGeneric(forward<ArgsGeneric>(args)...);
    }

    template <typename InitializedObjectGeneric>
    void destroy(RefPtr<InitializedObjectGeneric> p) {
      p->~U();
    }

  private:
    RefPtr<MemoryResourceProtocol> m_resource;
  };
}  // namespace wesos::mem
