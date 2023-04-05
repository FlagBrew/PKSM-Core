/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "enums/Language.hpp"
#include "utils/coretypes.h"
#include <array>
#include <codecvt>
#include <concepts>
#include <locale>
#include <memory>
#include <optional>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <type_traits>
#include <vector>

namespace StringUtils
{
    inline constexpr char32_t CODEPOINT_INVALID = 0xFFFD;
    // Standard UTF-8/16/32 conversions
    [[nodiscard]] std::u16string UTF8toUTF16(const std::string_view& src);
    [[nodiscard]] std::string UTF16toUTF8(const std::u16string_view& src);

    [[nodiscard]] std::u32string UTF8toUTF32(const std::string_view& src);
    [[nodiscard]] std::string UTF32toUTF8(const std::u32string_view& src);

    [[nodiscard]] std::u32string UTF16toUTF32(const std::u16string_view& src);
    [[nodiscard]] std::u16string UTF32toUTF16(const std::u32string_view& src);

    // UCS-2 is UTF-16 without the extended codepage(s). This is the format used in recent Pokémon
    // games. It can be seen as the valid UTF-16 codepoints between 0x0000 and 0xFFFF, inclusive.
    // Note that passing a UTF-8 string that contains only codepoints in that region to UTF8toUTF16
    // will also result in a UCS-2 string.
    [[nodiscard]] std::u16string UTF8toUCS2(const std::string_view& src);
    [[nodiscard]] std::u16string UTF16toUCS2(const std::u16string_view& src);
    [[nodiscard]] std::u16string UTF32toUCS2(const std::u32string_view& src);

    [[nodiscard]] inline std::string UCS2toUTF8(const std::u16string_view& src)
    {
        return UTF16toUTF8(src);
    }

    [[nodiscard]] inline std::u16string UCS2toUTF16(const std::u16string_view& src)
    {
        return std::u16string(src);
    }

    [[nodiscard]] inline std::u32string UCS2toUTF32(const std::u16string_view& src)
    {
        return UTF16toUTF32(src);
    }

    [[nodiscard]] std::pair<std::array<char, 4>, size_t> codepointToUTF8(char32_t codepoint);
    [[nodiscard]] std::pair<std::array<char16_t, 2>, size_t> codepointToUTF16(char32_t codepoint);
    [[nodiscard]] u16 codepointToUCS2(char32_t codepoint);

    [[nodiscard]] std::pair<char32_t, size_t> UTF8toCodepoint(const char* data, size_t maxSize);
    [[nodiscard]] std::pair<char32_t, size_t> UTF16toCodepoint(
        const char16_t* data, size_t maxSize);

    [[nodiscard]] inline std::pair<char32_t, size_t> UCS2toCodepoint(const char16_t* data)
    {
        return UTF16toCodepoint(data, 1);
    }

    // All of these take a pointer to a buffer with a UCS-2 little-endian char16_t array at data +
    // ofs, terminated by term, and turn them into the format indicated by the method name.
    [[nodiscard]] std::u32string getU32String(
        const u8* data, int ofs, int len, char16_t term = u'\0');
    [[nodiscard]] std::u16string getUCS2String(
        const u8* data, int ofs, int len, char16_t term = u'\0');

    [[nodiscard]] inline std::u16string getU16String(
        const u8* data, int ofs, int len, char16_t term = u'\0')
    {
        return getUCS2String(data, ofs, len, term);
    }

    [[nodiscard]] std::string getString(const u8* data, int ofs, int len, char16_t term = u'\0');

    // All of these take a pointer to a buffer with a UCS-2 char16_t array at data + ofs and write
    // the given string to them, replacing unrepresentable codepoints with 0xFFFD, and using
    // terminator as the terminator and padding as the characters to set after the terminator.
    // Note that any codepoint >= 0x10000 will be converted to 0xFFFD
    // Note that the u16string_view overload takes a UTF-16 string
    void setString(u8* data, const std::u32string_view& v, int ofs, int len,
        char16_t terminator = u'\0', char16_t padding = u'\0');
    void setString(u8* data, const std::u16string_view& v, int ofs, int len,
        char16_t terminator = u'\0', char16_t padding = u'\0');
    void setString(u8* data, const std::string_view& v, int ofs, int len,
        char16_t terminator = u'\0', char16_t padding = u'\0');

    [[nodiscard]] std::string getString4(const u8* data, int ofs, int len);
    void setString4(u8* data, const std::string_view& v, int ofs, int len);
    [[nodiscard]] std::string getString3(const u8* data, int ofs, int len, bool jp);
    void setString3(u8* data, const std::string_view& v, int ofs, int len, bool jp, int padTo = 0,
        u8 padWith = 0xFF);
    [[nodiscard]] std::string getString1(
        const u8* data, int ofs, int len, pksm::Language lang, bool transporter = false);
    void setString1(u8* data, const std::string_view& v, int ofs, int len, pksm::Language lang,
        int padTo = 0, u8 padWith = 0x50);
    [[nodiscard]] std::string getString2(
        const u8* data, int ofs, int len, pksm::Language lang, bool transporter = false);
    void setString2(u8* data, const std::string_view& v, int ofs, int len, pksm::Language lang,
        int padTo = 0, u8 padWith = 0x50);

