/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>

namespace wesos::mem::testing {
  struct BenchmarkOptions {
    usize m_size_min, m_size_max;
    PowerOfTwo<usize> m_align_min, m_align_max;

    constexpr BenchmarkOptions(auto size_min, auto size_max, auto align_min, auto align_max)
        : m_size_min(size_min),
          m_size_max(size_max),
          m_align_min(align_min),
          m_align_max(align_max) {}
  };

  void allocator_benchmark(MemoryResourceProtocol& mm, bool sync, BenchmarkOptions options,
                           usize& alloc_count);
}  // namespace wesos::mem::testing
