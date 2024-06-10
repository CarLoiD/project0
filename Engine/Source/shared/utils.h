#pragma once

#include "shared/types.h"

namespace Util {
    void Log(const char* tag, const char* fmt, ...);
    void Assert(bool expression, const char* message);
} // namespace Util