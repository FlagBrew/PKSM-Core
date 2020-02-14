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

#include "wcx/WC8.hpp"
#include "personal/personal.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/utils.hpp"

namespace
{
    int langIndex(Language lang)
    {
        if (lang > Language::UNUSED)
        {
            return u8(lang) - 2;
        }
        else if (lang == Language::UNUSED)
        {
            return 2 - 1; // English
        }
        else
        {
            return u8(lang) - 1;
        }
    }

    constexpr std::array<std::pair<size_t, size_t>, numRibbons()> OFFSETS()
    {
        std::array<std::pair<size_t, size_t>, numRibbons()> ret{};

        for (auto& pair : ret)
        {
            pair.first = 0xFFFFFFFF;
        }

        ret[size_t(Ribbon::ChampionKalos)].first  = 0x24C;
        ret[size_t(Ribbon::ChampionKalos)].second = 0;

        ret[size_t(Ribbon::ChampionG3Hoenn)].first  = 0x24C;
        ret[size_t(Ribbon::ChampionG3Hoenn)].second = 1;

        ret[size_t(Ribbon::ChampionSinnoh)].first  = 0x24C;
        ret[size_t(Ribbon::ChampionSinnoh)].second = 2;

        ret[size_t(Ribbon::BestFriends)].first  = 0x24C;
        ret[size_t(Ribbon::BestFriends)].second = 3;

        ret[size_t(Ribbon::Training)].first  = 0x24C;
        ret[size_t(Ribbon::Training)].second = 4;

        ret[size_t(Ribbon::BattlerSkillful)].first  = 0x24C;
        ret[size_t(Ribbon::BattlerSkillful)].second = 5;

        ret[size_t(Ribbon::BattlerExpert)].first  = 0x24C;
        ret[size_t(Ribbon::BattlerExpert)].second = 6;

        ret[size_t(Ribbon::Effort)].first  = 0x24C;
        ret[size_t(Ribbon::Effort)].second = 7;

        ret[size_t(Ribbon::Alert)].first  = 0x24D;
        ret[size_t(Ribbon::Alert)].second = 0;

        ret[size_t(Ribbon::Shock)].first  = 0x24D;
        ret[size_t(Ribbon::Shock)].second = 1;

        ret[size_t(Ribbon::Downcast)].first  = 0x24D;
        ret[size_t(Ribbon::Downcast)].second = 2;

        ret[size_t(Ribbon::Careless)].first  = 0x24D;
        ret[size_t(Ribbon::Careless)].second = 3;

        ret[size_t(Ribbon::Relax)].first  = 0x24D;
        ret[size_t(Ribbon::Relax)].second = 4;

        ret[size_t(Ribbon::Snooze)].first  = 0x24D;
        ret[size_t(Ribbon::Snooze)].second = 5;

        ret[size_t(Ribbon::Smile)].first  = 0x24D;
        ret[size_t(Ribbon::Smile)].second = 6;

        ret[size_t(Ribbon::Gorgeous)].first  = 0x24D;
        ret[size_t(Ribbon::Gorgeous)].second = 7;

        ret[size_t(Ribbon::Royal)].first  = 0x24E;
        ret[size_t(Ribbon::Royal)].second = 0;

        ret[size_t(Ribbon::GorgeousRoyal)].first  = 0x24E;
        ret[size_t(Ribbon::GorgeousRoyal)].second = 1;

        ret[size_t(Ribbon::Artist)].first  = 0x24E;
        ret[size_t(Ribbon::Artist)].second = 2;

        ret[size_t(Ribbon::Footprint)].first  = 0x24E;
        ret[size_t(Ribbon::Footprint)].second = 3;

        ret[size_t(Ribbon::Record)].first  = 0x24E;
        ret[size_t(Ribbon::Record)].second = 4;

        ret[size_t(Ribbon::Legend)].first  = 0x24E;
        ret[size_t(Ribbon::Legend)].second = 5;

        ret[size_t(Ribbon::Country)].first  = 0x24E;
        ret[size_t(Ribbon::Country)].second = 6;

        ret[size_t(Ribbon::National)].first  = 0x24E;
        ret[size_t(Ribbon::National)].second = 7;

        ret[size_t(Ribbon::Earth)].first  = 0x24F;
        ret[size_t(Ribbon::Earth)].second = 0;

        ret[size_t(Ribbon::World)].first  = 0x24F;
        ret[size_t(Ribbon::World)].second = 1;

        ret[size_t(Ribbon::Classic)].first  = 0x24F;
        ret[size_t(Ribbon::Classic)].second = 2;

        ret[size_t(Ribbon::Premier)].first  = 0x24F;
        ret[size_t(Ribbon::Premier)].second = 3;

        ret[size_t(Ribbon::Event)].first  = 0x24F;
        ret[size_t(Ribbon::Event)].second = 4;

        ret[size_t(Ribbon::Birthday)].first  = 0x24F;
        ret[size_t(Ribbon::Birthday)].second = 5;

        ret[size_t(Ribbon::Special)].first  = 0x24F;
        ret[size_t(Ribbon::Special)].second = 6;

        ret[size_t(Ribbon::Souvenir)].first  = 0x24F;
        ret[size_t(Ribbon::Souvenir)].second = 7;

        ret[size_t(Ribbon::Wishing)].first  = 0x250;
        ret[size_t(Ribbon::Wishing)].second = 0;

        ret[size_t(Ribbon::ChampionBattle)].first  = 0x250;
        ret[size_t(Ribbon::ChampionBattle)].second = 1;

        ret[size_t(Ribbon::ChampionRegional)].first  = 0x250;
        ret[size_t(Ribbon::ChampionRegional)].second = 2;

        ret[size_t(Ribbon::ChampionNational)].first  = 0x250;
        ret[size_t(Ribbon::ChampionNational)].second = 3;

        ret[size_t(Ribbon::ChampionWorld)].first  = 0x250;
        ret[size_t(Ribbon::ChampionWorld)].second = 4;

        ret[size_t(Ribbon::MemoryContest)].first  = 0x250;
        ret[size_t(Ribbon::MemoryContest)].second = 5;

        ret[size_t(Ribbon::MemoryBattle)].first  = 0x250;
        ret[size_t(Ribbon::MemoryBattle)].second = 6;

        ret[size_t(Ribbon::ChampionG6Hoenn)].first  = 0x250;
        ret[size_t(Ribbon::ChampionG6Hoenn)].second = 7;

        ret[size_t(Ribbon::ContestStar)].first  = 0x251;
        ret[size_t(Ribbon::ContestStar)].second = 0;

        ret[size_t(Ribbon::MasterCoolness)].first  = 0x251;
        ret[size_t(Ribbon::MasterCoolness)].second = 1;

        ret[size_t(Ribbon::MasterBeauty)].first  = 0x251;
        ret[size_t(Ribbon::MasterBeauty)].second = 2;

        ret[size_t(Ribbon::MasterCuteness)].first  = 0x251;
        ret[size_t(Ribbon::MasterCuteness)].second = 3;

        ret[size_t(Ribbon::MasterCleverness)].first  = 0x251;
        ret[size_t(Ribbon::MasterCleverness)].second = 4;

        ret[size_t(Ribbon::MasterToughness)].first  = 0x251;
        ret[size_t(Ribbon::MasterToughness)].second = 5;

        ret[size_t(Ribbon::ChampionAlola)].first  = 0x251;
        ret[size_t(Ribbon::ChampionAlola)].second = 6;

        ret[size_t(Ribbon::BattleRoyale)].first  = 0x251;
        ret[size_t(Ribbon::BattleRoyale)].second = 7;

        ret[size_t(Ribbon::BattleTreeGreat)].first  = 0x252;
        ret[size_t(Ribbon::BattleTreeGreat)].second = 0;

        ret[size_t(Ribbon::BattleTreeMaster)].first  = 0x252;
        ret[size_t(Ribbon::BattleTreeMaster)].second = 1;

        ret[size_t(Ribbon::ChampionGalar)].first  = 0x252;
        ret[size_t(Ribbon::ChampionGalar)].second = 2;

        ret[size_t(Ribbon::TowerMaster)].first  = 0x252;
        ret[size_t(Ribbon::TowerMaster)].second = 3;

        ret[size_t(Ribbon::MasterRank)].first  = 0x252;
        ret[size_t(Ribbon::MasterRank)].second = 4;

        ret[size_t(Ribbon::MarkLunchtime)].first  = 0x252;
        ret[size_t(Ribbon::MarkLunchtime)].second = 5;

        ret[size_t(Ribbon::MarkSleepyTime)].first  = 0x252;
        ret[size_t(Ribbon::MarkSleepyTime)].second = 6;

        ret[size_t(Ribbon::MarkDusk)].first  = 0x252;
        ret[size_t(Ribbon::MarkDusk)].second = 7;

        ret[size_t(Ribbon::MarkDawn)].first  = 0x253;
        ret[size_t(Ribbon::MarkDawn)].second = 0;

        ret[size_t(Ribbon::MarkCloudy)].first  = 0x253;
        ret[size_t(Ribbon::MarkCloudy)].second = 1;

        ret[size_t(Ribbon::MarkRainy)].first  = 0x253;
        ret[size_t(Ribbon::MarkRainy)].second = 2;

        ret[size_t(Ribbon::MarkStormy)].first  = 0x253;
        ret[size_t(Ribbon::MarkStormy)].second = 3;

        ret[size_t(Ribbon::MarkSnowy)].first  = 0x253;
        ret[size_t(Ribbon::MarkSnowy)].second = 4;

        ret[size_t(Ribbon::MarkBlizzard)].first  = 0x253;
        ret[size_t(Ribbon::MarkBlizzard)].second = 5;

        ret[size_t(Ribbon::MarkDry)].first  = 0x253;
        ret[size_t(Ribbon::MarkDry)].second = 6;

        ret[size_t(Ribbon::MarkSandstorm)].first  = 0x253;
        ret[size_t(Ribbon::MarkSandstorm)].second = 7;

        ret[size_t(Ribbon::MarkMisty)].first  = 0x258;
        ret[size_t(Ribbon::MarkMisty)].second = 0;

        ret[size_t(Ribbon::MarkDestiny)].first  = 0x258;
        ret[size_t(Ribbon::MarkDestiny)].second = 1;

        ret[size_t(Ribbon::MarkFishing)].first  = 0x258;
        ret[size_t(Ribbon::MarkFishing)].second = 2;

        ret[size_t(Ribbon::MarkCurry)].first  = 0x258;
        ret[size_t(Ribbon::MarkCurry)].second = 3;

        ret[size_t(Ribbon::MarkUncommon)].first  = 0x258;
        ret[size_t(Ribbon::MarkUncommon)].second = 4;

        ret[size_t(Ribbon::MarkRare)].first  = 0x258;
        ret[size_t(Ribbon::MarkRare)].second = 5;

        ret[size_t(Ribbon::MarkRowdy)].first  = 0x258;
        ret[size_t(Ribbon::MarkRowdy)].second = 6;

        ret[size_t(Ribbon::MarkAbsentMinded)].first  = 0x258;
        ret[size_t(Ribbon::MarkAbsentMinded)].second = 7;

        ret[size_t(Ribbon::MarkJittery)].first  = 0x259;
        ret[size_t(Ribbon::MarkJittery)].second = 0;

        ret[size_t(Ribbon::MarkExcited)].first  = 0x259;
        ret[size_t(Ribbon::MarkExcited)].second = 1;

        ret[size_t(Ribbon::MarkCharismatic)].first  = 0x259;
        ret[size_t(Ribbon::MarkCharismatic)].second = 2;

        ret[size_t(Ribbon::MarkCalmness)].first  = 0x259;
        ret[size_t(Ribbon::MarkCalmness)].second = 3;

        ret[size_t(Ribbon::MarkIntense)].first  = 0x259;
        ret[size_t(Ribbon::MarkIntense)].second = 4;

        ret[size_t(Ribbon::MarkZonedOut)].first  = 0x259;
        ret[size_t(Ribbon::MarkZonedOut)].second = 5;

        ret[size_t(Ribbon::MarkJoyful)].first  = 0x259;
        ret[size_t(Ribbon::MarkJoyful)].second = 6;

        ret[size_t(Ribbon::MarkAngry)].first  = 0x259;
        ret[size_t(Ribbon::MarkAngry)].second = 7;

        ret[size_t(Ribbon::MarkSmiley)].first  = 0x25A;
        ret[size_t(Ribbon::MarkSmiley)].second = 0;

        ret[size_t(Ribbon::MarkTeary)].first  = 0x25A;
        ret[size_t(Ribbon::MarkTeary)].second = 1;

        ret[size_t(Ribbon::MarkUpbeat)].first  = 0x25A;
        ret[size_t(Ribbon::MarkUpbeat)].second = 2;

        ret[size_t(Ribbon::MarkPeeved)].first  = 0x25A;
        ret[size_t(Ribbon::MarkPeeved)].second = 3;

        ret[size_t(Ribbon::MarkIntellectual)].first  = 0x25A;
        ret[size_t(Ribbon::MarkIntellectual)].second = 4;

        ret[size_t(Ribbon::MarkFerocious)].first  = 0x25A;
        ret[size_t(Ribbon::MarkFerocious)].second = 5;

        ret[size_t(Ribbon::MarkCrafty)].first  = 0x25A;
        ret[size_t(Ribbon::MarkCrafty)].second = 6;

        ret[size_t(Ribbon::MarkScowling)].first  = 0x25A;
        ret[size_t(Ribbon::MarkScowling)].second = 7;

        ret[size_t(Ribbon::MarkKindly)].first  = 0x25B;
        ret[size_t(Ribbon::MarkKindly)].second = 0;

        ret[size_t(Ribbon::MarkFlustered)].first  = 0x25B;
        ret[size_t(Ribbon::MarkFlustered)].second = 1;

        ret[size_t(Ribbon::MarkPumpedUp)].first  = 0x25B;
        ret[size_t(Ribbon::MarkPumpedUp)].second = 2;

        ret[size_t(Ribbon::MarkZeroEnergy)].first  = 0x25B;
        ret[size_t(Ribbon::MarkZeroEnergy)].second = 3;

        ret[size_t(Ribbon::MarkPrideful)].first  = 0x25B;
        ret[size_t(Ribbon::MarkPrideful)].second = 4;

        ret[size_t(Ribbon::MarkUnsure)].first  = 0x25B;
        ret[size_t(Ribbon::MarkUnsure)].second = 5;

        ret[size_t(Ribbon::MarkHumble)].first  = 0x25B;
        ret[size_t(Ribbon::MarkHumble)].second = 6;

        ret[size_t(Ribbon::MarkThorny)].first  = 0x25B;
        ret[size_t(Ribbon::MarkThorny)].second = 7;

        ret[size_t(Ribbon::MarkVigor)].first  = 0x25C;
        ret[size_t(Ribbon::MarkVigor)].second = 0;

        ret[size_t(Ribbon::MarkSlump)].first  = 0x25C;
        ret[size_t(Ribbon::MarkSlump)].second = 1;

        return ret;
    }
}

