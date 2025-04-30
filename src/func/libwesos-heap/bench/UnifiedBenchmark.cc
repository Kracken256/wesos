/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include "UnifiedBenchmark.hh"

namespace wesos::heap::testing {
  namespace detail {
    template <class ClassGeneric, class MethodGeneric>
    class ZeroCostDelegate final {
      ClassGeneric& m_base;
      MethodGeneric m_method;

    public:
      constexpr ZeroCostDelegate(ClassGeneric& base, MethodGeneric method)
          : m_base(base), m_method(method) {}

      template <typename... ArgsGeneric>
      constexpr auto operator()(ArgsGeneric... args) const {
        return (m_base.*m_method)(args...);
      }
    };
  }  // namespace detail

  using AllocateFunc = detail::ZeroCostDelegate<HeapProtocol, decltype(&HeapProtocol::allocate)>;
  using DeallocateFunc =
      detail::ZeroCostDelegate<HeapProtocol, decltype(&HeapProtocol::deallocate)>;

  static auto is_pool_allocator(const BenchmarkOptions& options) -> bool {
    return options.m_size_min == options.m_size_max;
  }

  static void benchmark_pool_allocator(const AllocateFunc& allocate,
                                       const DeallocateFunc& deallocate, usize size,
                                       PowerOfTwo<usize> align, usize& alloc_count) {
    auto ptr = allocate(size, align, false);
    deallocate(ptr);

    alloc_count += ptr.isset();
  }

  static void benchmark_crunch(const AllocateFunc& allocate, const DeallocateFunc& deallocate,
                               const BenchmarkOptions& options, usize& alloc_count) {
    (void)options;

    /// TODO: Do many allocations
    /// TODO: Implement realistic allocation patterns

    auto ptr = allocate(options.m_size_min, options.m_align_min, false);
    deallocate(ptr);

    alloc_count += ptr.isset();
  }

}  // namespace wesos::heap::testing

void wesos::heap::testing::allocator_benchmark(HeapProtocol& mm, bool sync,
                                               BenchmarkOptions options, usize& alloc_count) {
  const auto allocate = [&]() {
    if (sync) {
      return AllocateFunc(mm, &HeapProtocol::allocate);
    } else {
      return AllocateFunc(mm, &HeapProtocol::allocate_nosync);
    }
  }();

  const auto deallocate = [&]() {
    if (sync) {
      return DeallocateFunc(mm, &HeapProtocol::deallocate);
    } else {
      return DeallocateFunc(mm, &HeapProtocol::deallocate_nosync);
    }
  }();

  if (is_pool_allocator(options)) {
    benchmark_pool_allocator(allocate, deallocate, options.m_size_max, options.m_align_max,
                             alloc_count);
    return;
  }

  benchmark_crunch(allocate, deallocate, options, alloc_count);
}
