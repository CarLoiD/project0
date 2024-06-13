#ifndef RHI_COLOR_H
#define RHI_COLOR_H

#include <cstdint>

namespace RHI {
    union Color {
        struct { uint8_t a, b, g, r; } elem;
        uint32_t ucolor {};
        
        Color() = default;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        Color(uint32_t ucolor);
    };
} // namespace RHI

#endif // RHI_COLOR_H