WC8::WC8(u8* dt)
{
    std::copy(dt, dt + length, data);
    for (int i = 0; i < 6; i++)
    {
        if (object(i) != 0)
        {
            numItems++;
        }
        else
            break;
    }
}

Generation WC8::generation() const
{
    return Generation::EIGHT;
}

u16 WC8::ID() const
{
    return LittleEndian::convertTo<u16>(data + 8);
}

std::string WC8::title() const
{
    return title(titleIndex());
}

std::string WC8::title(Language lang) const
{
    // TODO: use titleIndex to get offset, then get string from offset
    (void)lang;
    return "Wonder Card";
}

Language WC8::titleIndex() const
{
    return Language(data[0x15]);
}

u8 WC8::type() const
{
    return data[0x11];
}

u8 WC8::flags() const
{
    return data[0x10];
}

bool WC8::multiObtainable() const
{
    return (flags() & 1) == 1;
}

bool WC8::BP(void) const
{
    return type() == 3;
}

bool WC8::item(void) const
{
    return type() == 2;
}

bool WC8::pokemon(void) const
{
    return type() == 1;
}

bool WC8::clothing(void) const
{
    return type() == 4;
}

u16 WC8::object(void) const
{
    return object(0);
}

u16 WC8::object(int index) const
{
    return LittleEndian::convertTo<u16>(data + 0x20 + index * 4);
}

