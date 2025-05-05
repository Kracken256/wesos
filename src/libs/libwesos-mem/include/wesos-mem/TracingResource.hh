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
  class TracingResource final : public MemoryResourceProtocol {
    [[nodiscard]] auto virt_embezzle(usize max_size) -> View<u8> override;
    [[nodiscard]] auto virt_allocate(usize size, PowerOfTwo<usize> align) -> NullableOwnPtr<void> override;
    void virt_deallocate(OwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) override;
    auto virt_utilize(View<u8> pool) -> void override;

  public:
    using PrintCallback = int (*)(const char* fmt, ...);

    TracingResource(MemoryResourceProtocol& debugee, PrintCallback print);
    TracingResource(const TracingResource&) = delete;
    TracingResource(TracingResource&&) = delete;
    auto operator=(const TracingResource&) -> TracingResource& = delete;
    auto operator=(TracingResource&&) -> TracingResource& = delete;
    ~TracingResource() override = default;

  private:
    MemoryResourceProtocol& m_debugee;
    PrintCallback m_print;
  };
}  // namespace wesos::mem
