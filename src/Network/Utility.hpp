#pragma once

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
    };
}