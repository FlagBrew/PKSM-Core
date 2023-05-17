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

#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include "utils/coretypes.h"
#include <array>
#include <bit>
#include <cmath>
#include <functional>
#include <ranges>
#include <span>
#include <string.h>
#include <type_traits>
#include <vector>

namespace EndianTraits
{
    constexpr bool EasyArch =
        (std::endian::big == std::endian::native) || (std::endian::little == std::endian::native);

    template <typename T>
    struct EndianConvertible_s
    {
        static constexpr bool value =
            std::is_integral_v<T> ||
            (std::is_floating_point_v<T> && std::numeric_limits<T>::is_iec559);
    };

    template <typename T>
    concept EndianConvertible = EndianConvertible_s<T>::value;

    namespace EndianCommon
    {
        // DEPENDENT CONVERSIONS

        // Converts arithmetic types into big-endian byte representations. Integer types and IEEE
        // 754 compliant single and double-precision floating-point types are supported.
        template <EndianConvertible T, std::endian TargetEndianness>
        constexpr void convertFrom(u8* dest, const T& orig)
        {
            if constexpr (EasyArch)
            {
                auto obtained = std::bit_cast<std::array<u8, sizeof(T)>>(orig);
                if constexpr (std::endian::native != TargetEndianness)
                {
                    std::copy(obtained.rbegin(), obtained.rend(), dest);
                }
                else
                {
                    std::copy(obtained.begin(), obtained.end(), dest);
                }
            }
            else if constexpr (std::is_integral_v<T>)
            {
                auto origVal = std::bit_cast<std::make_unsigned_t<T>>(orig);

                if constexpr (TargetEndianness == std::endian::little)
                {
                    for (size_t i = 0; i < sizeof(T); i++)
                    {
                        dest[i] = u8(origVal);
                        origVal >>= 8;
                    }
                }
                else
                {
                    for (size_t i = 0; i < sizeof(T); i++)
                    {
                        dest[sizeof(T) - i - 1] = u8(origVal);
                        origVal                 >>= 8;
                    }
                }
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                bool negative = std::signbit(orig);
                int exponent;
                T normalized = std::frexp(orig, &exponent); // gets biased exponent
                if constexpr (std::is_same_v<T, float>)
                {
                    u32 write = negative ? (1u << 31) : 0;
                    switch (std::fpclassify(orig))
                    {
                        case FP_INFINITE:
                            write |= u32(0xFF)
                                  << 23; // should be written as 0x7F800000 or 0xFF800000
                            break;
                        case FP_NAN:
                            write = 0xFFFFFFFF; // might as well just use a constant NaN number
                            break;
                        case FP_ZERO:
                            // do nothing; it should be written as either 0x80000000 or 0x00000000
                            break;
                        case FP_NORMAL:
                            write |= u32(exponent + 127) << 23;
                            // falls through
                        case FP_SUBNORMAL:
                            normalized *= 1 << 23;
                            write      |= u32(normalized);
                            break;
                    }
                    convertFrom<u32, TargetEndianness>(dest, write);
                }
                else if constexpr (std::is_same_v<T, double>)
                {
                    u64 write = negative ? (u64(1) << 63) : 0;
                    switch (std::fpclassify(orig))
                    {
                        case FP_INFINITE:
                            write |= u64(0x7FF) << 52; // should be written as 0x7FF0000000000000 or
                                                       // 0xFFF0000000000000
                            break;
                        case FP_NAN:
                            write =
                                0xFFFFFFFFFFFFFFFF; // might as well just use a constant NaN number
                            break;
                        case FP_ZERO:
                            // do nothing; it should be written as either 0x8000000000000000 or
                            // 0x0000000000000000
                            break;
                        case FP_NORMAL:
                            write |= u64(exponent + 1023) << 52;
                            // falls through
                        case FP_SUBNORMAL:
                            normalized *= u64(1) << 52;
                            write      |= u64(normalized);
                            break;
                    }
                    convertFrom<u64, TargetEndianness>(dest, write);
                }
            }
        }

