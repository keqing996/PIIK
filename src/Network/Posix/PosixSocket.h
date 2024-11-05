#pragma once

#include "PIIK/PlatformDefine.h"

#if PLATFORM_SUPPORT_POSIX

#include "PIIK/Network/SocketState.h"

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

namespace Piik
{
    using SocketHandle = int;

    namespace Device
    {
        PIIK_FORCE_INLINE
        SocketHandle ToNativeHandle(void* handle);

        PIIK_FORCE_INLINE
        void* ToGeneralHandle(SocketHandle sock);
        
        SocketHandle GetInvalidSocket();

        void CloseSocket(void* handle);

        bool SetSocketBlocking(void* handle, bool block);
    }
}

#endif