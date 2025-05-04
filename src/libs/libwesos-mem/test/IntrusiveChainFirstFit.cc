/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <iostream>
#include <wesos-mem/IntrusiveChainFirstFit.hh>
#include <wesos-mem/TracingResource.hh>

static void deps_setup() {
  wesos::assert::register_output_callback(nullptr, [](void*, const char* message,
                                                      wesos::SourceLocation source) {
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

TEST(IntrusiveChainFirstFit, CreatePool) {
  deps_setup();

  using namespace wesos;

  constexpr auto buffer_size = 4096;

  std::vector<u8> buf(buffer_size);
  View<u8> pool(buf.data(), buf.size());

  auto mm = mem::IntrusiveChainFirstFit(pool);
}

TEST(IntrusiveChainFirstFit, Merge) {
  deps_setup();

  using namespace wesos;

  struct Hash {
    constexpr auto operator()(const NullableOwnPtr<u8>& x) const -> size_t { return x.as_uptr(); }
  };

  const auto size = 64;
  const PowerOfTwo<usize> align = 1;
  const auto allocation_count = 4;

  const auto mm_size = 1'000'000;

  auto storage = std::vector<u8>(mm_size);
  auto storage_view = View<u8>(storage.data(), storage.size());

  auto mm_inner = mem::IntrusiveChainFirstFit(storage_view);
  auto mm = mem::TracingResource(mm_inner, printf);

  for (usize i = 0; i < allocation_count; i++) {
    auto alloc_a = mm.allocate_bytes(size, align);
    ASSERT_NE(alloc_a, nullptr);

    auto alloc_b = mm.allocate_bytes(size, align);
    ASSERT_NE(alloc_b, nullptr);

    mm.deallocate_bytes(alloc_a, size, align);
    mm.deallocate_bytes(alloc_b, size, align);
  }
}

// TEST(IntrusiveChainFirstFit, Allocate) {
//   deps_setup();

//   using namespace wesos;

//   struct Hash {
//     constexpr auto operator()(const NullableOwnPtr<u8>& x) const -> size_t { return
//     x.as_uptr(); }
//   };

//   const auto min_size = 64;
//   const auto max_size = 64;
//   const PowerOfTwo<usize> min_align = 1;
//   const PowerOfTwo<usize> max_align = 1;

//   const auto max_space_per_alloc = ((max_size + max_align - 1) & -max_align);
//   const auto mm_size = max_space_per_alloc * ((max_size - min_size) * (max_align - min_align));

//   auto storage = std::vector<u8>(mm_size);
//   auto storage_view = View<u8>(storage.data(), storage.size());

//   auto mm_inner = mem::IntrusiveChainFirstFit(storage_view);
//   auto mm = mem::TracingResource(mm_inner, printf);

//   struct Record {
//     NullableOwnPtr<u8> m_ptr;
//     usize m_size = 0, m_align = 0;

//     [[nodiscard]] constexpr auto operator<=>(const Record&) const = default;
//   };

//   std::unordered_set<NullableOwnPtr<u8>, Hash> first_unique_ptr_set;
//   std::unordered_set<NullableOwnPtr<u8>, Hash> second_unique_ptr_set;
//   std::vector<Record> first_ptr_return_order;
//   std::vector<Record> second_ptr_return_order;

//   {
//     for (usize size = min_size; size <= max_size; size++) {
//       for (auto align = min_align; align <= max_align; align = align * 2) {
//         auto the_alloc_ptr = mm.allocate_bytes(size, align);
//         ASSERT_NE(the_alloc_ptr, null)
//             << "Failed on size(" << size << "), " << "align(" << align << ")";
//         // memset(the_alloc_ptr.unwrap(), 0, size);

//         ASSERT_FALSE(first_unique_ptr_set.contains(the_alloc_ptr));
//         first_unique_ptr_set.insert(the_alloc_ptr);
//         first_ptr_return_order.push_back({the_alloc_ptr, size, align});
//       }
//     }

//     for (auto [ptr, size, align] : first_ptr_return_order) {
//       mm.deallocate_bytes(ptr, size, align);
//     }
//   }

//   {
//     for (usize size = min_size; size <= max_size; size++) {
//       for (auto align = min_align; align <= max_align; align = align * 2) {
//         auto the_alloc_ptr = mm.allocate_bytes(size, align);
//         ASSERT_NE(the_alloc_ptr, null)
//             << "Failed on size(" << size << "), " << "align(" << align << ")";
//         // memset(the_alloc_ptr.unwrap(), 0, size);

//         ASSERT_FALSE(second_unique_ptr_set.contains(the_alloc_ptr));
//         second_unique_ptr_set.insert(the_alloc_ptr);
//         second_ptr_return_order.push_back({the_alloc_ptr, size, align});
//       }
//     }

//     for (auto [ptr, size, align] : second_ptr_return_order) {
//       mm.deallocate_bytes(ptr, size, align);
//     }
//   }

//   {  // Ensure this data structure is determinstic
//     ASSERT_EQ(first_unique_ptr_set, second_unique_ptr_set);
//     ASSERT_EQ(first_ptr_return_order, second_ptr_return_order);
//   }
// }
