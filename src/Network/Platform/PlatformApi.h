#pragma once

#include "PIIK/PlatformDefine.h"
#include "PIIK/Network/SocketState.h"

#if PLATFORM_WINDOWS

#include "PIIK/Platform/Windows/WindowsDefine.h"
#include <ws2tcpip.h>

using SocketHandle = SOCKET;
using SockLen = int;

#endif

#if PLATFORM_SUPPORT_POSIX

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
            // Macos does not allow void* -> int conversion.
            // So we have to convert to size_t first.
            return static_cast<SocketHandle>(reinterpret_cast<size_t>(handle));
        }

        PIIK_FORCE_INLINE
        static void* ToGeneralHandle(SocketHandle sock)
        {
            return reinterpret_cast<void*>(sock);
        }

    public:
        static void GlobalInit();

        static SocketHandle GetInvalidSocket();

        static void CloseSocket(void* handle);

        static bool SetSocketBlocking(void* handle, bool block);

        static SocketState GetErrorState();
    };
}

