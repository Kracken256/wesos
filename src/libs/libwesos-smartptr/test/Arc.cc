/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-alloc/IntrusivePool.hh>
#include <wesos-smartptr/Arc.hh>

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

TEST(Arc, CreateIntArc) {
  const auto buf_size = 64;
  const auto value = 42;

  auto bytes = Array<u8, buf_size>();
  auto mm = mem::IntrusivePool(Arc<int>::min_alloc_size(), Arc<int>::min_alloc_alignment(), bytes.as_view());

  auto int_arc_maybe = Arc<int>::create(mm, value);
  ASSERT_NE(int_arc_maybe, null);
  auto int_arc = move(int_arc_maybe.value());

  EXPECT_EQ(int_arc.get(), value);
  EXPECT_EQ(int_arc.ref_count(), 1);

  *int_arc = value + 1;

  EXPECT_EQ(*int_arc, value + 1);
  EXPECT_EQ(int_arc.ref_count(), 1);
}

TEST(Arc, ArgumentForwarding) {
  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto mm = mem::IntrusivePool(Arc<SemanticCounter>::min_alloc_size(), Arc<SemanticCounter>::min_alloc_alignment(),
                               bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Arc<SemanticCounter>::create(mm, SemanticCounter(constructed, moved, copied, destructed));
    ASSERT_NE(rc_maybe, null);
    auto arc = move(rc_maybe.value());

    EXPECT_EQ(arc.ref_count(), 1);

    auto arc_copy = arc;  // NOLINT(performance-unnecessary-copy-initialization)

    EXPECT_EQ(arc.ref_count(), 2);
    EXPECT_EQ(arc_copy.ref_count(), 2);

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

TEST(Arc, NoCopy) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto mm = mem::IntrusivePool(Arc<SemanticCounter>::min_alloc_size(), Arc<SemanticCounter>::min_alloc_alignment(),
                               bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Arc<SemanticCounter>::create(mm, constructed, moved, copied, destructed);
    ASSERT_NE(rc_maybe, null);
    auto arc = move(rc_maybe.value());

    EXPECT_EQ(copied, 0);
  }

  EXPECT_EQ(copied, 0);
}

TEST(Arc, NoMove) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto mm = mem::IntrusivePool(Arc<SemanticCounter>::min_alloc_size(), Arc<SemanticCounter>::min_alloc_alignment(),
                               bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto rc_maybe = Arc<SemanticCounter>::create(mm, constructed, moved, copied, destructed);
    ASSERT_NE(rc_maybe, null);
    auto arc = move(rc_maybe.value());

    EXPECT_EQ(moved, 0);
  }

  EXPECT_EQ(moved, 0);
}

TEST(Arc, Lifetime) {
  const auto buf_size = 64;
  auto bytes = Array<u8, buf_size>();
  auto mm = mem::IntrusivePool(Arc<SemanticCounter>::min_alloc_size(), Arc<SemanticCounter>::min_alloc_alignment(),
                               bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    Nullable<Arc<SemanticCounter>> ref_a;
    Nullable<Arc<SemanticCounter>> ref_b;

    {
      Nullable<Arc<SemanticCounter>> ref_c;

      {
        Nullable<Arc<SemanticCounter>> ref_d;

        {
          auto rc_maybe = Arc<SemanticCounter>::create(mm, SemanticCounter(constructed, moved, copied, destructed));
          ASSERT_NE(rc_maybe, null);
          auto rc = move(rc_maybe.value());

          EXPECT_EQ(rc.ref_count(), 1);

          ref_a = rc;
          ref_c = rc;
          ref_d = rc;

          EXPECT_EQ(rc.ref_count(), 4);
          EXPECT_EQ(ref_a.value().ref_count(), 4);
          EXPECT_EQ(ref_c.value().ref_count(), 4);
          EXPECT_EQ(ref_d.value().ref_count(), 4);

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
        EXPECT_EQ(ref_c.value().ref_count(), 3);
        EXPECT_EQ(ref_d.value().ref_count(), 3);
      }

      EXPECT_EQ(constructed, 1);
      EXPECT_EQ(copied, 0);
      EXPECT_EQ(moved, 1);
      EXPECT_EQ(destructed, 1);

      EXPECT_EQ(ref_a.value().ref_count(), 2);
      EXPECT_EQ(ref_c.value().ref_count(), 2);
    }

    EXPECT_EQ(ref_a.value().ref_count(), 1);

    ref_b = ref_a.value();

    EXPECT_EQ(ref_a.value().ref_count(), 2);
  }

  EXPECT_EQ(constructed, 1);
  EXPECT_EQ(copied, 0);
  EXPECT_EQ(moved, 1);
  EXPECT_EQ(destructed, 2);
}