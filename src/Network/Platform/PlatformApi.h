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
    namespace Npi
    {
        INFRA_FORCE_INLINE
        SocketHandle ToNativeHandle(void* handle);

        INFRA_FORCE_INLINE
        void* ToGeneralHandle(SocketHandle sock);

        SocketHandle GetInvalidSocket();

        void CloseSocket(void* handle);

        bool SetSocketBlocking(void* handle, bool block);

        SocketState GetErrorState();
    }
}

