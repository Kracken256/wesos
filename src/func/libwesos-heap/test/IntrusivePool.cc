/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <iostream>
#include <memory_resource>
#include <unordered_set>
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

  struct Hash {
    constexpr auto operator()(const Nullable<View<u8>>& x) const -> size_t {
      auto v = x.value_or({});
      return v.into_ptr().into_uptr() ^ v.size();
    }
  };

  wesos::assert::register_output_callback(nullptr, [](void*, const char* message) {
    std::cerr << "Assertion failed: " << message << std::endl;
  });

  constexpr auto prime_size_limit = 8193;
  constexpr auto align_limit = 129;
  constexpr auto alloc_limit = 9;

  std::pmr::polymorphic_allocator<u8> service;
  std::unordered_set<Nullable<View<u8>>, Hash> pointers;

  for (usize size = sizeof(void*); size < prime_size_limit; size++) {
    for (usize align = 1; align < align_limit; align *= 2) {
      const auto space_per_object = ((size + align - 1) & -align);
      const auto buffer_size = space_per_object * alloc_limit;

      u8* buf = reinterpret_cast<u8*>(service.allocate_bytes(buffer_size, align));
      ASSERT_NE(buf, nullptr);

      auto buf_view = View<u8>(buf, buffer_size);
      auto heap = heap::IntrusivePool(size, align, buf_view);

      {  // Use all avaiable memory in pool
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }
      }

      // Release all allocated pool objects
      for (const auto& ptr : pointers) {
        heap.deallocate(ptr);  // try mixing dealloc functions
      }

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }
      }

      // Release all allocated pool objects
      for (const auto& ptr : pointers) {
        heap.deallocate_nosync(ptr);
      }

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = heap.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }
      }

      service.deallocate_bytes(buf, buffer_size);
    }
  }
}
