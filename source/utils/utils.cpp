/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2025 Bernardo Giordano, Admiral Fish, piepie62
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

#include "utils/utils.hpp"
#include "g1text.hpp"
#include "g2text.hpp"
#include "g3text.hpp"
#include "g4text.hpp"
#include "utils/endian.hpp"
#include <algorithm>
#include <array>
#include <map>
#include <queue>
#include <vector>

namespace
{
    char32_t swapCodepoints45(char32_t codepoint)
    {
        switch (codepoint)
        {
            case u'\u2227':
                codepoint = u'\uE0A9';
                break;
            case u'\u2228':
                codepoint = u'\uE0AA';
                break;
            case u'\u2460':
                codepoint = u'\uE081';
                break;
            case u'\u2461':
                codepoint = u'\uE082';
                break;
            case u'\u2462':
                codepoint = u'\uE083';
                break;
            case u'\u2463':
                codepoint = u'\uE084';
                break;
            case u'\u2464':
                codepoint = u'\uE085';
                break;
            case u'\u2465':
                codepoint = u'\uE086';
                break;
            case u'\u2466':
                codepoint = u'\uE087';
                break;
            case u'\u2469':
                codepoint = u'\uE068';
                break;
            case u'\u246A':
                codepoint = u'\uE069';
                break;
            case u'\u246B':
                codepoint = u'\uE0AB';
                break;
            case u'\u246C':
                codepoint = u'\uE08D';
                break;
            case u'\u246D':
                codepoint = u'\uE08E';
                break;
            case u'\u246E':
                codepoint = u'\uE08F';
                break;
            case u'\u246F':
                codepoint = u'\uE090';
                break;
            case u'\u2470':
                codepoint = u'\uE091';
                break;
            case u'\u2471':
                codepoint = u'\uE092';
                break;
            case u'\u2472':
                codepoint = u'\uE093';
                break;
            case u'\u2473':
                codepoint = u'\uE094';
                break;
            case u'\u2474':
                codepoint = u'\uE095';
                break;
            case u'\u2475':
                codepoint = u'\uE096';
                break;
            case u'\u2476':
                codepoint = u'\uE097';
                break;
            case u'\u2477':
                codepoint = u'\uE098';
                break;
            case u'\u2478':
                codepoint = u'\uE099';
                break;
            case u'\u2479':
                codepoint = u'\uE09A';
                break;
            case u'\u247A':
                codepoint = u'\uE09B';
                break;
            case u'\u247B':
                codepoint = u'\uE09C';
                break;
            case u'\u247C':
                codepoint = u'\uE09D';
                break;
            case u'\u247D':
                codepoint = u'\uE09E';
                break;
            case u'\u247E':
                codepoint = u'\uE09F';
                break;
            case u'\u247F':
                codepoint = u'\uE0A0';
                break;
            case u'\u2480':
                codepoint = u'\uE0A1';
                break;
            case u'\u2481':
                codepoint = u'\uE0A2';
                break;
            case u'\u2482':
                codepoint = u'\uE0A3';
                break;
            case u'\u2483':
                codepoint = u'\uE0A4';
                break;
            case u'\u2484':
                codepoint = u'\uE0A5';
                break;
            case u'\u2485':
                codepoint = u'\uE06A';
                break;
            case u'\u2486':
                codepoint = u'\uE0A7';
                break;
            case u'\u2487':
                codepoint = u'\uE0A8';
                break;

            case u'\uE0A9':
                codepoint = u'\u2227';
                break;
            case u'\uE0AA':
                codepoint = u'\u2228';
                break;
            case u'\uE081':
                codepoint = u'\u2460';
                break;
            case u'\uE082':
                codepoint = u'\u2461';
                break;
            case u'\uE083':
                codepoint = u'\u2462';
                break;
            case u'\uE084':
                codepoint = u'\u2463';
                break;
            case u'\uE085':
                codepoint = u'\u2464';
                break;
            case u'\uE086':
                codepoint = u'\u2465';
                break;
            case u'\uE087':
                codepoint = u'\u2466';
                break;
            case u'\uE068':
                codepoint = u'\u2469';
                break;
            case u'\uE069':
                codepoint = u'\u246A';
                break;
            case u'\uE0AB':
                codepoint = u'\u246B';
                break;
            case u'\uE08D':
                codepoint = u'\u246C';
                break;
            case u'\uE08E':
                codepoint = u'\u246D';
                break;
            case u'\uE08F':
                codepoint = u'\u246E';
                break;
            case u'\uE090':
                codepoint = u'\u246F';
                break;
            case u'\uE091':
                codepoint = u'\u2470';
                break;
            case u'\uE092':
                codepoint = u'\u2471';
                break;
            case u'\uE093':
                codepoint = u'\u2472';
                break;
            case u'\uE094':
                codepoint = u'\u2473';
                break;
            case u'\uE095':
                codepoint = u'\u2474';
                break;
            case u'\uE096':
                codepoint = u'\u2475';
                break;
            case u'\uE097':
                codepoint = u'\u2476';
                break;
            case u'\uE098':
                codepoint = u'\u2477';
                break;
            case u'\uE099':
                codepoint = u'\u2478';
                break;
            case u'\uE09A':
                codepoint = u'\u2479';
                break;
            case u'\uE09B':
                codepoint = u'\u247A';
                break;
            case u'\uE09C':
                codepoint = u'\u247B';
                break;
            case u'\uE09D':
                codepoint = u'\u247C';
                break;
            case u'\uE09E':
                codepoint = u'\u247D';
                break;
            case u'\uE09F':
                codepoint = u'\u247E';
                break;
            case u'\uE0A0':
                codepoint = u'\u247F';
                break;
            case u'\uE0A1':
                codepoint = u'\u2480';
                break;
            case u'\uE0A2':
                codepoint = u'\u2481';
                break;
            case u'\uE0A3':
                codepoint = u'\u2482';
                break;
            case u'\uE0A4':
                codepoint = u'\u2483';
                break;
            case u'\uE0A5':
                codepoint = u'\u2484';
                break;
            case u'\uE06A':
                codepoint = u'\u2485';
                break;
            case u'\uE0A7':
                codepoint = u'\u2486';
                break;
            case u'\uE0A8':
                codepoint = u'\u2487';
                break;
            default:
                break;
        }
        return codepoint;
    }

