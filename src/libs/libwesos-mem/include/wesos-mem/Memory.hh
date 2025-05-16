/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/Global.hh>
#include <wesos-mem/MemoryEconomy.hh>
#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-mem/NullResource.hh>
#include <wesos-mem/TracingResource.hh>

namespace wesos::mem {
  struct Kickstart {
    static constexpr usize MINIMUM_INITIALIZATION_SIZE = 4096;

    OwnPtr<u8> m_buffer_ptr;
    Least<usize, MINIMUM_INITIALIZATION_SIZE> m_buffer_size;
  };

  [[nodiscard]] auto initialize(Kickstart initial_buffer) -> bool;
};  // namespace wesos::mem
