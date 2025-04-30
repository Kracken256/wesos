/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <benchmark/benchmark.h>

#include <iostream>
#include <wesos-heap/IntrusivePool.hh>

#include "UnifiedBenchmark.hh"

using namespace wesos;
using namespace wesos::heap;
using namespace wesos::heap::testing;

using ClampedAlign = ClampMost<PowerOfTwo<usize>, 4096ULL>;

static void BM_IntrusivePool_Creation(benchmark::State& state) {
  assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  usize object_size = 0;
  ClampedAlign object_align = PowerOfTwo(1ULL);

  const auto storage_size = 1'000'000;
  std::vector<u8> storage(storage_size);
  auto storage_view = View<u8>(storage.data(), storage.capacity());

  for (auto x : state) {
    auto mm = IntrusivePool(object_size, object_align, storage_view);

    object_size++;
    object_align = object_align->next();
  }
}

static void BM_IntrusivePool_Synchronized(benchmark::State& state) {
  assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  usize object_size = 0;
  ClampedAlign object_align = PowerOfTwo(1ULL);

  const auto storage_size = 1'000'000;
  std::vector<u8> storage(storage_size);
  auto storage_view = View<u8>(storage.data(), storage.capacity());

  for (auto x : state) {
    auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);

    auto mm = IntrusivePool(object_size, object_align, storage_view);
    synchronized_benchmark(mm, options);

    object_size++;
    object_align = object_align->next();
  }
}

static void BM_IntrusivePool_Unsynchronized(benchmark::State& state) {
  assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  usize object_size = 0;
  ClampedAlign object_align = PowerOfTwo(1ULL);

  const auto storage_size = 1'000'000;
  std::vector<u8> storage(storage_size);
  auto storage_view = View<u8>(storage.data(), storage.capacity());

  for (auto x : state) {
    auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);

    auto mm = IntrusivePool(object_size, object_align, storage_view);
    unsynchronized_benchmark(mm, options);

    object_size++;
    object_align = object_align->next();
  }
}

BENCHMARK(BM_IntrusivePool_Creation);
BENCHMARK(BM_IntrusivePool_Synchronized);
BENCHMARK(BM_IntrusivePool_Unsynchronized);
