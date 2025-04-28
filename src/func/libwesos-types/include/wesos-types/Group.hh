/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <class TypeAGeneric, class TypeBGeneric>
  class Pair {
    TypeAGeneric m_a;
    TypeBGeneric m_b;

  public:
    constexpr Pair() = default;
    constexpr Pair(TypeAGeneric a, TypeBGeneric b) : m_a(a), m_b(b) {}
    constexpr Pair(const Pair& other) = default;
    constexpr Pair(Pair&& other) = default;
    constexpr auto operator=(const Pair& other) -> Pair& = default;
    constexpr auto operator=(Pair&& other) -> Pair& = default;
    constexpr auto operator<=>(const Pair& other) const = default;

    constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    constexpr auto first() -> TypeAGeneric& { return m_a; }
    constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    constexpr auto second() -> TypeBGeneric& { return m_b; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
    }

    void reflect(void* m, auto cb, auto& depth) const {
      ++depth;
      m_a.reflect(m, cb, depth);
      m_b.reflect(m, cb, depth);
      --depth;
    }
  };

  template <class TypeAGeneric, class TypeBGeneric, class TypeCGeneric>
  class Triplet {
    TypeAGeneric m_a;
    TypeBGeneric m_b;
    TypeCGeneric m_c;

  public:
    constexpr Triplet() = default;
    constexpr Triplet(TypeAGeneric a, TypeBGeneric b, TypeCGeneric c) : m_a(a), m_b(b), m_c(c) {}
    constexpr Triplet(const Triplet& other) = default;
    constexpr Triplet(Triplet&& other) = default;
    constexpr auto operator=(const Triplet& other) -> Triplet& = default;
    constexpr auto operator=(Triplet&& other) -> Triplet& = default;
    constexpr auto operator<=>(const Triplet& other) const = default;

    constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    constexpr auto third() const -> const TypeCGeneric& { return m_c; }
    constexpr auto first() -> TypeAGeneric& { return m_a; }
    constexpr auto second() -> TypeBGeneric& { return m_b; }
    constexpr auto third() -> TypeCGeneric& { return m_c; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
      m_c = TypeCGeneric();
    }

    void reflect(void* m, auto cb, auto& depth) const {
      ++depth;
      m_a.reflect(m, cb, depth);
      m_b.reflect(m, cb, depth);
      m_c.reflect(m, cb, depth);
      --depth;
    }
  };

  template <class TypeAGeneric, class TypeBGeneric, class TypeCGeneric, class TypeDGeneric>
  class Quadlet {
    TypeAGeneric m_a;
    TypeBGeneric m_b;
    TypeCGeneric m_c;
    TypeDGeneric m_d;

  public:
    constexpr Quadlet() = default;
    constexpr Quadlet(TypeAGeneric a, TypeBGeneric b, TypeCGeneric c, TypeDGeneric d)
        : m_a(a), m_b(b), m_c(c), m_d(d) {}
    constexpr Quadlet(const Quadlet& other) = default;
    constexpr Quadlet(Quadlet&& other) = default;
    constexpr auto operator=(const Quadlet& other) -> Quadlet& = default;
    constexpr auto operator=(Quadlet&& other) -> Quadlet& = default;
    constexpr auto operator<=>(const Quadlet& other) const = default;

    constexpr auto first() const -> const TypeAGeneric& { return m_a; }
    constexpr auto second() const -> const TypeBGeneric& { return m_b; }
    constexpr auto third() const -> const TypeCGeneric& { return m_c; }
    constexpr auto fourth() const -> const TypeDGeneric& { return m_d; }
    constexpr auto first() -> TypeAGeneric& { return m_a; }
    constexpr auto second() -> TypeBGeneric& { return m_b; }
    constexpr auto third() -> TypeCGeneric& { return m_c; }
    constexpr auto fourth() -> TypeDGeneric& { return m_d; }

    constexpr void anew() {
      m_a = TypeAGeneric();
      m_b = TypeBGeneric();
      m_c = TypeCGeneric();
      m_d = TypeDGeneric();
    }

    void reflect(void* m, auto cb, auto& depth) const {
      ++depth;
      m_a.reflect(m, cb, depth);
      m_b.reflect(m, cb, depth);
      m_c.reflect(m, cb, depth);
      m_d.reflect(m, cb, depth);
      --depth;
    }
  };
}  // namespace wesos::types