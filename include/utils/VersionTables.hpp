/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62, Pk11
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

#ifndef VERSION_TABLES_HPP
#define VERSION_TABLES_HPP

#include "enums/Ability.hpp"
#include "enums/Ball.hpp"
#include "enums/GameVersion.hpp"
#include "enums/Generation.hpp"
#include "enums/Move.hpp"
#include "enums/Species.hpp"
#include <set>

namespace pksm::VersionTables
{
    [[nodiscard]] const std::set<int>& availableItems(GameVersion version);
    [[nodiscard]] const std::set<Move>& availableMoves(GameVersion version);
    [[nodiscard]] const std::set<Species>& availableSpecies(GameVersion version);
    [[nodiscard]] const std::set<Ability>& availableAbilities(GameVersion version);
    [[nodiscard]] const std::set<Ball>& availableBalls(GameVersion version);

    // Not guaranteed to be useful
    [[nodiscard]] int maxItem(GameVersion version);
    [[nodiscard]] Move maxMove(GameVersion version);
    [[nodiscard]] Species maxSpecies(GameVersion version);
    [[nodiscard]] Ability maxAbility(GameVersion version);
    [[nodiscard]] Ball maxBall(GameVersion version);

    // Takes version-specific forms into account
    [[nodiscard]] u8 formCount(GameVersion version, Species species);

    [[nodiscard]] u8 movePP(Generation gen, Move move, u8 ppUps);
}

#endif
