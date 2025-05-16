/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/AtomicInputStream.hh>
#include <wesos-sync/SpinLock.hh>

using namespace wesos;
using namespace wesos::stream;
using namespace wesos::smartptr;
using namespace wesos::sync;

SYM_EXPORT AtomicInputStreamRef::AtomicInputStreamRef(Box<SpinLock> lock, InputStreamProtocol& parent)
    : m_lock(move(lock)), m_inner(parent) {}

SYM_EXPORT auto AtomicInputStreamRef::virt_read_some(View<u8> someof) -> ReadResult {
  return m_lock->critical_section([&] { return m_inner.read_some(someof); });
}

SYM_EXPORT auto AtomicInputStreamRef::virt_read_byte() -> Nullable<u8> {
  return m_lock->critical_section([&] { return m_inner.read_byte(); });
}

SYM_EXPORT auto AtomicInputStreamRef::virt_read_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_inner.read_seek(pos); });
}

SYM_EXPORT auto AtomicInputStreamRef::virt_read_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_inner.read_pos(); });
}

SYM_EXPORT auto AtomicInputStreamRef::create(mem::MemoryResourceProtocol& mm,
                                             InputStreamProtocol& parent) -> Nullable<Box<AtomicInputStreamRef>> {
  if (auto basic_spinlock = Box<SpinLock>::create(mm)()) [[likely]] {
    Box<SpinLock> some_lock = move(basic_spinlock.value());
    return Box<AtomicInputStreamRef>::create(mm)(move(some_lock), parent);
  }

  return null;
}

//===============================================================================================================

SYM_EXPORT AtomicInputStream::AtomicInputStream(Box<SpinLock> lock, Box<InputStreamProtocol> parent)
    : m_lock(move(lock)), m_owned(move(parent)) {}

SYM_EXPORT auto AtomicInputStream::virt_read_some(View<u8> someof) -> ReadResult {
  return m_lock->critical_section([&] { return m_owned->read_some(someof); });
}

SYM_EXPORT auto AtomicInputStream::virt_read_byte() -> Nullable<u8> {
  return m_lock->critical_section([&] { return m_owned->read_byte(); });
}

SYM_EXPORT auto AtomicInputStream::virt_read_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_owned->read_seek(pos); });
}

SYM_EXPORT auto AtomicInputStream::virt_read_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_owned->read_pos(); });
}

SYM_EXPORT auto AtomicInputStream::create(mem::MemoryResourceProtocol& mm,
                                          Box<InputStreamProtocol> parent) -> Nullable<Box<AtomicInputStream>> {
  if (auto basic_spinlock = Box<SpinLock>::create(mm)()) [[likely]] {
    Box<SpinLock> some_lock = move(basic_spinlock.value());
    return Box<AtomicInputStream>::create(mm)(move(some_lock), move(parent));
  }

  return null;
}