int WC8::items(void) const
{
    return numItems;
}

u16 WC8::objectQuantity(void) const
{
    return objectQuantity(0);
}

u16 WC8::objectQuantity(int index) const
{
    return LittleEndian::convertTo<u16>(data + 0x22 + index * 4);
}

u8 WC8::PIDType(void) const
{
    return data[0x248];
}

bool WC8::shiny(void) const
{
    return PIDType() == 2 || PIDType() == 3;
}

u16 WC8::TID(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x20);
}

u16 WC8::SID(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x22);
}

u8 WC8::version(void) const
{
    return LittleEndian::convertTo<u32>(data + 0x24);
}

u32 WC8::encryptionConstant(void) const
{
    return LittleEndian::convertTo<u32>(data + 0x28);
}

u32 WC8::PID(void) const
{
    return LittleEndian::convertTo<u32>(data + 0x2C);
}

u8 WC8::ball(void) const
{
    return data[0x22C];
}

u16 WC8::heldItem(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x22E);
}

u16 WC8::move(u8 m) const
{
    return LittleEndian::convertTo<u16>(data + 0x230 + m * 2);
}

u16 WC8::relearnMove(u8 index) const
{
    return LittleEndian::convertTo<u16>(data + 0x238 + index * 2);
}

u16 WC8::species(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x240);
}

