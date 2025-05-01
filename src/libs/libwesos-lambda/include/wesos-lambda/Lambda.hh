/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Null.hh>

namespace wesos::lambda {
  template <typename FuncGeneric>
  class Lambda;

  template <typename ResultGeneric, typename... ArgsGeneric>
  class Lambda<ResultGeneric(ArgsGeneric...)> {
    class CallableBaseProtocol {
    public:
      virtual ~CallableBaseProtocol() = default;

      virtual auto operator()(ArgsGeneric...) -> ResultGeneric = 0;
      virtual auto virt_clone() -> CallableBaseProtocol* = 0;
    };

    template <typename FuncGeneric>
    class Callable final : public CallableBaseProtocol {
      FuncGeneric m_functor;

    public:
      Callable(FuncGeneric functor) : m_functor(functor) {}

      auto virt_clone() -> CallableBaseProtocol* override {
        return new Callable<FuncGeneric>(m_functor);
      }

      auto operator()(ArgsGeneric... d) -> ResultGeneric override { return m_functor(d...); }
    };

  public:
    using FunctionType = ResultGeneric(ArgsGeneric...);

    constexpr Lambda() : m_callable(nullptr) {}

    constexpr Lambda(types::Null) : m_callable(nullptr) {}

    constexpr auto operator<=>(const Lambda& other) const = default;

    Lambda(const Lambda& other)
        : m_callable(other.m_callable ? other.m_callable->clone() : nullptr) {}

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

    template <class FuncGeneric>
    Lambda(FuncGeneric&& f) {
      m_callable = new Callable<FuncGeneric>(f);
    }

    ~Lambda() { delete m_callable; }

    auto operator()(ArgsGeneric... args) const -> ResultGeneric { return (*m_callable)(args...); }

  private:
    CallableBaseProtocol* m_callable;
  };
}  // namespace wesos::lambda

namespace wesos {
  using lambda::Lambda;
}
