/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-mem/IntrusivePool.hh>
#include <wesos-smartptr/Box.hh>

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

TEST(Box, CreateIntBox) {
  const auto buf_size = mem::IntrusivePool::minimum_size();
  const auto value = 42;

  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(int), alignof(int), bytes.as_view());

  auto int_box_maybe = Box<int>::create(pmr, value);
  ASSERT_NE(int_box_maybe, null);
  auto int_box = move(int_box_maybe.value());

  EXPECT_EQ(int_box.get(), value);

  *int_box = value + 1;

  EXPECT_EQ(*int_box, value + 1);
}

TEST(Box, ArgumentForwarding) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = sizeof(SemanticCounter);
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(SemanticCounter), alignof(SemanticCounter), bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto box_maybe = Box<SemanticCounter>::create(pmr, SemanticCounter(constructed, moved, copied, destructed));
    ASSERT_NE(box_maybe, null);
    auto box = move(box_maybe.value());

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

TEST(Box, NoCopy) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = sizeof(SemanticCounter);
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(SemanticCounter), alignof(SemanticCounter), bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto box_maybe = Box<SemanticCounter>::create(pmr, constructed, moved, copied, destructed);
    ASSERT_NE(box_maybe, null);
    auto box = move(box_maybe.value());

    EXPECT_EQ(copied, 0);
  }

  EXPECT_EQ(copied, 0);
}

TEST(Box, NoMove) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = sizeof(SemanticCounter);
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(SemanticCounter), alignof(SemanticCounter), bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  {
    auto box_maybe = Box<SemanticCounter>::create(pmr, constructed, moved, copied, destructed);
    ASSERT_NE(box_maybe, null);
    auto box = move(box_maybe.value());

    EXPECT_EQ(moved, 0);
  }

  EXPECT_EQ(moved, 0);
}

TEST(Box, Disown) {
  static_assert(sizeof(SemanticCounter) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = sizeof(SemanticCounter);
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(SemanticCounter), alignof(SemanticCounter), bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  auto box_maybe = Box<SemanticCounter>::create(pmr, constructed, moved, copied, destructed);
  ASSERT_NE(box_maybe, null);
  auto box = move(box_maybe.value());

  EXPECT_EQ(constructed, 1);
  EXPECT_EQ(copied, 0);
  EXPECT_EQ(moved, 0);
  EXPECT_EQ(destructed, 0);

  box.disown();

  EXPECT_EQ(constructed, 1);
  EXPECT_EQ(copied, 0);
  EXPECT_EQ(moved, 0);
  EXPECT_EQ(destructed, 1);
}
