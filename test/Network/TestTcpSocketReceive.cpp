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

    auto ret = socket.Listen("10.12.16.56", 4869);
    auto [acceptRet, clientSocket] = socket.Accept();
    if (acceptRet != SocketState::Success)


}