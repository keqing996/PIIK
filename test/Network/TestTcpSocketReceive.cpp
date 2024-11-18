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

    auto listenRet = socket.Listen("10.12.16.23", 4869);
    if (listenRet != SocketState::Success)
    {
        std::cout << std::format("Socket listen failed with {}.", (int)listenRet) << std::endl;
        return -1;
    }

    auto [acceptRet, clientSocket] = socket.Accept();
    if (acceptRet != SocketState::Success)
    {
        std::cout << std::format("Socket accept failed with {}.", (int)listenRet) << std::endl;
        return -1;
    }

    ScopeGuard clientGuard([&clientSocket]()->void { clientSocket.Close(); });

    std::vector<char> receiveBuf(1024);
    auto [recvRet, recvSize] = clientSocket.Receive(receiveBuf.data(), receiveBuf.size());
    if (recvRet != SocketState::Success)
    {
        std::cout << std::format("Socket recv failed with {}.", (int)recvRet) << std::endl;
        return -1;
    }

    std::cout << std::format("Receive: {}", receiveBuf.data()) << std::endl;

    std::string_view str = "Hello World!";
    auto [sendRet, sendSize] = clientSocket.Send((void*)str.data(), str.length());
    if (sendRet != SocketState::Success)
    {
        std::cout << std::format("Socket send failed with {}.", (int)sendRet) << std::endl;
        return -1;
    }

    std::cout << std::format("Send: {}", str) << std::endl;

    return 0;

}