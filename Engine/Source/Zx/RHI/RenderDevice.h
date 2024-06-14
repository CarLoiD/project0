#ifndef RHI_RENDER_DEVICE_H
#define RHI_RENDER_DEVICE_H

#include "Color.h"

namespace RHI {
    struct RenderDevice {
        virtual ~RenderDevice() {}
        virtual void clearFrameBuffer(const Color& clearColor) = 0;
        virtual void swapBuffers() = 0;
    };
} // namespace RHI

#endif // RHI_RENDER_DEVICE_H