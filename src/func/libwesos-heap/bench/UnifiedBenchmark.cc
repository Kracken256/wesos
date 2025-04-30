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
    class MemberLambda final {
      ClassGeneric& m_base;
      MethodGeneric m_method;

    public:
      MemberLambda(ClassGeneric& base, MethodGeneric method) : m_base(base), m_method(method) {}

      template <typename... ArgsGeneric>
      auto operator()(ArgsGeneric... args) const {
        return (m_base.*m_method)(args...);
      }
    };
  }  // namespace detail

  using AllocateFunc = detail::MemberLambda<HeapProtocol, decltype(&HeapProtocol::allocate)>;
  using DeallocateFunc = detail::MemberLambda<HeapProtocol, decltype(&HeapProtocol::deallocate)>;

  static auto is_pool_allocator(const BenchmarkOptions& options) -> bool {
    return options.m_size_min == options.m_size_max;
  }

  static void benchmark_pool_allocator(const AllocateFunc& allocate,
                                       const DeallocateFunc& deallocate, usize size,
                                       PowerOfTwo<usize> align) {
    auto ptr = allocate(size, align, false);
    deallocate(ptr);
  }

  static void benchmark(const AllocateFunc& allocate, const DeallocateFunc& deallocate,
                        BenchmarkOptions options) {
    if (is_pool_allocator(options)) {
      benchmark_pool_allocator(allocate, deallocate, options.m_size_max, options.m_align_max);
      return;
    }

    /// TODO: Implement general allocator benchmarks
  }

  void synchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) {
    const AllocateFunc allocate(mm, &HeapProtocol::allocate);
    const DeallocateFunc deallocate(mm, &HeapProtocol::deallocate);

    benchmark(allocate, deallocate, options);
  }

  void unsynchronized_benchmark(HeapProtocol& mm, BenchmarkOptions options) {
    const AllocateFunc allocate(mm, &HeapProtocol::allocate_nosync);
    const DeallocateFunc deallocate(mm, &HeapProtocol::deallocate_nosync);

    benchmark(allocate, deallocate, options);
  }
}  // namespace wesos::heap::testing
