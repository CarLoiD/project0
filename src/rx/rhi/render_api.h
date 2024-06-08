#pragma once

// Vulkan platforms: Windows, Linux and Android
// NGP platforms: PlayStation(R)2
#if defined(ZX_WINDOWS) | defined(ZX_ANDROID)
    //#include "vulkan/render_api.h"
#elif defined(ZX_PLAYSTATION_2)
    //#include "ngps/render_api.h
#endif