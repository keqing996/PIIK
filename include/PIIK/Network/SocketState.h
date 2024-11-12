#pragma once

namespace Piik
{
    enum class SocketState
    {
        Success,
        RoleNotMatch,
        Busy,
        Disconnect,
        Error
    };
}