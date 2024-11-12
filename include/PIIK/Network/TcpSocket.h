#pragma once

#include "Socket.h"

namespace Piik
{
    class TcpSocket: public Socket
    {
    public:
        explicit TcpSocket(IpAddress::Family af = IpAddress::Family::IpV4);

        enum class Role { None, Client, Server };

    public:
        bool CreateAsClient();
        bool CreateAsServer();
        Role GetRole() const;

        // Get remote ip & port
        bool TryGetRemoteEndpoint(EndPoint& outEndpoint) const;

        // Send & Recv
        std::pair<SocketState, size_t> Send(void* pData, size_t size) const;
        std::pair<SocketState, size_t> Receive(void* pBuffer, size_t size) const;

        // Connect an endpoint.
        SocketState Connect(const std::string& ip, uint16_t port, int timeOutInMs = -1);
        SocketState Connect(const IpAddress& ip, uint16_t port, int timeOutInMs = -1);
        SocketState Connect(const EndPoint& endpoint, int timeOutInMs = -1);

        // Connect an endpoint.
        SocketState Listen(const std::string& ip, uint16_t port);
        SocketState Listen(const IpAddress& ip, uint16_t port);
        SocketState Listen(const EndPoint& endpoint);

        // Accept
        SocketState Accept(Socket& outSocket);

    private:
        bool InternalCreateSocket();

    private:
        Role _role;
    };
}