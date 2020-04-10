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

enum class GameVersion : u8
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
};

Generation versionToGen(GameVersion version);

#endif