        // Converts big-endian byte representations into arithmetic types. Integer types and IEEE
        // 754 compliant single and double-precision floating-point types are supported.
        template <EndianTraits::EndianConvertible T, std::endian TargetEndianness>
        constexpr T convertTo(const u8* from)
        {
            if constexpr (EasyArch)
            {
                std::array<u8, sizeof(T)> data;
                if constexpr (std::endian::native != TargetEndianness)
                {
                    std::copy(from, from + sizeof(T), data.rbegin());
                }
                else
                {
                    std::copy(from, from + sizeof(T), data.begin());
                }
                return std::bit_cast<T>(data);
            }
            else if constexpr (std::is_integral_v<T>)
            {
                std::make_unsigned_t<T> dest = 0;
                for (size_t i = 0; i < sizeof(T); i++)
                {
                    dest |= std::make_unsigned_t<T>(from[i]) << ((sizeof(T) - i - 1) * 8);
                }
                return dest;
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                u32 data      = convertTo<u32, TargetEndianness>(from);
                bool negative = (data & 0x80000000) != 0;
                int exponent  = (data & 0x7F800000) >> 23;
                int fraction  = data & ~0xFF800000;
                if (exponent == 0 && fraction == 0)
                {
                    return std::copysign(T(0), negative ? -1 : 1);
                }
                else if (exponent == 0xFF)
                {
                    if (fraction == 0)
                    {
                        return negative ? -std::numeric_limits<T>::infinity()
                                        : std::numeric_limits<T>::infinity();
                    }
                    else
                    {
                        return std::numeric_limits<T>::signaling_NaN(); // Going to ignore the
                                                                        // difference between quiet
                                                                        // and signaling NaN
                    }
                }
                else
                {
                    // fraction is currently shifted 23 bits from where it should be, so fix that
                    // and set the proper exponent
                    T ret = std::ldexp(fraction, -23);
                    if (exponent == 0) // Denormal number
                    {
                        ret = std::ldexp(ret, -126);
                    }
                    else
                    {
                        // Unbias the exponent & add the one necessary because it's not a denormal
                        // number
                        ret = std::ldexp(ret + 1, exponent - 127);
                    }
                    return std::copysign(ret, negative ? -1 : 1);
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                u64 data      = convertTo<u64, TargetEndianness>(from);
                bool negative = (data & 0x8000000000000000) != 0;
                int exponent  = (data & 0x7FF0000000000000) >> 52;
                int fraction  = data & ~0xFFF0000000000000;
                if (exponent == 0 && fraction == 0)
                {
                    return std::copysign(T(0), negative ? -1 : 1);
                }
                else if (exponent == 0x7FF)
                {
                    if (fraction == 0)
                    {
                        return negative ? -std::numeric_limits<T>::infinity()
                                        : std::numeric_limits<T>::infinity();
                    }
                    else
                    {
                        return std::numeric_limits<T>::signaling_NaN(); // Going to ignore the
                                                                        // difference between quiet
                                                                        // and signaling NaN
                    }
                }
                else
                {
                    // fraction is currently shifted 52 bits from where it should be, so fix that
                    // and set the proper exponent
                    T ret = std::ldexp(fraction, -52);
                    if (exponent == 0) // Denormal number
                    {
                        ret = std::ldexp(ret, -1022);
                    }
                    else
                    {
                        // Unbias the exponent & add the one necessary because it's not a denormal
                        // number
                        ret = std::ldexp(ret + 1, exponent - 1023);
                    }
                    return std::copysign(ret, negative ? -1 : 1);
                }
            }
        }

        // Binary-Coded Decimal (BCD) conversions
        template <std::unsigned_integral T, size_t arrayLength, std::endian TargetEndianness>
        constexpr T BCDtoUInteger(const u8* src)
        {
            T out             = 0;
            T j               = 1;
            auto [begin, end] = std::invoke(
                [&src]
                {
                    if constexpr (TargetEndianness == std::endian::big)
                    {
                        return std::make_pair(std::make_reverse_iterator(src + arrayLength - 1),
                            std::make_reverse_iterator(src - 1));
                    }
                    else
                    {
                        return std::make_pair(src, src + arrayLength);
                    }
                });
            for (; begin != end; ++begin)
            {
                out += ((*begin & 0x0F) * j) + (((*begin & 0xF0) >> 4) * (j * 10));
            }
            return out;
        }

        template <std::unsigned_integral T, size_t arrayLength = sizeof(T),
            std::endian TargetEndianness>
        constexpr void UIntegerToBCD(u8* dest, T src)
        {
            auto [begin, end] = std::invoke(
                [&dest]
                {
                    if constexpr (TargetEndianness == std::endian::big)
                    {
                        return std::make_pair(std::make_reverse_iterator(dest + arrayLength - 1),
                            std::make_reverse_iterator(dest - 1));
                    }
                    else
                    {
                        return std::make_pair(dest, dest + arrayLength);
                    }
                });

            for (; begin != end; ++begin, ++dest, src /= 100)
            {
                *dest = (((src / 10) % 10) << 4) | (src % 10);
            }
        }

        // NON-DEPENDENT CONVERSIONS

        template <std::ranges::range R, std::endian TargetEndianness>
            requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
        constexpr void convertFrom(u8* dest, R&& range)
        {
            for (auto it = std::ranges::begin(range); it != std::ranges::end(range); ++it)
            {
                convertFrom<std::ranges::range_value_t<R>, TargetEndianness>(dest, *it);
                dest += sizeof(std::ranges::range_value_t<R>);
            }
        }

        // Same as above, just with automatic handling of data lifetime
        template <EndianTraits::EndianConvertible T, std::endian TargetEndianness>
        constexpr std::array<u8, sizeof(T)> convertFrom(const T& orig)
        {
            std::array<u8, sizeof(T)> ret{};
            convertFrom<T, TargetEndianness>(ret.data(), orig);
            return ret;
        }

        template <EndianTraits::EndianConvertible T, std::size_t Extent,
            std::endian TargetEndianness>
            requires (Extent != std::dynamic_extent)
        constexpr std::array<u8, sizeof(T) * Extent> convertFrom(std::span<T, Extent> span)
        {
            std::array<u8, sizeof(T) * Extent> ret{};
            for (size_t i = 0; i < Extent; i++)
            {
                convertFrom<T, TargetEndianness>(ret.data() + i * sizeof(T), span[i]);
            }
            return ret;
        }

        template <std::ranges::range R, std::endian TargetEndianness>
            requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
        constexpr std::vector<u8> convertFrom(R&& range)
        {
            std::vector<u8> ret = std::invoke(
                [&]
                {
                    if constexpr (requires { requires std::ranges::sized_range<R>; })
                    {
                        return std::vector<u8>(
                            std::ranges::size(range) * sizeof(std::ranges::range_value_t<R>));
                    }
                    else
                    {
                        return std::vector<u8>();
                    }
                });
            std::size_t current = 0;
            for (auto it = std::ranges::begin(range); it != std::ranges::end(range);
                 ++it, current += sizeof(std::ranges::range_value_t<R>))
            {
                if constexpr (!requires { requires std::ranges::sized_range<R>; })
                {
                    ret.insert(ret.end(), sizeof(std::ranges::range_value_t<R>), 0);
                }
                convertFrom<std::ranges::range_value_t<R>, TargetEndianness>(
                    ret.data() + current, *it);
            }
            return ret;
        }

        template <EndianTraits::EndianConvertible T, size_t N, std::endian TargetEndianness>
        constexpr std::array<T, N> convertTo(const u8* from)
        {
            std::array<T, N> ret{};
            for (size_t i = 0; i < N; i++)
            {
                ret[i] = convertTo<T, TargetEndianness>(from + i * sizeof(T));
            }
            return ret;
        }

        template <std::unsigned_integral T, size_t arrayLength = sizeof(T),
            std::endian TargetEndianness>
        constexpr std::array<u8, arrayLength> UIntegerToBCD(T src)
        {
            std::array<u8, arrayLength> out;
            UIntegerToBCD<T, arrayLength, TargetEndianness>(out.data(), src);
            return out;
        }
    }
}

namespace BigEndian
{
    template <EndianTraits::EndianConvertible T>
    constexpr void convertFrom(u8* dest, const T& orig)
    {
        return EndianTraits::EndianCommon::convertFrom<T, std::endian::big>(dest, orig);
    }

