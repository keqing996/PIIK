#pragma once

#include "../../PlatformDefine.h"

#if PLATFORM_ANDROID

namespace Piik::Android
{
    int GetSystemProperty(const char* name);
    int GetDeviceApiLevel();
}

#endif