#include "PIIK/System/Library.h"

namespace Piik
{
    void* Library::GetFunction(const char* dllName, const char* funcName)
    {
        void* pDll = IsLibraryLoaded(dllName);
        if (pDll == nullptr)
            pDll = Load(dllName);

        if (pDll == nullptr)
            return nullptr;

        return GetFunction(pDll, funcName);
    }
}