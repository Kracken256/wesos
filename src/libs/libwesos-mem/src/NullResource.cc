/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-mem/NullResource.hh>

using namespace wesos;

SYM_EXPORT ThreadSafe<mem::NullResource> mem::NULL_RESOURCE_GLOBAL;
