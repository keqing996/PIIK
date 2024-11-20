
#include "iostream"
#include "PIIK/Utility/String.h"
#include "PIIK/PlatformDefine.h"

#if PLATFORM_WINDOWS
#include "PIIK/Platform/Windows/Console.h"
#endif

int main()
{
#if PLATFORM_WINDOWS
    Piik::Windows::Console::SetConsoleOutputUtf8();

    // Change wcout locale to zh_CN
    std::wcout.imbue(std::locale("zh_CN"));
#endif

    // wstring & string
    std::cout << "Test string & wstring" << std::endl;
    {
        std::string str = "测试";
        std::cout << str << std::endl;
        std::wstring wstr = Piik::String::StringToWideString(str);
        std::wcout << wstr << std::endl;
        std::string str2 = Piik::String::WideStringToString(wstr);
        std::cout << str2 << std::endl;
    }
    std::cout << std::endl;

    // split
    std::cout << "Test split" << std::endl;
    {
        // char
        {
            std::string str = "abc,def,sdf,zxc,vfg,";
            auto splitViews = Piik::String::SplitView(str, ',');
            for (auto view: splitViews)
            {
                std::cout << '[' << view << "] ";
            }
            std::cout << std::endl;
            auto split = Piik::String::Split(str, ',');
            for (auto& s: split)
            {
                std::cout << '[' << s << "] ";
            }
            std::cout << std::endl;
        }
        // str
        {
            std::string str = "abc,,def,,sdf,,zxc,,vfg,,";
            auto splitViews = Piik::String::SplitView(str, std::string(",,"));
            for (auto view: splitViews)
            {
                std::cout << '[' << view << "] ";
            }
            std::cout << std::endl;
            auto split = Piik::String::Split(str, std::string(",,"));
            for (auto& s: split)
            {
                std::cout << '[' << s << "] ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}