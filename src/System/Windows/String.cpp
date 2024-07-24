#include "Infra/PlatformDefine.h"

#if PLATFORM_WINDOWS

#include "Infra/String.h"
#include "Infra/Windows/WindowsDefine.h"

namespace Infra
{
    std::string String::WideStringToString(const std::wstring& wStr)
    {
        const wchar_t* wideStr = wStr.c_str();
        if (wideStr == nullptr)
            return std::string{};

        int requiredSize = ::WideCharToMultiByte(CP_ACP, 0, wideStr,
                                                 wStr.size(), nullptr, 0,
                                                 nullptr, nullptr);

        std::vector<char> charBuffer(requiredSize + 1, 0);
        int size = ::WideCharToMultiByte(CP_ACP, 0, wideStr,
                                         wStr.size(), charBuffer.data(), requiredSize,
                                         nullptr, nullptr);
        if (size < 0)
            return std::string{};

        return std::string(charBuffer.data());
    }

    std::wstring String::StringToWideString(const std::string& str)
    {
        const char* multiBytesStr = str.c_str();
        if (multiBytesStr == nullptr)
            return std::wstring{};

        int requiredSize = ::MultiByteToWideChar(CP_ACP, 0, multiBytesStr,
                                                 str.size(), nullptr, 0);

        std::vector<wchar_t> wideCharBuffer(requiredSize + 1, 0);
        int size = ::MultiByteToWideChar(CP_ACP, 0, multiBytesStr,
                                         str.size(), wideCharBuffer.data(), requiredSize);
        if (size < 0)
            return std::wstring{};

        return std::wstring(wideCharBuffer.data());
    }
}

#endif