    char32_t swapCodepoints67(char32_t codepoint)
    {
        switch (codepoint)
        {
            case u'\uE088':
                codepoint = u'\u00D7';
                break;
            case u'\uE089':
                codepoint = u'\u00F7';
                break;
            case u'\uE08A':
                codepoint = u'\uE068';
                break;
            case u'\uE08B':
                codepoint = u'\uE069';
                break;
            case u'\uE08C':
                codepoint = u'\uE0AB';
                break;
            case u'\uE0A6':
                codepoint = u'\uE06A';
                break;

            case u'\u00D7':
                codepoint = u'\uE088';
                break;
            case u'\u00F7':
                codepoint = u'\uE089';
                break;
            case u'\uE068':
                codepoint = u'\uE08A';
                break;
            case u'\uE069':
                codepoint = u'\uE08B';
                break;
            case u'\uE0AB':
                codepoint = u'\uE08C';
                break;
            case u'\uE06A':
                codepoint = u'\uE0A6';
                break;
        }
        return codepoint;
    }

    // Converts a single latin character from half-width to full-width
    char16_t tofullwidth(char16_t c)
    {
        if (c == ' ')
        {
            c = u'　';
        }
        else if (c >= '!' && c <= '~')
        {
            c += 0xFEE0;
        }
        return c;
    }
}

std::u16string StringUtils::UTF8toUTF16(const std::string_view& src)
{
    std::u16string ret;
    ret.reserve(src.size());
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF8toCodepoint(src.data() + i, src.size() - i);
        auto [data, newSize]      = codepointToUTF16(codepoint);
        ret.append(data.data(), newSize);
        i += advance;
    }
    return ret;
}

// Nearly identical, just without the 21-bit case
std::u16string StringUtils::UTF8toUCS2(const std::string_view& src)
{
    std::u16string ret;
    ret.reserve(src.size());
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF8toCodepoint(src.data() + i, src.size() - i);
        ret.push_back(codepointToUCS2(codepoint));
        i += advance;
    }
    return ret;
}

std::u32string StringUtils::UTF8toUTF32(const std::string_view& src)
{
    std::u32string ret;
    ret.reserve(src.size());
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF8toCodepoint(src.data() + i, src.size() - i);
        ret.push_back(codepoint);
        i += advance;
    }
    return ret;
}

std::string StringUtils::UTF16toUTF8(const std::u16string_view& src)
{
    std::string ret;
    ret.reserve(src.size()); // Size must be greater than or equal to this
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF16toCodepoint(src.data() + i, src.size() - i);
        auto [data, newSize]      = codepointToUTF8(codepoint);
        ret.append(data.data(), newSize);
        i += advance;
    }
    return ret;
}

