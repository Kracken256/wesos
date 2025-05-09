/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Move.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <class TypeA, class TypeB>
  class Pair {
  public:
    TypeA m_a{};
    TypeB m_b{};

    constexpr Pair() = default;
    constexpr Pair(TypeA a, TypeB b) : m_a(move(a)), m_b(move(b)) {}
    constexpr Pair(const Pair&) = default;
    constexpr Pair(Pair&&) = default;
    constexpr auto operator=(const Pair&) -> Pair& = default;
    constexpr auto operator=(Pair&&) -> Pair& = default;
    constexpr ~Pair() = default;

    [[nodiscard]] constexpr auto operator<=>(const Pair&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeA& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeA& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeB& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeB& { return m_b; }

    constexpr void anew() {
      m_a = TypeA();
      m_b = TypeB();
    }
  };

  template <class TypeA, class TypeB, class TypeC>
  class Triplet {
  public:
    TypeA m_a{};
    TypeB m_b{};
    TypeC m_c{};

    constexpr Triplet() = default;
    constexpr Triplet(TypeA a, TypeB b, TypeC c) : m_a(move(a)), m_b(move(b)), m_c(move(c)) {}
    constexpr Triplet(const Triplet&) = default;
    constexpr Triplet(Triplet&&) = default;
    constexpr auto operator=(const Triplet&) -> Triplet& = default;
    constexpr auto operator=(Triplet&&) -> Triplet& = default;
    constexpr ~Triplet() = default;

    [[nodiscard]] constexpr auto operator<=>(const Triplet&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeA& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeA& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeB& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeB& { return m_b; }
    [[nodiscard]] constexpr auto third() const -> const TypeC& { return m_c; }
    [[nodiscard]] constexpr auto third() -> TypeC& { return m_c; }

    constexpr void anew() {
      m_a = TypeA();
      m_b = TypeB();
      m_c = TypeC();
    }
  };

  template <class TypeA, class TypeB, class TypeC, class TypeD>
  class Quadlet {
  public:
    TypeA m_a{};
    TypeB m_b{};
    TypeC m_c{};
    TypeD m_d{};

    constexpr Quadlet() = default;
    constexpr Quadlet(TypeA a, TypeB b, TypeC c, TypeD d) : m_a(move(a)), m_b(move(b)), m_c(move(c)), m_d(move(d)) {}
    constexpr Quadlet(const Quadlet&) = default;
    constexpr Quadlet(Quadlet&&) = default;
    constexpr auto operator=(const Quadlet&) -> Quadlet& = default;
    constexpr auto operator=(Quadlet&&) -> Quadlet& = default;
    constexpr ~Quadlet() = default;

    [[nodiscard]] constexpr auto operator<=>(const Quadlet&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeA& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeA& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeB& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeB& { return m_b; }
    [[nodiscard]] constexpr auto third() const -> const TypeC& { return m_c; }
    [[nodiscard]] constexpr auto third() -> TypeC& { return m_c; }
    [[nodiscard]] constexpr auto fourth() const -> const TypeD& { return m_d; }
    [[nodiscard]] constexpr auto fourth() -> TypeD& { return m_d; }

    constexpr void anew() {
      m_a = TypeA();
      m_b = TypeB();
      m_c = TypeC();
      m_d = TypeD();
    }
  };
}  // namespace wesos::types
