/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types {
  /**
   * Most magic in this file was generated with ChatGPT 2025.
   * I don't know what it does and I don't want to. Hopefully,
   * and bugs fail as compile time?!...
   */

  // NOLINTBEGIN(readability-identifier-naming)

  template <typename T, T v>
  struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const { return value; }
    constexpr auto operator()() const -> value_type { return value; }
  };

  using false_type = integral_constant<bool, false>;
  using true_type = integral_constant<bool, true>;

  ///===========================================================================

  template <typename TpGeneric, typename UpGeneric>
  inline constexpr bool is_same_v = __is_same(TpGeneric, UpGeneric);

  template <typename T>
  struct remove_reference {
    using type = T;
  };

  template <typename T>
  struct remove_reference<T&> {
    using type = T;
  };

  template <typename T>
  struct remove_reference<T&&> {
    using type = T;
  };

  template <typename T>
  using remove_reference_t = typename remove_reference<T>::type;

  ///===========================================================================

  template <typename T>
  struct remove_pointer {
    using type = T;
  };

  template <typename T>
  struct remove_pointer<T*> {
    using type = T;
  };

  template <typename T>
  struct remove_pointer<T* const> {
    using type = T;
  };

  template <typename T>
  struct remove_pointer<T* volatile> {
    using type = T;
  };

  template <typename T>
  struct remove_pointer<T* const volatile> {
    using type = T;
  };

  template <typename T>
  using remove_pointer_t = typename remove_pointer<T>::type;

  ///===========================================================================

  template <typename T>
  struct is_pointer : false_type {};

  template <typename T>
  struct is_pointer<T*> : true_type {};

  template <typename T>
  struct is_pointer<T* const> : true_type {};

  template <typename T>
  struct is_pointer<T* volatile> : true_type {};

  template <typename T>
  struct is_pointer<T* const volatile> : true_type {};

  template <typename T>
  inline constexpr bool is_pointer_v = is_pointer<T>::value;

  ///===========================================================================

  template <typename T>
  struct is_reference : false_type {};

  template <typename T>
  struct is_reference<T&> : true_type {};

  template <typename T>
  struct is_reference<T&&> : true_type {};

  template <typename T>
  inline constexpr bool is_reference_v = is_reference<T>::value;

  ///===========================================================================

  template <bool Cond, typename T, typename F>
  struct conditional {
    using type = T;
  };

  template <typename T, typename F>
  struct conditional<false, T, F> {
    using type = F;
  };

  template <bool Cond, typename T, typename F>
  using conditional_t = typename conditional<Cond, T, F>::type;

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
