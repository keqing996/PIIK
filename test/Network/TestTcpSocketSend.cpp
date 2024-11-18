#include <iostream>
#include <format>
#include <PIIK/Utility/ScopeGuard.h>
#include <PIIK/Network/Socket.h>
#include <PIIK/Network/Network.h>
#include <PIIK/Network/TcpSocket.h>

using namespace Piik;

int main()
{
    ScopeGuard pauseGuard([]()->void { ::system("pause"); });

    Network::Initialize();

    auto socketOpt = TcpSocket::Create(IpAddress::Family::IpV4);
    if (!socketOpt)
    {
        std::cout << "Socket create failed." << std::endl;
        return -1;
    }

    TcpSocket socket = socketOpt.value();
    ScopeGuard guard([&socket]()->void { socket.Close(); });

    auto ret = socket.Connect("10.12.16.56", 4869);
    if (ret != SocketState::Success)
    {
        std::cout << std::format("Socket connect failed with {}.", (int)ret) << std::endl;
        system("pause");
        return 0;
    }

    std::string_view str = "Hello World!";
    auto [sendRet, sendSize] = socket.Send((void*)str.data(), str.length());
    if (sendRet != SocketState::Success)
    {
        std::cout << std::format("Socket send failed with {}.", (int)sendRet) << std::endl;
        system("pause");
        return 0;
    }

    std::cout << std::format("Send: {}", str) << std::endl;

    std::vector<char> receiveBuf(1024);
    auto [recvRet, recvSize] = socket.Receive(receiveBuf.data(), receiveBuf.size());
    if (recvRet != SocketState::Success)
    {
        std::cout << std::format("Socket recv failed with {}.", (int)recvRet) << std::endl;
        system("pause");
        return 0;
    }

    std::cout << std::format("Receive: {}", receiveBuf.data()) << std::endl;
}