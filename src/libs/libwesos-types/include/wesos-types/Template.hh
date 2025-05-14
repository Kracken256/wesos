/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types {
  /*
   * Most magic in this file was generated with ChatGPT 2025.
   * I don't know what it does and I don't want to. Hopefully,
   * and bugs fail as compile time?!...
   */

  // NOLINTBEGIN(readability-identifier-naming)

  //=======================================================================

  template <typename T>
  struct add_rvalue_reference {
    using type = T;
  };

  template <typename T>
  struct add_rvalue_reference<T&&> {
    using type = T&&;
  };

  template <typename T>
  struct add_rvalue_reference<T&> {
    using type = T&;
  };

  template <typename T>
  struct add_rvalue_reference<T const> {
    using type = typename add_rvalue_reference<T>::type const;
  };

  template <typename T>
  using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

  //=======================================================================

  template <typename T>
  auto declval() noexcept -> add_rvalue_reference<T>::type;

  //=======================================================================

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

  //===========================================================================

  template <typename Tp, typename Up>
  inline constexpr bool is_same_v = __is_same(Tp, Up);

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

  //===========================================================================

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

  //===========================================================================

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

  //===========================================================================

  template <typename T>
  struct is_reference : false_type {};

  template <typename T>
  struct is_reference<T&> : true_type {};

  template <typename T>
  struct is_reference<T&&> : true_type {};

  template <typename T>
  inline constexpr bool is_reference_v = is_reference<T>::value;

  //===========================================================================

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

  //===========================================================================

  template <typename T>
  struct is_lvalue_reference : false_type {};

  template <typename T>
  struct is_lvalue_reference<T&> : true_type {};

  template <typename T>
  inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

  //===========================================================================

  template <typename T>
  constexpr auto forward(remove_reference_t<T>& t) -> T&& {
    return static_cast<T&&>(t);
  }

  template <typename T>
  constexpr auto forward(remove_reference_t<T>&& t) -> T&& {
    static_assert(!is_lvalue_reference_v<T>, "bad forward: can't forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
  }

  //===========================================================================

  template <typename T>
  auto lvalue_ref_helper(T&) -> T&;

  template <typename T>
  auto test(decltype(lvalue_ref_helper<T>(declval<T>()))*) -> char;

  template <typename T>
  auto test(...) -> int;

  template <typename T, bool = (sizeof(test<T>(nullptr)) == sizeof(char))>
  struct add_lvalue_reference_impl {
    using type = T&;
  };

  template <typename T>
  struct add_lvalue_reference_impl<T, false> {
    using type = T;
  };

  template <typename T>
  struct add_lvalue_reference : add_lvalue_reference_impl<T> {};

  template <typename T>
  using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

  //===========================================================================

  template <bool Condition, typename T = void>
  struct enable_if {};

  template <typename T>
  struct enable_if<true, T> {
    using type = T;
  };

  template <bool Condition, typename T = void>
  using enable_if_t = typename enable_if<Condition, T>::type;

  //===========================================================================

  template <typename T>
  struct is_move_constructible {
  private:
    // Try to instantiate a move constructor
    template <typename U, typename = decltype(U(declval<U&&>()))>
    static auto test(int) -> true_type;

    // Fallback if above is ill-formed
    template <typename>
    static auto test(...) -> false_type;

  public:
    static constexpr bool value = decltype(test<T>(0))::value;
  };

  template <typename T>
  inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

  template <typename T>
  struct is_copy_constructible {
  private:
    // Try to instantiate a copy constructor
    template <typename U, typename = decltype(U(declval<const U&>()))>
    static auto test(int) -> true_type;

    // Fallback
    template <typename>
    static auto test(...) -> false_type;

  public:
    static constexpr bool value = decltype(test<T>(0))::value;
  };

  template <typename T>
  inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

  //===========================================================================

  template <typename From, typename To>
  auto test_convert(int) -> decltype(static_cast<To>(declval<From>()), true_type{});

  template <typename, typename>
  auto test_convert(...) -> false_type;

  template <typename From, typename To>
  struct is_convertible : decltype(test_convert<From, To>(0)) {};

  template <typename From, typename To>
  inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

  //===========================================================================

  template <typename T>
  struct is_integral {
    static constexpr bool value = false;
  };

  template <>
  struct is_integral<bool> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<char> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<signed char> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<unsigned char> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<wchar_t> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<char16_t> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<char32_t> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<short> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<unsigned short> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<int> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<unsigned int> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<long> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<unsigned long> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<long long> {
    static constexpr bool value = true;
  };
  template <>
  struct is_integral<unsigned long long> {
    static constexpr bool value = true;
  };

  template <typename T>
  inline constexpr bool is_integral_v = is_integral<T>::value;

  //===========================================================================

  template <typename T>
  struct is_signed : false_type {};
  template <>
  struct is_signed<signed char> : true_type {};
  template <>
  struct is_signed<short> : true_type {};
  template <>
  struct is_signed<int> : true_type {};
  template <>
  struct is_signed<long> : true_type {};
  template <>
  struct is_signed<long long> : true_type {};

  template <typename T>
  inline constexpr bool is_signed_v = is_signed<T>::value;

  //===========================================================================

  template <typename T>
  struct is_unsigned : false_type {};
  template <>
  struct is_unsigned<unsigned char> : true_type {};
  template <>
  struct is_unsigned<unsigned short> : true_type {};
  template <>
  struct is_unsigned<unsigned int> : true_type {};
  template <>
  struct is_unsigned<unsigned long> : true_type {};
  template <>
  struct is_unsigned<unsigned long long> : true_type {};

  template <typename T>
  inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

  //===========================================================================

  template <typename T>
  struct make_unsigned;

  template <>
  struct make_unsigned<signed char> {
    using type = unsigned char;
  };
  template <>
  struct make_unsigned<short> {
    using type = unsigned short;
  };
  template <>
  struct make_unsigned<int> {
    using type = unsigned int;
  };
  template <>
  struct make_unsigned<long> {
    using type = unsigned long;
  };
  template <>
  struct make_unsigned<long long> {
    using type = unsigned long long;
  };

  template <>
  struct make_unsigned<unsigned char> {
    using type = unsigned char;
  };
  template <>
  struct make_unsigned<unsigned short> {
    using type = unsigned short;
  };
  template <>
  struct make_unsigned<unsigned int> {
    using type = unsigned int;
  };
  template <>
  struct make_unsigned<unsigned long> {
    using type = unsigned long;
  };
  template <>
  struct make_unsigned<unsigned long long> {
    using type = unsigned long long;
  };

  template <typename T>
  using make_unsigned_t = typename make_unsigned<T>::type;

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
