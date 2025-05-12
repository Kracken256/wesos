/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <benchmark/benchmark.h>

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <wesos-mem/IntrusivePool.hh>
#include <wesos-mem/TracingResource.hh>

#include "UnifiedBenchmark.hh"

using namespace wesos;
using namespace wesos::mem;
using namespace wesos::mem::testing;

using ClampedAlign = ClampMost<PowerOfTwo<usize>, 4096ULL>;

static void deps_setup() {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message, wesos::SourceLocation source) {
    std::cerr << "\n==========================================================================="
                 "===========\n"
              << "| Assertion Failed: \"" << message << "\";\n"
              << "| Function: [" << source.function_name() << "]: " << source.line_number() << ";\n"
              << "| File: \"" << source.file_name() << "\";\n"
              << "============================================================================="
                 "=========\n"
              << std::endl;
  });
}

static void BM_IntrusivePool_Evo_Creation(benchmark::State& state) {
  deps_setup();

  IntrusivePool::ObjectSize object_size = 1;
  ClampedAlign object_align = PowerOfTwo(1UL);
  std::vector<u8> storage;

  for (auto x : state) {
    storage.resize(object_size + object_align->unwrap());
    auto storage_view = View<u8>(storage.data(), storage.size());

    auto mm = IntrusivePool(object_size, object_align, storage_view);

    object_size = object_size + 1;
    object_align = object_align->next();
  }
}

static void BM_IntrusivePool_Evo_Synchronized(benchmark::State& state) {
  deps_setup();

  usize alloc_count = 0;
  isize bytes_processed = 0;
  IntrusivePool::ObjectSize object_size = 1;
  ClampedAlign object_align = PowerOfTwo(1UL);
  std::vector<u8> storage;

  for (auto x : state) {
    const auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);

    storage.resize(object_size + object_align->unwrap());
    auto storage_view = View<u8>(storage.data(), storage.size());

    auto mm = IntrusivePool(object_size, object_align, storage_view);
    allocator_benchmark(mm, true, options, alloc_count);

    bytes_processed += object_size;

    object_size = object_size + 1;
    object_align = object_align->next();
  }

  state.SetItemsProcessed(isize(alloc_count));
  state.SetBytesProcessed(bytes_processed);
}

static void BM_IntrusivePool_Evo_Unsynchronized(benchmark::State& state) {
  deps_setup();

  usize alloc_count = 0;
  isize bytes_processed = 0;
  IntrusivePool::ObjectSize object_size = 1;
  ClampedAlign object_align = PowerOfTwo(1UL);
  std::vector<u8> storage;

  for (auto x : state) {
    const auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);

    storage.resize(object_size + object_align->unwrap());
    auto storage_view = View<u8>(storage.data(), storage.size());

    auto mm = IntrusivePool(object_size, object_align, storage_view);
    allocator_benchmark(mm, false, options, alloc_count);

    bytes_processed += object_size;

    object_size = object_size + 1;
    object_align = object_align->next();
  }

  state.SetItemsProcessed(isize(alloc_count));
  state.SetBytesProcessed(bytes_processed);
}

static void BM_IntrusivePool_Mono_Creation(benchmark::State& state) {
  deps_setup();

  constexpr IntrusivePool::ObjectSize object_size = sizeof(int);
  constexpr usize object_align = alignof(int);
  [[gnu::aligned(object_align)]] Array<u8, object_size.unwrap()> storage;

  for (auto x : state) {
    auto mm = IntrusivePool(object_size, object_align, storage.as_view());
  }
}

static void BM_IntrusivePool_Mono_Synchronized(benchmark::State& state) {
  deps_setup();

  struct MockData {
    u64 m_data;
    MockData *m_prev, *m_next;
  };

  usize alloc_count = 0;
  constexpr IntrusivePool::ObjectSize object_size = sizeof(MockData);
  constexpr usize object_align = alignof(MockData);
  [[gnu::aligned(object_align)]] Array<u8, object_size.unwrap()> storage;

  const auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);
  auto mm = IntrusivePool(object_size, object_align, storage.as_view());

  for (auto x : state) {
    allocator_benchmark(mm, true, options, alloc_count);
  }

  state.SetItemsProcessed(isize(alloc_count));
  state.SetBytesProcessed(isize(alloc_count * object_size));
}

static void BM_IntrusivePool_Mono_Unsynchronized(benchmark::State& state) {
  deps_setup();

  struct MockData {
    u64 m_data;
    MockData *m_prev, *m_next;
  };

  usize alloc_count = 0;
  constexpr IntrusivePool::ObjectSize object_size = sizeof(MockData);
  constexpr usize object_align = alignof(MockData);
  [[gnu::aligned(object_align)]] Array<u8, object_size.unwrap()> storage;

  const auto options = BenchmarkOptions(object_size, object_size, object_align, object_align);
  auto mm = IntrusivePool(object_size, object_align, storage.as_view());

  for (auto x : state) {
    allocator_benchmark(mm, false, options, alloc_count);
  }

  state.SetItemsProcessed(isize(alloc_count));
  state.SetBytesProcessed(isize(alloc_count * object_size));
}

BENCHMARK(BM_IntrusivePool_Evo_Creation);
BENCHMARK(BM_IntrusivePool_Evo_Synchronized);
BENCHMARK(BM_IntrusivePool_Evo_Unsynchronized);
BENCHMARK(BM_IntrusivePool_Mono_Creation);
BENCHMARK(BM_IntrusivePool_Mono_Synchronized);
BENCHMARK(BM_IntrusivePool_Mono_Unsynchronized);
