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

  static auto benchmark_pool_allocator(const AllocateFunc& allocate,
                                       const DeallocateFunc& deallocate, usize size,
                                       PowerOfTwo<usize> align) -> bool {
    auto ptr = allocate(size, align, false);
    deallocate(ptr);

    return ptr.isset();
  }

  static auto benchmark(const AllocateFunc& allocate, const DeallocateFunc& deallocate,
                        BenchmarkOptions options) -> bool {
    if (is_pool_allocator(options)) {
      return benchmark_pool_allocator(allocate, deallocate, options.m_size_max,
                                      options.m_align_max);
    }

    /// TODO: Implement general allocator benchmarks

    return false;
  }

  auto synchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) -> bool {
    const AllocateFunc allocate(mm, &HeapProtocol::allocate);
    const DeallocateFunc deallocate(mm, &HeapProtocol::deallocate);

    return benchmark(allocate, deallocate, options);
  }

  auto unsynchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) -> bool {
    const AllocateFunc allocate(mm, &HeapProtocol::allocate_nosync);
    const DeallocateFunc deallocate(mm, &HeapProtocol::deallocate_nosync);

    return benchmark(allocate, deallocate, options);
  }
}  // namespace wesos::heap::testing
