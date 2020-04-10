/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2020 Bernardo Giordano, Admiral Fish, piepie62, Pk11
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

#ifndef GAMEVERSION_HPP
#define GAMEVERSION_HPP

#include "utils/coretypes.h"
#include "utils/generation.hpp"

class GameVersion_impl
{
    friend class GameVersion;

private:
    enum class Version : u8
    {
        INVALID = 0,
        /// Pokémon Sapphire (GBA)
        S = 1,
        /// Pokémon Ruby (GBA)
        R = 2,
        /// Pokémon Emerald (GBA)
        E = 3,
        /// Pokémon FireRed (GBA)
        FR = 4,
        /// Pokémon LeafGreen (GBA)
        LG = 5,
        /// Pokémon Colosseum & Pokémon XD (GameCube)
        CXD = 15,
        /// Pokémon Diamond (NDS)
        D = 10,
        /// Pokémon Pearl (NDS)
        P = 11,
        /// Pokémon Platinum (NDS)
        Pt = 12,
        /// Pokémon Heart Gold (NDS)
        HG = 7,
        /// Pokémon Soul Silver (NDS)
        SS = 8,
        /// Pokémon White (NDS)
        W = 20,
        /// Pokémon Black (NDS)
        B = 21,
        /// Pokémon White 2 (NDS)
        W2 = 22,
        /// Pokémon Black 2 (NDS)
        B2 = 23,
        /// Pokémon X (3DS)
        X = 24,
        /// Pokémon Y (3DS)
        Y = 25,
        /// Pokémon Alpha Sapphire (3DS)
        AS = 26,
        /// Pokémon Omega Ruby (3DS)
        OR = 27,
        /// Pokémon Sun (3DS)
        SN = 30,
        /// Pokémon Moon (3DS)
        MN = 31,
        /// Pokémon Ultra Sun (3DS)
        US = 32,
        /// Pokémon Ultra Moon (3DS)
        UM = 33,
        /// Pokémon GO (GO -> Lets Go transfers)
        GO = 34,
        /// Pokémon Red (3DS Virtual Console)
        RD = 35,
        /// Pokémon Green[JP]/Blue[INT] (3DS Virtual Console)
        GN = 36,
        /// Pokémon Blue[JP] (3DS Virtual Console)
        BU = 37,
        /// Pokémon Yellow [JP] (3DS Virtual Console)
        YW = 38,
        /// Pokémon Gold (3DS Virtual Console)
        GD = 39,
        /// Pokémon Silver (3DS Virtual Console)
        SV = 40,
        /// Pokémon Crystal (3DS Virtual Console)
        C = 41,
        /// Pokémon Let's Go Pikachu (NX)
        GP = 42,
        /// Pokémon Let's Go Eevee (NX)
        GE = 43,
        /// Pokémon Sword (NX)
        SW = 44,
        /// Pokémon Shield (NX)
        SH = 45,
    } v;