    void gbStringFailsafe(u8* data, int ofs, int len);

    std::string getTradeOT(pksm::Language lang);

    pksm::Language guessLanguage12(const std::string_view& v);
    std::string fixJapaneseNameTransporter(const std::string_view& v);

    [[nodiscard]] std::vector<u16> stringToG4(const std::string_view& v);
    std::string& toLower(std::string& in);
    [[nodiscard]] std::string toLower(const std::string_view& in);
    std::string& toUpper(std::string& in);
    [[nodiscard]] std::string toUpper(const std::string_view& in);
    std::u16string& toFullWidth(std::u16string& in);
    /** Swaps and reswaps Pokemon specific characters, namely:
     * 0x2227, 0x2228, 0x2460, 0x2461, 0x2462,
     * 0x2463, 0x2464, 0x2465, 0x2466, 0x2469,
     * 0x246A, 0x246B, 0x246C, 0x246D, 0x246E,
     * 0x246F, 0x2470, 0x2471, 0x2472, 0x2473,
     * 0x2474, 0x2475, 0x2476, 0x2477, 0x2478,
     * 0x2479, 0x247A, 0x247B, 0x247C, 0x247D,
     * 0x247E, 0x247F, 0x2480, 0x2481, 0x2482,
     * 0x2483, 0x2484, 0x2485, 0x2486, and 0x2487
     */
    [[nodiscard]] std::string transString45(const std::string_view& str);
    [[nodiscard]] std::u16string transString45(const std::u16string_view& str);
    /** Swaps and reswaps Pokemon specific characters, namely:
     * 0xE088, 0xE089, 0xE08A, 0xE08B, 0xE08C, 0xE0A6
     */
    [[nodiscard]] std::string transString67(const std::string_view& str);
    [[nodiscard]] std::u16string transString67(const std::u16string_view& str);

    namespace internal
    {
        template <typename RetType, typename First, typename... Params>
            requires (std::same_as<std::string, RetType> || std::same_as<std::u16string, RetType> ||
                         std::same_as<std::u32string, RetType>) &&
                     (std::convertible_to<First, std::string_view> ||
                         std::convertible_to<First, std::u16string_view> ||
                         std::convertible_to<First, std::u32string_view>) &&
                     ((std::convertible_to<Params, std::string_view> ||
                          std::convertible_to<Params, std::u16string_view> ||
                          std::convertible_to<Params, std::u32string_view>) &&
                         ...)
        void concatImpl(RetType& ret, First&& f, Params&&... rest)
        {
            static constexpr auto toGenericView = [](First&& x)
                -> std::conditional_t<std::is_convertible_v<First, std::string_view>,
                    std::string_view,
                    std::conditional_t<std::is_convertible_v<First, std::u16string_view>,
                        std::u16string_view, std::u32string_view>> { return x; };

            if constexpr (std::is_same_v<RetType, std::string> &&
                          std::is_convertible_v<First, std::string_view>)
            {
                std::string_view view{std::forward<decltype(f)>(f)};
                ret.append(view.data(), view.size());
            }
            else if constexpr (std::is_same_v<RetType, std::u16string> &&
                               std::is_convertible_v<First, std::u16string_view>)
            {
                std::u16string_view view{std::forward<decltype(f)>(f)};
                ret.append(view.data(), view.size());
            }
            else if constexpr (std::is_same_v<RetType, std::u32string> &&
                               std::is_convertible_v<First, std::u32string_view>)
            {
                std::u32string_view view{std::forward<decltype(f)>(f)};
                ret.append(view.data(), view.size());
            }
            else
            {
                auto view = toGenericView(std::forward<decltype(f)>(f));
                size_t i  = 0;
                while (i < view.size())
                {
                    size_t iMod = 1;
                    char32_t codepoint;
                    if constexpr (std::is_same_v<decltype(view), std::string_view>)
                    {
                        std::tie(codepoint, iMod) =
                            UTF8toCodepoint(view.data() + i, view.size() - i);
                    }
                    else if constexpr (std::is_same_v<decltype(view), std::u16string_view>)
                    {
                        std::tie(codepoint, iMod) =
                            UTF16toCodepoint(view.data() + i, view.size() - i);
                    }
                    else if constexpr (std::is_same_v<decltype(view), std::u32string_view>)
                    {
                        codepoint = view[i];
                    }

                    if constexpr (std::is_same_v<RetType, std::string>)
                    {
                        auto [data, size] = codepointToUTF8(codepoint);
                        ret.append(data.data(), size);
                    }
                    else if constexpr (std::is_same_v<RetType, std::u16string>)
                    {
                        auto [data, size] = codepointToUTF16(codepoint);
                        ret.append(data.data(), size);
                    }
                    else if constexpr (std::is_same_v<RetType, std::u32string>)
                    {
                        ret += codepoint;
                    }

                    i += iMod;
                }
            }

            if constexpr (sizeof...(rest) != 0)
            {
                concatImpl(ret, std::forward<decltype(rest)>(rest)...);
            }
        }

