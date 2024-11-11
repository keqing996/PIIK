#pragma once

#include "Socket.h"

namespace Piik
{
    class TcpSocket: public Socket
    {
    public:
        TcpSocket(IpAddress::Family af = IpAddress::Family::IpV4);

    public:
        // Connect an endpoint.
        SocketState Connect(const EndPoint& endpoint, int timeOutInMs = -1);

        // Disconnect
        void Disconnect();

        // Get remote ip & port
        bool TryGetRemoteEndpoint(EndPoint& outEndpoint) const;

        // Send
        std::pair<SocketState, size_t> Send(void* pData, size_t size);
        std::pair<SocketState, size_t> Receive(void* pBuffer, size_t size);
    };
}