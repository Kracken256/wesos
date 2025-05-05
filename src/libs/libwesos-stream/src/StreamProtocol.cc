/**
 * This file is part of the WesOS project.
 *
 * WesOS is public domain software: you can redistribute it and/or modify
 * it under the terms of the Unlicense(https://unlicense.org/).
 */

#include <wesos-builtin/Export.hh>
#include <wesos-stream/StreamProtocol.hh>

using namespace wesos;
using namespace wesos::stream;

SYM_EXPORT auto StreamProtocol::input() -> InputStreamProtocol& { return *this; }
SYM_EXPORT auto StreamProtocol::input() const -> const InputStreamProtocol& { return *this; }
SYM_EXPORT auto StreamProtocol::output() -> OutputStreamProtocol& { return *this; }
SYM_EXPORT auto StreamProtocol::output() const -> const OutputStreamProtocol& { return *this; }
