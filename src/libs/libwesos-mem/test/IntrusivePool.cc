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

static void deps_setup() {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message, wesos::SourceLocation source) {
    std::cerr << "\n==========================================================================="
                 "===========\n"
              << "| Assertion Failed: \"" << message << "\";\n"
              << "| Function: [" << source.function_name() << "]: " << source.line_number() << ";\n"
              << "| File: \"" << source.file_name() << "\";\n"
              << "============================================================================="
                 "=========\n"
              << std::endl;
  });
}

TEST(IntrusivePool, CreatePool) {
  deps_setup();

  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> pool(buf.data(), buf.size());

  auto mm = mem::IntrusivePool(sizeof(int), alignof(int), pool);
}

TEST(IntrusivePool, Allocate) {
  deps_setup();

  using namespace wesos;

  struct Hash {
    constexpr auto operator()(const NullableOwnPtr<u8>& x) const -> size_t { return x.as_uptr(); }
  };

  const auto min_size = 16;
  const auto max_size = 257;
  const PowerOfTwo<usize> min_align = mem::IntrusivePool::minimum_alignment();
  const PowerOfTwo<usize> max_align = 64;

  constexpr auto alloc_limit = 9;

  std::pmr::polymorphic_allocator<u8> service;
  std::unordered_set<NullableOwnPtr<u8>, Hash> pointers;

  for (usize size = min_size; size < max_size; size++) {
    for (usize align = min_align; align < max_align; align *= 2) {
      const auto space_per_object = ((size + align - 1) & -align);
      const auto exact_buffer_size = space_per_object * alloc_limit;

      u8* buf = bit_cast<u8*>(service.allocate_bytes(exact_buffer_size, align));
      ASSERT_NE(buf, nullptr);

      auto buf_view = View<u8>(buf, exact_buffer_size);
      auto mm = mem::IntrusivePool(size, align, buf_view);

      {  // Use all avaiable memory in pool
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_bytes(size, align);

          ASSERT_NE(the_alloc_ptr, null) << "Failed on size(" << size << "), " << "align(" << align << ")";

          memset(the_alloc_ptr.unwrap(), 0, size);

          ASSERT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }
      }

      auto expected_objects = pointers;

      // Release all allocated pool objects
      for (const auto& ptr : pointers) {
        // we should be able to mix sync and nosync methods
        mm.deallocate_bytes(ptr, size, align);
      }

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_bytes(size, align);

          ASSERT_NE(the_alloc_ptr, null) << "Failed on size(" << size << "), " << "align(" << align << ")";

          memset(the_alloc_ptr.unwrap(), 0, size);

          ASSERT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }

        ASSERT_EQ(pointers, expected_objects);
      }

      // Release all allocated pool objects
      for (const auto& ptr : pointers) {
        // we should be able to mix sync and nosync methods
        mm.deallocate_bytes(ptr, size, align);
      }

      {  // We expect to be able to get back all the memory that was freed
        pointers.clear();

        for (usize alloc_i = 0; alloc_i < alloc_limit; alloc_i++) {
          auto the_alloc_ptr = mm.allocate_bytes(size, align);

          ASSERT_NE(the_alloc_ptr, null) << "Failed on size(" << size << "), " << "align(" << align << ")";

          memset(the_alloc_ptr.unwrap(), 0, size);

          ASSERT_FALSE(pointers.contains(the_alloc_ptr));
          pointers.insert(the_alloc_ptr);
        }

        ASSERT_EQ(pointers, expected_objects);
      }

      service.deallocate_bytes(buf, exact_buffer_size, align);
    }
  }
}
