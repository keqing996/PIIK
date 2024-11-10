#pragma once

#include "PIIK/PlatformDefine.h"

#if PLATFORM_WINDOWS

#include "PIIK/Network/SocketState.h"
#include "PIIK/Platform/Windows/WindowsDefine.h"
#include <ws2tcpip.h>

using SocketHandle = SOCKET;
using SockLen = int;

#endif

#if PLATFORM_POSIX

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

using SocketHandle = int;
using SockLen = socklen_t;

#endif

namespace Piik
{
    // Network platform interface
    class Npi
    {
    public:
        Npi() = delete;

    public:
        PIIK_FORCE_INLINE
        static SocketHandle ToNativeHandle(void* handle)
        {
            return reinterpret_cast<SocketHandle>(handle);
        }

        PIIK_FORCE_INLINE
        static void* ToGeneralHandle(SocketHandle sock)
        {
            return reinterpret_cast<void*>(sock);
        }

        static SocketHandle GetInvalidSocket();

        static void CloseSocket(void* handle);

        static bool SetSocketBlocking(void* handle, bool block);

        static SocketState GetErrorState();
    };
}

