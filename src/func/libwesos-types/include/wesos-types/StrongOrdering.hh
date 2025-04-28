/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace std {
  // NOLINTBEGIN(readability-identifier-naming)

  namespace cmp_cat {
    using type = signed char;

    enum class _Ord : type { equivalent = 0, less = -1, greater = 1 };

    enum class _Ncmp : type { _Unordered = 2 };

    struct unspec {
      consteval unspec(unspec*) noexcept {}
    };
  }  // namespace cmp_cat

  class partial_ordering {
    cmp_cat::type m_value;

    constexpr explicit partial_ordering(cmp_cat::_Ord v) noexcept : m_value(cmp_cat::type(v)) {}

    constexpr explicit partial_ordering(cmp_cat::_Ncmp v) noexcept : m_value(cmp_cat::type(v)) {}

    friend class weak_ordering;
    friend class strong_ordering;

  public:
    static const partial_ordering less;
    static const partial_ordering equivalent;
    static const partial_ordering greater;
    static const partial_ordering unordered;

    [[nodiscard]]
    friend constexpr auto operator==(partial_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value == 0;
    }

    [[nodiscard]]
    friend constexpr auto operator==(partial_ordering, partial_ordering) noexcept -> bool = default;

    [[nodiscard]]
    friend constexpr auto operator<(partial_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value == -1;
    }

    [[nodiscard]]
    friend constexpr auto operator>(partial_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value == 1;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(partial_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value <= 0;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(partial_ordering v, cmp_cat::unspec) noexcept -> bool {
      return cmp_cat::type(v.m_value & 1) == v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<(cmp_cat::unspec, partial_ordering v) noexcept -> bool {
      return v.m_value == 1;
    }

    [[nodiscard]]
    friend constexpr auto operator>(cmp_cat::unspec, partial_ordering v) noexcept -> bool {
      return v.m_value == -1;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(cmp_cat::unspec, partial_ordering v) noexcept -> bool {
      return cmp_cat::type(v.m_value & 1) == v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(cmp_cat::unspec, partial_ordering v) noexcept -> bool {
      return 0 >= v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(partial_ordering v,
                                      cmp_cat::unspec) noexcept -> partial_ordering {
      return v;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(cmp_cat::unspec,
                                      partial_ordering v) noexcept -> partial_ordering {
      if ((v.m_value & 1) != 0) {
        return partial_ordering(cmp_cat::_Ord(-v.m_value));
      }
      return v;
    }
  };

  inline constexpr partial_ordering partial_ordering::less(cmp_cat::_Ord::less);
  inline constexpr partial_ordering partial_ordering::equivalent(cmp_cat::_Ord::equivalent);
  inline constexpr partial_ordering partial_ordering::greater(cmp_cat::_Ord::greater);
  inline constexpr partial_ordering partial_ordering::unordered(cmp_cat::_Ncmp::_Unordered);

  class weak_ordering {
    cmp_cat::type m_value;

    constexpr explicit weak_ordering(cmp_cat::_Ord v) noexcept : m_value(cmp_cat::type(v)) {}

    friend class strong_ordering;

  public:
    static const weak_ordering less;
    static const weak_ordering equivalent;
    static const weak_ordering greater;

    [[nodiscard]]
    constexpr operator partial_ordering() const noexcept {
      return partial_ordering(cmp_cat::_Ord(m_value));
    }

    [[nodiscard]]
    friend constexpr auto operator==(weak_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value == 0;
    }

    [[nodiscard]]
    friend constexpr auto operator==(weak_ordering, weak_ordering) noexcept -> bool = default;

    [[nodiscard]]
    friend constexpr auto operator<(weak_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value < 0;
    }

    [[nodiscard]]
    friend constexpr auto operator>(weak_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value > 0;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(weak_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value <= 0;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(weak_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value >= 0;
    }

    [[nodiscard]]
    friend constexpr auto operator<(cmp_cat::unspec, weak_ordering v) noexcept -> bool {
      return 0 < v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator>(cmp_cat::unspec, weak_ordering v) noexcept -> bool {
      return 0 > v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(cmp_cat::unspec, weak_ordering v) noexcept -> bool {
      return 0 <= v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(cmp_cat::unspec, weak_ordering v) noexcept -> bool {
      return 0 >= v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(weak_ordering v, cmp_cat::unspec) noexcept -> weak_ordering {
      return v;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(cmp_cat::unspec, weak_ordering v) noexcept -> weak_ordering {
      return weak_ordering(cmp_cat::_Ord(-v.m_value));
    }
  };

  inline constexpr weak_ordering weak_ordering::less(cmp_cat::_Ord::less);
  inline constexpr weak_ordering weak_ordering::equivalent(cmp_cat::_Ord::equivalent);
  inline constexpr weak_ordering weak_ordering::greater(cmp_cat::_Ord::greater);

  class strong_ordering {
    cmp_cat::type m_value;

    constexpr explicit strong_ordering(cmp_cat::_Ord v) noexcept : m_value(cmp_cat::type(v)) {}

  public:
    static const strong_ordering less;
    static const strong_ordering equal;
    static const strong_ordering equivalent;
    static const strong_ordering greater;

    [[nodiscard]]
    constexpr operator partial_ordering() const noexcept {
      return partial_ordering(cmp_cat::_Ord(m_value));
    }

    [[nodiscard]]
    constexpr operator weak_ordering() const noexcept {
      return weak_ordering(cmp_cat::_Ord(m_value));
    }

    [[nodiscard]]
    friend constexpr auto operator==(strong_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value == 0;
    }

    [[nodiscard]]
    friend constexpr auto operator==(strong_ordering, strong_ordering) noexcept -> bool = default;

    [[nodiscard]]
    friend constexpr auto operator<(strong_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value < 0;
    }

    [[nodiscard]]
    friend constexpr auto operator>(strong_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value > 0;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(strong_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value <= 0;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(strong_ordering v, cmp_cat::unspec) noexcept -> bool {
      return v.m_value >= 0;
    }

    [[nodiscard]]
    friend constexpr auto operator<(cmp_cat::unspec, strong_ordering v) noexcept -> bool {
      return 0 < v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator>(cmp_cat::unspec, strong_ordering v) noexcept -> bool {
      return 0 > v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<=(cmp_cat::unspec, strong_ordering v) noexcept -> bool {
      return 0 <= v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator>=(cmp_cat::unspec, strong_ordering v) noexcept -> bool {
      return 0 >= v.m_value;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(strong_ordering v,
                                      cmp_cat::unspec) noexcept -> strong_ordering {
      return v;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(cmp_cat::unspec,
                                      strong_ordering v) noexcept -> strong_ordering {
      return strong_ordering(cmp_cat::_Ord(-v.m_value));
    }
  };

  inline constexpr strong_ordering strong_ordering::less(cmp_cat::_Ord::less);
  inline constexpr strong_ordering strong_ordering::equal(cmp_cat::_Ord::equivalent);
  inline constexpr strong_ordering strong_ordering::equivalent(cmp_cat::_Ord::equivalent);
  inline constexpr strong_ordering strong_ordering::greater(cmp_cat::_Ord::greater);

  // NOLINTEND(readability-identifier-naming)
}  // namespace std