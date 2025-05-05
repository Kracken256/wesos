/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/OutputStreamProtocol.hh>

using namespace wesos;
using namespace wesos::stream;

SYM_EXPORT auto OutputStreamProtocol::virt_write_byte(u8 b) -> bool { return virt_write_some({&b, 1}).count() == 1; }
SYM_EXPORT auto OutputStreamProtocol::virt_write_seek(isize) -> bool { return false; }
SYM_EXPORT auto OutputStreamProtocol::virt_write_flush() -> bool { return false; }
SYM_EXPORT auto OutputStreamProtocol::virt_write_set_cache(usize) -> usize { return 0; }
SYM_EXPORT auto OutputStreamProtocol::virt_write_pos() const -> Nullable<usize> { return null; }
SYM_EXPORT auto OutputStreamProtocol::virt_write_cache_size() const -> usize { return 0; }

SYM_EXPORT auto OutputStreamProtocol::write_some(View<u8> someof) -> WriteResult { return virt_write_some(someof); }
SYM_EXPORT auto OutputStreamProtocol::write_byte(u8 b) -> bool { return virt_write_byte(b); }
SYM_EXPORT auto OutputStreamProtocol::write_seek(isize pos) -> bool { return virt_write_seek(pos); }
SYM_EXPORT auto OutputStreamProtocol::write_pos() const -> Nullable<usize> { return virt_write_pos(); }
SYM_EXPORT auto OutputStreamProtocol::write_flush() -> bool { return virt_write_flush(); }
SYM_EXPORT auto OutputStreamProtocol::write_set_cache(usize size) -> usize { return virt_write_set_cache(size); }
SYM_EXPORT auto OutputStreamProtocol::write_cache_size() const -> usize { return virt_write_cache_size(); }
SYM_EXPORT auto OutputStreamProtocol::write(View<u8> allof) -> bool {
  while (!allof.empty()) {
    auto res = write_some(allof);
    if (res.failed()) [[unlikely]] {
      return false;
    }

    allof = allof.subview_unchecked(res.count());
  }

  return true;
}
