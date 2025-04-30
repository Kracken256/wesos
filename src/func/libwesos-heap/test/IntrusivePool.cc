/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <iostream>
#include <memory_resource>
#include <wesos-heap/IntrusivePool.hh>

TEST(IntrusivePool, CreatePool) {
  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> pool(buf.data(), buf.size());

  auto heap = heap::IntrusivePool(sizeof(int), alignof(int), pool);
}

TEST(IntrusivePool, Allocate) {
  using namespace wesos;

  wesos::assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  constexpr auto prime_size_limit = 0x9527;
  constexpr auto align_limit = 256;
  constexpr auto alloc_limit = 31;

  std::pmr::polymorphic_allocator<u8> service;

  for (usize size = sizeof(void*); size < prime_size_limit; size++) {
    for (usize align = 1; align < align_limit; align *= 2) {
      const auto space_per_object = ((size + align - 1) & -align);
      const auto buffer_size = space_per_object * alloc_limit;

      u8* buf = reinterpret_cast<u8*>(service.allocate_bytes(buffer_size, align));
      ASSERT_NE(buf, nullptr);

      auto buf_view = View<u8>(buf, buffer_size);
      auto heap = heap::IntrusivePool(size, align, buf_view);

      for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
        EXPECT_NE(heap.allocate_nosync(size, align, false), null)
            << "Failed on size(" << size << "), " << "align(" << align << ")";
      }

      service.deallocate_bytes(buf, buffer_size);
    }
  }
}

/// TODO: Write more tests