u8 WC8::alternativeForm(void) const
{
    return data[0x242];
}

u8 WC8::gender(void) const
{
    return data[0x243];
}

u8 WC8::level(void) const
{
    return data[0x244];
}

bool WC8::egg(void) const
{
    return data[0x245] == 1;
}

u8 WC8::nature(void) const
{
    return data[0x246];
}

u8 WC8::abilityType(void) const
{
    return data[0x247];
}

u16 WC8::ability(void) const
{
    u8 abilitynum, type = abilityType();

    if (type == 2)
        abilitynum = 2;
    else if (type == 4)
        abilitynum = 2;
    else
        abilitynum = 0;

    return PersonalSWSH::ability(species(), abilitynum);
}

u8 WC8::metLevel(void) const
{
    return data[0x249];
}

u8 WC8::dynamaxLevel(void) const
{
    return data[0x24A];
}

bool WC8::canGigantamax() const
{
    return data[0x24B] != 0;
}

u8 WC8::ribbonValue(int index) const
{
    return data[0x24C + index];
}

u8 WC8::iv(Stat index) const
{
    return data[0x26C + u8(index)];
}

u8 WC8::otGender(void) const
{
    return data[0x272];
}

u8 WC8::ev(Stat index) const
{
    return data[0x273 + u8(index)];
}

