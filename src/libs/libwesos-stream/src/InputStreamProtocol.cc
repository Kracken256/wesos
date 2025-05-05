/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/InputStreamProtocol.hh>

using namespace wesos;
using namespace wesos::stream;

SYM_EXPORT auto InputStreamProtocol::virt_read_seek(isize) -> bool { return false; }
SYM_EXPORT auto InputStreamProtocol::virt_read_pos() const -> Nullable<usize> { return null; }
SYM_EXPORT auto InputStreamProtocol::virt_read_byte() -> Nullable<u8> {
  if (u8 b; virt_read_some({&b, 1}).count() == 1) [[likely]] {
    return b;
  }

  return null;
}

SYM_EXPORT auto InputStreamProtocol::read_some(View<u8> someof) -> ReadResult { return virt_read_some(someof); }
SYM_EXPORT auto InputStreamProtocol::read_byte() -> Nullable<u8> { return virt_read_byte(); }
SYM_EXPORT auto InputStreamProtocol::read_seek(isize pos) -> bool { return virt_read_seek(pos); }
SYM_EXPORT auto InputStreamProtocol::read_pos() const -> Nullable<usize> { return virt_read_pos(); }
SYM_EXPORT auto InputStreamProtocol::read(View<u8> allof) -> bool {
  while (!allof.empty()) {
    auto res = read_some(allof);
    if (res.failed()) [[unlikely]] {
      return false;
    }

    allof = allof.subview_unchecked(res.count());
  }

  return true;
}
