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
  }

  ~SemanticCounter() { ++m_destructed; }
};

TEST(Box, CreateIntBox) {
  const auto buf_size = mem::IntrusivePool::minimum_size();
  const auto value = 42;

  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(int), alignof(int), bytes.as_view());

  auto int_box_maybe = Box<int>::create(value)(pmr);
  ASSERT_NE(int_box_maybe, null);
  auto int_box = move(int_box_maybe.value());

  EXPECT_EQ(int_box.get(), value);
}

TEST(Box, ArgumentForwarding) {
  struct Task {
    SemanticCounter m_value;

    Task(SemanticCounter arg) : m_value(move(arg)) {}
  };

  static_assert(sizeof(Task) >= mem::IntrusivePool::minimum_size());

  const auto buf_size = sizeof(Task);
  auto bytes = Array<u8, buf_size>();
  auto pmr = mem::IntrusivePool(sizeof(Task), alignof(Task), bytes.as_view());

  usize constructed = 0;
  usize moved = 0;
  usize copied = 0;
  usize destructed = 0;

  auto ctr = SemanticCounter(constructed, moved, copied, destructed);

  auto box_maybe = Box<Task>::create(ctr)(pmr);
  ASSERT_NE(box_maybe, null);
  auto box = move(box_maybe.value());

  // EXPECT_EQ(int_box.get(), value);
}
