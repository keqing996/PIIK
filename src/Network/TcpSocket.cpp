#include <cstring>
#include "Utility.hpp"
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
        , _role(Role::None)
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
    }

    bool TcpSocket::CreateAsClient()
    {
        if (!InternalCreateSocket())
            return false;

        _role = Role::Client;
        return true;
    }

    bool TcpSocket::CreateAsServer()
    {
        if (!InternalCreateSocket())
            return false;

        _role = Role::Server;
        return true;
    }

    TcpSocket::Role TcpSocket::GetRole() const
    {
        return _role;
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
        if (_role != Role::Client)
            return SocketState::RoleNotMatch;

        auto ipOp = IpAddress::TryParse(ip);
        if (!ipOp)
            return SocketState::Error;

        return Connect(ipOp.value(), port, timeOutInMs);
    }

    SocketState TcpSocket::Connect(const IpAddress& ip, uint16_t port, int timeOutInMs)
    {
        if (_role != Role::Client)
            return SocketState::RoleNotMatch;

        return Connect(EndPoint(ip, port), timeOutInMs);
    }

    SocketState TcpSocket::Connect(const EndPoint& endpoint, int timeOutInMs)
    {
        if (_role != Role::Client)
            return SocketState::RoleNotMatch;

        if (!IsValid())
            return SocketState::InvalidSocket;

        // Check address families match.
        if (endpoint.GetAddressFamily() != _addressFamily)
            return SocketState::AddressFamilyNotMatch;

        union SockAddr
        {
            sockaddr_in v4;
            sockaddr_in6 v6;
        };

        SockAddr address {};
        sockaddr* pSockAddr = reinterpret_cast<sockaddr*>(&address);
        int structLen;

        switch (_addressFamily)
        {
            case IpAddress::Family::IpV4:
                address.v4.sin_addr.s_addr = htonl(endpoint.GetIp().GetV4Addr());
                address.v4.sin_family = AF_INET;
                address.v4.sin_port = htons(endpoint.GetPort());
                structLen = sizeof(sockaddr_in);
                break;
            case IpAddress::Family::IpV6:
                ::memcpy(&address.v6.sin6_addr, endpoint.GetIp().GetV6Addr(), IpAddress::IPV6_ADDR_SIZE_BYTE);
                address.v6.sin6_family = AF_INET6;
                address.v6.sin6_scope_id = endpoint.GetV6ScopeId();
                address.v6.sin6_port = htons(endpoint.GetPort());
                structLen = sizeof(sockaddr_in6);
                break;
            default:
                return SocketState::Error;
        }

        // [NonTimeout + Blocking/NonBlocking] -> just connect
        if (timeOutInMs <= 0)
            return ConnectNoSelect(_handle, pSockAddr, structLen);

        // [Timeout + NonBlocking] -> just connect
        if (!IsBlocking())
            return ConnectNoSelect(_handle, pSockAddr, structLen);

        // [Timeout + Blocking] -> set nonblocking and select
        SetBlocking(false);
        ScopeGuard guard([this]()->void { SetBlocking(true); });

        return ConnectWithSelect(this, pSockAddr, structLen, timeOutInMs);
    }
}
