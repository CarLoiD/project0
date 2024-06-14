#include "RHI/Color.h"

using namespace RHI;

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    elem.r = r;
    elem.g = g;
    elem.b = b;
    elem.a = a;
}

Color::Color(uint32_t ucolor) {
    this->ucolor = ucolor;
}