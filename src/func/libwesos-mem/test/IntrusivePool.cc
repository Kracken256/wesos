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
#include <wesos-mem/IntrusivePool.hh>

TEST(IntrusivePool, CreatePool) {
  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> pool(buf.data(), buf.size());

  auto mm = mem::IntrusivePool(sizeof(int), alignof(int), pool);
}

TEST(IntrusivePool, Allocate) {
  using namespace wesos;

  struct Hash {
    constexpr auto operator()(const Nullable<View<u8>>& x) const -> size_t {
      auto v = x.value_or({});
      return v.into_ptr().into_uptr() ^ v.size();
    }
  };

  wesos::assert::register_output_callback(
      nullptr,
      [](void*, const char* message, const char* func_name, const char* file_name, int line) {
        std::cerr << "\n==========================================================================="
                     "===========\n"
                  << "| Assertion Failed: \"" << message << "\";\n"
                  << "| Function: [" << func_name << "]: " << line << ";\n"
                  << "| File: \"" << file_name << "\";\n"
                  << "============================================================================="
                     "=========\n"
                  << std::endl;
      });

  const auto min_size = 16;
  const auto max_size = 257;
  const PowerOfTwo<usize> min_align = mem::IntrusivePool::minimum_alignment();
  const PowerOfTwo<usize> max_align = 64;

  constexpr auto alloc_limit = 9;

  std::pmr::polymorphic_allocator<u8> service;
  std::unordered_set<Nullable<View<u8>>, Hash> pointers;

  for (usize size = min_size; size < max_size; size++) {
    for (usize align = min_align; align < max_align; align *= 2) {
      const auto space_per_object = ((size + align - 1) & -align);
      const auto exact_buffer_size = space_per_object * alloc_limit;

      u8* buf = reinterpret_cast<u8*>(service.allocate_bytes(exact_buffer_size, align));
      ASSERT_NE(buf, nullptr);

      auto buf_view = View<u8>(buf, exact_buffer_size);
      auto mm = mem::IntrusivePool(size, align, buf_view);

      {  // Use all avaiable memory in pool
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }
      }

      auto expected_objects = pointers;

      // Release all allocated pool objects
      mm.anew();

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }

        EXPECT_EQ(pointers, expected_objects);
      }

      // Release all allocated pool objects
      for (const auto& ptr : pointers) {
        // we should be able to mix sync and nosync methods
        mm.deallocate(ptr);
      }

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_nosync(size, align, false);

          EXPECT_NE(the_alloc_ptr, null)
              << "Failed on size(" << size << "), " << "align(" << align << ")";

          EXPECT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }

        EXPECT_EQ(pointers, expected_objects);
      }

      service.deallocate_bytes(buf, exact_buffer_size, align);
    }
  }
}
