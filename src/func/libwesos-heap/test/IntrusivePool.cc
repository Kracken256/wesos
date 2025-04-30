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
  std::vector<Nullable<View<u8>>> pointers;

  for (usize size = sizeof(void*); size < prime_size_limit; size++) {
    for (usize align = 1; align < align_limit; align *= 2) {
      pointers.clear();

      const auto space_per_object = ((size + align - 1) & -align);
      const auto buffer_size = space_per_object * alloc_limit;

      u8* buf = reinterpret_cast<u8*>(service.allocate_bytes(buffer_size, align));
      ASSERT_NE(buf, nullptr);

      auto buf_view = View<u8>(buf, buffer_size);
      auto heap = heap::IntrusivePool(size, align, buf_view);

      {  // Use all avaiable memory in pool
        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);
          pointers.push_back(the_alloc_ptr);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";
        }
      }

      {  // Release all allocated pool objects
        for (const auto& ptr : pointers) {
          heap.deallocate_nosync(ptr);
        }
      }

      {  // We expect to be able to get back all the memory that was freed
        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);
          pointers.push_back(the_alloc_ptr);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";
        }
      }

      {  // It shouldn't matter if we mix deallocate() and deallocate_nosync()
        for (const auto& ptr : pointers) {
          heap.deallocate(ptr);
        }
      }

      {  // We expect to be able to get back all the memory that was freed
        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);
          pointers.push_back(the_alloc_ptr);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";
        }
      }

      service.deallocate_bytes(buf, buffer_size);
    }
  }
}
