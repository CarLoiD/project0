#include "utils.h"

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <windows.h>

namespace Util {
    constexpr const uint32_t ASSERT_ERROR = -2;
    
    void Log(const char* tag, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        printf("[%s] ", tag);        
        vprintf(fmt, args);
        putchar('\n');
        
        va_end(args);
    }
    
    void Assert(bool expression, const char* message) {
        if (!expression) {
            MessageBoxA(nullptr, message, "Assertion Error!", MB_OK|MB_ICONERROR);
            exit(ASSERT_ERROR);
        }
    }
} // namespace Util 
