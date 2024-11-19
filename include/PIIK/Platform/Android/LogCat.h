#pragma once

#include "../../PlatformDefine.h"

#if PLATFORM_ANDROID

namespace Piik::Android
{
    class LogCat
    {
    public:
        LogCat() = delete;

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
        static void Log(LogLevel level, const char* tag, const char* msg);
    };
}

#endif