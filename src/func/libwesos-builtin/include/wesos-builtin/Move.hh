/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Template.hh>

namespace wesos {
  template <typename TGeneric>
  constexpr auto move(TGeneric&& t) -> types::remove_reference_t<TGeneric>&& {
    return static_cast<types::remove_reference_t<TGeneric>&&>(t);
  }
}  // namespace wesos