#include <iostream>
#include <format>
#include <PIIK/Network/DNS.h>
#include <PIIK/Network/Socket.h>
#include <PIIK/Network/Network.h>

using namespace Piik;

int main()
{
    Network::Initialize();

    auto localAddr = DNS::GetLocalIpAddress();
    for (auto addr: localAddr)
    {
        std::cout << std::format("Address: {}", addr.ToString()) << std::endl;
    }

    system("pause");
}