#include "PIIK/PlatformDefine.h"

#if PLATFORM_WINDOWS

#include "PIIK/System/Library.h"
#include "PIIK/Platform/Windows/WindowsDefine.h"

namespace Piik
{
    void* Library::Load(const char* dllName)
    {
        return ::LoadLibraryA(dllName);
    }

    void* Library::IsLibraryLoaded(const char* dllName)
    {
        return ::GetModuleHandleA(dllName);
    }

    void* Library::GetFunction(void* dll, const char* funcName)
    {
        if (dll == nullptr)
            return nullptr;

        return ::GetProcAddress(static_cast<HMODULE>(dll), funcName);
    }

    void Library::Unload(void* dll)
    {
        ::FreeLibrary(static_cast<HMODULE>(dll));
    }
}

#endif