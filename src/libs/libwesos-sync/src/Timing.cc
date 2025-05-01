/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-sync/Timing.hh>

SYM_EXPORT void wesos::sync::ephemeral_pause() {
#if defined(__x86_64__)
  asm volatile("pause" ::: "memory");
#else
#error "This implementation of ephemeral_pause() does not support your architecure. Sorry.."
#endif
}