std::u16string StringUtils::UTF16toUCS2(const std::u16string_view& src)
{
    std::u16string ret;
    ret.reserve(src.size());
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF16toCodepoint(src.data() + i, src.size() - i);
        ret.push_back(codepointToUCS2(codepoint));
        i += advance;
    }
    return ret;
}

std::u32string StringUtils::UTF16toUTF32(const std::u16string_view& src)
{
    std::u32string ret;
    ret.reserve(src.size());
    size_t i = 0;
    while (i < src.size())
    {
        auto [codepoint, advance] = UTF16toCodepoint(src.data() + i, src.size() - i);
        ret.push_back(codepoint);
        i += advance;
    }
    return ret;
}

std::string StringUtils::UTF32toUTF8(const std::u32string_view& src)
{
    std::string ret;
    ret.reserve(src.size()); // Length *must* be greater than or equal to this.
    for (const char32_t codepoint : src)
    {
        auto [data, size] = codepointToUTF8(codepoint);
        ret.append(data.data(), size);
    }
    return ret;
}

std::u16string StringUtils::UTF32toUTF16(const std::u32string_view& src)
{
    std::u16string ret;
    ret.reserve(src.size());
    for (char32_t codepoint : src)
    {
        auto [data, size] = codepointToUTF16(codepoint);
        ret.append(data.data(), size);
    }
    return ret;
}

std::u16string StringUtils::UTF32toUCS2(const std::u32string_view& src)
{
    std::u16string ret;
    for (const char32_t& codepoint : src)
    {
        ret.push_back(codepointToUCS2(codepoint));
    }
    return ret;
}

