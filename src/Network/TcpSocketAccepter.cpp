#include "PIIK/Network/TcpSocketAccepter.h"
#include "Platform/PlatformApi.h"
#include "SocketUtil/SocketUtil.h"

namespace Piik
{
    TcpSocketAccepter::TcpSocketAccepter(IpAddress::Family af)
        : Socket(af)
    {
    }

    SocketState TcpSocketAccepter::Listen(const std::string &ip, uint16_t port)
    {
        auto ipOp = IpAddress::TryParse(ip);
        if (!ipOp)
            return SocketState::Error;

        return Listen(ipOp.value(), port);
    }

    SocketState TcpSocketAccepter::Listen(const IpAddress &ip, uint16_t port)
    {
        return Listen(EndPoint(ip, port));
    }

    SocketState TcpSocketAccepter::Listen(const EndPoint& endpoint)
    {
        if (!IsValid())
            return SocketState::InvalidSocket;

        // Check address families match.
        if (endpoint.GetAddressFamily() != _addressFamily)
            return SocketState::AddressFamilyNotMatch;

        sockaddr sockAddr {};
        SockLen structLen;
        if (!SocketUtil::CreateSocketAddress(endpoint, &sockAddr, &structLen))
            return SocketState::Error;

        if (::bind(Npi::ToNativeHandle(_handle), &sockAddr, structLen) == -1)
            return Npi::GetErrorState();

        if (::listen(Npi::ToNativeHandle(_handle), SOMAXCONN) == -1)
            return Npi::GetErrorState();

        return SocketState::Success;
    }

    std::pair<SocketState, TcpSocket> TcpSocketAccepter::Accept()
    {
        if (!IsValid())
            return { SocketState::InvalidSocket, TcpSocket(_addressFamily) };

        sockaddr_in address {};
        SockLen length = sizeof(address);
        SocketHandle result = ::accept(Npi::ToNativeHandle(_handle), reinterpret_cast<sockaddr*>(&address), &length);

        if (result == Npi::GetInvalidSocket())
            return { Npi::GetErrorState(), TcpSocket(_addressFamily) };

        outSocket.Close();
        outSocket

        return SocketState::Success;
    }
}
