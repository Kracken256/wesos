/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-builtin/Move.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <typename ElementGeneric>
  class View {
    using Pointer = NullableRefPtr<ElementGeneric>;
    using ConstPointer = NullableRefPtr<const ElementGeneric>;

    Pointer m_base;
    usize m_size;

  public:
    constexpr View() : m_base(nullptr), m_size(0){};
    constexpr View(Pointer base, usize count) : m_base(base), m_size(count) {}
    constexpr View(Pointer base, Pointer end) : m_base(base), m_size(end - base) {}
    constexpr View(const View<ElementGeneric>&) = default;
    constexpr View(View<ElementGeneric>&&) = default;
    constexpr auto operator=(const View<ElementGeneric>&) -> View<ElementGeneric>& = default;
    constexpr auto operator=(View<ElementGeneric>&&) -> View<ElementGeneric>& = default;
    constexpr ~View() = default;

    constexpr auto operator<=>(const View<ElementGeneric>& o) const {
      const usize min_size = size() < o.size() ? size() : o.size();

      for (usize i = 0; i < min_size; ++i) {
        if (auto cmp = get_unchecked(i) <=> o.get_unchecked(i); cmp != 0) {
          return cmp;
        }
      }

      return size() <=> o.size();
    }

    [[nodiscard]] constexpr auto size() const -> usize { return m_size; }
    [[nodiscard]] constexpr auto empty() const -> bool { return size() == 0; }

    [[nodiscard]] constexpr auto begin() -> Pointer { return m_base; }
    [[nodiscard]] constexpr auto end() -> Pointer { return m_base.add(size()); }
    [[nodiscard]] constexpr auto cbegin() const -> ConstPointer { return m_base; }
    [[nodiscard]] constexpr auto cend() const -> ConstPointer { return m_base.add(size()); }

    [[nodiscard]] constexpr auto into_ptr() const { return m_base; }

    [[nodiscard]] constexpr auto get(usize i) const -> const ElementGeneric& {
      always_assert(i < size());
      return *(m_base.add(i));
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) const -> const ElementGeneric& {
      return *(m_base.add(i));
    }

    [[nodiscard]] constexpr auto get(usize i) -> ElementGeneric& {
      always_assert(i < size());
      return *(m_base.add(i));
    }

    [[nodiscard]] constexpr auto get_unchecked(usize i) -> ElementGeneric& {
      return *(m_base.add(i));
    }

    [[nodiscard]] constexpr auto subview(usize i, usize count) const -> View<const ElementGeneric> {
      always_assert(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i, usize count) const
        -> View<const ElementGeneric> {
      assert_invariant(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview(usize i, usize count) -> View<ElementGeneric> {
      always_assert(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i, usize count) -> View<ElementGeneric> {
      assert_invariant(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview(usize i) const -> View<const ElementGeneric> {
      always_assert(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i) const -> View<const ElementGeneric> {
      assert_invariant(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] constexpr auto subview(usize i) -> View<ElementGeneric> {
      always_assert(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i) -> View<ElementGeneric> {
      assert_invariant(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] static constexpr auto create_empty() -> View { return View(); }

    ///========================================================================================

    constexpr auto set(usize i, ElementGeneric x) -> View& {
      always_assert(i <= size());
      *m_base.add(i) = move(x);
      return *this;
    }

    constexpr auto set_unchecked(usize i, ElementGeneric x) -> View& {
      assert_invariant(i <= size());
      *m_base.add(i) = move(x);
      return *this;
    }

    constexpr auto fill(const ElementGeneric& x) -> View& {
      for (auto& ele : *this) {
        ele = x;
      }
      return *this;
    }
  };
}  // namespace wesos::types
