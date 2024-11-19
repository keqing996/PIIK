#pragma once

#include "../../PlatformDefine.h"

#if PLATFORM_ANDROID

namespace Piik::Android
{
    int CreateSharedMemory(const char* name, int memSize);
    void DestroySharedMemory(int fd);
}

#endif