/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

namespace wesos::types {
  // NOLINTBEGIN(readability-identifier-naming)

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

  template <typename T, T v>
  struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;  // For compatibility with type traits

    constexpr operator value_type() const { return value; }
    constexpr auto operator()() const -> value_type { return value; }
  };

  using false_type = integral_constant<bool, false>;
  using true_type = integral_constant<bool, true>;

  template <typename T>
  struct is_pointer : false_type {};

  // Specialization: T* is a pointer
  template <typename T>
  struct is_pointer<T*> : true_type {};

  // Specialization for cv-qualified pointers
  template <typename T>
  struct is_pointer<T* const> : true_type {};

  template <typename T>
  struct is_pointer<T* volatile> : true_type {};

  template <typename T>
  struct is_pointer<T* const volatile> : true_type {};

  template <typename T>
  inline constexpr bool is_pointer_v = is_pointer<T>::value;

  // NOLINTEND(readability-identifier-naming)
}  // namespace wesos::types
