
#include <iostream>
#include "PIIK/Utility/Logger.h"
#include "PIIK/Utility/ScopeGuard.h"
#include "PIIK/Platform/Android/LogCat.h"
#include "PIIK/Platform/Windows/Console.h"

namespace Piik
{
    Logger::Level       Logger::_filterLevel    = Level::Info;
    Logger::LogCallBack Logger::_logInfoCallVec = LogStd;
    bool                Logger::_isThreadSafe   = true;
    std::mutex          Logger::_mutex;

    void Logger::SetFilterLevel(Level targetLevel)
    {
        _filterLevel = targetLevel;
    }

    Logger::Level Logger::GetCurrentFilterLevel()
    {
        return _filterLevel;
    }

    void Logger::SetTreadSafe(bool treadSafe)
    {
        _isThreadSafe = treadSafe;
    }

    bool Logger::GetTreadSafe()
    {
        return _isThreadSafe;
    }

    void Logger::SetLogger(const LogCallBack& pFunc)
    {
        _logInfoCallVec = pFunc;
    }

    void Logger::LogInfo(const char* message)
    {
        Log(Level::Info, message);
    }

    void Logger::LogWarn(const char* message)
    {
        Log(Level::Warning, message);
    }

    void Logger::LogError(const char* message)
    {
        Log(Level::Error, message);
    }

    void Logger::Log(Level level, const char* message)
    {
        Log(level, nullptr, message);
    }

    void Logger::LogInfo(const char* tag, const char* message)
    {
        Log(Level::Info, tag, message);
    }

    void Logger::LogWarn(const char* tag, const char* message)
    {
        Log(Level::Warning, tag, message);
    }

    void Logger::LogError(const char* tag, const char* message)
    {
        Log(Level::Error, tag, message);
    }

    void Logger::Log(Level level, const char* tag, const char* message)
    {
        if (static_cast<int>(_filterLevel) > static_cast<int>(level))
            return;

        if (_logInfoCallVec == nullptr)
            return;

        if (_isThreadSafe)
        {
            std::lock_guard guard(_mutex);
            _logInfoCallVec(level, tag, message);
        }
        else
        {
            _logInfoCallVec(level, tag, message);
        }
    }

    void Logger::LogStd(Level level, const char* tag, const char* message)
    {
#if PLATFORM_ANDROID

        Android::LogCat::LogLevel androidLogLevel = Android::LogCat::LogLevel::Info;
        switch (level)
        {
            case Level::Info:       androidLogLevel = Android::LogCat::LogLevel::Info; break;
            case Level::Warning:    androidLogLevel = Android::LogCat::LogLevel::Warn; break;
            case Level::Error:      androidLogLevel = Android::LogCat::LogLevel::Error; break;
        }

        Android::LogCat::Log(androidLogLevel, tag, message);

#else
        const char* levelStr = nullptr;
        switch (level)
        {
            case Level::Info:       levelStr = "[I] "; break;
            case Level::Warning:    levelStr = "[W] "; break;
            case Level::Error:      levelStr = "[E] "; break;
        }

#if PLATFORM_WINDOWS
        if (level == Level::Warning)
            Windows::Console::SetStdOutColor(Windows::Console::Color::Yellow, Windows::Console::Color::Black);
        else if (level == Level::Error)
            Windows::Console::SetStdOutColor(Windows::Console::Color::Red, Windows::Console::Color::Black);

        ScopeGuard guard([]()->void
        {
            Windows::Console::SetStdOutColor(Windows::Console::Color::White, Windows::Console::Color::Black);
        });
#endif

        if (tag != nullptr)
            std::cout << '[' << tag << "] " << levelStr << message;
        else
            std::cout << levelStr << message;
#endif
    }
}
