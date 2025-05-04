/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-builtin/Move.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <class TypeAGeneric, class TypeBGeneric>
  class Pair {
    TypeAGeneric m_a{};
    TypeBGeneric m_b{};

  public:
    constexpr Pair() = default;
    constexpr Pair(TypeAGeneric a, TypeBGeneric b) : m_a(move(a)), m_b(move(b)) {}
    constexpr Pair(const Pair&) = default;
    constexpr Pair(Pair&&) = default;
    constexpr auto operator=(const Pair&) -> Pair& = default;
    constexpr auto operator=(Pair&&) -> Pair& = default;
    constexpr ~Pair() = default;

    [[nodiscard]] constexpr auto operator<=>(const Pair&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeBGeneric& { return m_b; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
    }
  };

  template <class TypeAGeneric, class TypeBGeneric, class TypeCGeneric>
  class Triplet {
    TypeAGeneric m_a{};
    TypeBGeneric m_b{};
    TypeCGeneric m_c{};

  public:
    constexpr Triplet() = default;
    constexpr Triplet(TypeAGeneric a, TypeBGeneric b, TypeCGeneric c) : m_a(move(a)), m_b(move(b)), m_c(move(c)) {}
    constexpr Triplet(const Triplet&) = default;
    constexpr Triplet(Triplet&&) = default;
    constexpr auto operator=(const Triplet&) -> Triplet& = default;
    constexpr auto operator=(Triplet&&) -> Triplet& = default;
    constexpr ~Triplet() = default;

    [[nodiscard]] constexpr auto operator<=>(const Triplet&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeBGeneric& { return m_b; }
    [[nodiscard]] constexpr auto third() const -> const TypeCGeneric& { return m_c; }
    [[nodiscard]] constexpr auto third() -> TypeCGeneric& { return m_c; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
      m_c = TypeCGeneric();
    }
  };

  template <class TypeAGeneric, class TypeBGeneric, class TypeCGeneric, class TypeDGeneric>
  class Quadlet {
    TypeAGeneric m_a{};
    TypeBGeneric m_b{};
    TypeCGeneric m_c{};
    TypeDGeneric m_d{};

  public:
    constexpr Quadlet() = default;
    constexpr Quadlet(TypeAGeneric a, TypeBGeneric b, TypeCGeneric c, TypeDGeneric d)
        : m_a(move(a)), m_b(move(b)), m_c(move(c)), m_d(move(d)) {}
    constexpr Quadlet(const Quadlet&) = default;
    constexpr Quadlet(Quadlet&&) = default;
    constexpr auto operator=(const Quadlet&) -> Quadlet& = default;
    constexpr auto operator=(Quadlet&&) -> Quadlet& = default;
    constexpr ~Quadlet() = default;

    [[nodiscard]] constexpr auto operator<=>(const Quadlet&) const = default;

    [[nodiscard]] constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto first() -> TypeAGeneric& { return m_a; }
    [[nodiscard]] constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    [[nodiscard]] constexpr auto second() -> TypeBGeneric& { return m_b; }
    [[nodiscard]] constexpr auto third() const -> const TypeCGeneric& { return m_c; }
    [[nodiscard]] constexpr auto third() -> TypeCGeneric& { return m_c; }
    [[nodiscard]] constexpr auto fourth() const -> const TypeDGeneric& { return m_d; }
    [[nodiscard]] constexpr auto fourth() -> TypeDGeneric& { return m_d; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
      m_c = TypeCGeneric();
      m_d = TypeDGeneric();
    }
  };
}  // namespace wesos::types
