/*
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#pragma once

#include <wesos-smartptr/Arc.hh>
#include <wesos-smartptr/Box.hh>
#include <wesos-smartptr/Rc.hh>

/**
 * @brief Defines the namespace `wesos::smartptr` for smart pointer utilities.
 *
 * @namespace wesos::smartptr
 * @details This namespace contains various smart pointer classes and utilities,
 *          including `Box`, `Rc`, and `Arc`. These classes provide ownership
 *          semantics for dynamically allocated objects, ensuring proper
 *          destruction and deallocation. The namespace also includes utility
 *          functions for accessing and managing the underlying objects.
 */
namespace wesos::smartptr {}
