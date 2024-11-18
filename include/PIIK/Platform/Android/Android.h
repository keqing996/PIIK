#pragma once

#include "../../PlatformDefine.h"

#if PLATFORM_ANDROID

namespace Piik
{
    class Android
    {
    public:
        Android() = delete;

        enum LogLevel
        {
            Default,
            Verbose,
            Debug,
            Info,
            Warn,
            Error,
            Fatal,
            Silent
        };

    public:
        /* Log */
        static void LogCat(LogLevel level, const char* tag, const char* msg);

        /* System */
        static int GetSystemProperty(const char* name);
        static int GetDeviceApiLevel();
    };
}

#endif