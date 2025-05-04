// /**
//  * This file is part of the WesOS project.
//  *
//  * WesOS is public domain software: you can redistribute it and/or modify
//  * it under the terms of the Unlicense(https://unlicense.org/).
//  */

// #pragma once

// #include <wesos-mem/IntrusivePool.hh>  /// TODO: Remove me
// #include <wesos-mem/MemoryResourceProtocol.hh>
// #include <wesos-types/NullableOwnPtr.hh>

// namespace wesos::smartptr {
//   template <class PointeeGeneric>
//   class Box {
//     NullableOwnPtr<PointeeGeneric> m_ptr;
//     mem::MemoryResourceProtocol* m_pmr;

//     Box(OwnPtr<PointeeGeneric> ptr, mem::MemoryResourceProtocol& pmr) : m_ptr(ptr), m_pmr(&pmr)
//     {}

//   public:
//     constexpr Box(const Box& o) = delete;
//     constexpr auto operator=(const Box& o) -> Box& = delete;

//     constexpr Box(Box&& o) : m_ptr(o.m_ptr), m_pmr(o.m_pmr) { o.m_ptr = nullptr; };

//     constexpr auto operator=(Box&& o) -> Box& {
//       if (this != &o) [[likely]] {
//         m_ptr = o.m_ptr;
//         o.m_ptr = nullptr;
//       }
//     };

//     ~Box() {
//       assert_invariant(m_pmr != nullptr);

//       auto* object_ptr = m_ptr.unwrap();
//       const auto object_range = View<u8>(bit_cast<u8*>(object_ptr),
//       sizeof(PointeeGeneric));

//       object_ptr->~PointeeGeneric();
//       m_pmr->deallocate_bytes(object_range, alignof(PointeeGeneric));

// #ifndef NDEBUG
//       m_ptr = nullptr;
// #endif
//     };

//     template <class... ArgsGeneric>
//     [[nodiscard]] static auto create(mem::MemoryResourceProtocol& pmr) {
//       return [&pmr](ArgsGeneric... args) -> Nullable<Box> {
//         auto object_ptr = pmr.allocate_bytes(sizeof(PointeeGeneric), alignof(PointeeGeneric));
//         if (object_ptr.is_null()) [[unlikely]] {
//           return null;
//         }

//         new (object_ptr.unwrap_unchecked().into_ptr()) PointeeGeneric(args...);

//         (void)pmr;
//         return nullptr;
//         /// TODO:
//       };
//     }

//     /// TODO:
//   };

//   static void test() {  //
//     Array<u8, 4096> bytes;
//     mem::IntrusivePool pmr(sizeof(int), alignof(int), bytes.as_view());

//     auto hello = Box<int>::create(pmr)().unwrap();

//     constexpr auto sz = sizeof(hello);
//   }
// }  // namespace wesos::smartptr
