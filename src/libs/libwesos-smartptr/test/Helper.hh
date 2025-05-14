/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::smartptr {
  class SemanticCounter final {
    usize &m_constructed, &m_moved, &m_copied, &m_destructed;
    bool m_alive = true;

  public:
    constexpr SemanticCounter(usize &constructed, usize &moved, usize &copied, usize &destructed)
        : m_constructed(constructed), m_moved(moved), m_copied(copied), m_destructed(destructed) {
      ++m_constructed;
    }

    constexpr SemanticCounter(const SemanticCounter &o)
        : m_constructed(o.m_constructed), m_moved(o.m_moved), m_copied(o.m_copied), m_destructed(o.m_destructed) {
      ++m_copied;
    }

    constexpr SemanticCounter(SemanticCounter &&o)
        : m_constructed(o.m_constructed), m_moved(o.m_moved), m_copied(o.m_copied), m_destructed(o.m_destructed) {
      ++m_moved;
      o.m_alive = false;
    };

    constexpr auto operator=(const SemanticCounter &o) -> SemanticCounter & {
      if (this != &o) {
        m_constructed = o.m_constructed;
        m_moved = o.m_moved;
        m_copied = o.m_copied;
        m_destructed = o.m_destructed;
        ++m_copied;
      }

      return *this;
    }

    constexpr auto operator=(SemanticCounter &&o) -> SemanticCounter & {
      if (this != &o) {
        m_constructed = o.m_constructed;
        m_moved = o.m_moved;
        m_copied = o.m_copied;
        m_destructed = o.m_destructed;
        ++m_moved;
        o.m_alive = false;
      }

      return *this;
    }

    constexpr ~SemanticCounter() { ++m_destructed; }

    [[nodiscard]] constexpr auto is_alive() const -> bool { return m_alive; }
  };
}  // namespace wesos::smartptr
