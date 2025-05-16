/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/Memory.hh>

SYM_EXPORT auto wesos::mem::get_default_resource() -> MemoryResourceProtocol & {
  /// TODO: Make a thread-safe wrapper that implements the MemoryResourceProtocol

  static NullResource NULL_RESOURCE_STATIC;
  static MemoryResourceProtocol &DEFAULT_RESOURCE_STATIC = NULL_RESOURCE_STATIC;

  return DEFAULT_RESOURCE_STATIC;
}

SYM_EXPORT auto wesos::mem::initialize(Kickstart initial_buffer) -> bool {
  /// TODO: Implement the memory library initialization logic
  return false;
}