    template <EndianTraits::EndianConvertible T>
    constexpr T convertTo(const u8* from)
    {
        return EndianTraits::EndianCommon::convertTo<T, std::endian::big>(from);
    }

    template <std::unsigned_integral T, size_t arrayLength>
    constexpr T BCDtoUInteger(const u8* src)
    {
        return EndianTraits::EndianCommon::BCDtoUInteger<T, arrayLength, std::endian::big>(src);
    }

    template <std::unsigned_integral T, size_t arrayLength = sizeof(T)>
    constexpr void UIntegerToBCD(u8* dest, T src)
    {
        return EndianTraits::EndianCommon::UIntegerToBCD<T, arrayLength, std::endian::big>(
            dest, src);
    }

    template <std::ranges::range R>
        requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
    constexpr void convertFrom(u8* dest, R&& range)
    {
        return EndianTraits::EndianCommon::convertFrom<R, std::endian::big>(
            dest, std::forward<decltype(range)>(range));
    }

    template <EndianTraits::EndianConvertible T>
    constexpr std::array<u8, sizeof(T)> convertFrom(const T& orig)
    {
        return EndianTraits::EndianCommon::convertFrom<T, std::endian::big>(orig);
    }

    template <EndianTraits::EndianConvertible T, std::size_t Extent>
        requires (Extent != std::dynamic_extent)
    constexpr std::array<u8, sizeof(T) * Extent> convertFrom(std::span<T, Extent> span)
    {
        return EndianTraits::EndianCommon::convertFrom<T, Extent, std::endian::big>(span);
    }

