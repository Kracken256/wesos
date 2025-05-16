/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-mem/MemoryResourceProtocol.hh>
#include <wesos-types/NullableOwnPtr.hh>
#include <wesos-types/Template.hh>

namespace wesos::smartptr {
  /**
   * @class Box
   * @brief A smart pointer wrapper that manages the lifetime of an object and its associated memory resource.
   *
   * The `Box` class provides ownership semantics for dynamically allocated objects, ensuring proper
   * destruction and deallocation. It supports move semantics and disallows copy semantics. The class
   * also provides utility functions for accessing and managing the underlying object.
   *
   * @tparam Object The type of the object being managed by the `Box`.
   */
  template <class Object>
  class Box {
    NullableOwnPtr<Object> m_ptr;
    RefPtr<mem::MemoryResourceProtocol> m_mm;

    constexpr void destruct() {
      if (!m_ptr.is_null()) {
        unwrap()->~Object();
        m_mm->deallocate_bytes(unwrap(), sizeof(Object), alignof(Object));

        disown();
      }
    }

    constexpr Box(OwnPtr<Object> ptr, RefPtr<mem::MemoryResourceProtocol> mm) : m_ptr(ptr), m_mm(mm) {}

  public:
    constexpr Box(const Box& o) = delete;
    constexpr auto operator=(const Box& o) -> Box& = delete;

    /**
     * @brief Move constructor for the Box class.
     *
     * This constructor allows the creation of a new Box instance by transferring ownership
     * of the managed object from another Box instance of a compatible type.
     *
     * @tparam U The type of the object managed by the source Box.
     *
     * @param o The source Box instance to transfer ownership from. After the transfer,
     *          the source Box will no longer own the object.
     *
     * @note This constructor is only enabled if the type `U*` is convertible to `Object*`.
     *
     * @details The constructor initializes the new Box instance with the pointer and memory
     *          management strategy of the source Box. If the source and destination are not
     *          the same object, the source Box is disowned (its ownership is relinquished).
     */
    template <class U = Object>
    constexpr Box(Box<U>&& o)
      requires(types::is_convertible_v<U*, Object*>)
        : m_ptr(o.m_ptr), m_mm(o.m_mm) {
      if (this != &o) [[likely]] {
        o.disown();
      }
    }

    /**
     * @brief Move assignment operator for the Box class.
     *
     * @tparam U A type that must be convertible to Object*.
     * @param o The Box object to be moved from.
     * @return A reference to the current Box object after the move.
     *
     * @details This operator moves the contents of the given Box object `o` into the current Box object.
     *          It first checks if the current object and `o` are not the same (using `this != &o`).
     *          If they are different, it destructs the current object's contents, transfers the pointer
     *          (`m_ptr`) and memory manager (`m_mm`) from `o` to the current object, and then disowns
     *          the contents of `o` to ensure it no longer manages the moved resources.
     *
     * @note Unlike standard C++20, the memory manager (`m_mm`) propagates with the data during the move.
     * @warning Ensure that the type `U` is convertible to `Object*` to satisfy the `requires` clause.
     */
    template <class U = Object>
    constexpr auto operator=(Box<U>&& o) -> Box&
      requires(types::is_convertible_v<U*, Object*>)
    {
      if (this != &o) [[likely]] {
        destruct();

        m_ptr = o.m_ptr;
        m_mm = o.m_mm;  // Unlike in standard c++20, the pmr does propagate with the data;

        o.disown();
      }

      return *this;
    }

    /**
     * @brief Destructor for the Box class.
     *
     * If the Box instance is not null, it destructs the managed object and deallocates
     * the associated memory.
     */
    constexpr ~Box() { destruct(); }

    /**
     * @brief Compares the pointer of the current Box with another Box's pointer.
     *
     * @tparam U The type of the object contained in the other Box. Defaults to Object.
     * @param o The other Box to compare with.
     * @return A std::strong_ordering value indicating the result of the comparison.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto operator<=>(const Box<U>& o) const -> std::strong_ordering {
      return unwrap() <=> o.unwrap();
    }

    /**
     * @brief Compares the current object with a null value.
     *
     * This operator checks if the current object is null by invoking the `is_null()` method.
     *
     * @param null A placeholder parameter of type `types::Null` used to represent a null value.
     * @return true if the object is null, false otherwise.
     */
    [[nodiscard]] constexpr auto operator==(types::Null) const { return is_null(); }

    /**
     * @brief Compares the current object with a nullptr.
     *
     * @return true if the object is null, false otherwise.
     */
    [[nodiscard]] constexpr auto operator==(types::nullptr_t) const { return is_null(); }

    //=========================================================================================
    // LIFETIME MANAGEMENT
    //=========================================================================================

    /**
     * @brief Releases ownership of the managed object and returns a raw pointer to it.
     *
     * This function sets the internal smart pointer to null, effectively disowning
     * the managed object. It then returns the raw pointer to the previously managed
     * object. The caller is responsible for managing the lifetime of the returned
     * raw pointer to avoid resource leaks and undefined behavior.
     *
     * @return Object* A raw pointer to the previously managed object.
     */
    constexpr auto disown() -> Object* {
      m_ptr = null;
      return unwrap();
    }

    /**
     * @brief Obtains a reference to the memory resource associated with this Box.
     *
     * This function returns a reference to the memory resource used for allocation
     * and deallocation of the managed object. The memory resource is responsible
     * for managing the memory pool and allocation strategy.
     *
     * @return A reference to the memory resource associated with this Box.
     */
    [[nodiscard]] constexpr auto memory_resource() -> mem::MemoryResourceProtocol& { return *m_mm; }

    //=========================================================================================
    // POINTER ACCESS
    //=========================================================================================

