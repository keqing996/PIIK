#include "PIIK/Platform/Android/Android.h"

#if PLATFORM_ANDROID

#include <android/log.h>
#include "PIIK/Platform/Android/LogCat.h"

namespace Piik::Android
{
    static int GetAndroidLogLevel(LogCat::LogLevel level)
    {
        switch (level)
        {
            case LogCat::Default:
                return ANDROID_LOG_DEFAULT;
            case LogCat::Verbose:
                return ANDROID_LOG_VERBOSE;
            case LogCat::Debug:
                return ANDROID_LOG_DEBUG;
            case LogCat::Info:
                return ANDROID_LOG_INFO;
            case LogCat::Warn:
                return ANDROID_LOG_WARN;
            case LogCat::Error:
                return ANDROID_LOG_ERROR;
            case LogCat::Fatal:
                return ANDROID_LOG_FATAL;
            case LogCat::Silent:
                return ANDROID_LOG_SILENT;
        }

        return ANDROID_LOG_DEFAULT;
    }

    void LogCat::Log(LogLevel level, const char* tag, const char* msg)
    {
        __android_log_print(GetAndroidLogLevel(level), tag, "%s", msg);
    }
}

#endif