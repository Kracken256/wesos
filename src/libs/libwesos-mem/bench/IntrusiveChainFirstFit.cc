// /**
//  * This file is part of the WesOS project.
//  *
//  * WesOS is public domain software: you can redistribute it and/or modify
//  * it under the terms of the Unlicense(https://unlicense.org/).
//  */

// #include <benchmark/benchmark.h>

// #include <iostream>
// #include <wesos-mem/IntrusiveChainFirstFit.hh>

// #include "UnifiedBenchmark.hh"

// using namespace wesos;
// using namespace wesos::mem;
// using namespace wesos::mem::testing;

// using ClampedAlign = ClampMost<PowerOfTwo<usize>, 16ULL>;

// static void deps_setup() {
//   wesos::assert::register_output_callback(
//       nullptr,
//       [](void*, const char* message, const char* func_name, const char* file_name, int line) {
//         std::cerr <<
//         "\n==========================================================================="
//                      "===========\n"
//                   << "| Assertion Failed: \"" << message << "\";\n"
//                   << "| Function: [" << func_name << "]: " << line << ";\n"
//                   << "| File: \"" << file_name << "\";\n"
//                   <<
//                   "============================================================================="
//                      "=========\n"
//                   << std::endl;
//       });
// }

// static void BM_IntrusiveChainFirstFit_Evo_Creation(benchmark::State& state) {
//   deps_setup();

//   constexpr usize pool_size = 1'000'000;
//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//   }
// }

// static void BM_IntrusiveChainFirstFit_Evo_Synchronized(benchmark::State& state) {
//   deps_setup();

//   const usize pool_size = 1'000'000;
//   const auto options = BenchmarkOptions(1U, 4096U, 1U, 16U);

//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   usize alloc_count = 0;
//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//     allocator_benchmark(mm, true, options, alloc_count);
//   }

//   state.SetItemsProcessed(isize(alloc_count));
// }

// static void BM_IntrusiveChainFirstFit_Evo_Unsynchronized(benchmark::State& state) {
//   deps_setup();

//   const usize pool_size = 1'000'000;
//   const auto options = BenchmarkOptions(1U, 4096U, 1U, 16U);

//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   usize alloc_count = 0;
//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//     allocator_benchmark(mm, false, options, alloc_count);
//   }

//   state.SetItemsProcessed(isize(alloc_count));
// }

// static void BM_IntrusiveChainFirstFit_Mono_Creation(benchmark::State& state) {
//   deps_setup();

//   constexpr usize pool_size = 1'000'000;
//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//   }
// }

// static void BM_IntrusiveChainFirstFit_Mono_Synchronized(benchmark::State& state) {
//   deps_setup();

//   const usize pool_size = 1'000'000;
//   const auto options = BenchmarkOptions(32U, 32U, 8U, 8U);

//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   usize alloc_count = 0;
//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//     allocator_benchmark(mm, true, options, alloc_count);
//   }

//   state.SetItemsProcessed(isize(alloc_count));
// }

// static void BM_IntrusiveChainFirstFit_Mono_Unsynchronized(benchmark::State& state) {
//   deps_setup();

//   const usize pool_size = 1'000'000;
//   const auto options = BenchmarkOptions(32U, 32U, 8U, 8U);

//   auto storage = std::vector<u8>(pool_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   usize alloc_count = 0;
//   for (auto x : state) {
//     auto mm = IntrusiveChainFirstFit(storage_view);
//     allocator_benchmark(mm, false, options, alloc_count);
//   }

//   state.SetItemsProcessed(isize(alloc_count));
// }

// BENCHMARK(BM_IntrusiveChainFirstFit_Evo_Creation);
// BENCHMARK(BM_IntrusiveChainFirstFit_Evo_Synchronized);
// BENCHMARK(BM_IntrusiveChainFirstFit_Evo_Unsynchronized);
// BENCHMARK(BM_IntrusiveChainFirstFit_Mono_Creation);
// BENCHMARK(BM_IntrusiveChainFirstFit_Mono_Synchronized);
// BENCHMARK(BM_IntrusiveChainFirstFit_Mono_Unsynchronized);
