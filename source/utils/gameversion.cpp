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

#include "gameversion.hpp"

Generation versionToGen(GameVersion version)
{
    switch (version)
    {
        case GameVersion::S:
        case GameVersion::R:
        case GameVersion::E:
        case GameVersion::FR:
        case GameVersion::LG:
        case GameVersion::CXD:
            return Generation::THREE;
        case GameVersion::D:
        case GameVersion::P:
        case GameVersion::Pt:
        case GameVersion::HG:
        case GameVersion::SS:
            return Generation::FOUR;
        case GameVersion::W:
        case GameVersion::B:
        case GameVersion::W2:
        case GameVersion::B2:
            return Generation::FIVE;
        case GameVersion::X:
        case GameVersion::Y:
        case GameVersion::AS:
        case GameVersion::OR:
            return Generation::SIX;
        case GameVersion::SN:
        case GameVersion::MN:
        case GameVersion::US:
        case GameVersion::UM:
            return Generation::SEVEN;
        case GameVersion::GP:
        case GameVersion::GE:
            return Generation::LGPE;
        case GameVersion::SW:
        case GameVersion::SH:
            return Generation::EIGHT;
        case GameVersion::GO:
        case GameVersion::RD:
        case GameVersion::GN:
        case GameVersion::BU:
        case GameVersion::YW:
        case GameVersion::GD:
        case GameVersion::SV:
        case GameVersion::C:
        default:
            return Generation::UNUSED;
    }
}
