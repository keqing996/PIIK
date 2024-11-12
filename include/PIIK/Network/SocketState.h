#pragma once

namespace Piik
{
    enum class SocketState
    {
        Success,
        RoleNotMatch,
        InvalidSocket,
        Busy,
        Disconnect,
        AddressFamilyNotMatch,
        Error
    };
}