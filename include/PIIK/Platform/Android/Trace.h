#pragma once

#include "../../PlatformDefine.h"

#if PLATFORM_ANDROID

namespace Piik::Android
{
    class Trace
    {
    public:
        Trace() = delete;

    public:
        struct Scoped
        {
            explicit Scoped(const char* name);
            ~Scoped();
        };

    public:
        static void Begin(const char* name);
        static void End();
    };
}

#endif