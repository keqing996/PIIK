#include "PIIK/PlatformDefine.h"

#if PLATFORM_SUPPORT_POSIX

#include <dlfcn.h>
#include "PIIK/System/Library.h"

namespace Piik
{
    void* Library::Load(const char* dllName)
    {
        return ::dlopen(dllName, RTLD_NOW);
    }

    void* Library::IsLibraryLoaded(const char* dllName)
    {
        return ::dlopen(dllName, RTLD_NOLOAD);
    }

    void* Library::GetFunction(void* dll, const char* funcName)
    {
        if (dll == nullptr)
            return nullptr;

        return ::dlsym(dll, funcName);
    }

    void Library::Unload(void* dll)
    {
        ::dlclose(dll);
    }
}

#endif