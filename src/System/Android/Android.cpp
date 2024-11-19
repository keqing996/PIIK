
#include "PIIK/Platform/Android/Android.h"

#if PLATFORM_ANDROID

#include <string>
#include <sys/system_properties.h>

namespace Piik::Android
{
    int GetSystemProperty(const char* name)
    {
        int result = 0;
        char value[PROP_VALUE_MAX] = {};
        if (::__system_property_get(name, value) >= 1)
            result = std::stoi(value);
        return result;
    }

    int GetDeviceApiLevel()
    {
        static int apiLevel = -1;

        if (apiLevel < 0)
            apiLevel = GetSystemProperty("ro.build.version.sdk");

        return apiLevel;
    }
}

#endif