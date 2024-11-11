#include <iostream>
#include <format>
#include <PIIK/Utility/ScopeGuard.h>
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
    if (!socket.Create())
    {
        std::cout << "Socket create failed." << std::endl;
        system("pause");
        return 0;
    }

    ScopeGuard guard([&socket]()->void { socket.Close(); });

    auto ret = socket.Connect("10.12.16.56", 4869);
    if (ret != SocketState::Success)
    {
        std::cout << std::format("Socket connect failed with {}.", ret) << std::endl;
        system("pause");
        return 0;
    }

    const char* str = "Hello World!";
    auto [sendRet, sendSize] = socket.Send((void*)str, sizeof(str));
    if (sendRet != SocketState::Success)
    {
        std::cout << std::format("Socket send failed with {}.", sendRet) << std::endl;
        system("pause");
        return 0;
    }

    std::cout << std::format("Send: {}", str) << std::endl;

    char receiveBuf[1024];
    auto [recvRet, recvSize] = socket.Receive(receiveBuf, sizeof(receiveBuf));
    if (recvRet != SocketState::Success)
    {
        std::cout << std::format("Socket recv failed with {}.", recvRet) << std::endl;
        system("pause");
        return 0;
    }

    std::cout << std::format("Receive: {}", receiveBuf) << std::endl;

    system("pause");
}