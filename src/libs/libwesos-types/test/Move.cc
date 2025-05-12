/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <gtest/gtest.h>

#include <wesos-types/Move.hh>

TEST(Move, Move) {
  class MoveSemanticsTest {
    bool m_active = true;
    volatile unsigned &m_counter;

  public:
    MoveSemanticsTest(volatile unsigned &counter) : m_counter(counter) {}
    MoveSemanticsTest(MoveSemanticsTest &&other) : m_counter(other.m_counter) { other.m_active = false; }
    MoveSemanticsTest(const MoveSemanticsTest &) = default;
    ~MoveSemanticsTest() {
      if (m_active) {
        m_counter = m_counter + 1;
      }
    }
  };

  volatile unsigned counter = 0;

  {  // Test move semantics
    {
      auto obj = MoveSemanticsTest(counter);
      auto obj2 = wesos::move(obj);
      (void)obj2;
    }

    EXPECT_EQ(counter, 1);
  }

  counter = 0;

  {  // Test copy semantics
    {
      auto obj = MoveSemanticsTest(counter);
      auto obj2 = obj;
      (void)obj2;
    }

    EXPECT_EQ(counter, 2);
  }
}
