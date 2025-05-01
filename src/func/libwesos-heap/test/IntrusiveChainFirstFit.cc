/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <iostream>
#include <unordered_set>
#include <wesos-heap/IntrusiveChainFirstFit.hh>

TEST(IntrusiveChainFirstFit, CreatePool) {
  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> pool(buf.data(), buf.size());

  auto heap = heap::IntrusiveChainFirstFit(pool);
}

TEST(IntrusiveChainFirstFit, Allocate) {
  using namespace wesos;

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

  struct Hash {
    constexpr auto operator()(const Nullable<View<u8>>& x) const -> size_t {
      auto v = x.value_or({});
      return v.into_ptr().into_uptr() ^ v.size();
    }
  };

  const auto min_size = 16;
  const auto max_size = 257;
  const PowerOfTwo<usize> min_align = 1;
  const PowerOfTwo<usize> max_align = 64;

  const auto max_space_per_alloc = ((max_size + max_align - 1) & -max_align);
  const auto heap_size = max_space_per_alloc * ((max_size - min_size) * (max_align - min_align));

  auto storage = std::vector<u8>(heap_size);
  auto storage_view = View<u8>(storage.data(), storage.size());

  auto heap = heap::IntrusiveChainFirstFit(storage_view);

  std::unordered_set<Nullable<View<u8>>, Hash> first_unique_ptr_set;
  std::unordered_set<Nullable<View<u8>>, Hash> second_unique_ptr_set;
  std::vector<Nullable<View<u8>>> first_ptr_return_order;
  std::vector<Nullable<View<u8>>> second_ptr_return_order;

  {
    for (usize size = min_size; size < max_size; size++) {
      for (auto align = min_align; align < max_align; align = align.next()) {
        auto ptr = heap.allocate_nosync(size, align);
        EXPECT_NE(ptr, null) << "Failed on size(" << size << "), " << "align(" << align << ")";

        EXPECT_FALSE(first_unique_ptr_set.contains(ptr));
        first_unique_ptr_set.insert(ptr);
        first_ptr_return_order.push_back(ptr);
      }
    }

    heap.anew();
  }

  {
    for (usize size = min_size; size < max_size; size++) {
      for (auto align = min_align; align < max_align; align = align.next()) {
        auto ptr = heap.allocate_nosync(size, align);
        EXPECT_NE(ptr, null) << "Failed on size(" << size << "), " << "align(" << align << ")";

        EXPECT_FALSE(second_unique_ptr_set.contains(ptr));
        second_unique_ptr_set.insert(ptr);
        second_ptr_return_order.push_back(ptr);
      }
    }

    heap.anew();
  }

  {  // Ensure this data structure is determinstic
    EXPECT_EQ(first_unique_ptr_set, second_unique_ptr_set);
    EXPECT_EQ(first_ptr_return_order, second_ptr_return_order);
  }
}
