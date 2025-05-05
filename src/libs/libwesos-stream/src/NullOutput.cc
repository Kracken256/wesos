/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/NullOutput.hh>

using namespace wesos;
using namespace wesos::stream;

SYM_EXPORT auto NullOutput::virt_write_some(View<u8> someof) -> WriteResult {
  m_offset += someof.size();
  return someof.size();
}

SYM_EXPORT auto NullOutput::virt_write_byte(u8) -> bool {
  m_offset += 1;
  return true;
}

SYM_EXPORT auto NullOutput::virt_write_seek(isize pos) -> bool {
  m_offset = static_cast<usize>(isize(m_offset) + pos);
  return true;
}

SYM_EXPORT auto NullOutput::virt_flush() -> bool { return true; }

SYM_EXPORT auto NullOutput::virt_set_cache(usize size) -> usize {
  m_cache_size = size;
  return m_cache_size;
}

SYM_EXPORT auto NullOutput::virt_write_pos() const -> Nullable<usize> { return m_offset; }

SYM_EXPORT auto NullOutput::virt_cache_size() const -> usize { return m_cache_size; }
