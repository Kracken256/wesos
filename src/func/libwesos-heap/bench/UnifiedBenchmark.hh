/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-heap/HeapProtocol.hh>

namespace wesos::heap::testing {
  struct BenchmarkOptions {
    usize m_alloc_min;
    usize m_alloc_max;
    usize m_align_min;
    usize m_align_max;

    constexpr BenchmarkOptions(usize alloc_min, usize alloc_max, usize align_min, usize align_max)
        : m_alloc_min(alloc_min),
          m_alloc_max(alloc_max),
          m_align_min(align_min),
          m_align_max(align_max) {}
  };

  void synchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options);
  void unsynchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options);
}  // namespace wesos::heap::testing
