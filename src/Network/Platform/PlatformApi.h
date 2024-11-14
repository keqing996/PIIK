#pragma once

#include <cstdint>
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
#include <netinet/tcp.h>

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
        static SocketHandle ToNativeHandle(int64_t handle)
        {
            return static_cast<SocketHandle>(handle);
        }

        PIIK_FORCE_INLINE
        static int64_t ToGeneralHandle(SocketHandle sock)
        {
            return static_cast<int64_t>(sock);
        }

    public:
        static void GlobalInit();

        static SocketHandle GetInvalidSocket();

        static void CloseSocket(int64_t handle);

        static bool SetSocketBlocking(int64_t handle, bool block);

        static SocketState GetErrorState();
    };
}

