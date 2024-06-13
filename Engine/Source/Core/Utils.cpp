#include "Core/Utils.h"

#if defined(ZX_WINDOWS)
#include <windows.h>
#endif

void assert(const bool expression, const char* errorMessage) {
    if (!expression) {
    #if defined(ZX_WINDOWS)
        MessageBoxA(nullptr, errorMessage, "Assertion Error", MB_OK | MB_ICONERROR);
        PostQuitMessage(0);
    #endif
    }
}

void log(const char* tag, const char* fmt, ...) {
    // TODO: Not implemented
}