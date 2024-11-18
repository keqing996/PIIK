
#include "PIIK/Platform/Android/Android.h"

#if PLATFORM_ANDROID

#include <string>
#include <sys/system_properties.h>
#include <android/log.h>

namespace Piik
{
    static int GetAndroidLogLevel(Android::LogLevel level)
    {
        switch (level)
        {
            case Android::Default:
                return ANDROID_LOG_DEFAULT;
            case Android::Verbose:
                return ANDROID_LOG_VERBOSE;
            case Android::Debug:
                return ANDROID_LOG_DEBUG;
            case Android::Info:
                return ANDROID_LOG_INFO;
            case Android::Warn:
                return ANDROID_LOG_WARN;
            case Android::Error:
                return ANDROID_LOG_ERROR;
            case Android::Fatal:
                return ANDROID_LOG_FATAL;
            case Android::Silent:
                return ANDROID_LOG_SILENT;
        }

        return ANDROID_LOG_DEFAULT;
    }

    void Android::LogCat(Android::LogLevel level, const char* tag, const char* msg)
    {
        __android_log_print(GetAndroidLogLevel(level), tag, "%s", msg);
    }

    int Android::GetSystemProperty(const char* name)
    {
        int result = 0;
        char value[PROP_VALUE_MAX] = {};
        if (::__system_property_get(name, value) >= 1)
            result = std::stoi(value);
        return result;
    }

    int Android::GetDeviceApiLevel()
    {
        static int apiLevel = -1;

        if (apiLevel < 0)
            apiLevel = GetSystemProperty("ro.build.version.sdk");

        return apiLevel;
    }
}

#endif