        template <typename StringType>
            requires std::same_as<std::string, StringType> ||
                     std::same_as<std::u16string, StringType> ||
                     std::same_as<std::u32string, StringType>
        size_t concatMaxSizeInCodeUnits()
        {
            return 0;
        }

        template <typename StringType, typename First, typename... Params>
            requires (std::same_as<std::string, StringType> ||
                         std::same_as<std::u16string, StringType> ||
                         std::same_as<std::u32string, StringType>) &&
                     (std::convertible_to<First, std::string_view> ||
                         std::convertible_to<First, std::u16string_view> ||
                         std::convertible_to<First, std::u32string_view>) &&
                     ((std::convertible_to<Params, std::string_view> ||
                          std::convertible_to<Params, std::u16string_view> ||
                          std::convertible_to<Params, std::u32string_view>) &&
                         ...)
        size_t concatMaxSizeInCodeUnits(First&& f, Params&&... args)
        {
            if constexpr (std::is_same_v<StringType, std::string>)
            {
                if constexpr (std::is_convertible_v<First, std::string_view>)
                {
                    return toView(std::forward<decltype(f)>(f)).size() +
                           concatMaxSizeInCodeUnits<StringType>(
                               std::forward<decltype(args)>(args)...);
                }
                else if constexpr (std::is_convertible_v<First, std::u16string_view>)
                {
                    return toView(std::forward<decltype(f)>(f)).size() * 2 +
                           concatMaxSizeInCodeUnits<StringType>(
                               std::forward<decltype(args)>(args)...);
                }
                else if constexpr (std::is_convertible_v<First, std::u32string_view>)
                {
                    return toView(std::forward<decltype(f)>(f)).size() * 4 +
                           concatMaxSizeInCodeUnits<StringType>(
                               std::forward<decltype(args)>(args)...);
                }
            }
            else if constexpr (std::is_same_v<StringType, std::u16string>)
            {
                if constexpr (std::is_convertible_v<First, std::u32string_view>)
                {
                    return toView(std::forward<decltype(f)>(f)).size() * 2 +
                           concatMaxSizeInCodeUnits<StringType>(
                               std::forward<decltype(args)>(args)...);
                }
                return toView(std::forward<decltype(f)>(f)).size() +
                       concatMaxSizeInCodeUnits<StringType>(std::forward<decltype(args)>(args)...);
            }
            else if constexpr (std::is_same_v<StringType, std::u32string>)
            {
                return toView(std::forward<decltype(f)>(f)).size() +
                       concatMaxSizeInCodeUnits<StringType>(std::forward<decltype(args)>(args)...);
            }
        }
    }

    template <typename EncString, typename OrigString>
        requires (std::same_as<std::string, EncString> || std::same_as<std::u16string, EncString> ||
                     std::same_as<std::u32string, EncString>) &&
                 (std::convertible_to<OrigString, std::string_view> ||
                     std::convertible_to<OrigString, std::u16string_view> ||
                     std::convertible_to<OrigString, std::u32string_view>)
    EncString convert(OrigString&& str)
    {
        if constexpr (std::is_same_v<EncString, std::string>)
        {
            if constexpr (std::is_convertible_v<OrigString, std::string_view>)
            {
                return EncString(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u16string_view>)
            {
                return UTF16toUTF8(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u32string_view>)
            {
                return UTF32toUTF8(str);
            }
        }
        else if constexpr (std::is_same_v<EncString, std::u16string>)
        {
            if constexpr (std::is_convertible_v<OrigString, std::string_view>)
            {
                return UTF8toUTF16(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u16string_view>)
            {
                return EncString(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u32string_view>)
            {
                return UTF32toUTF16(str);
            }
        }
        else if constexpr (std::is_same_v<EncString, std::u32string>)
        {
            if constexpr (std::is_convertible_v<OrigString, std::string_view>)
            {
                return UTF8toUTF32(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u16string_view>)
            {
                return UTF16toUTF32(str);
            }
            else if constexpr (std::is_convertible_v<OrigString, std::u32string_view>)
            {
                return EncString(str);
            }
        }
    }

    template <typename RetType, bool ShrinkAfter = false, typename... Params>
        requires (std::same_as<RetType, std::string> || std::same_as<RetType, std::u16string> ||
                     std::same_as<RetType, std::u32string>) &&
                 ((std::convertible_to<Params, std::string_view> ||
                      std::convertible_to<Params, std::u16string_view> ||
                      std::convertible_to<Params, std::u32string_view>) &&
                     ...)
    RetType concat(Params&&... rest)
    {
        if constexpr (sizeof...(rest) == 0)
        {
            return RetType();
        }

        RetType ret;

        ret.reserve(
            internal::concatMaxSizeInCodeUnits<RetType>(std::forward<decltype(rest)>(rest)...));

        internal::concatImpl(ret, rest...);

        if constexpr (ShrinkAfter)
        {
            ret.shrink_to_fit();
        }

        return ret;
    }
}

#endif