    template <std::ranges::range R>
        requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
    constexpr std::vector<u8> convertFrom(R&& range)
    {
        return EndianTraits::EndianCommon::convertFrom<R, std::endian::big>(
            std::forward<decltype(range)>(range));
    }

    template <EndianTraits::EndianConvertible T, size_t N>
    constexpr std::array<T, N> convertTo(const u8* from)
    {
        return EndianTraits::EndianCommon::convertTo<T, N, std::endian::big>(from);
    }

    template <std::unsigned_integral T, size_t arrayLength = sizeof(T)>
    constexpr std::array<u8, arrayLength> UIntegerToBCD(T src)
    {
        return EndianTraits::EndianCommon::UIntegerToBCD<T, arrayLength, std::endian::big>(src);
    }
}

namespace LittleEndian
{
    template <EndianTraits::EndianConvertible T>
    constexpr void convertFrom(u8* dest, const T& orig)
    {
        return EndianTraits::EndianCommon::convertFrom<T, std::endian::little>(dest, orig);
    }

    template <EndianTraits::EndianConvertible T>
    constexpr T convertTo(const u8* from)
    {
        return EndianTraits::EndianCommon::convertTo<T, std::endian::little>(from);
    }

    template <std::unsigned_integral T, size_t arrayLength>
    constexpr T BCDtoUInteger(const u8* src)
    {
        return EndianTraits::EndianCommon::BCDtoUInteger<T, arrayLength, std::endian::little>(src);
    }

    template <std::unsigned_integral T, size_t arrayLength = sizeof(T)>
    constexpr void UIntegerToBCD(u8* dest, T src)
    {
        return EndianTraits::EndianCommon::UIntegerToBCD<T, arrayLength, std::endian::little>(
            dest, src);
    }

    template <std::ranges::range R>
        requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
    constexpr void convertFrom(u8* dest, R&& range)
    {
        return EndianTraits::EndianCommon::convertFrom<R, std::endian::little>(
            dest, std::forward<decltype(range)>(range));
    }

    template <EndianTraits::EndianConvertible T>
    constexpr std::array<u8, sizeof(T)> convertFrom(const T& orig)
    {
        return EndianTraits::EndianCommon::convertFrom<T, std::endian::little>(orig);
    }

    template <EndianTraits::EndianConvertible T, std::size_t Extent>
        requires (Extent != std::dynamic_extent)
    constexpr std::array<u8, sizeof(T) * Extent> convertFrom(std::span<T, Extent> span)
    {
        return EndianTraits::EndianCommon::convertFrom<T, Extent, std::endian::little>(span);
    }

    template <std::ranges::range R>
        requires EndianTraits::EndianConvertible<std::ranges::range_value_t<R>>
    constexpr std::vector<u8> convertFrom(R&& range)
    {
        return EndianTraits::EndianCommon::convertFrom<R, std::endian::little>(
            std::forward<decltype(range)>(range));
    }

    template <EndianTraits::EndianConvertible T, size_t N>
    constexpr std::array<T, N> convertTo(const u8* from)
    {
        return EndianTraits::EndianCommon::convertTo<T, N, std::endian::little>(from);
    }

    template <std::unsigned_integral T, size_t arrayLength = sizeof(T)>
    constexpr std::array<u8, arrayLength> UIntegerToBCD(T src)
    {
        return EndianTraits::EndianCommon::UIntegerToBCD<T, arrayLength, std::endian::little>(src);
    }
}

#endif
