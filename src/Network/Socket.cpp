
#include "PIIK/Network/Socket.h"
#include "Platform/PlatformApi.h"

namespace Piik
{
    Socket::Socket(IpAddress::Family af)
        : _addressFamily(af)
        , _isBlocking(false)
        , _handle(0)
    {
    }

    int64_t Socket::GetNativeHandle() const
    {
        return _handle;
    }

    bool Socket::IsValid() const
    {
        return _handle != 0;
    }

    bool Socket::IsBlocking() const
    {
        return _isBlocking;
    }

    bool Socket::SetBlocking(bool block, bool force)
    {
        if (!force && block == IsBlocking())
            return true;

        return Npi::SetSocketBlocking(_handle, block);
    }

    void Socket::Close()
    {
        Npi::CloseSocket(_handle);
    }

    SocketState Socket::SelectRead(const Socket* pSocket, int timeoutInMs)
    {
        if (timeoutInMs <= 0)
            return SocketState::Error;

        if (pSocket == nullptr)
            return SocketState::Error;

        int64_t handle = pSocket->GetNativeHandle();

        fd_set selector;
        FD_ZERO(&selector);
        FD_SET(Npi::ToNativeHandle(handle), &selector);

        timeval time{};
        time.tv_sec  = static_cast<long>(timeoutInMs / 1000);
        time.tv_usec = timeoutInMs % 1000 * 1000;

        if (::select(static_cast<int>(Npi::ToNativeHandle(handle) + 1), &selector, nullptr, nullptr, &time) > 0)
            return SocketState::Success;

        return Npi::GetErrorState();
    }

    SocketState Socket::SelectWrite(const Socket* pSocket, int timeoutInMs)
    {
        if (timeoutInMs <= 0)
            return SocketState::Error;

        if (pSocket == nullptr)
            return SocketState::Error;

        int64_t handle = pSocket->GetNativeHandle();

        fd_set selector;
        FD_ZERO(&selector);
        FD_SET(Npi::ToNativeHandle(handle), &selector);

        timeval time{};
        time.tv_sec  = static_cast<long>(timeoutInMs / 1000);
        time.tv_usec = timeoutInMs % 1000 * 1000;

        if (::select(static_cast<int>(Npi::ToNativeHandle(handle) + 1), nullptr, &selector, nullptr, &time) > 0)
            return SocketState::Success;

        return Npi::GetErrorState();
    }

    SocketState Socket::SelectRead(int timeoutInMs)
    {
        return SelectRead(this, timeoutInMs);
    }

    SocketState Socket::SelectWrite(int timeoutInMs)
    {
        return SelectWrite(this, timeoutInMs);
    }
}
