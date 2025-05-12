/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Null.hh>

namespace wesos::lambda {
  template <class Func>
  class Lambda;

  template <class Result, class... Args>
  class Lambda<Result(Args...)> {
    class CallableBaseProtocol {
    public:
      virtual ~CallableBaseProtocol() = default;

      virtual auto operator()(Args...) -> Result = 0;
      virtual auto virt_clone() -> CallableBaseProtocol* = 0;
    };

    template <class Func>
    class Callable final : public CallableBaseProtocol {
      Func m_functor;

    public:
      Callable(Func functor) : m_functor(functor) {}

      auto virt_clone() -> CallableBaseProtocol* override { return new Callable<Func>(m_functor); }

      auto operator()(Args... d) -> Result override { return m_functor(d...); }
    };

  public:
    using FunctionType = Result(Args...);

    constexpr Lambda() : m_callable(nullptr) {}

    constexpr Lambda(types::Null) : m_callable(nullptr) {}

    constexpr auto operator<=>(const Lambda& other) const = default;

    Lambda(const Lambda& other) : m_callable(other.m_callable ? other.m_callable->clone() : nullptr) {}

    constexpr Lambda(Lambda&& other) {
      if (this != &other) [[likely]] {
        m_callable = other.m_callable;
        other.m_callable = nullptr;
      }
    }

    auto operator=(const Lambda& other) -> Lambda& {
      if (this != &other) [[likely]] {
        delete m_callable;
        m_callable = other.m_callable ? other.m_callable->clone() : nullptr;
      }
      return *this;
    };

    auto operator=(Lambda&& other) -> Lambda& {
      if (this != &other) [[likely]] {
        delete m_callable;
        m_callable = other.m_callable;
        other.m_callable = nullptr;
      }
      return *this;
    }

    auto operator=(types::Null) -> Lambda& {
      delete m_callable;
      m_callable = nullptr;
      return *this;
    }

    template <class Func>
    Lambda(Func&& f) {
      m_callable = new Callable<Func>(f);
    }

    ~Lambda() { delete m_callable; }

    auto operator()(Args... args) const -> Result { return (*m_callable)(args...); }

  private:
    CallableBaseProtocol* m_callable;
  };
}  // namespace wesos::lambda

namespace wesos {
  using lambda::Lambda;
}
