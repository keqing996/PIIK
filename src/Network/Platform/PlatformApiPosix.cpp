#include "PlatformApi.h"

#if PLATFORM_SUPPORT_POSIX

namespace Piik
{
    SocketHandle Npi::ToNativeHandle(void* handle)
    {
        return reinterpret_cast<SocketHandle>(handle);
    }

    void* Npi::ToGeneralHandle(SocketHandle sock)
    {
        return reinterpret_cast<void*>(sock);
    }

    SocketHandle Npi::GetInvalidSocket()
    {
        return 0;
    }

    void Npi::CloseSocket(void* handle)
    {
        ::close(ToNativeHandle(handle));
    }

    bool Npi::SetSocketBlocking(void* handle, bool block)
    {
        int sock = ToNativeHandle(handle);
        const int status = ::fcntl(sock, F_GETFL);
        if (block)
        {
            int ret = ::fcntl(sock, F_SETFL, status & ~O_NONBLOCK);
            if (ret == -1)
                return false;
        }
        else
        {
            int ret = ::fcntl(sock, F_SETFL, status | O_NONBLOCK);
            if (ret == -1)
                return false;
        }

        return true;
    }

    SocketState Npi::GetErrorState()
    {
        // todo
    }
}

#endif


