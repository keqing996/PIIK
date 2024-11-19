#include "PIIK/Platform/Android/Android.h"

#if PLATFORM_ANDROID

#include <mutex>
#include <unistd.h>
#include "PIIK/System/Library.h"
#include "PIIK/Platform/Android/Android.h"
#include "PIIK/Platform/Android/SharedMemory.h"

namespace Piik::Android
{
    using FuncASharedMemoryCreate = int(*)(const char *, size_t);

    static std::mutex gLock;
    static bool gInitFinish = false;
    static FuncASharedMemoryCreate pSharedMemCreate = nullptr;

    struct SharedMemory
    {
        int fd = -1;
        int size = 0;
        void* ptr = nullptr;
    };

    static void Prepare()
    {
        if (gInitFinish)
            return;

        std::lock_guard guard(gLock);

        if (GetDeviceApiLevel() >= 26)
        {
            pSharedMemCreate = static_cast<FuncASharedMemoryCreate>(
                Library::GetFunction("libandroid.so", "ASharedMemory_create"));
        }
    }

    int CreateSharedMemory(const char* name, int memSize)
    {
        Prepare();

        if (pSharedMemCreate == nullptr)
            return -1;

        return pSharedMemCreate(name, memSize);
    }

    void DestroySharedMemory(int fd)
    {
        ::close(fd);
    }
}

#endif