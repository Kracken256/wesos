/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Move.hh>
#include <wesos-types/Types.hh>

using namespace wesos;

class MoveSemanticsTest {
  bool m_active = true;
  usize &m_copy_ctr;
  usize &m_move_ctr;

public:
  constexpr MoveSemanticsTest(usize &copy_ctr, usize &move_ctr) : m_copy_ctr(copy_ctr), m_move_ctr(move_ctr) {
    m_copy_ctr = 1;
    m_move_ctr = 0;
  }

  constexpr MoveSemanticsTest(MoveSemanticsTest &&o)
      : m_active(o.m_active), m_copy_ctr(o.m_copy_ctr), m_move_ctr(o.m_move_ctr) {
    o.m_active = false;
    m_move_ctr++;
  }

  constexpr MoveSemanticsTest(const MoveSemanticsTest &o)
      : m_active(o.m_active), m_copy_ctr(o.m_copy_ctr), m_move_ctr(o.m_move_ctr) {
    m_copy_ctr++;
  }

  constexpr auto operator=(const MoveSemanticsTest &o) -> MoveSemanticsTest & {
    if (this != &o) {
      m_active = o.m_active;
      m_copy_ctr = o.m_copy_ctr;
      m_move_ctr = o.m_move_ctr;
      m_copy_ctr++;
    }

    return *this;
  }

  constexpr auto operator=(MoveSemanticsTest &&o) -> MoveSemanticsTest & {
    if (this != &o) {
      m_active = o.m_active;
      m_copy_ctr = o.m_copy_ctr;
      m_move_ctr = o.m_move_ctr;
      o.m_active = false;
      m_move_ctr++;
    }

    return *this;
  }

  [[nodiscard]] constexpr auto is_active() const -> bool { return m_active; }
};

TEST(wesos_types, Move_Control) {
  usize copy_ctr = 0;
  usize move_ctr = 0;

  {  // Test move semantics
    auto obj = MoveSemanticsTest(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj.is_active());

    auto obj2 = obj;
    EXPECT_EQ(copy_ctr, 2);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj2.is_active());
    EXPECT_TRUE(obj.is_active());
  }
}

TEST(wesos_types, Move_Test) {
  usize copy_ctr = 0;
  usize move_ctr = 0;

  {  // Test move semantics
    auto obj = MoveSemanticsTest(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj.is_active());

    auto obj2 = wesos::move(obj);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 1);
    EXPECT_TRUE(obj2.is_active());
    EXPECT_FALSE(obj.is_active());
  }
}

TEST(wesos_types, MoveAssign_Control) {
  usize copy_ctr = 0;
  usize move_ctr = 0;

  {  // Test move semantics
    auto obj = MoveSemanticsTest(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj.is_active());

    MoveSemanticsTest obj2(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj2.is_active());

    obj2 = obj;
    EXPECT_EQ(copy_ctr, 2);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj2.is_active());
    EXPECT_TRUE(obj.is_active());
  }
}

TEST(wesos_types, MoveAssign_Test) {
  usize copy_ctr = 0;
  usize move_ctr = 0;

  {  // Test move semantics
    auto obj = MoveSemanticsTest(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj.is_active());

    MoveSemanticsTest obj2(copy_ctr, move_ctr);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 0);
    EXPECT_TRUE(obj2.is_active());

    obj2 = wesos::move(obj);
    EXPECT_EQ(copy_ctr, 1);
    EXPECT_EQ(move_ctr, 1);
    EXPECT_TRUE(obj2.is_active());
    EXPECT_FALSE(obj.is_active());
  }
}
