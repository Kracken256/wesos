/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-assert/Assert.hh>
#include <wesos-types/Nullable.hh>
#include <wesos-types/NullableRefPtr.hh>
#include <wesos-types/Numeric.hh>

namespace wesos::types {
  template <typename ElementGeneric>
  class View {
    using Ptr = NullableRefPtr<ElementGeneric>;

    Ptr m_base;
    usize m_size;

  public:
    constexpr View() = default;
    constexpr View(Ptr base, usize count) : m_base(base), m_size(count) {}
    constexpr View(Ptr base, Ptr end) : m_base(base), m_size(end - base) {}
    constexpr View(const View<ElementGeneric>& other) = default;
    constexpr View(View<ElementGeneric>&& other) = default;
    constexpr auto operator=(const View<ElementGeneric>& other) -> View<ElementGeneric>& = default;
    constexpr auto operator=(View<ElementGeneric>&& other) -> View<ElementGeneric>& = default;
    constexpr auto operator<=>(const View<ElementGeneric>& other) const = default;
    constexpr ~View() = default;

    [[nodiscard]] constexpr auto begin() const -> Ptr { return m_base; }
    [[nodiscard]] constexpr auto end() const -> Ptr { return m_base + m_size; }
    [[nodiscard]] constexpr auto cbegin() const -> NullableRefPtr<const ElementGeneric> {
      return m_base;
    }
    [[nodiscard]] constexpr auto cend() const -> NullableRefPtr<const ElementGeneric> {
      return m_base + m_size;
    }

    [[nodiscard]] constexpr auto size() const -> usize { return m_size; }
    [[nodiscard]] constexpr auto empty() const -> bool { return m_size == 0; }

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

    [[nodiscard]] constexpr auto subview(usize i,
                                         usize count = usize::max()) const -> View<ElementGeneric> {
      always_assert(i < size());
      return View<ElementGeneric>(m_base.add(i), i + count > size() ? size() - i : count);
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i, usize count = usize::max()) const
        -> View<ElementGeneric> {
      return View<ElementGeneric>(m_base.add(i), i + count > size() ? size() - i : count);
    }

    void reflect(void* m, auto cb, auto& depth) const {
      ++depth;
      for (const auto& ele : *this) {
        ele.reflect(m, cb, depth);
      }
      --depth;
    }
  };
}  // namespace wesos::types
