#include "WindowsSocket.h"

#if PLATFORM_WINDOWS

#pragma comment(lib, "Ws2_32.lib")

namespace Piik::Device
{
    struct WinSocketGuard
    {
        WinSocketGuard()
        {
            WSADATA init;
            ::WSAStartup(MAKEWORD(2, 2), &init);
        }

        ~WinSocketGuard()
        {
            ::WSACleanup();
        }
    };

    WinSocketGuard gWinSocketGuard;

    SocketHandle ToNativeHandle(void* handle)
    {
        return reinterpret_cast<SocketHandle>(handle);
    }

    void* ToGeneralHandle(SocketHandle sock)
    {
        return reinterpret_cast<void*>(sock);
    }

    SocketHandle GetInvalidSocket()
    {
        return INVALID_SOCKET;
    }

    void CloseSocket(void* handle)
    {
        ::closesocket(ToNativeHandle(handle));
    }

    bool SetSocketBlocking(void* handle, bool block)
    {
        u_long blocking = block ? 0 : 1;
        auto ret = ::ioctlsocket(ToNativeHandle(handle), static_cast<long>(FIONBIO), &blocking);
        return ret == 0;
    }

    SocketState GetErrorState()
    {
        switch (::WSAGetLastError())
        {
            // https://stackoverflow.com/questions/14546362/how-to-resolve-wsaewouldblock-error
            // WSAEWOULDBLOCK is not really an error but simply tells you that your send buffers are full.
            // This can happen if you saturate the network or if the other side simply doesn't acknowledge
            // the received data.
            case WSAEWOULDBLOCK:
            // Operation already in progress.
            // An operation was attempted on a nonblocking socket with an operation
            // already in progress—that is, calling connect a second time on a
            // nonblocking socket that is already connecting, or canceling an
            // asynchronous request(WSAAsyncGetXbyY) that has already been canceled
            // or completed.
            case WSAEALREADY:
                return SocketState::Busy;
            // Software caused connection abort.
            case WSAECONNABORTED:
            // Connection reset by peer.
            case WSAECONNRESET:
            // Connection timed out.
            case WSAETIMEDOUT:
            // Network dropped connection on reset.
            case WSAENETRESET:
            // Socket is not connected.
            case WSAENOTCONN:
                return SocketState::Disconnect;
            // Socket is already connected.
            case WSAEISCONN:
                return SocketState::Success; // when connecting a non-blocking socket
            default:
                return SocketState::Error;
        }
    }
}


#endif