    constexpr explicit GameVersion_impl(Version v) : v(v) {}
    constexpr GameVersion_impl(const GameVersion_impl&) = default;
    constexpr GameVersion_impl(GameVersion_impl&&)      = default;
    constexpr GameVersion_impl& operator=(const GameVersion_impl&) = default;
    constexpr GameVersion_impl& operator=(GameVersion_impl&&) = default;

public:
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        static_assert(std::is_integral_v<T>);
        return T(v);
    }
    constexpr explicit operator Generation() const noexcept
    {
        switch (v)
        {
            case Version::S:   // 1:
            case Version::R:   // 2:
            case Version::E:   // 3:
            case Version::FR:  // 4:
            case Version::LG:  // 5:
            case Version::CXD: // 15:
                return Generation::THREE;
            case Version::D:  // 10:
            case Version::P:  // 11:
            case Version::Pt: // 12:
            case Version::HG: // 7:
            case Version::SS: // 8:
                return Generation::FOUR;
            case Version::W:  // 20:
            case Version::B:  // 21:
            case Version::W2: // 22:
            case Version::B2: // 23:
                return Generation::FIVE;
            case Version::X:  // 24:
            case Version::Y:  // 25:
            case Version::AS: // 26:
            case Version::OR: // 27:
                return Generation::SIX;
            case Version::SN: // 30:
            case Version::MN: // 31:
            case Version::US: // 32:
            case Version::UM: // 33:
                return Generation::SEVEN;
            case Version::GP: // 42:
            case Version::GE: // 43:
                return Generation::LGPE;
            case Version::SW: // 44:
            case Version::SH: // 45:
                return Generation::EIGHT;
            case Version::INVALID: // 0:
            case Version::GO:      // 34:
            case Version::RD:      // 35:
            case Version::GN:      // 36:
            case Version::BU:      // 37:
            case Version::YW:      // 38:
            case Version::GD:      // 39:
            case Version::SV:      // 40:
            case Version::C:       // 41:
                return Generation::UNUSED;
        }
        return Generation::UNUSED;
    }
    constexpr operator Version() const noexcept { return v; }

    constexpr bool operator<(const GameVersion_impl& other) const noexcept { return v < other.v; }
    constexpr bool operator<=(const GameVersion_impl& other) const noexcept { return v <= other.v; }

    constexpr bool operator>(const GameVersion_impl& other) const noexcept { return v > other.v; }
    constexpr bool operator>=(const GameVersion_impl& other) const noexcept { return v >= other.v; }

    constexpr bool operator==(const GameVersion_impl& other) const noexcept { return v == other.v; }
    constexpr bool operator!=(const GameVersion_impl& other) const noexcept { return v != other.v; }
};

