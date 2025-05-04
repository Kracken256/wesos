/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-lambda/Lambda.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/Types.hh>

namespace wesos::mem {
  class TracingResource final : public MemoryResourceProtocol {
    [[nodiscard]] auto virt_do_utilize(View<u8> pool) -> LeftoverMemory override;
    [[nodiscard]] auto virt_do_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<u8> override;
    void virt_do_deallocate(OwnPtr<u8> ptr, usize size, PowerOfTwo<usize> align) override;

  public:
    using PrintCallback = int (*)(const char* fmt, ...);

    TracingResource(MemoryResourceProtocol& debugee, PrintCallback print);
    TracingResource(const TracingResource&) = delete;
    TracingResource(TracingResource&&) = default;
    auto operator=(const TracingResource&) -> TracingResource& = delete;
    auto operator=(TracingResource&&) -> TracingResource& = default;
    ~TracingResource() override = default;

    // [[nodiscard]] constexpr auto operator<=>(const TracingResource&) const = default;

  private:
    MemoryResourceProtocol* m_debugee;
    PrintCallback m_print;
  };
}  // namespace wesos::mem
