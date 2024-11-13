#include <cstring>
#include "SocketUtil/SocketUtil.h"
#include "PIIK/Network/TcpSocket.h"
#include "Platform/PlatformApi.h"
#include "PIIK/Utility/ScopeGuard.h"

namespace Piik
{
    static SocketState ConnectNoSelect(int64_t handle, sockaddr* pSockAddr, int structLen)
    {
        if (::connect(Npi::ToNativeHandle(handle), pSockAddr, structLen) == -1)
            return Npi::GetErrorState();

        return SocketState::Success;
    }

    static SocketState ConnectWithSelect(const Socket* pSocket, sockaddr* pSockAddr, int structLen, int timeOutInMs)
    {
        // Connect once, if connection is success, no need to select.
        if (::connect(Npi::ToNativeHandle(pSocket->GetNativeHandle()), pSockAddr, structLen) >= 0)
            return SocketState::Success;

        return Socket::SelectWrite(pSocket, timeOutInMs);
    }

    TcpSocket::TcpSocket(IpAddress::Family af)
        : Socket(af)
    {
    }

    bool TcpSocket::InternalCreateSocket()
    {
        if (!IsValid())
            Close();

        auto addressFamily = SocketUtil::GetAddressFamily(_addressFamily);
        auto [wsaSocketType, wsaProtocol] = SocketUtil::GetTcpProtocol();

        const SocketHandle handle = ::socket(addressFamily, wsaSocketType, wsaProtocol);
        if (handle != Npi::GetInvalidSocket())
        {
            _handle = Npi::ToGeneralHandle(handle);

            SetBlocking(true, true);
            return true;
        }

        return false;
    }

    bool TcpSocket::TryGetRemoteEndpoint(EndPoint& outEndpoint) const
    {
        if (!IsValid())
            return false;

        if (_addressFamily == IpAddress::Family::IpV4)
        {
            sockaddr_in address{};
            SockLen structLen = sizeof(sockaddr_in);
            if (::getpeername(Npi::ToNativeHandle(_handle), reinterpret_cast<sockaddr*>(&address), &structLen) != -1)
            {
                outEndpoint = EndPoint(IpAddress(ntohl(address.sin_addr.s_addr)), ntohs(address.sin_port));
                return true;
            }

            return false;
        }

        if (_addressFamily == IpAddress::Family::IpV6)
        {
            sockaddr_in6 address{};
            SockLen structLen = sizeof(sockaddr_in6);
            if (::getpeername(Npi::ToNativeHandle(_handle), reinterpret_cast<sockaddr*>(&address), &structLen) != -1)
            {
                outEndpoint = EndPoint(IpAddress(address.sin6_addr.s6_addr), ntohs(address.sin6_port));
                return true;
            }

            return false;
        }

        return false;
    }

    std::pair<SocketState, size_t> TcpSocket::Send(void* pData, size_t size) const
    {
        if (!IsValid())
            return { SocketState::InvalidSocket, 0 };

        if (pData == nullptr || size == 0)
            return { SocketState::Error, 0 };

        int result = ::send(Npi::ToNativeHandle(_handle), static_cast<const char*>(pData), size, 0);
        if (result < 0)
            return { Npi::GetErrorState(), 0 };

        return { SocketState::Success, result };
    }

    std::pair<SocketState, size_t> TcpSocket::Receive(void* pBuffer, size_t size) const
    {
        if (!IsValid())
            return { SocketState::InvalidSocket, 0 };

        if (pBuffer == nullptr || size == 0)
            return { SocketState::Error, 0 };

        int result = ::recv(Npi::ToNativeHandle(_handle), static_cast<char*>(pBuffer), size, 0);
        if (result == 0)
            return { SocketState::Disconnect, 0 };

        if (result < 0)
            return { Npi::GetErrorState(), 0 };

        return { SocketState::Success, result };
    }

    SocketState TcpSocket::Connect(const std::string& ip, uint16_t port, int timeOutInMs)
    {
        auto ipOp = IpAddress::TryParse(ip);
        if (!ipOp)
            return SocketState::Error;

        return Connect(ipOp.value(), port, timeOutInMs);
    }

    SocketState TcpSocket::Connect(const IpAddress& ip, uint16_t port, int timeOutInMs)
    {
        return Connect(EndPoint(ip, port), timeOutInMs);
    }

    SocketState TcpSocket::Connect(const EndPoint& endpoint, int timeOutInMs)
    {
        if (!IsValid())
            return SocketState::InvalidSocket;

        // Check address families match.
        if (endpoint.GetAddressFamily() != _addressFamily)
            return SocketState::AddressFamilyNotMatch;

        sockaddr sockAddr {};
        int structLen;
        if (!SocketUtil::CreateSocketAddress(endpoint, &sockAddr, &structLen))
            return SocketState::Error;

        // [NonTimeout + Blocking/NonBlocking] -> just connect
        if (timeOutInMs <= 0)
            return ConnectNoSelect(_handle, &sockAddr, structLen);

        // [Timeout + NonBlocking] -> just connect
        if (!IsBlocking())
            return ConnectNoSelect(_handle, &sockAddr, structLen);

        // [Timeout + Blocking] -> set nonblocking and select
        SetBlocking(false);
        ScopeGuard guard([this]()->void { SetBlocking(true); });

        return ConnectWithSelect(this, &sockAddr, structLen, timeOutInMs);
    }
}