std::pair<std::array<char, 4>, size_t> StringUtils::codepointToUTF8(char32_t codepoint)
{
    if (codepoint < 0x0000'0080)
    {
        return {
            {(char)codepoint, 0, 0, 0},
            1
        };
    }
    else if (codepoint < 0x0000'0800)
    {
        return {
            {(char)(0xC0 | ((codepoint >> 6) & 0x1F)), (char)(0x80 | (codepoint & 0x3F)), 0, 0},
            2
        };
    }
    else if (codepoint < 0x0001'0000)
    {
        return {
            {(char)(0xE0 | ((codepoint >> 12) & 0x0F)), (char)(0x80 | ((codepoint >> 6) & 0x3F)),
             (char)(0x80 | (codepoint & 0x3F)), 0},
            3
        };
    }
    // Current maximum codepoint is defined to be U+10FFFF
    else if (codepoint < 0x0011'0000)
    {
        return {
            {(char)(0xF0 | ((codepoint >> 18) & 0x07)), (char)(0x80 | ((codepoint >> 12) & 0x3F)),
             (char)(0x80 | ((codepoint >> 6) & 0x3F)), (char)(0x80 | (codepoint & 0x3F))},
            4
        };
    }
    else
    {
        return {
            {(char)(0xE0 | ((CODEPOINT_INVALID >> 12) & 0x1F)),
             (char)(0x80 | ((CODEPOINT_INVALID >> 6) & 0x3F)),
             (char)(0x80 | (CODEPOINT_INVALID & 0x3F)), 0},
            3
        };
    }
}

std::pair<std::array<char16_t, 2>, size_t> StringUtils::codepointToUTF16(char32_t codepoint)
{
    if (codepoint <= 0x0000'D7FF || (codepoint >= 0x0000'E000 && codepoint < 0x0001'0000))
    {
        return {
            {(u16)codepoint, 0},
            1
        };
    }
    // Current maximum codepoint is defined to be U+10FFFF
    else if (codepoint >= 0x0001'0000 && codepoint < 0x0011'0000)
    {
        codepoint -= 0x10000; // Make it fit in 20 bits
        return {
            {(u16)(0xD800 | ((codepoint >> 10) & 0x3FF)), (u16)(0xDC00 | (codepoint & 0x03FF))},
            2
        };
    }
    else
    {
        return {
            {CODEPOINT_INVALID, 0},
            1
        };
    }
}

u16 StringUtils::codepointToUCS2(char32_t codepoint)
{
    if (codepoint <= 0x0000'D7FF || (codepoint >= 0x0000'E000 && codepoint < 0x0001'0000))
    {
        return codepoint;
    }
    else
    {
        return CODEPOINT_INVALID;
    }
}

std::pair<char32_t, size_t> StringUtils::UTF8toCodepoint(const char* src, size_t maxSize)
{
    char32_t codepoint = CODEPOINT_INVALID;
    size_t size        = 1;
    if ((src[0] & 0xF8) == 0xF0 && 3 < maxSize && (src[1] & 0xC0) == 0x80 &&
        (src[2] & 0xC0) == 0x80 && (src[3] & 0xC0) == 0x80)
    {
        codepoint = src[0] & 0x07;
        codepoint = codepoint << 6 | (src[1] & 0x3F);
        codepoint = codepoint << 6 | (src[2] & 0x3F);
        codepoint = codepoint << 6 | (src[3] & 0x3F);
        size      = 4;
    }
    else if ((src[0] & 0xF0) == 0xE0 && 2 < maxSize && (src[1] & 0xC0) == 0x80 &&
             (src[2] & 0xC0) == 0x80)
    {
        codepoint = src[0] & 0x0F;
        codepoint = codepoint << 6 | (src[1] & 0x3F);
        codepoint = codepoint << 6 | (src[2] & 0x3F);
        size      = 3;
    }
    else if ((src[0] & 0xE0) == 0xC0 && 1 < maxSize && (src[1] & 0xC0) == 0x80)
    {
        codepoint = src[0] & 0x1F;
        codepoint = codepoint << 6 | (src[1] & 0x3F);
        size      = 2;
    }
    else if (!(src[0] & 0x80))
    {
        codepoint = src[0];
    }

    return {codepoint, size};
}

std::pair<char32_t, size_t> StringUtils::UTF16toCodepoint(const char16_t* src, size_t maxSize)
{
    char32_t codepoint = CODEPOINT_INVALID;
    size_t size        = 1;
    if (src[0] <= 0xD7FF || src[0] >= 0xE000)
    {
        codepoint = src[0];
    }
    else if (((src[0] & 0xFC00) == 0xD800) && 1 < maxSize && ((src[1] & 0xFC00) == 0xDC00))
    {
        codepoint = (char32_t(src[0] & 0x03FF) << 10) | (src[1] & 0x03FF);
        codepoint += 0x10000; // 20->21 bits
        size      = 2;
    }

    return {codepoint, size};
}

std::u32string StringUtils::getU32String(const u8* data, int ofs, int len, char16_t term)
{
    std::u32string ret;
    ret.reserve(len);
    for (int i = 0; i < len; i++)
    {
        char32_t chr = LittleEndian::convertTo<char16_t>(data + ofs + i * 2);
        if (chr == term)
        {
            return ret;
        }
        ret.push_back(chr);
    }
    return ret;
}

std::u16string StringUtils::getUCS2String(const u8* data, int ofs, int len, char16_t term)
{
    std::u16string ret;
    ret.reserve(len);
    for (int i = 0; i < len; i++)
    {
        char16_t chr = LittleEndian::convertTo<char16_t>(data + ofs + i * 2);
        if (chr == term)
        {
            return ret;
        }
        ret.push_back(chr);
    }
    return ret;
}

std::string StringUtils::getString(const u8* data, int ofs, int len, char16_t term)
{
    std::string ret;
    ret.reserve(len);
    for (int i = 0; i < len; i++)
    {
        char16_t codeunit = LittleEndian::convertTo<char16_t>(data + ofs + i * 2);
        if (codeunit == term)
        {
            break;
        }
        auto [data, size] = codepointToUTF8((char32_t)codeunit);
        ret.append(data.data(), size);
    }
    return ret;
}

void StringUtils::setString(
    u8* data, const std::u32string_view& v, int ofs, int len, char16_t terminator, char16_t padding)
{
    int i = 0;
    for (; i < std::min(len - 1, (int)v.size()); i++)
    {
        LittleEndian::convertFrom<char16_t>(data + ofs + i * 2, codepointToUCS2(v[i]));
    }
    LittleEndian::convertFrom<char16_t>(data + ofs + i++ * 2, terminator); // Set terminator
    for (; i < len; i++)
    {
        LittleEndian::convertFrom<char16_t>(data + ofs + i * 2, padding); // Set final padding bytes
    }
}

void StringUtils::setString(
    u8* data, const std::u16string_view& v, int ofs, int len, char16_t terminator, char16_t padding)
{
    size_t i   = 0;
    int outOfs = 0;
    while (outOfs < len - 1 && i < v.size())
    {
        auto [codepoint, size] = UTF16toCodepoint(v.data() + i, v.size() - i);
        LittleEndian::convertFrom<char16_t>(data + ofs + outOfs++ * 2, codepointToUCS2(codepoint));
        i += size;
    }
    LittleEndian::convertFrom<char16_t>(data + ofs + outOfs++ * 2, terminator); // Set terminator
    for (; outOfs < len; outOfs++)
    {
        LittleEndian::convertFrom<char16_t>(
            data + ofs + outOfs * 2, padding); // Set final padding bytes
    }
}

void StringUtils::setString(
    u8* data, const std::string_view& v, int ofs, int len, char16_t terminator, char16_t padding)
{
    int outOfs = 0;
    size_t i   = 0;
    while (i < v.size() && outOfs < len - 1)
    {
        auto [codepoint, size] = UTF8toCodepoint(v.data() + i, v.size() - i);
        LittleEndian::convertFrom<char16_t>(data + ofs + outOfs++ * 2, codepointToUCS2(codepoint));
        i += size;
    }
    LittleEndian::convertFrom<char16_t>(data + ofs + (outOfs++ * 2), terminator); // Set terminator
    for (; outOfs < len; outOfs++)
    {
        LittleEndian::convertFrom<char16_t>(data + ofs + (outOfs * 2),
            padding); // Set final padding bytes
    }
}

std::string StringUtils::getString4(const u8* data, int ofs, int len)
{
    std::string output;
    len *= 2;
    for (u8 i = 0; i < len; i += 2)
    {
        u16 temp = LittleEndian::convertTo<u16>(data + ofs + i);
        if (temp == 0xFFFF)
        {
            break;
        }
        auto found =
            std::find(pksm::internal::G4Values.begin(), pksm::internal::G4Values.end(), temp);
        // Treat an invalid value as a terminator
        if (found == pksm::internal::G4Values.end())
        {
            break;
        }
        u16 codepoint =
            pksm::internal::G4Chars[std::distance(pksm::internal::G4Values.begin(), found)];
        if (codepoint == 0xFFFF)
        {
            break;
        }
        auto [data, size] = codepointToUTF8((char32_t)codepoint);
        output.append(data.data(), size);
    }
    return output;
}

std::vector<u16> StringUtils::stringToG4(const std::string_view& v)
{
    std::vector<u16> ret;
    size_t charIndex = 0;
    while (charIndex < v.length())
    {
        auto [codepoint, size] = UTF8toCodepoint(v.data() + charIndex, v.length() - charIndex);

        auto found =
            std::find(pksm::internal::G4Chars.begin(), pksm::internal::G4Chars.end(), codepoint);
        ret.push_back(
            found != pksm::internal::G4Chars.end()
                ? pksm::internal::G4Values[std::distance(pksm::internal::G4Chars.begin(), found)]
                : 0x0000);

        charIndex += size;
    }
    if (ret.back() != 0xFFFF)
    {
        ret.push_back(0xFFFF);
    }
    return ret;
}

void StringUtils::setString4(u8* data, const std::string_view& v, int ofs, int len)
{
    u16 outIndex = 0, charIndex = 0;
    while (outIndex < len - 1 && charIndex < v.length())
    {
        auto [codepoint, size] = UTF8toCodepoint(v.data() + charIndex, v.length() - charIndex);

        auto found =
            std::find(pksm::internal::G4Chars.begin(), pksm::internal::G4Chars.end(), codepoint);
        LittleEndian::convertFrom<u16>(data + ofs + outIndex++ * 2,
            found != pksm::internal::G4Chars.end()
                ? pksm::internal::G4Values[std::distance(pksm::internal::G4Chars.begin(), found)]
                : 0x0000);

        charIndex += size;
    }
    LittleEndian::convertFrom<u16>(data + ofs + outIndex * 2, 0xFFFF);
}

std::string& StringUtils::toUpper(std::string& in)
{
    std::transform(in.begin(), in.end(), in.begin(), ::toupper);
    // Just saying, I have NO clue why two outer braces levels are necessary
    static constexpr std::array<std::pair<std::string_view, std::string_view>, 12> transStrings = {
        {{"í", "Í"}, {"ó", "Ó"}, {"ú", "Ú"}, {"é", "É"}, {"á", "Á"}, {"ì", "Ì"}, {"ò", "Ò"},
         {"ù", "Ù"}, {"è", "È"}, {"à", "À"}, {"ñ", "Ñ"}, {"æ", "Æ"}}
    };
    for (const auto& str : transStrings)
    {
        size_t found;
        while ((found = in.find(str.first)) != std::string::npos)
        {
            in.replace(found, str.first.size(), str.second);
        }
    }
    return in;
}

std::string StringUtils::toUpper(const std::string_view& in)
{
    std::string ret = std::string(in);
    return toUpper(ret);
}

std::string& StringUtils::toLower(std::string& in)
{
    std::transform(in.begin(), in.end(), in.begin(), ::tolower);
    // Use static const instead of static constexpr for string literals with UTF-8 characters
    static const std::array<std::pair<std::string, std::string>, 12> transStrings = {{
        {"Í", "í"}, {"Ó", "ó"}, {"Ú", "ú"}, {"É", "é"}, {"Á", "á"}, {"Ì", "ì"}, {"Ò", "ò"},
        {"Ù", "ù"}, {"È", "è"}, {"À", "à"}, {"Ñ", "ñ"}, {"Æ", "æ"}
    }};
    for (const auto& str : transStrings)
    {
        size_t found;
        while ((found = in.find(str.first)) != std::string::npos)
        {
            in.replace(found, str.first.size(), str.second);
        }
    }
    return in;
}

std::string StringUtils::toLower(const std::string_view& in)
{
    std::string ret = std::string(in);
    return toLower(ret);
}

std::u16string& StringUtils::toFullWidth(std::u16string& in)
{
    std::transform(in.begin(), in.end(), in.begin(), tofullwidth);
    return in;
}

std::string StringUtils::transString45(const std::string_view& str)
{
    return UTF16toUTF8(transString45(UTF8toUTF16(str)));
}

std::u16string StringUtils::transString45(const std::u16string_view& str)
{
    std::u16string ret = std::u16string(str);
    std::transform(str.begin(), str.end(), ret.begin(),
        [](const char16_t& chr) { return (char16_t)swapCodepoints45(chr); });
    return ret;
}

std::string StringUtils::transString67(const std::string_view& str)
{
    return UTF16toUTF8(transString67(UTF8toUTF16(str)));
}

std::u16string StringUtils::transString67(const std::u16string_view& str)
{
    std::u16string ret = std::u16string(str);
    std::transform(str.begin(), str.end(), ret.begin(),
        [](const char16_t& chr) { return (char16_t)swapCodepoints67(chr); });
    return ret;
}

std::string StringUtils::getString3(const u8* data, int ofs, int len, bool jp)
{
    auto& characters = jp ? pksm::internal::G3_JP : pksm::internal::G3_EN;
    std::u16string outString;

    for (int i = 0; i < len; i++)
    {
        if (data[ofs + i] < characters.size())
        {
            outString += characters[data[ofs + i]];
        }
        else
        {
            break;
        }
    }

    return StringUtils::UTF16toUTF8(outString);
}

void StringUtils::setString3(
    u8* data, const std::string_view& v, int ofs, int len, bool jp, int padTo, u8 padWith)
{
    auto& characters   = jp ? pksm::internal::G3_JP : pksm::internal::G3_EN;
    std::u16string str = StringUtils::UTF8toUTF16(v);
    if (jp)
    {
        str = StringUtils::toFullWidth(str);
    }

    size_t outPos;
    for (outPos = 0; outPos < std::min((size_t)len, str.size()); outPos++)
    {
        auto it = std::find(characters.begin(), characters.end(), str[outPos]);
        if (it != characters.end())
        {
            data[ofs + outPos] = (u8)std::distance(characters.begin(), it);
        }
        else
        {
            break;
        }
    }

    if (outPos < (size_t)len)
    {
        data[ofs + outPos] = 0xFF;
    }

    while (outPos < (size_t)padTo)
    {
        data[ofs + outPos] = padWith;
        outPos++;
    }
}

// The only guessable languages are German, Japanese, and Korean, and German is guessable only if an
// umlaut is found. Japanese and Korean should already be filtered out by this point, so this is
// essentially "German or not". "not" is returned as English.
pksm::Language StringUtils::guessLanguage12(const std::string_view& v)
{
    std::u16string str = StringUtils::UTF8toUTF16(v);
    for (u32 i = 0; i < str.size(); i++)
    {
        switch (str[i])
        {
            case u'Ä':
            case u'Ö':
            case u'Ü':
            case u'ä':
            case u'ö':
            case u'ü':
                return pksm::Language::GER;
        }
    }
    return pksm::Language::ENG;
}

std::string StringUtils::fixJapaneseNameTransporter(const std::string_view& v)
{
    std::u16string str = StringUtils::UTF8toUTF16(v);
    std::u16string outStr;
    for (u32 i = 0; i < str.size(); i++)
    {
        switch (str[i])
        {
            case u'ベ':
                outStr += u'べ';
                break;
            case u'ペ':
                outStr += u'ぺ';
                break;
            case u'ヘ':
                outStr += u'へ';
                break;
            case u'リ':
                outStr += u'り';
                break;
            default:
                outStr += str[i];
        }
    }
    return StringUtils::UTF16toUTF8(outStr);
}

std::string StringUtils::getTradeOT(pksm::Language lang)
{
    switch (lang)
    {
        case pksm::Language::JPN:
            return "トレーナー";
        case pksm::Language::ENG:
            return "Trainer";
        case pksm::Language::FRE:
            return "Dresseur";
        case pksm::Language::ITA:
            return "Allenatore";
        case pksm::Language::GER:
            return "Trainer";
        case pksm::Language::SPA:
            return "Entrenador";
        case pksm::Language::KOR:
            return "트레이너";
        default:
            return "";
    }
}

void StringUtils::gbStringFailsafe(u8* data, int ofs, int len)
{
    if (data[ofs + len - 1] != 0x50)
    {
        if (std::find(&data[ofs], &data[ofs + len], 0x50) == &data[ofs + len])
        {
            data[ofs + len - 1] = 0x50;
        }
        else if (data[ofs + len - 1] != 0)
        {
            data[ofs + len - 1] = 0;
        }
    }
}

std::string StringUtils::getString1(
    const u8* data, int ofs, int len, pksm::Language lang, bool transporter)
{
    if (data[ofs] == 0x5D)
    {
        return transporter ? getTradeOT(lang) : StringUtils::toUpper(getTradeOT(lang));
    }

    std::string ret;
    ret.reserve(len);

    if (lang == pksm::Language::JPN)
    {
        for (int i = 0; i < len; i++)
        {
            auto found = std::find(
                pksm::internal::G1JPVals.begin(), pksm::internal::G1JPVals.end(), data[i + ofs]);
            if (found == pksm::internal::G1JPVals.end())
            {
                break; // treat invalid character as terminator
            }
            char16_t codepoint =
                pksm::internal::G1JPChars[std::distance(pksm::internal::G1JPVals.begin(), found)];
            if (codepoint == u'\0')
            {
                break;
            }
            if (codepoint == pksm::internal::tradeOTChar)
            {
                continue;
            }
            if (transporter)
            {
                switch (codepoint)
                {
                    case u'?':
                    case u'!':
                        codepoint = u' ';
                }
            }

            auto [data, size] = codepointToUTF8(codepoint);

            ret.append(data.data(), size);
        }
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            auto found = std::find(
                pksm::internal::G1ENVals.begin(), pksm::internal::G1ENVals.end(), data[i + ofs]);
            if (found == pksm::internal::G1ENVals.end())
            {
                break; // treat invalid character as terminator
            }
            char16_t codepoint =
                pksm::internal::G1ENChars[std::distance(pksm::internal::G1ENVals.begin(), found)];
            if (codepoint == u'\0')
            {
                break;
            }
            if (codepoint == pksm::internal::tradeOTChar)
            {
                continue;
            }
            if (transporter)
            {
                switch (codepoint)
                {
                    case u'[':
                        codepoint = u'(';
                        break;
                    case u']':
                        codepoint = u')';
                        break;
                    case u'×':
                        codepoint = u'x';
                }
            }

            auto [data, size] = codepointToUTF8(codepoint);

            ret.append(data.data(), size);
        }
    }

    return ret;
}

void StringUtils::setString1(u8* data, const std::string_view& v, int ofs, int len,
    pksm::Language lang, int padTo, u8 padWith)
{
    int outPos   = 0;
    size_t inPos = 0;

    if (lang == pksm::Language::JPN)
    {
        while (outPos < len && inPos < v.size())
        {
            auto [codepoint, advance] = UTF8toCodepoint(v.data() + inPos, v.size() - inPos);

            codepoint = tofullwidth(codepointToUCS2(codepoint));

            auto found = std::find(
                pksm::internal::G1JPChars.begin(), pksm::internal::G1JPChars.end(), codepoint);
            if (found == pksm::internal::G1JPChars.end())
            {
                break;
            }
            data[ofs + outPos++] =
                pksm::internal::G1JPVals[std::distance(pksm::internal::G1JPChars.begin(), found)];

            inPos += advance;
        }
    }
    else
    {
        while (outPos < len && inPos < v.size())
        {
            auto [codepoint, advance] = UTF8toCodepoint(v.data() + inPos, v.size() - inPos);

            auto found = std::find(pksm::internal::G1ENChars.begin(),
                pksm::internal::G1ENChars.end(), codepointToUCS2(codepoint));
            if (found == pksm::internal::G1ENChars.end())
            {
                break;
            }
            data[ofs + outPos++] =
                pksm::internal::G1ENVals[std::distance(pksm::internal::G1ENChars.begin(), found)];

            inPos += advance;
        }
    }

    if (outPos < len)
    {
        data[ofs + outPos] = 0x50;
        outPos++;
    }

    while (outPos < padTo)
    {
        data[ofs + outPos] = padWith;
        outPos++;
    }

    StringUtils::gbStringFailsafe(data, ofs, len);
}

std::string StringUtils::getString2(
    const u8* data, int ofs, int len, pksm::Language lang, bool transporter)
{
    // every language other than KOR is the same as last gen, and as well the case of the trade ot
    // char is handled there
    if (lang != pksm::Language::KOR || data[ofs] == 0x5D)
    {
        return getString1(data, ofs, len, lang, transporter);
    }

    std::string ret;
    ret.reserve(len);
    int inPos = 0;

    while (inPos < len)
    {
        if (data[ofs + inPos] <= 0xB)
        {
            auto found = std::find(pksm::internal::G2KORVals.begin(),
                pksm::internal::G2KORVals.end(), BigEndian::convertTo<u16>(data + ofs + inPos));
            if (found == pksm::internal::G2KORVals.end())
            {
                break; // treat invalid value as terminator
            }

            inPos += 2;

            auto [data, size] = codepointToUTF8(pksm::internal::G2KORChars[std::distance(
                pksm::internal::G2KORVals.begin(), found)]);

            ret.append(data.data(), size);
        }
        else
        {
            auto found = std::find(pksm::internal::G1ENVals.begin(), pksm::internal::G1ENVals.end(),
                data[ofs + inPos]);
            if (found == pksm::internal::G1ENVals.end())
            {
                break; // treat invalid value as terminator
            }

            inPos++;

            char16_t codepoint =
                pksm::internal::G1ENChars[std::distance(pksm::internal::G1ENVals.begin(), found)];

            if (codepoint == u'\0')
            {
                break;
            }
            else if (codepoint == pksm::internal::tradeOTChar)
            {
                continue;
            }

            auto [data, size] = codepointToUTF8(codepoint);

            ret.append(data.data(), size);
        }
    }

    return ret;
}

void StringUtils::setString2(u8* data, const std::string_view& v, int ofs, int len,
    pksm::Language lang, int padTo, u8 padWith)
{
    if (lang != pksm::Language::KOR)
    {
        setString1(data, v, ofs, len, lang, padTo, padWith);
        return;
    }

    size_t inPos = 0;
    int outPos   = 0;

    while (outPos < len && inPos < v.size())
    {
        auto [codepoint, advance] = UTF8toCodepoint(v.data() + inPos, v.size() - inPos);

        codepoint = tofullwidth(codepointToUCS2(codepoint));

        auto found = std::find(
            pksm::internal::G2KORChars.begin(), pksm::internal::G2KORChars.end(), codepoint);
        if (found == pksm::internal::G2KORChars.end())
        {
            auto found = std::find(
                pksm::internal::G1ENChars.begin(), pksm::internal::G1ENChars.end(), codepoint);
            if (found == pksm::internal::G1ENChars.end())
            {
                break; // treat invalid character as terminator
            }

            data[ofs + outPos++] =
                pksm::internal::G1ENVals[std::distance(pksm::internal::G1ENChars.begin(), found)];
        }
        else
        {
            u16 val =
                pksm::internal::G2KORVals[std::distance(pksm::internal::G2KORChars.begin(), found)];
            BigEndian::convertFrom<u16>(data + outPos, val);
            outPos += 2; // needs to be incremented twice, not just once
        }

        inPos += advance;
    }

    if (outPos < len)
    {
        data[ofs + outPos] = 0x50;
        outPos++;
    }

    while (outPos < padTo)
    {
        data[ofs + outPos] = padWith;
        outPos++;
    }

    StringUtils::gbStringFailsafe(data, ofs, len);
}
