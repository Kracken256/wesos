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

    Kickstart(OwnPtr<u8> buffer_ptr, Least<usize, MINIMUM_INITIALIZATION_SIZE> buffer_size)
        : m_buffer_ptr(move(buffer_ptr)), m_buffer_size(buffer_size) {}
  };

  /**
   * @brief Initializes the memory system with a given initial buffer.
   *
   * This function sets up the memory system using the provided `Kickstart`
   * object as the initial buffer. It ensures that the memory system is
   * ready for further operations.
   *
   * @param initial_buffer The initial buffer used to kickstart the memory system.
   * @return true if the initialization was successful, false otherwise.
   */
  [[nodiscard]] auto initialize(Kickstart initial_buffer) -> bool;
};  // namespace wesos::mem

#ifdef WESOS_MEM_INITIALIZE_WITH_MALLOC
#include <cstdlib>

namespace wesos::mem {
  [[nodiscard]] static inline auto initialize_with_malloc() -> Nullable<RefPtr<void>> {
    void* buffer = std::malloc(Kickstart::MINIMUM_INITIALIZATION_SIZE);
    if (!buffer) {
      return null;
    }

    auto initial_buffer = Kickstart(static_cast<u8*>(buffer), Kickstart::MINIMUM_INITIALIZATION_SIZE);

    if (!initialize(initial_buffer)) {
      std::free(buffer);
      return null;
    }

    return RefPtr<void>(buffer);
  }
}  // namespace wesos::mem
#endif