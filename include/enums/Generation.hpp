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

#ifndef GENERATION_HPP
#define GENERATION_HPP

#include "utils/coretypes.h"
#include <compare>
#include <concepts>
#include <string>
#include <type_traits>

namespace pksm
{
    class Generation;

    namespace internal
    {
        class Generation_impl
        {
            friend class pksm::Generation;

        private:
            // values MUST remain consistent
            enum class GenerationEnum : u32
            {
                ONE   = 7,
                TWO   = 8,
                THREE = 6,
                FOUR  = 0,
                FIVE  = 1,
                SIX   = 2,
                SEVEN = 3,
                LGPE  = 4,
                EIGHT = 5,

                UNUSED [[maybe_unused]] = 0xFFFFFFFF
            } v;

            constexpr explicit Generation_impl(GenerationEnum v) : v(v) {}

            constexpr Generation_impl(const Generation_impl&)            = default;
            constexpr Generation_impl(Generation_impl&&)                 = default;
            constexpr Generation_impl& operator=(const Generation_impl&) = default;
            constexpr Generation_impl& operator=(Generation_impl&&)      = default;

        public:
            template <std::integral T>
            [[nodiscard]] constexpr explicit operator T() const noexcept
            {
                return T(v);
            }

            [[nodiscard]] constexpr explicit operator const char*() const noexcept
            {
                switch (v)
                {
                    using enum GenerationEnum;
                    case LGPE:
                        return "LGPE";
                    case ONE:
                        return "1";
                    case TWO:
                        return "2";
                    case THREE:
                        return "3";
                    case FOUR:
                        return "4";
                    case FIVE:
                        return "5";
                    case SIX:
                        return "6";
                    case SEVEN:
                        return "7";
                    case EIGHT:
                        return "8";
                    case UNUSED:
                        return "INVALID";
                }
                return "INVALID";
            }

            [[nodiscard]] constexpr explicit operator std::string_view() const
            {
                return std::string_view{static_cast<const char*>(*this)};
            }

            [[nodiscard]] explicit operator std::string() const
            {
                return std::string{static_cast<const char*>(*this)};
            }

            [[nodiscard]] constexpr operator GenerationEnum() const noexcept { return v; }

            [[nodiscard]] constexpr std::strong_ordering operator<=>(
                const Generation_impl& other) const noexcept
            {
                switch (v)
                {
                    using enum GenerationEnum;
                    case ONE:
                        switch (other.v)
                        {
                            case ONE:
                                return std::strong_ordering::equal;
                            case TWO:
                            case THREE:
                            case FOUR:
                            case FIVE:
                            case SIX:
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case TWO:
                        switch (other.v)
                        {
                            case ONE:
                                return std::strong_ordering::greater;
                            case TWO:
                                return std::strong_ordering::equal;
                            case THREE:
                            case FOUR:
                            case FIVE:
                            case SIX:
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case THREE:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                                return std::strong_ordering::greater;
                            case THREE:
                                return std::strong_ordering::equal;
                            case FOUR:
                            case FIVE:
                            case SIX:
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case FOUR:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                                return std::strong_ordering::greater;
                            case FOUR:
                                return std::strong_ordering::equal;
                            case FIVE:
                            case SIX:
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case FIVE:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                            case FOUR:
                                return std::strong_ordering::greater;
                            case FIVE:
                                return std::strong_ordering::equal;
                            case SIX:
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case SIX:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                            case FOUR:
                            case FIVE:
                                return std::strong_ordering::greater;
                            case SIX:
                                return std::strong_ordering::equal;
                            case SEVEN:
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case SEVEN:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                            case FOUR:
                            case FIVE:
                            case SIX:
                                return std::strong_ordering::greater;
                            case SEVEN:
                                return std::strong_ordering::equal;
                            case LGPE:
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case LGPE:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                            case FOUR:
                            case FIVE:
                            case SIX:
                            case SEVEN:
                                return std::strong_ordering::greater;
                            case LGPE:
                                return std::strong_ordering::equal;
                            case EIGHT:
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case EIGHT:
                        switch (other.v)
                        {
                            case ONE:
                            case TWO:
                            case THREE:
                            case FOUR:
                            case FIVE:
                            case SIX:
                            case SEVEN:
                            case LGPE:
                                return std::strong_ordering::greater;
                            case EIGHT:
                                return std::strong_ordering::equal;
                            case UNUSED:
                                return std::strong_ordering::less;
                        }
                        return std::strong_ordering::less;
                    case UNUSED:
                        return other.v == UNUSED ? std::strong_ordering::equal
                                                 : std::strong_ordering::greater;
                }
                return v <=> other.v;
            }
        };
    }

    class Generation
    {
    private:
        internal::Generation_impl impl;

    public:
        using EnumType = internal::Generation_impl::GenerationEnum;

        constexpr Generation() noexcept : impl(EnumType::UNUSED) {}

        constexpr Generation(const internal::Generation_impl& impl) noexcept : impl(impl) {}

        constexpr explicit Generation(std::underlying_type_t<EnumType> v) noexcept
            : impl(EnumType{v})
        {
        }

        template <std::integral T>
        [[nodiscard]] constexpr explicit operator T() const noexcept
        {
            return static_cast<T>(impl);
        }

        [[nodiscard]] constexpr operator EnumType() const noexcept
        {
            return static_cast<EnumType>(impl);
        }

        [[nodiscard]] constexpr explicit operator const char*() const noexcept
        {
            return static_cast<const char*>(impl);
        }

        [[nodiscard]] constexpr explicit operator std::string_view() const
        {
            return static_cast<std::string_view>(impl);
        }

        [[nodiscard]] explicit operator std::string() const
        {
            return static_cast<std::string>(impl);
        }

        [[nodiscard]] constexpr std::strong_ordering operator<=>(
            const Generation& other) const noexcept = default;

        [[nodiscard]] static constexpr Generation fromString(const std::string_view& str)
        {
            if (str == "1")
            {
                return Generation::ONE;
            }
            else if (str == "2")
            {
                return Generation::TWO;
            }
            else if (str == "3")
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

        static constexpr internal::Generation_impl ONE{EnumType::ONE};
        static constexpr internal::Generation_impl TWO{EnumType::TWO};
        static constexpr internal::Generation_impl THREE{EnumType::THREE};
        static constexpr internal::Generation_impl FOUR{EnumType::FOUR};
        static constexpr internal::Generation_impl FIVE{EnumType::FIVE};
        static constexpr internal::Generation_impl SIX{EnumType::SIX};
        static constexpr internal::Generation_impl SEVEN{EnumType::SEVEN};
        static constexpr internal::Generation_impl LGPE{EnumType::LGPE};
        static constexpr internal::Generation_impl EIGHT{EnumType::EIGHT};
        static constexpr internal::Generation_impl UNUSED{EnumType::UNUSED};
    };
}

#endif
