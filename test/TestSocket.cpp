#include <iostream>
#include <format>
#include <PIIK/Network/DNS.h>
#include <PIIK/Network/Socket.h>
#include <PIIK/Network/Network.h>
#include <PIIK/Network/TcpSocket.h>

using namespace Piik;

int main()
{
    Network::Initialize();

    auto hostName = DNS::GetHostName();
    std::cout << std::format("Hostname: {}", hostName) << std::endl;

    auto localAddr = DNS::GetLocalIpAddress();
    for (auto addr: localAddr)
    {
        std::cout << std::format("Host address: {}", addr.ToString()) << std::endl;
    }

    TcpSocket socket;


    system("pause");
}