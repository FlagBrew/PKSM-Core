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

#include "utils/coretypes.h"
#include <array>
#include <bit>
#include <string.h>
#include <type_traits>
#include <vector>

namespace BigEndian
{
    // Only works with integral types
    template <typename T>
    constexpr void convertFrom(u8* dest, const T& orig)
    {
        static_assert(std::is_integral_v<T>);
        if (!std::is_constant_evaluated() && std::endian::big == std::endian::native)
        {
            memcpy(dest, &orig, sizeof(T));
        }
        else
        {
            std::make_unsigned_t<T> origVal = orig;
            for (size_t i = 0; i < sizeof(T); i++)
            {
                dest[sizeof(T) - i - 1] = u8(origVal);
                origVal >>= 8;
            }
        }
    }

    // Array-based implementation
    template <typename T, size_t N>
    constexpr void convertFrom(u8* dest, const T (&array)[N])
    {
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(dest, array[i]);
            dest += sizeof(T);
        }
    }

    // Another array-based implementation
    template <typename T, size_t N>
    constexpr void convertFrom(u8* dest, const std::array<T, N>& array)
    {
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(dest, array[i]);
            dest += sizeof(T);
        }
    }

    // And now... vector. Can't be constexpr, because vectors aren't constexpr
    template <typename T>
    void convertFrom(u8* dest, const std::vector<T>& vector)
    {
        for (size_t i = 0; i < vector.size(); i++)
        {
            convertFrom(dest, vector[i]);
            dest += sizeof(T);
        }
    }

    // Same as above, just with automatic handling of data lifetime
    template <typename T>
    constexpr auto convertFrom(const T& orig) -> std::array<u8, sizeof(T)>
    {
        static_assert(std::is_integral_v<T>);
        std::array<u8, sizeof(T)> ret{};
        convertFrom(ret.data(), orig);
        return ret;
    }

    template <typename T, size_t N>
    constexpr auto convertFrom(const T (&array)[N]) -> std::array<u8, sizeof(T) * N>
    {
        std::array<u8, sizeof(T) * N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(ret.data() + i * sizeof(T), array[i]);
        }
        return ret;
    }

    template <typename T, size_t N>
    constexpr auto convertFrom(const std::array<T, N>& array) -> std::array<u8, sizeof(T) * N>
    {
        std::array<u8, sizeof(T) * N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(ret.data() + i * sizeof(T), array[i]);
        }
        return ret;
    }

    // Can't be constexpr because vectors aren't constexpr
    template <typename T>
    std::vector<u8> convertFrom(const std::vector<T>& vector)
    {
        std::vector<u8> ret(vector.size() * sizeof(T));
        convertFrom(ret.data(), vector);
        return ret;
    }

    template <typename T>
    constexpr T convertTo(const u8* from)
    {
        static_assert(std::is_integral_v<T>);
        std::make_unsigned_t<T> dest = 0;
        if (!std::is_constant_evaluated() && std::endian::native == std::endian::big)
        {
            memcpy(&dest, from, sizeof(T));
        }
        else
        {
            for (size_t i = 0; i < sizeof(T); i++)
            {
                dest |= std::make_unsigned_t<T>(from[i]) << ((sizeof(T) - i - 1) * 8);
            }
        }
        return dest;
    }

    template <typename T, size_t N>
    constexpr std::array<T, N> convertTo(const u8* from)
    {
        std::array<T, N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            ret[i] = convertTo<T>(from + i * sizeof(T));
        }
        return ret;
    }
}

namespace LittleEndian
{
    // Only works with integral types
    template <typename T>
    constexpr void convertFrom(u8* dest, const T& orig)
    {
        static_assert(std::is_integral_v<T>);
        if (!std::is_constant_evaluated() && std::endian::little == std::endian::native)
        {
            memcpy(dest, &orig, sizeof(T));
        }
        else
        {
            std::make_unsigned_t<T> origVal = orig;
            for (size_t i = 0; i < sizeof(T); i++)
            {
                dest[i] = u8(origVal);
                origVal >>= 8;
            }
        }
    }

    // Array-based implementation
    template <typename T, size_t N>
    constexpr void convertFrom(u8* dest, const T (&array)[N])
    {
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(dest, array[i]);
            dest += sizeof(T);
        }
    }

    // Another array-based implementation
    template <typename T, size_t N>
    constexpr void convertFrom(u8* dest, const std::array<T, N>& array)
    {
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(dest, array[i]);
            dest += sizeof(T);
        }
    }

    // And now... vector. Can't be constexpr, because vectors aren't constexpr
    template <typename T>
    void convertFrom(u8* dest, const std::vector<T>& vector)
    {
        for (size_t i = 0; i < vector.size(); i++)
        {
            convertFrom(dest, vector[i]);
            dest += sizeof(T);
        }
    }

    // Same as above, just with automatic handling of data lifetime
    template <typename T>
    constexpr auto convertFrom(const T& orig) -> std::array<u8, sizeof(T)>
    {
        static_assert(std::is_integral_v<T>);
        std::array<u8, sizeof(T)> ret{};
        convertFrom(ret.data(), orig);
        return ret;
    }

    template <typename T, size_t N>
    constexpr auto convertFrom(const T (&array)[N]) -> std::array<u8, sizeof(T) * N>
    {
        std::array<u8, sizeof(T) * N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(ret.data() + i * sizeof(T), array[i]);
        }
        return ret;
    }

    template <typename T, size_t N>
    constexpr auto convertFrom(const std::array<T, N>& array) -> std::array<u8, sizeof(T) * N>
    {
        std::array<u8, sizeof(T) * N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            convertFrom(ret.data() + i * sizeof(T), array[i]);
        }
        return ret;
    }

    // Can't be constexpr because vectors aren't constexpr
    template <typename T>
    std::vector<u8> convertFrom(const std::vector<T>& vector)
    {
        std::vector<u8> ret(vector.size() * sizeof(T));
        convertFrom(ret.data(), vector);
        return ret;
    }

    template <typename T>
    constexpr T convertTo(const u8* from)
    {
        static_assert(std::is_integral_v<T>);
        std::make_unsigned_t<T> dest = 0;
        if (!std::is_constant_evaluated() && std::endian::native == std::endian::little)
        {
            memcpy(&dest, from, sizeof(T));
        }
        else
        {
            for (size_t i = 0; i < sizeof(T); i++)
            {
                dest |= std::make_unsigned_t<T>(from[i]) << (i * 8);
            }
        }
        return dest;
    }

    template <typename T, size_t N>
    constexpr std::array<T, N> convertTo(const u8* from)
    {
        std::array<T, N> ret{};
        for (size_t i = 0; i < N; i++)
        {
            ret[i] = convertTo<T>(from + i * sizeof(T));
        }
        return ret;
    }
}
