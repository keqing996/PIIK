#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace Piik
{
    class String
    {
    public:
        String() = delete;

    public:
        /// \brief Convert wide string to utf8 char string. Should set local before calling this.
        static std::string WideStringToString(const std::wstring& wStr);

        /// \brief Convert utf8 char string to wide string. Should set local before calling this.
        static std::wstring StringToWideString(const std::string& str);

        template <typename Encoding>
        static std::vector<std::basic_string_view<Encoding>> SplitView(const std::basic_string<Encoding>& inputStr, Encoding delimiter)
        {
            std::vector<std::basic_string_view<Encoding>> result;
            size_t start = 0;
            size_t end = 0;

            while ((end = inputStr.find(delimiter, start)) != std::string::npos)
            {
                result.emplace_back(inputStr.data() + start, end - start);
                start = end + 1;
            }

            if (start < inputStr.size())
                result.emplace_back(inputStr.data() + start, inputStr.size() - start);

            return result;
        }

        template <typename Encoding>
        static std::vector<std::basic_string_view<Encoding>> SplitView(const std::basic_string<Encoding>& inputStr, const std::basic_string<Encoding>& delimiter)
        {
            std::vector<std::basic_string_view<Encoding>> result;
            size_t start = 0;
            size_t end = 0;

            while ((end = inputStr.find(delimiter, start)) != std::string::npos)
            {
                result.emplace_back(inputStr.data() + start, end - start);
                start = end + delimiter.length();
            }

            if (start < inputStr.size())
                result.emplace_back(inputStr.data() + start, inputStr.size() - start);

            return result;
        }

        template <typename Encoding>
        static std::vector<std::basic_string<Encoding>> Split(const std::basic_string<Encoding>& inputStr, Encoding delimiter)
        {
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = inputStr.find(delimiter);

            while (end != std::string::npos)
            {
                tokens.push_back(inputStr.substr(start, end - start));
                start = end + 1;
                end = inputStr.find(delimiter, start);
            }

            if (start < inputStr.size())
                tokens.push_back(inputStr.substr(start));

            return tokens;
        }

        template <typename Encoding>
        static std::vector<std::basic_string<Encoding>> Split(const std::basic_string<Encoding>& inputStr, const std::basic_string<Encoding>& delimiter)
        {
            std::vector<std::string> tokens;
            size_t start = 0;
            size_t end = inputStr.find(delimiter);

            while (end != std::string::npos)
            {
                tokens.push_back(inputStr.substr(start, end - start));
                start = end + delimiter.length();
                end = inputStr.find(delimiter, start);
            }

            if (start < inputStr.size())
                tokens.push_back(inputStr.substr(start));

            return tokens;
        }

        template <typename Encoding>
        static std::basic_string<Encoding> Join(const std::vector<std::basic_string<Encoding>>& strVec, const std::basic_string<Encoding>& delim)
        {
            std::basic_ostringstream<Encoding> oss;

            for (auto itr = strVec.begin(); itr != strVec.end(); ++itr)
            {
                oss << *itr;
                if (itr != strVec.end() - 1)
                    oss << delim;
            }

            return oss.str();
        }

        template <typename Encoding>
        static void Replace(std::basic_string<Encoding>& inStr, const std::basic_string<Encoding>& from, const std::basic_string<Encoding>& to)
        {
            size_t startPos = 0;
            while ((startPos = inStr.find(from, startPos)) != std::string::npos)
            {
                inStr.replace(startPos, from.length(), to);
                startPos += to.length();
            }
        }

        template <typename Encoding>
        static void TrimStart(std::basic_string<Encoding>& str)
        {
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](Encoding ch)
            {
                return !std::isspace(static_cast<int>(ch));
            }));
        }

        template <typename Encoding>
        static void TrimEnd(std::basic_string<Encoding>& str)
        {
            str.erase(std::find_if(str.rbegin(), str.rend(), [](Encoding ch)
            {
                return !std::isspace(static_cast<int>(ch));
            }).base(), str.end());
        }

        template <typename Encoding>
        static void Trim(std::basic_string<Encoding>& str)
        {
            TrimStart(str);
            TrimEnd(str);
        }
    };
}
