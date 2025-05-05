/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#ifndef _NEW

#include <wesos-types/Numeric.hh>

inline auto operator new(wesos::types::usize, void *p) noexcept -> void * { return p; }
inline auto operator new[](wesos::types::usize, void *p) noexcept -> void * { return p; }
inline void operator delete(void *, void *) noexcept {};
inline void operator delete[](void *, void *) noexcept {};

#endif
