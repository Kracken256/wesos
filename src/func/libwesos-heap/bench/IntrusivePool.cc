/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <benchmark/benchmark.h>

#include <wesos-heap/IntrusivePool.hh>

#include "UnifiedBenchmark.hh"

using namespace wesos;
using namespace wesos::heap;
using namespace wesos::heap::testing;

static void BM_IntrusivePool_Creation(benchmark::State& state) {
  usize object_size = 0;
  usize object_align = 1;

  for (auto _ : state) {
    auto mm = IntrusivePool(object_size, object_align);

    object_size++;
    object_align *= 2;
  }
}

static void BM_IntrusivePool_Synchronized(benchmark::State& state) {
  usize object_size = 0;
  usize object_align = 1;

  for (auto _ : state) {
    auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);
    auto mm = IntrusivePool(object_size, object_align);
    synchronized_benchmark(mm, options);

    object_size++;
    object_align *= 2;
  }
}

static void BM_IntrusivePool_Unsynchronized(benchmark::State& state) {
  usize object_size = 0;
  usize object_align = 1;

  for (auto x : state) {
    auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);
    auto mm = IntrusivePool(object_size, object_align);
    unsynchronized_benchmark(mm, options);

    object_size++;
    object_align *= 2;
  }
}

BENCHMARK(BM_IntrusivePool_Creation);
BENCHMARK(BM_IntrusivePool_Synchronized);
BENCHMARK(BM_IntrusivePool_Unsynchronized);
