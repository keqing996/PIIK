#include "PIIK/Platform/Android/Android.h"

#if PLATFORM_ANDROID

#include <mutex>
#include "PIIK/Platform/Android/Trace.h"
#include "PIIK/System/Library.h"

namespace Piik
{
    using FuncATraceBeginSection = void* (*)(const char*);
    using FuncATraceEndSection = void* (*)(void);

    static std::mutex gLock;
    static FuncATraceBeginSection pATraceBeginSection = nullptr;
    static FuncATraceEndSection pATraceEndSection = nullptr;

    static void InitTrace()
    {
        if (pATraceBeginSection != nullptr && pATraceEndSection != nullptr)
            return;

        std::lock_guard guard(gLock);

        pATraceBeginSection = static_cast<FuncATraceBeginSection>(
            Library::GetFunction("libandroid.so", "ATrace_beginSection"));
        pATraceEndSection = static_cast<FuncATraceEndSection>(
            Library::GetFunction("libandroid.so", "ATrace_endSection"));
    }

    Trace::Scoped::Scoped(const char* name)
    {
        Trace::Begin(name);
    }

    Trace::Scoped::~Scoped()
    {
        Trace::End();
    }

    void Trace::Begin(const char* name)
    {
        InitTrace();

        if (pATraceBeginSection != nullptr)
            pATraceBeginSection(name);
    }

    void Trace::End()
    {
        if (pATraceEndSection != nullptr)
            pATraceEndSection();
    }
}

#endif