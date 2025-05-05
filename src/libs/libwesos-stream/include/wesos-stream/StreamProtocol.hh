/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-stream/InputStreamProtocol.hh>
#include <wesos-stream/OutputStreamProtocol.hh>

namespace wesos::stream {
  class StreamProtocol : public InputStreamProtocol, public OutputStreamProtocol {
  public:
    constexpr StreamProtocol() = default;
    constexpr StreamProtocol(const StreamProtocol&) = delete;
    constexpr StreamProtocol(StreamProtocol&&) = delete;
    constexpr auto operator=(const StreamProtocol&) -> StreamProtocol& = delete;
    constexpr auto operator=(StreamProtocol&&) -> StreamProtocol& = delete;
    constexpr ~StreamProtocol() override = default;

    [[nodiscard]] constexpr auto operator<=>(const StreamProtocol&) const = default;
  };
}  // namespace wesos::stream
