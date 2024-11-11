#pragma once

#include "SocketState.h"
#include "EndPoint.h"

namespace Piik
{
    class Socket
    {
    public:
        explicit Socket(IpAddress::Family af);
        virtual ~Socket() = default;

    public:
        virtual bool Create() = 0;

        // Return is socket in blocking mode.
        bool IsBlocking() const;

        // Set socket blocking mode.
        bool SetBlocking(bool block, bool force = false);

        // Close socket.
        void Close();

        // Return native handle by platform, SOCKET on widnows, int on posix plat.
        int64_t GetNativeHandle() const;

        bool IsValid() const;

        // Select
        static SocketState SelectRead(const Socket* pSocket, int timeoutInMs = -1);
        static SocketState SelectWrite(const Socket* pSocket, int timeoutInMs = -1);
        SocketState SelectRead(int timeoutInMs = -1);
        SocketState SelectWrite(int timeoutInMs = -1);

    protected:
        IpAddress::Family _addressFamily;
        bool _isBlocking;
        int64_t _handle;
    };
}