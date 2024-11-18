#pragma once

namespace Piik
{
    class Library
    {
    public:
        Library() = delete;

    public:
        static void* Load(const char* dllName);
        static void* IsLibraryLoaded(const char* dllName);
        static void* GetFunction(void* dll, const char* funcName);
        static void* GetFunction(const char* dllName, const char* funcName);
        static void Unload(void* dll);
    };
}