u8 WC8::otIntensity(void) const
{
    return data[0x279];
}

u8 WC8::otMemory(void) const
{
    return data[0x27A];
}

u8 WC8::otFeeling(void) const
{
    return data[0x27B];
}

u16 WC8::otTextvar(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x27C);
}

u16 WC8::eggLocation(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x228);
}

u16 WC8::metLocation(void) const
{
    return LittleEndian::convertTo<u16>(data + 0x22A);
}

bool WC8::hasRibbon(Ribbon ribbon) const
{
    constexpr std::array<std::pair<size_t, size_t>, numRibbons()> offsets = OFFSETS();
    return offsets[size_t(ribbon)].first != 0xFFFFFFFF;
}

bool WC8::ribbon(Ribbon ribbon) const
{
    if (hasRibbon(ribbon))
    {
        constexpr std::array<std::pair<size_t, size_t>, numRibbons()> offsets = OFFSETS();
        return FlagUtil::getFlag(data, offsets[size_t(ribbon)].first, offsets[size_t(ribbon)].second);
    }
    return false;
}

bool WC8::nicknamed(Language lang) const
{
    return nickname(lang)[0] != 0;
}

std::string WC8::nickname(Language lang) const
{
    return StringUtils::getString(data, 0x30 + langIndex(lang) * 0x1C, 13);
}

std::string WC8::otName(Language lang) const
{
    return StringUtils::getString(data, 0x12C + langIndex(lang) * 0x1C, 13);
}

std::string WC8::nickname() const
{
    return nickname(Language::ENG);
}

std::string WC8::otName() const
{
    return otName(Language::ENG);
}

bool WC8::used() const
{
    return false;
}

u16 WC8::formSpecies(void) const
{
    u16 tmpSpecies = species();
    u8 form        = alternativeForm();
    u8 formcount   = PersonalSWSH::formCount(tmpSpecies);

    if (form && form < formcount)
    {
        u16 backSpecies = tmpSpecies;
        tmpSpecies      = PersonalSWSH::formStatIndex(tmpSpecies);
        if (!tmpSpecies)
        {
            tmpSpecies = backSpecies;
        }
        else
        {
            tmpSpecies += form - 1;
        }
    }

    return tmpSpecies;
}
