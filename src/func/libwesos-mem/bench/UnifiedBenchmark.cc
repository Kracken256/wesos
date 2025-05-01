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

  static void benchmark_crunch(const AllocateFunc& allocate, const DeallocateFunc& deallocate,
                               const BenchmarkOptions& options, usize& alloc_count) {
    /// TODO: Implement realistic allocation patterns

    const auto [min_size, max_size, min_align, max_align] = options;

    for (usize size = min_size; size <= max_size; size++) {
      for (auto align = min_align; align <= max_align; align = align.next()) {
        auto ptr = allocate(size, align, false);
        alloc_count += ptr.isset();
        deallocate(ptr);
      }
    }
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

  benchmark_crunch(allocate, deallocate, options, alloc_count);
}
