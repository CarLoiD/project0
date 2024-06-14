#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <cstdint>

void assert(const bool expression, const char* errorMessage);
void log(const char* tag, const char* fmt, ...);

#endif // CORE_UTILS_H