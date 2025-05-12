/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/EmptyInput.hh>

using namespace wesos;
using namespace wesos::stream;

SYM_EXPORT auto EmptyInput::virt_read_some(View<u8>) -> ReadResult { return ReadResult::null(); }

SYM_EXPORT auto EmptyInput::virt_read_byte() -> Nullable<u8> { return null; }

SYM_EXPORT auto EmptyInput::virt_read_seek(isize pos) -> bool {
  m_offset = static_cast<usize>(isize(m_offset) - pos);
  return true;
}

SYM_EXPORT auto EmptyInput::virt_read_pos() const -> Nullable<usize> { return m_offset; }