class GameVersion
{
private:
    GameVersion_impl impl;

public:
    using EnumType = GameVersion_impl::Version;
    constexpr GameVersion(const GameVersion_impl& impl) noexcept : impl(impl) {}
    constexpr explicit GameVersion(u8 v) noexcept : impl(GameVersion_impl::Version{v}) {}
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        static_assert(std::is_integral_v<T>);
        return T(impl);
    }
    constexpr explicit operator Generation() const noexcept { return (Generation)impl; }
    constexpr operator GameVersion_impl::Version() const noexcept { return (GameVersion_impl::Version)impl; }

    constexpr bool operator<(const GameVersion& other) const noexcept { return impl < other.impl; }
    constexpr bool operator<=(const GameVersion& other) const noexcept { return impl <= other.impl; }

    constexpr bool operator>(const GameVersion& other) const noexcept { return impl > other.impl; }
    constexpr bool operator>=(const GameVersion& other) const noexcept { return impl >= other.impl; }

    constexpr bool operator==(const GameVersion& other) const noexcept { return impl == other.impl; }
    constexpr bool operator!=(const GameVersion& other) const noexcept { return impl != other.impl; }

    constexpr bool operator<(const GameVersion_impl& other) const noexcept { return impl < other; }
    constexpr bool operator<=(const GameVersion_impl& other) const noexcept { return impl <= other; }

    constexpr bool operator>(const GameVersion_impl& other) const noexcept { return impl > other; }
    constexpr bool operator>=(const GameVersion_impl& other) const noexcept { return impl >= other; }

    constexpr bool operator==(const GameVersion_impl& other) const noexcept { return impl == other; }
    constexpr bool operator!=(const GameVersion_impl& other) const noexcept { return impl != other; }

    static constexpr GameVersion_impl INVALID{GameVersion_impl::Version::INVALID};
    /// Pokémon Sapphire (GBA)
    static constexpr GameVersion_impl S{GameVersion_impl::Version::S};
    /// Pokémon Ruby (GBA)
    static constexpr GameVersion_impl R{GameVersion_impl::Version::R};
    /// Pokémon Emerald (GBA)
    static constexpr GameVersion_impl E{GameVersion_impl::Version::E};
    /// Pokémon FireRed (GBA)
    static constexpr GameVersion_impl FR{GameVersion_impl::Version::FR};
    /// Pokémon LeafGreen (GBA)
    static constexpr GameVersion_impl LG{GameVersion_impl::Version::LG};
    /// Pokémon Colosseum & Pokémon XD (GameCube)
    static constexpr GameVersion_impl CXD{GameVersion_impl::Version::CXD};
    /// Pokémon Diamond (NDS)
    static constexpr GameVersion_impl D{GameVersion_impl::Version::D};
    /// Pokémon Pearl (NDS)
    static constexpr GameVersion_impl P{GameVersion_impl::Version::P};
    /// Pokémon Platinum (NDS)
    static constexpr GameVersion_impl Pt{GameVersion_impl::Version::Pt};
    /// Pokémon Heart Gold (NDS)
    static constexpr GameVersion_impl HG{GameVersion_impl::Version::HG};
    /// Pokémon Soul Silver (NDS)
    static constexpr GameVersion_impl SS{GameVersion_impl::Version::SS};
    /// Pokémon White (NDS)
    static constexpr GameVersion_impl W{GameVersion_impl::Version::W};
    /// Pokémon Black (NDS)
    static constexpr GameVersion_impl B{GameVersion_impl::Version::B};
    /// Pokémon White 2 (NDS)
    static constexpr GameVersion_impl W2{GameVersion_impl::Version::W2};
    /// Pokémon Black 2 (NDS)
    static constexpr GameVersion_impl B2{GameVersion_impl::Version::B2};
    /// Pokémon X (3DS)
    static constexpr GameVersion_impl X{GameVersion_impl::Version::X};
    /// Pokémon Y (3DS)
    static constexpr GameVersion_impl Y{GameVersion_impl::Version::Y};
    /// Pokémon Alpha Sapphire (3DS)
    static constexpr GameVersion_impl AS{GameVersion_impl::Version::AS};
    /// Pokémon Omega Ruby (3DS)
    static constexpr GameVersion_impl OR{GameVersion_impl::Version::OR};
    /// Pokémon Sun (3DS)
    static constexpr GameVersion_impl SN{GameVersion_impl::Version::SN};
    /// Pokémon Moon (3DS)
    static constexpr GameVersion_impl MN{GameVersion_impl::Version::MN};
    /// Pokémon Ultra Sun (3DS)
    static constexpr GameVersion_impl US{GameVersion_impl::Version::US};
    /// Pokémon Ultra Moon (3DS)
    static constexpr GameVersion_impl UM{GameVersion_impl::Version::UM};
    /// Pokémon GO (GO -> Lets Go transfers)
    static constexpr GameVersion_impl GO{GameVersion_impl::Version::GO};
    /// Pokémon Red (3DS Virtual Console)
    static constexpr GameVersion_impl RD{GameVersion_impl::Version::RD};
    /// Pokémon Green[JP]/Blue[INT] (3DS Virtual Console)
    static constexpr GameVersion_impl GN{GameVersion_impl::Version::GN};
    /// Pokémon Blue[JP] (3DS Virtual Console)
    static constexpr GameVersion_impl BU{GameVersion_impl::Version::BU};
    /// Pokémon Yellow [JP] (3DS Virtual Console)
    static constexpr GameVersion_impl YW{GameVersion_impl::Version::YW};
    /// Pokémon Gold (3DS Virtual Console)
    static constexpr GameVersion_impl GD{GameVersion_impl::Version::GD};
    /// Pokémon Silver (3DS Virtual Console)
    static constexpr GameVersion_impl SV{GameVersion_impl::Version::SV};
    /// Pokémon Crystal (3DS Virtual Console)
    static constexpr GameVersion_impl C{GameVersion_impl::Version::C};
    /// Pokémon Let's Go Pikachu (NX)
    static constexpr GameVersion_impl GP{GameVersion_impl::Version::GP};
    /// Pokémon Let's Go Eevee (NX)
    static constexpr GameVersion_impl GE{GameVersion_impl::Version::GE};
    /// Pokémon Sword (NX)
    static constexpr GameVersion_impl SW{GameVersion_impl::Version::SW};
    /// Pokémon Shield (NX)
    static constexpr GameVersion_impl SH{GameVersion_impl::Version::SH};
};

#endif