    /**
     * @brief Retrieves the raw pointer managed by the Box.
     *
     * @return Object* The raw pointer to the managed object.
     *         Returns nullptr if the Box does not manage an object.
     */
    [[nodiscard]] constexpr auto unwrap() -> Object* { return m_ptr.unwrap(); }

    /**
     * @brief Retrieves the raw pointer to the encapsulated object.
     *
     * @return A constant pointer to the managed object.
     *         Returns nullptr if the Box does not manage an object.
     */
    [[nodiscard]] constexpr auto unwrap() const -> const Object* { return m_ptr.unwrap(); }

    /**
     * @brief Checks if the internal pointer is set.
     *
     * This function determines whether this Box instance is currently managing
     * an object.
     *
     * @return true if an object is managed, false otherwise.
     */
    [[nodiscard]] constexpr auto isset() const -> bool { return m_ptr.isset(); }

    /**
     * @brief Checks if the smart pointer is null.
     *
     * @return true if the smart pointer does not hold a valid object, false otherwise.
     */
    [[nodiscard]] constexpr auto is_null() const -> bool { return m_ptr.is_null(); }

    /**
     * @brief Overloaded arrow operator to access the underlying pointer.
     *
     * This operator allows access to the members of the object managed by the Box
     * through pointer dereferencing. It ensures that the pointer is set before
     * accessing it and unwraps the underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return U* A pointer to the underlying object.
     *
     * @note This operator is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto operator->() -> U* requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return unwrap();
    };

    /**
     * @brief Overloaded constant arrow operator to access the underlying pointer.
     *
     * This operator allows access to the members of the object managed by the Box
     * through pointer dereferencing. It ensures that the pointer is set before
     * accessing it and unwraps the underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return const U* A constant pointer to the managed object.
     *
     * @note This operator is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto operator->() const -> const U* requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return unwrap();
    };

    /**
     * @brief Overloaded dereference operator to access the underlying object.
     *
     * This operator allows direct access to the object managed by the Box.
     * It ensures that the pointer is set before accessing it and unwraps the
     * underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return U& A reference to the underlying object.
     *
     * @note This operator is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto operator*() -> U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Overloaded constant dereference operator to access the underlying object.
     *
     * This operator allows direct access to the object managed by the Box.
     * It ensures that the pointer is set before accessing it and unwraps the
     * underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return const U& A constant reference to the managed object.
     *
     * @note This operator is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto operator*() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Retrieves the managed object.
     *
     * This function returns a reference to the managed object. It ensures that
     * the pointer is set before accessing it and unwraps the underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return U& A reference to the underlying object.
     *
     * @note This function is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto get() -> U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Retrieves the managed object.
     *
     * This function returns a constant reference to the managed object. It ensures that
     * the pointer is set before accessing it and unwraps the underlying pointer.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return const U& A constant reference to the managed object.
     *
     * @note This function is only available when `U` is not `void`.
     *
     * @pre The internal pointer (`m_ptr`) must be set. This is enforced by an assertion.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto get() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_always(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Retrieves the managed object without checking if the pointer is set.
     *
     * This function returns a reference to the managed object without checking
     * if the pointer is set. It is the caller's responsibility to ensure that
     * the pointer is valid before calling this function.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return U& A reference to the underlying object.
     *
     * @note This function is only available when `U` is not `void`.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto get_unchecked() -> U& requires(!types::is_same_v<U, void>) {
      assert_invariant(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Retrieves the managed object without checking if the pointer is set.
     *
     * This function returns a constant reference to the managed object without checking
     * if the pointer is set. It is the caller's responsibility to ensure that
     * the pointer is valid before calling this function.
     *
     * @tparam U The type of the object being pointed to. Defaults to `Object`.
     * @return const U& A constant reference to the managed object.
     *
     * @note This function is only available when `U` is not `void`.
     */
    template <class U = Object>
    [[nodiscard]] constexpr auto get_unchecked() const -> const U& requires(!types::is_same_v<U, void>) {
      assert_invariant(m_ptr.isset());
      return *unwrap();
    };

    /**
     * @brief Creates a factory function for constructing a `Box` object.
     *
     * This static constexpr function generates a callable object (lambda) that
     * constructs a `Box` instance using the provided memory resource protocol
     * and forwarded arguments for the `Object` constructor. The lambda ensures
     * proper memory allocation and object construction, returning a nullable
     * `Box` in case of allocation failure.
     *
     * @param mm A reference to a `mem::MemoryResourceProtocol` instance used
     *           for memory allocation.
     *
     * @return A callable lambda function that takes variadic arguments for
     *         constructing an `Object` and returns a `Nullable<Box>`. If memory
     *         allocation fails, the lambda returns `null`.
     *
     * @note The lambda performs placement new to construct the `Object` in the
     *       allocated memory. It also wraps the allocated memory and memory
     *       resource protocol in smart pointers (`OwnPtr` and `RefPtr`) to
     *       ensure proper resource management.
     */
    [[nodiscard]] static constexpr auto create(mem::MemoryResourceProtocol& mm) {
      return [&mm](auto&&... args) -> Nullable<Box> {
        const auto object_storage = mm.allocate_bytes(sizeof(Object), alignof(Object));
        if (object_storage.is_null()) [[unlikely]] {
          return null;
        }

        ::new (object_storage.unwrap()) Object(forward<decltype(args)>(args)...);

        const auto object_ptr = OwnPtr(static_cast<Object*>(object_storage.unwrap()));
        const auto mm_ptr = RefPtr<mem::MemoryResourceProtocol>(&mm);

        return Box(object_ptr, mm_ptr);
      };
    }
  };

  static_assert(sizeof(Box<void>) == sizeof(void*) * 2);
}  // namespace wesos::smartptr
