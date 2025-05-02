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
//   class Rc {
//     NullableOwnPtr<PointeeGeneric> m_ptr;
//     usize& m_rc;
//     mem::MemoryResourceProtocol* m_pmr;

//     Rc(OwnPtr<PointeeGeneric> ptr, usize& rc, mem::MemoryResourceProtocol& pmr)
//         : m_ptr(ptr), m_rc(rc), m_pmr(&pmr) {}

//   public:
//     constexpr Rc(const Rc& o) : m_ptr(o.m_ptr), m_rc(++o.m_rc), m_pmr(o.m_pmr){};

//     constexpr Rc(Rc&& o) : m_ptr(o.m_ptr), m_rc(o.m_rc), m_pmr(o.m_pmr) {
//       o.m_ptr = nullptr;
//       o.m_rc = 0;
//     };

//     constexpr auto operator=(const Rc& o) -> Rc& {
//       m_ptr = o.m_ptr;
//       m_rc = ++o.m_rc;
//       m_pmr = o.m_pmr;
//     };

//     constexpr auto operator=(Rc&& o) -> Rc& {
//       m_ptr = o.m_ptr;
//       m_rc = o.m_rc;
//       m_pmr = o.m_pmr;

//       o.m_ptr = nullptr;
//       o.m_rc = 0;
//     };

//     ~Rc() {
//       if (--m_rc == 0) [[unlikely]] {
//         auto* object_ptr = m_ptr.unwrap();
//         const auto object_range =
//             View<u8>(reinterpret_cast<u8*>(object_ptr), sizeof(PointeeGeneric));

//         object_ptr->~PointeeGeneric();
//         m_pmr->deallocate_bytes(object_range, alignof(PointeeGeneric));

// #ifndef NDEBUG
//         m_ptr = nullptr;
// #endif
//       }
//     };

//     template <class... ArgsGeneric>
//     [[nodiscard]] static auto create(mem::MemoryResourceProtocol& pmr) {
//       return [&pmr](ArgsGeneric... args) -> Nullable<Rc> {
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

//     auto hello = Rc<int>::create(pmr)().unwrap();

//     constexpr auto sz = sizeof(hello);
//   }
// }  // namespace wesos::smartptr
