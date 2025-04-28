/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-types/Types.hh>

namespace wesos::kern::boot {
  class Options {
    using StringView = View<const char>;
    using OptionString = Pair<StringView, StringView>;

  public:
  };
}  // namespace wesos::kern::boot
