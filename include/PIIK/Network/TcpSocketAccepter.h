#pragma once

#include "TcpSocket.h"

namespace Piik
{
    class TcpSocketAccepter: public Socket
    {
    public:
        explicit TcpSocketAccepter(IpAddress::Family af = IpAddress::Family::IpV4);

    public:
        // Connect an endpoint.
        SocketState Listen(const std::string& ip, uint16_t port);
        SocketState Listen(const IpAddress& ip, uint16_t port);
        SocketState Listen(const EndPoint& endpoint);

        // Accept
        std::pair<SocketState, TcpSocket> Accept();
    };
}