/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2020 Bernardo Giordano, Admiral Fish, piepie62
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

#ifndef GENERATION_HPP
#define GENERATION_HPP

#include "utils/coretypes.h"
#include "utils/endian.hpp"
#include <string>

class Generation_impl
{
    friend class Generation;

private:
    // values MUST remain consistent
    enum class Gen : u32
    {
        THREE  = 6,
        FOUR   = 0,
        FIVE   = 1,
        SIX    = 2,
        SEVEN  = 3,
        LGPE   = 4,
        EIGHT  = 5,
        UNUSED = 0xFFFFFFFF
    } v;

    constexpr explicit Generation_impl(Gen v) : v(v) {}
    constexpr Generation_impl(const Generation_impl&) = default;
    constexpr Generation_impl(Generation_impl&&)      = default;
    constexpr Generation_impl& operator=(const Generation_impl&) = default;
    constexpr Generation_impl& operator=(Generation_impl&&) = default;

public:
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        static_assert(std::is_integral_v<T> && sizeof(T) >= sizeof(Gen));
        return T(v);
    }
    constexpr explicit operator const char*() const noexcept
    {
        switch (v)
        {
            case Gen::LGPE:
                return "LGPE";
            case Gen::THREE:
                return "3";
            case Gen::FOUR:
                return "4";
            case Gen::FIVE:
                return "5";
            case Gen::SIX:
                return "6";
            case Gen::SEVEN:
                return "7";
            case Gen::EIGHT:
                return "8";
            case Gen::UNUSED:
                return "INVALID";
        }
        return "INVALID";
    }
    constexpr explicit operator std::string_view() const { return std::string_view{(const char*)*this}; }
    explicit operator std::string() const { return std::string{(const char*)*this}; }
    constexpr operator Gen() const noexcept { return v; }

    constexpr bool operator<(const Generation_impl& other) const noexcept
    {
        switch (v)
        {
            case Gen::THREE:
                if (other.v == Gen::FOUR)
                {
                    return true;
                }
                // falls through
            case Gen::FOUR:
                if (other.v == Gen::FIVE)
                {
                    return true;
                }
                // falls through
            case Gen::FIVE:
                if (other.v == Gen::SIX)
                {
                    return true;
                }
                // falls through
            case Gen::SIX:
                if (other.v == Gen::SEVEN)
                {
                    return true;
                }
                // falls through
            case Gen::SEVEN:
                if (other.v == Gen::LGPE)
                {
                    return true;
                }
                // falls through
            case Gen::LGPE:
                if (other.v == Gen::EIGHT)
                {
                    return true;
                }
                // falls through
            case Gen::EIGHT:
                if (other.v == Gen::UNUSED)
                {
                    return true;
                }
                // falls through
            case Gen::UNUSED:
            default:
                return false;
        }
    }
    constexpr bool operator>(const Generation_impl& other) const noexcept
    {
        if (*this == other)
        {
            return false;
        }
        return !(*this < other);
    }

    constexpr bool operator<=(const Generation_impl& other) const noexcept { return *this < other || *this == other; }
    constexpr bool operator>=(const Generation_impl& other) const noexcept { return v >= other.v; }

    constexpr bool operator==(const Generation_impl& other) const noexcept { return v == other.v; }
    constexpr bool operator!=(const Generation_impl& other) const noexcept { return v != other.v; }
};

class Generation
{
private:
    Generation_impl impl;

public:
    using EnumType = Generation_impl::Gen;
    constexpr Generation() noexcept : impl(Generation_impl::Gen::UNUSED) {}
    constexpr Generation(const Generation_impl& impl) noexcept : impl(impl) {}
    constexpr explicit Generation(u32 v) noexcept : impl(Generation_impl::Gen{v}) {}
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        return T(impl);
    }
    constexpr operator Generation_impl::Gen() const noexcept { return (Generation_impl::Gen)impl; }

    constexpr explicit operator const char*() const noexcept { return (const char*)impl; }
    constexpr explicit operator std::string_view() const { return (std::string_view)impl; }
    explicit operator std::string() const { return (std::string)impl; }

    static constexpr Generation fromString(const std::string_view& str)
    {
        if (str == "3")
        {
            return Generation::THREE;
        }
        else if (str == "4")
        {
            return Generation::FOUR;
        }
        else if (str == "5")
        {
            return Generation::FIVE;
        }
        else if (str == "6")
        {
            return Generation::SIX;
        }
        else if (str == "7")
        {
            return Generation::SEVEN;
        }
        else if (str == "LGPE")
        {
            return Generation::LGPE;
        }
        else if (str == "8")
        {
            return Generation::EIGHT;
        }
        return Generation::UNUSED;
    }

    constexpr bool operator<(const Generation& other) const noexcept { return impl < other.impl; }
    constexpr bool operator<=(const Generation& other) const noexcept { return impl <= other.impl; }

    constexpr bool operator>(const Generation& other) const noexcept { return impl > other.impl; }
    constexpr bool operator>=(const Generation& other) const noexcept { return impl >= other.impl; }

    constexpr bool operator==(const Generation& other) const noexcept { return impl == other.impl; }
    constexpr bool operator!=(const Generation& other) const noexcept { return impl != other.impl; }

    constexpr bool operator<(const Generation_impl& other) const noexcept { return impl < other; }
    constexpr bool operator<=(const Generation_impl& other) const noexcept { return impl <= other; }

    constexpr bool operator>(const Generation_impl& other) const noexcept { return impl > other; }
    constexpr bool operator>=(const Generation_impl& other) const noexcept { return impl >= other; }

    constexpr bool operator==(const Generation_impl& other) const noexcept { return impl == other; }
    constexpr bool operator!=(const Generation_impl& other) const noexcept { return impl != other; }

    static constexpr Generation_impl THREE{Generation_impl::Gen::THREE};
    static constexpr Generation_impl FOUR{Generation_impl::Gen::FOUR};
    static constexpr Generation_impl FIVE{Generation_impl::Gen::FIVE};
    static constexpr Generation_impl SIX{Generation_impl::Gen::SIX};
    static constexpr Generation_impl SEVEN{Generation_impl::Gen::SEVEN};
    static constexpr Generation_impl LGPE{Generation_impl::Gen::LGPE};
    static constexpr Generation_impl EIGHT{Generation_impl::Gen::EIGHT};
    static constexpr Generation_impl UNUSED{Generation_impl::Gen::UNUSED};
};

template <>
#if __cplusplus > 201703L
constexpr Generation LittleEndian::convertTo<Generation>(const u8* data)
#else
Generation LittleEndian::convertTo<Generation>(const u8* data)
#endif
{
    return Generation{LittleEndian::convertTo<u32>(data)};
}

template <>
#if __cplusplus > 201703L
constexpr void LittleEndian::convertFrom<Generation>(u8* data, const Generation& gen)
#else
void LittleEndian::convertFrom<Generation>(const u8* data, const Generation& gen)
#endif
{
    LittleEndian::convertFrom<u32>(data, (u32)gen);
}

template <>
#if __cplusplus > 201703L
constexpr Generation BigEndian::convertTo<Generation>(const u8* data)
#else
Generation BigEndian::convertTo<Generation>(const u8* data)
#endif
{
    return Generation{BigEndian::convertTo<u32>(data)};
}

template <>
#if __cplusplus > 201703L
constexpr void BigEndian::convertFrom<Generation>(u8* data, const Generation& gen)
#else
void BigEndian::convertFrom<Generation>(const u8* data, const Generation& gen)
#endif
{
    BigEndian::convertFrom<u32>(data, (u32)gen);
}

#endif
