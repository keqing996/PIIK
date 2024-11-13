#pragma once

namespace Piik
{
    enum class SocketState
    {
        Success,
        InvalidSocket,
        Busy,
        Disconnect,
        AddressFamilyNotMatch,
        Error
    };
}