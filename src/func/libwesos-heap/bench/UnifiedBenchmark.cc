/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include "UnifiedBenchmark.hh"

void wesos::heap::testing::synchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) {
  auto ptr = mm.allocate(options.m_alloc_min, options.m_align_min, false);
  mm.deallocate(ptr);

  /// TODO: Implement benchmark
}

void wesos::heap::testing::unsynchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) {
  auto ptr = mm.allocate_nosync(options.m_alloc_min, options.m_align_min, false);
  mm.deallocate_nosync(ptr);

  /// TODO: Implement benchmark
}
