/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <mutex>

#define DEFER_INCLUDE
#include "UnifiedBenchmark.hh"

namespace wesos::mem::testing {
  namespace detail {
    template <class Class, class Method>
    class ZeroCostDelegate final {
      Class& m_base;
      Method m_method;

    public:
      constexpr ZeroCostDelegate(Class& base, Method method) : m_base(base), m_method(method) {}

      template <class... Args>
      constexpr auto operator()(Args... args) const {
        return (m_base.*m_method)(args...);
      }
    };
  }  // namespace detail

  static void benchmark_crunch(const auto& allocate, const auto& deallocate, const BenchmarkOptions& options,
                               usize& alloc_count) {
    /// FIXME: Implement realistic allocation patterns

    const auto [min_size, max_size, min_align, max_align] = options;

    for (usize size = min_size; size <= max_size; size++) {
      for (auto align = min_align; align <= max_align; align = align.next()) {
        auto ptr = allocate(size, align);
        alloc_count += ptr.isset();
        deallocate(ptr, size, align);
      }
    }
  }

}  // namespace wesos::mem::testing

static wesos::sync::SpinLock simulate_global_lock;

void wesos::mem::testing::allocator_benchmark(MemoryResourceProtocol& mm, bool sync, BenchmarkOptions options,
                                              usize& alloc_count) {
  const auto allocate_sync = [&](usize size, PowerOfTwo<usize> align) {
    std::lock_guard lock(simulate_global_lock);
    return mm.allocate_bytes(size, align);
  };

  const auto deallocate_sync = [&](NullableOwnPtr<void> ptr, usize size, PowerOfTwo<usize> align) {
    std::lock_guard lock(simulate_global_lock);
    return mm.deallocate_bytes(ptr, size, align);
  };

  using AllocateFunc =
      detail::ZeroCostDelegate<MemoryResourceProtocol, decltype(&MemoryResourceProtocol::allocate_bytes)>;
  using DeallocateFunc =
      detail::ZeroCostDelegate<MemoryResourceProtocol, decltype(&MemoryResourceProtocol::deallocate_bytes)>;

  const auto allocate_nosync = [&]() { return AllocateFunc(mm, &MemoryResourceProtocol::allocate_bytes); }();

  const auto deallocate_nosync = [&]() { return DeallocateFunc(mm, &MemoryResourceProtocol::deallocate_bytes); }();

  if (sync) {
    benchmark_crunch(allocate_sync, deallocate_sync, options, alloc_count);
  } else {
    benchmark_crunch(allocate_nosync, deallocate_nosync, options, alloc_count);
  }
}
