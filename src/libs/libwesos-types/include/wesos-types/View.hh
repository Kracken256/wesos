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
#include <wesos-types/Ptr.hh>

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
    constexpr View(Pointer base, Pointer end) : m_base(base), m_size(usize(end - base)) {}
    constexpr View(const View&) = default;
    constexpr View(View&&) = default;
    constexpr auto operator=(const View&) -> View& = default;
    constexpr auto operator=(View&&) -> View& = default;
    constexpr ~View() = default;

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

    [[nodiscard]] constexpr auto subview(usize i, usize count) const -> View {
      always_assert(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i, usize count) const -> View {
      assert_invariant(i <= size() && count <= size() - i);
      return {m_base.add(i), count};
    }

    [[nodiscard]] constexpr auto subview(usize i) const -> View {
      always_assert(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] constexpr auto subview_unchecked(usize i) const -> View {
      assert_invariant(i <= size());
      return {m_base.add(i), size() - i};
    }

    [[nodiscard]] static constexpr auto create_empty() -> View { return View(); }

    constexpr auto operator<=>(const View& o) const -> std::strong_ordering
      requires requires { get(0) <=> o.get(0); }
    {
      const usize min_size = size() < o.size() ? size() : o.size();

      for (usize i = 0; i < min_size; ++i) {
        if (auto cmp = get_unchecked(i) <=> o.get_unchecked(i); cmp != 0) {
          return cmp;
        }
      }

      return size() <=> o.size();
    }

    [[nodiscard]] constexpr auto operator==(const View& o) const -> bool {
      if (size() != o.size()) {
        return false;
      }

      for (usize i = 0; i < size(); ++i) {
        if (get_unchecked(i) != o.get_unchecked(i)) {
          return false;
        }
      }

      return true;
    }

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

    constexpr auto clear() -> View& {
      m_size = 0;
      return *this;
    }
  };

  using ChunkAlignedRemaining = View<u8>;
  static inline auto for_each_chunk_aligned(View<u8> raw, usize chunk_size,
                                            PowerOfTwo<usize> chunk_align,
                                            auto lambda) -> ChunkAlignedRemaining {
    while (true) {
      const auto left_pad = raw.into_ptr().align_pow2(chunk_align).into_uptr()  //
                            - raw.into_ptr().into_uptr();
      if (raw.size() < left_pad + chunk_size) {
        break;
      }

      raw = raw.subview_unchecked(left_pad);
      assert_invariant(is_aligned_pow2(raw.into_ptr(), chunk_align) && raw.size() >= chunk_size);

      const auto chunk_range = raw.subview_unchecked(0, chunk_size);
      lambda(chunk_range);

      raw = raw.subview_unchecked(chunk_size);
    }

    return raw;
  }
}  // namespace wesos::types
