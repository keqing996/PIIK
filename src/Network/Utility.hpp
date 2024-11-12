#pragma once

#include <PIIK/Network/EndPoint.h>
#include <PIIK/Network/IpAddress.h>

#include "Platform/PlatformApi.h"

namespace Piik
{
    class SocketUtil
    {
    public:
        SocketUtil() = delete;

    public:
        static int GetAddressFamily(IpAddress::Family family)
        {
            switch (family)
            {
                case IpAddress::Family::IpV4:
                    return AF_INET;
                case IpAddress::Family::IpV6:
                    return AF_INET6;
            }

            return AF_INET;
        }

        static std::pair<int, int> GetTcpProtocol()
        {
            return std::make_pair(IPPROTO_TCP, SOCK_STREAM);
        }

        static std::pair<int, int> GetUdpProtocol()
        {
            return std::make_pair(IPPROTO_UDP, SOCK_DGRAM);
        }

        static bool CreateSocketAddress(const EndPoint& endpoint, sockaddr* pResult, int* structLen)
        {
            switch (endpoint.GetAddressFamily())
            {
                case IpAddress::Family::IpV4:
                {
                    auto* pAddr = reinterpret_cast<sockaddr_in*>(pResult);
                    pAddr->sin_addr.s_addr = htonl(endpoint.GetIp().GetV4Addr());
                    pAddr->sin_family = AF_INET;
                    pAddr->sin_port = htons(endpoint.GetPort());
                    *structLen = sizeof(sockaddr_in);
                    return true;
                }
                case IpAddress::Family::IpV6:
                {
                    auto* pAddr = reinterpret_cast<sockaddr_in6*>(pResult);
                    ::memcpy(&pAddr->sin6_addr, endpoint.GetIp().GetV6Addr(), IpAddress::IPV6_ADDR_SIZE_BYTE);
                    pAddr->sin6_family = AF_INET6;
                    pAddr->sin6_scope_id = endpoint.GetV6ScopeId();
                    pAddr->sin6_port = htons(endpoint.GetPort());
                    *structLen = sizeof(sockaddr_in6);
                    return true;
                }
                default:
                    return false;
            }
        }
    };
}