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

SYM_EXPORT auto InputStreamProtocol::virt_read(View<u8>) -> ReadResult { return ReadResult::null(); }
SYM_EXPORT auto InputStreamProtocol::virt_read_byte() -> Nullable<u8> { return null; }
SYM_EXPORT auto InputStreamProtocol::virt_read_seek(isize) -> bool { return false; }
SYM_EXPORT auto InputStreamProtocol::virt_read_pos() const -> Nullable<usize> { return null; }
SYM_EXPORT auto InputStreamProtocol::virt_is_atomic() const -> bool { return true; }

SYM_EXPORT auto InputStreamProtocol::read(View<u8> space) -> ReadResult { return virt_read(space); };
SYM_EXPORT auto InputStreamProtocol::read_byte() -> Nullable<u8> { return virt_read_byte(); }
SYM_EXPORT auto InputStreamProtocol::read_seek(isize off) -> bool { return virt_read_seek(off); }
SYM_EXPORT auto InputStreamProtocol::read_pos() const -> Nullable<usize> { return virt_read_pos(); }
SYM_EXPORT auto InputStreamProtocol::is_atomic() const -> bool { return virt_is_atomic(); }
