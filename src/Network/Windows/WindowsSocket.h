#pragma once

#include "PIIK/PlatformDefine.h"

#if PLATFORM_WINDOWS

#include "PIIK/Network/SocketState.h"
#include "PIIK/Platform/Windows/WindowsDefine.h"

#include <ws2tcpip.h>

namespace Piik
{
    using SocketHandle = SOCKET;

    namespace Device
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

#endif