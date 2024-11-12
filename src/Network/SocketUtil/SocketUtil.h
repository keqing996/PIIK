#pragma once

#include <PIIK/Network/EndPoint.h>
#include <PIIK/Network/IpAddress.h>
#include "../Platform/PlatformApi.h"

namespace Piik
{
    class SocketUtil
    {
    public:
        SocketUtil() = delete;

    public:
        static int GetAddressFamily(IpAddress::Family family);

        static std::pair<int, int> GetTcpProtocol();

        static std::pair<int, int> GetUdpProtocol();

        static bool CreateSocketAddress(const EndPoint& endpoint, sockaddr* pResult, SockLen* structLen);
    };
}