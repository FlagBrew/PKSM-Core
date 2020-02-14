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

#ifndef RIBBONS_HPP
#define RIBBONS_HPP

#include "coretypes.h"
#include "utils/flagUtil.hpp"
#include <array>

enum class Ribbon
{
    // G8 ribbons, in proper order
    ChampionKalos,
    ChampionG3Hoenn,
    ChampionSinnoh,
    BestFriends,
    Training,
    BattlerSkillful,
    BattlerExpert,
    Effort,
    Alert,
    Shock,
    Downcast,
    Careless,
    Relax,
    Snooze,
    Smile,
    Gorgeous,
    Royal,
    GorgeousRoyal,
    Artist,
    Footprint,
    Record,
    Legend,
    Country,
    National,
    Earth,
    World,
    Classic,
    Premier,
    Event,
    Birthday,
    Special,
    Souvenir,
    Wishing,
    ChampionBattle,
    ChampionRegional,
    ChampionNational,
    ChampionWorld,
    MemoryContest,
    MemoryBattle,
    ChampionG6Hoenn,
    ContestStar,
    MasterCoolness,
    MasterBeauty,
    MasterCuteness,
    MasterCleverness,
    MasterToughness,
    ChampionAlola,
    BattleRoyale,
    BattleTreeGreat,
    BattleTreeMaster,
    ChampionGalar,
    TowerMaster,
    MasterRank,

    MarkLunchtime,
    MarkSleepyTime,
    MarkDusk,
    MarkDawn,
    MarkCloudy,
    MarkRainy,
    MarkStormy,
    MarkSnowy,
    MarkBlizzard,
    MarkDry,
    MarkSandstorm,
    MarkMisty,
    MarkDestiny,
    MarkFishing,
    MarkCurry,
    MarkUncommon,
    MarkRare,
    MarkRowdy,
    MarkAbsentMinded,
    MarkJittery,
    MarkExcited,
    MarkCharismatic,
    MarkCalmness,
    MarkIntense,
    MarkZonedOut,
    MarkJoyful,
    MarkAngry,
    MarkSmiley,
    MarkTeary,
    MarkUpbeat,
    MarkPeeved,
    MarkIntellectual,
    MarkFerocious,
    MarkCrafty,
    MarkScowling,
    MarkKindly,
    MarkFlustered,
    MarkPumpedUp,
    MarkZeroEnergy,
    MarkPrideful,
    MarkUnsure,
    MarkHumble,
    MarkThorny,
    MarkVigor,
    MarkSlump,

    // All others that are not included in the above:
    Winning,
    Victory,
    Ability,
    AbilityGreat,
    AbilityDouble,
    AbilityMulti,
    AbilityPair,
    AbilityWorld,
    G3Cool,
    G3CoolSuper,
    G3CoolHyper,
    G3CoolMaster,
    G3Beauty,
    G3BeautySuper,
    G3BeautyHyper,
    G3BeautyMaster,
    G3Cute,
    G3CuteSuper,
    G3CuteHyper,
    G3CuteMaster,
    G3Smart,
    G3SmartSuper,
    G3SmartHyper,
    G3SmartMaster,
    G3Tough,
    G3ToughSuper,
    G3ToughHyper,
    G3ToughMaster,
    G4Cool,
    G4CoolSuper,
    G4CoolHyper,
    G4CoolMaster,
    G4Beauty,
    G4BeautySuper,
    G4BeautyHyper,
    G4BeautyMaster,
    G4Cute,
    G4CuteSuper,
    G4CuteHyper,
    G4CuteMaster,
    G4Smart,
    G4SmartSuper,
    G4SmartHyper,
    G4SmartMaster,
    G4Tough,
    G4ToughSuper,
    G4ToughHyper,
    G4ToughMaster,
    Master,

    G4CoolGreat   = G4CoolSuper,
    G4CoolUltra   = G4CoolHyper,
    G4BeautyGreat = G4BeautySuper,
    G4BeautyUltra = G4BeautyHyper,
    G4CuteGreat   = G4CuteSuper,
    G4CuteUltra   = G4CuteHyper,
    G4SmartGreat  = G4SmartSuper,
    G4SmartUltra  = G4SmartHyper,
    G4ToughGreat  = G4ToughSuper,
    G4ToughUltra  = G4ToughHyper,
};

constexpr size_t numRibbons()
{
    return size_t(Ribbon::Master) + 1;
}

#endif
