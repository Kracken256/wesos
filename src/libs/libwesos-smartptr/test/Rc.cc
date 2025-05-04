/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-mem/IntrusivePool.hh>
#include <wesos-smartptr/Rc.hh>

using namespace wesos;
using namespace wesos::smartptr;

class SemanticCounter final {
  usize &m_constructed, &m_moved, &m_copied, &m_destructed;

public:
  SemanticCounter(usize &constructed, usize &moved, usize &copied, usize &destructed)
      : m_constructed(constructed), m_moved(moved), m_copied(copied), m_destructed(destructed) {
    ++m_constructed;
  }
  SemanticCounter(const SemanticCounter &o)
      : m_constructed(o.m_constructed), m_moved(o.m_moved), m_copied(o.m_copied), m_destructed(o.m_destructed) {
    ++m_copied;
  }
  SemanticCounter(SemanticCounter &&o)
      : m_constructed(o.m_constructed), m_moved(o.m_moved), m_copied(o.m_copied), m_destructed(o.m_destructed) {
    ++m_moved;
  };

  ~SemanticCounter() { ++m_destructed; }
};

TEST(Rc, CreateIntRc) {
  const auto buf_size = 64;
  const auto value = 42;

  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(Rc<int>::min_alloc_size(), Rc<int>::min_alloc_alignment(), bytes.as_view());

  auto int_rc_maybe = Rc<int>::create(pmr, value);
  ASSERT_NE(int_rc_maybe, null);
  auto int_rc = move(int_rc_maybe.value());

  EXPECT_EQ(int_rc.get(), value);
  EXPECT_EQ(int_rc.ref_count(), 1);

  *int_rc = value + 1;

  EXPECT_EQ(*int_rc, value + 1);
  EXPECT_EQ(int_rc.ref_count(), 1);
}

TEST(Rc, ArgumentForwarding) {
  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(Rc<SemanticCounter>::min_alloc_size(), Rc<SemanticCounter>::min_alloc_alignment(),
                                bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Rc<SemanticCounter>::create(pmr, SemanticCounter(constructed, moved, copied, destructed));
    ASSERT_NE(rc_maybe, null);
    auto rc = move(rc_maybe.value());

    EXPECT_EQ(rc.ref_count(), 1);

    auto rc_copy = rc;  // NOLINT(performance-unnecessary-copy-initialization)

    EXPECT_EQ(rc.ref_count(), 2);
    EXPECT_EQ(rc_copy.ref_count(), 2);

    EXPECT_EQ(constructed, 1);
    EXPECT_EQ(copied, 0);
    EXPECT_EQ(moved, 1);
    EXPECT_EQ(destructed, 1);
  }

  EXPECT_EQ(constructed, 1);
  EXPECT_EQ(copied, 0);
  EXPECT_EQ(moved, 1);
  EXPECT_EQ(destructed, 2);
}

TEST(Rc, NoCopy) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(Rc<SemanticCounter>::min_alloc_size(), Rc<SemanticCounter>::min_alloc_alignment(),
                                bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Rc<SemanticCounter>::create(pmr, constructed, moved, copied, destructed);
    ASSERT_NE(rc_maybe, null);
    auto rc = move(rc_maybe.value());

    EXPECT_EQ(copied, 0);
  }

  EXPECT_EQ(copied, 0);
}

TEST(Rc, NoMove) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(Rc<SemanticCounter>::min_alloc_size(), Rc<SemanticCounter>::min_alloc_alignment(),
                                bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Rc<SemanticCounter>::create(pmr, constructed, moved, copied, destructed);
    ASSERT_NE(rc_maybe, null);
    auto rc = move(rc_maybe.value());

    EXPECT_EQ(moved, 0);
  }

  EXPECT_EQ(moved, 0);
}

TEST(Rc, Disown) {
  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(Rc<SemanticCounter>::min_alloc_size(), Rc<SemanticCounter>::min_alloc_alignment(),
                                bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  Nullable<Rc<SemanticCounter>> ref_a;

  {
    Nullable<Rc<SemanticCounter>> ref_b;

    {
      Nullable<Rc<SemanticCounter>> ref_c;

      {
        auto rc_maybe = Rc<SemanticCounter>::create(pmr, SemanticCounter(constructed, moved, copied, destructed));
        ASSERT_NE(rc_maybe, null);
        auto rc = move(rc_maybe.value());

        EXPECT_EQ(rc.ref_count(), 1);

        ref_a = rc;
        ref_b = rc;
        ref_c = rc;

        EXPECT_EQ(rc.ref_count(), 4);
        EXPECT_EQ(ref_a.value().ref_count(), 4);
        EXPECT_EQ(ref_b.value().ref_count(), 4);
        EXPECT_EQ(ref_c.value().ref_count(), 4);

        EXPECT_EQ(constructed, 1);
        EXPECT_EQ(copied, 0);
        EXPECT_EQ(moved, 1);
        EXPECT_EQ(destructed, 1);
      }

      EXPECT_EQ(constructed, 1);
      EXPECT_EQ(copied, 0);
      EXPECT_EQ(moved, 1);
      EXPECT_EQ(destructed, 1);

      EXPECT_EQ(ref_a.value().ref_count(), 3);
      EXPECT_EQ(ref_b.value().ref_count(), 3);
      EXPECT_EQ(ref_c.value().ref_count(), 3);
    }

    EXPECT_EQ(constructed, 1);
    EXPECT_EQ(copied, 0);
    EXPECT_EQ(moved, 1);
    EXPECT_EQ(destructed, 1);

    EXPECT_EQ(ref_a.value().ref_count(), 2);
    EXPECT_EQ(ref_b.value().ref_count(), 2);

    ref_b->disown();

    EXPECT_EQ(ref_a.value().ref_count(), 0);
    EXPECT_EQ(ref_b.value().ref_count(), 0);
  }

  EXPECT_EQ(ref_a.value().ref_count(), 0);

  EXPECT_EQ(constructed, 1);
  EXPECT_EQ(copied, 0);
  EXPECT_EQ(moved, 1);
  EXPECT_EQ(destructed, 2);
}
