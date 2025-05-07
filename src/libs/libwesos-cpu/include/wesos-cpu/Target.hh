/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#if defined(__x86_64__)
#define ARCH_X86_64 1
#define ADDRESS_SIZE 8
#elif defined(__i386__)
#define ARCH_X86_32 1
#define ADDRESS_SIZE 4
#else
#error "Sorry, WesOS does not currently support your target architecture"
#endif
