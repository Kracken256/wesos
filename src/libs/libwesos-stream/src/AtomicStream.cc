/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/AtomicStream.hh>
#include <wesos-sync/SpinLock.hh>

using namespace wesos;
using namespace wesos::stream;
using namespace wesos::smartptr;
using namespace wesos::sync;

SYM_EXPORT AtomicStreamRef::AtomicStreamRef(Box<LockProtocol> lock, StreamProtocol& parent)
    : m_lock(move(lock)), m_inner(parent) {}

SYM_EXPORT auto AtomicStreamRef::virt_read_some(View<u8> someof) -> ReadResult {
  return m_lock->critical_section([&] { return m_inner.read_some(someof); });
}

SYM_EXPORT auto AtomicStreamRef::virt_read_byte() -> Nullable<u8> {
  return m_lock->critical_section([&] { return m_inner.read_byte(); });
}

SYM_EXPORT auto AtomicStreamRef::virt_read_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_inner.read_seek(pos); });
}

SYM_EXPORT auto AtomicStreamRef::virt_read_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_inner.read_pos(); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_write_some(View<u8> someof) -> WriteResult {
  return m_lock->critical_section([&] { return m_inner.write_some(someof); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_write_byte(u8 b) -> bool {
  return m_lock->critical_section([&] { return m_inner.write_byte(b); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_write_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_inner.write_seek(pos); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_flush() -> bool {
  return m_lock->critical_section([&] { return m_inner.flush(); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_set_cache(usize size) -> usize {
  return m_lock->critical_section([&] { return m_inner.set_cache(size); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_write_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_inner.write_pos(); });
}

SYM_EXPORT auto AtomicOutputStreamRef::virt_cache_size() const -> usize {
  return m_lock->critical_section([&] { return m_inner.cache_size(); });
}

SYM_EXPORT auto AtomicStreamRef::create(mem::MemoryResourceProtocol& mm,
                                        StreamProtocol& parent) -> Nullable<Box<AtomicStreamRef>> {
  if (auto basic_spinlock = Box<SpinLock>::create(mm)) [[likely]] {
    auto some_lock = box_cast<LockProtocol>(move(basic_spinlock.value()));
    return Box<AtomicStreamRef>::create(mm, move(some_lock), parent);
  }

  return null;
}

///===============================================================================================================

SYM_EXPORT AtomicStream::AtomicStream(Box<LockProtocol> lock, Box<StreamProtocol> parent)
    : m_lock(move(lock)), m_owned(move(parent)) {}

SYM_EXPORT auto AtomicStream::virt_read_some(View<u8> someof) -> ReadResult {
  return m_lock->critical_section([&] { return m_owned->read_some(someof); });
}

SYM_EXPORT auto AtomicStream::virt_read_byte() -> Nullable<u8> {
  return m_lock->critical_section([&] { return m_owned->read_byte(); });
}

SYM_EXPORT auto AtomicStream::virt_read_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_owned->read_seek(pos); });
}

SYM_EXPORT auto AtomicStream::virt_read_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_owned->read_pos(); });
}

SYM_EXPORT auto AtomicOutputStream::virt_write_some(View<u8> someof) -> WriteResult {
  return m_lock->critical_section([&] { return m_owned->write_some(someof); });
}

SYM_EXPORT auto AtomicOutputStream::virt_write_byte(u8 b) -> bool {
  return m_lock->critical_section([&] { return m_owned->write_byte(b); });
}

SYM_EXPORT auto AtomicOutputStream::virt_write_seek(isize pos) -> bool {
  return m_lock->critical_section([&] { return m_owned->write_seek(pos); });
}

SYM_EXPORT auto AtomicOutputStream::virt_flush() -> bool {
  return m_lock->critical_section([&] { return m_owned->flush(); });
}

SYM_EXPORT auto AtomicOutputStream::virt_set_cache(usize size) -> usize {
  return m_lock->critical_section([&] { return m_owned->set_cache(size); });
}

SYM_EXPORT auto AtomicOutputStream::virt_write_pos() const -> Nullable<usize> {
  return m_lock->critical_section([&] { return m_owned->write_pos(); });
}

SYM_EXPORT auto AtomicOutputStream::virt_cache_size() const -> usize {
  return m_lock->critical_section([&] { return m_owned->cache_size(); });
}

SYM_EXPORT auto AtomicStream::create(mem::MemoryResourceProtocol& mm,
                                     Box<StreamProtocol> parent) -> Nullable<Box<AtomicStream>> {
  if (auto basic_spinlock = Box<SpinLock>::create(mm)) [[likely]] {
    auto some_lock = box_cast<LockProtocol>(move(basic_spinlock.value()));
    return Box<AtomicStream>::create(mm, move(some_lock), move(parent));
  }

  return null;
}
