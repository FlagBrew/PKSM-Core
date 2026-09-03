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

#include "pkx/PK8.hpp"
#include "pkx/PA9.hpp"
#include "pkx/PK9.hpp"
#include "sav/Sav.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/utils.hpp"

#define RIBBON_ABSENT 0xFFFFFFFF

namespace
{
    std::pair<size_t, size_t> OFFSET_OF(pksm::Ribbon rib)
    {
        switch (rib)
        {
            case pksm::Ribbon::ChampionKalos:
                return {0x34, 0};
            case pksm::Ribbon::ChampionG3Hoenn:
                return {0x34, 1};
            case pksm::Ribbon::ChampionSinnoh:
                return {0x34, 2};
            case pksm::Ribbon::BestFriends:
                return {0x34, 3};
            case pksm::Ribbon::Training:
                return {0x34, 4};
            case pksm::Ribbon::BattlerSkillful:
                return {0x34, 5};
            case pksm::Ribbon::BattlerExpert:
                return {0x34, 6};
            case pksm::Ribbon::Effort:
                return {0x34, 7};
            case pksm::Ribbon::Alert:
                return {0x35, 0};
            case pksm::Ribbon::Shock:
                return {0x35, 1};
            case pksm::Ribbon::Downcast:
                return {0x35, 2};
            case pksm::Ribbon::Careless:
                return {0x35, 3};
            case pksm::Ribbon::Relax:
                return {0x35, 4};
            case pksm::Ribbon::Snooze:
                return {0x35, 5};
            case pksm::Ribbon::Smile:
                return {0x35, 6};
            case pksm::Ribbon::Gorgeous:
                return {0x35, 7};
            case pksm::Ribbon::Royal:
                return {0x36, 0};
            case pksm::Ribbon::GorgeousRoyal:
                return {0x36, 1};
            case pksm::Ribbon::Artist:
                return {0x36, 2};
            case pksm::Ribbon::Footprint:
                return {0x36, 3};
            case pksm::Ribbon::Record:
                return {0x36, 4};
            case pksm::Ribbon::Legend:
                return {0x36, 5};
            case pksm::Ribbon::Country:
                return {0x36, 6};
            case pksm::Ribbon::National:
                return {0x36, 7};
            case pksm::Ribbon::Earth:
                return {0x37, 0};
            case pksm::Ribbon::World:
                return {0x37, 1};
            case pksm::Ribbon::Classic:
                return {0x37, 2};
            case pksm::Ribbon::Premier:
                return {0x37, 3};
            case pksm::Ribbon::Event:
                return {0x37, 4};
            case pksm::Ribbon::Birthday:
                return {0x37, 5};
            case pksm::Ribbon::Special:
                return {0x37, 6};
            case pksm::Ribbon::Souvenir:
                return {0x37, 7};
            case pksm::Ribbon::Wishing:
                return {0x38, 0};
            case pksm::Ribbon::ChampionBattle:
                return {0x38, 1};
            case pksm::Ribbon::ChampionRegional:
                return {0x38, 2};
            case pksm::Ribbon::ChampionNational:
                return {0x38, 3};
            case pksm::Ribbon::ChampionWorld:
                return {0x38, 4};
            case pksm::Ribbon::MemoryContest:
                return {0x38, 5};
            case pksm::Ribbon::MemoryBattle:
                return {0x38, 6};
            case pksm::Ribbon::ChampionG6Hoenn:
                return {0x38, 7};
            case pksm::Ribbon::ContestStar:
                return {0x39, 0};
            case pksm::Ribbon::MasterCoolness:
                return {0x39, 1};
            case pksm::Ribbon::MasterBeauty:
                return {0x39, 2};
            case pksm::Ribbon::MasterCuteness:
                return {0x39, 3};
            case pksm::Ribbon::MasterCleverness:
                return {0x39, 4};
            case pksm::Ribbon::MasterToughness:
                return {0x39, 5};
            case pksm::Ribbon::ChampionAlola:
                return {0x39, 6};
            case pksm::Ribbon::BattleRoyale:
                return {0x39, 7};
            case pksm::Ribbon::BattleTreeGreat:
                return {0x3A, 0};
            case pksm::Ribbon::BattleTreeMaster:
                return {0x3A, 1};
            case pksm::Ribbon::ChampionGalar:
                return {0x3A, 2};
            case pksm::Ribbon::TowerMaster:
                return {0x3A, 3};
            case pksm::Ribbon::MasterRank:
                return {0x3A, 4};
            case pksm::Ribbon::MarkLunchtime:
                return {0x3A, 5};
            case pksm::Ribbon::MarkSleepyTime:
                return {0x3A, 6};
            case pksm::Ribbon::MarkDusk:
                return {0x3A, 7};
            case pksm::Ribbon::MarkDawn:
                return {0x3B, 0};
            case pksm::Ribbon::MarkCloudy:
                return {0x3B, 1};
            case pksm::Ribbon::MarkRainy:
                return {0x3B, 2};
            case pksm::Ribbon::MarkStormy:
                return {0x3B, 3};
            case pksm::Ribbon::MarkSnowy:
                return {0x3B, 4};
            case pksm::Ribbon::MarkBlizzard:
                return {0x3B, 5};
            case pksm::Ribbon::MarkDry:
                return {0x3B, 6};
            case pksm::Ribbon::MarkSandstorm:
                return {0x3B, 7};
            case pksm::Ribbon::MarkMisty:
                return {0x40, 0};
            case pksm::Ribbon::MarkDestiny:
                return {0x40, 1};
            case pksm::Ribbon::MarkFishing:
                return {0x40, 2};
            case pksm::Ribbon::MarkCurry:
                return {0x40, 3};
            case pksm::Ribbon::MarkUncommon:
                return {0x40, 4};
            case pksm::Ribbon::MarkRare:
                return {0x40, 5};
            case pksm::Ribbon::MarkRowdy:
                return {0x40, 6};
            case pksm::Ribbon::MarkAbsentMinded:
                return {0x40, 7};
            case pksm::Ribbon::MarkJittery:
                return {0x41, 0};
            case pksm::Ribbon::MarkExcited:
                return {0x41, 1};
            case pksm::Ribbon::MarkCharismatic:
                return {0x41, 2};
            case pksm::Ribbon::MarkCalmness:
                return {0x41, 3};
            case pksm::Ribbon::MarkIntense:
                return {0x41, 4};
            case pksm::Ribbon::MarkZonedOut:
                return {0x41, 5};
            case pksm::Ribbon::MarkJoyful:
                return {0x41, 6};
            case pksm::Ribbon::MarkAngry:
                return {0x41, 7};
            case pksm::Ribbon::MarkSmiley:
                return {0x42, 0};
            case pksm::Ribbon::MarkTeary:
                return {0x42, 1};
            case pksm::Ribbon::MarkUpbeat:
                return {0x42, 2};
            case pksm::Ribbon::MarkPeeved:
                return {0x42, 3};
            case pksm::Ribbon::MarkIntellectual:
                return {0x42, 4};
            case pksm::Ribbon::MarkFerocious:
                return {0x42, 5};
            case pksm::Ribbon::MarkCrafty:
                return {0x42, 6};
            case pksm::Ribbon::MarkScowling:
                return {0x42, 7};
            case pksm::Ribbon::MarkKindly:
                return {0x43, 0};
            case pksm::Ribbon::MarkFlustered:
                return {0x43, 1};
            case pksm::Ribbon::MarkPumpedUp:
                return {0x43, 2};
            case pksm::Ribbon::MarkZeroEnergy:
                return {0x43, 3};
            case pksm::Ribbon::MarkPrideful:
                return {0x43, 4};
            case pksm::Ribbon::MarkUnsure:
                return {0x43, 5};
            case pksm::Ribbon::MarkHumble:
                return {0x43, 6};
            case pksm::Ribbon::MarkThorny:
                return {0x43, 7};
            case pksm::Ribbon::MarkVigor:
                return {0x44, 0};
            case pksm::Ribbon::MarkSlump:
                return {0x44, 1};

            default:
                return {RIBBON_ABSENT, 0};
        }
    }
}

namespace pksm
{
    void PK8::encrypt(void)
    {
        if (!isEncrypted())
        {
            u8 sv = (encryptionConstant() >> 13) & 31;
            refreshChecksum();
            pksm::crypto::pkm::blockShuffle<BLOCK_LENGTH>(
                data + ENCRYPTION_START, pksm::crypto::pkm::InvertedBlockPositions[sv]);
            pksm::crypto::pkm::crypt<BOX_LENGTH - ENCRYPTION_START>(
                data + ENCRYPTION_START, encryptionConstant());
            if (isParty())
            {
                pksm::crypto::pkm::crypt<PARTY_LENGTH - BOX_LENGTH>(
                    data + BOX_LENGTH, encryptionConstant());
            }
        }
    }

    void PK8::decrypt(void)
    {
        if (isEncrypted())
        {
            u8 sv = (encryptionConstant() >> 13) & 31;
            pksm::crypto::pkm::crypt<BOX_LENGTH - ENCRYPTION_START>(
                data + ENCRYPTION_START, encryptionConstant());
            if (isParty())
            {
                pksm::crypto::pkm::crypt<PARTY_LENGTH - BOX_LENGTH>(
                    data + BOX_LENGTH, encryptionConstant());
            }
            pksm::crypto::pkm::blockShuffle<BLOCK_LENGTH>(data + ENCRYPTION_START, sv);
        }
    }

    bool PK8::isEncrypted() const
    {
        return LittleEndian::convertTo<u16>(data + 0x70) != 0 &&
               LittleEndian::convertTo<u16>(data + 0xC0) != 0;
    }

    PK8::PK8(PrivateConstructor, u8* dt, bool party, bool direct)
        : PKX(dt, party ? PARTY_LENGTH : BOX_LENGTH, direct)
    {
        if (isEncrypted())
        {
            decrypt();
        }
    }

    std::unique_ptr<PKX> PK8::clone(void) const
    {
        return PKX::getPKM<Generation::EIGHT>(data, isParty() ? PARTY_LENGTH : BOX_LENGTH);
    }

    std::unique_ptr<PKX> PK8::convertToG9(Sav& save) const
    {
        bool targetIsZA = (save.version() == GameVersion::ZA);

        std::unique_ptr<PKX> pk9;
        if (targetIsZA)
        {
            pk9 = PKX::getPKM<PA9>(nullptr, PA9::BOX_LENGTH);
        }
        else
        {
            pk9 = PKX::getPKM<PK9>(nullptr, PK9::BOX_LENGTH);
        }

        pk9->encryptionConstant(encryptionConstant());
        pk9->species(species());
        pk9->TID(TID());
        pk9->SID(SID());
        pk9->experience(experience());
        pk9->PID(PID());

        if (ability() == PersonalSWSH::ability(formSpecies(), abilityNumber() >> 1))
        {
            pk9->setAbility(abilityNumber() >> 1);
        }
        else
        {
            pk9->ability(ability());
            pk9->abilityNumber(abilityNumber());
        }

        pk9->language(language());
        pk9->heldItem(heldItem());
        pk9->markValue(markValue());

        for (Stat stat : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPATK, Stat::SPDEF, Stat::SPD})
        {
            pk9->ev(stat, ev(stat));
            pk9->iv(stat, iv(stat));
            pk9->hyperTrain(stat, hyperTrain(stat));
        }

        for (size_t i = 0; i < 4; i++)
        {
            pk9->move(i, move(i));
            pk9->PPUp(i, PPUp(i));
            pk9->PP(i, PP(i));
            pk9->relearnMove(i, relearnMove(i));
        }

        pk9->egg(egg());
        pk9->nicknamed(nicknamed());
        pk9->nickname(nickname());
        pk9->fatefulEncounter(fatefulEncounter());
        pk9->gender(gender());
        pk9->otGender(otGender());
        pk9->alternativeForm(alternativeForm());
        pk9->nature(origNature());

        pk9->version(version());
        pk9->otName(otName());
        pk9->otFriendship(otFriendship());
        pk9->metDate(metDate());
        pk9->eggDate(eggDate());
        pk9->metLocation(metLocation());
        pk9->eggLocation(eggLocation());
        pk9->ball(ball());
        pk9->metLevel(metLevel());
        pk9->currentHandler(currentHandler());
        pk9->htFriendship(htFriendship());

        pk9->pkrsStrain(pkrsStrain());
        pk9->pkrsDays(pkrsDays());

        for (size_t i = 0; i < 6; i++)
        {
            pk9->contest(i, contest(i));
        }

        pk9->ribbon(Ribbon::ChampionKalos, ribbon(Ribbon::ChampionKalos));
        pk9->ribbon(Ribbon::ChampionG3Hoenn, ribbon(Ribbon::ChampionG3Hoenn));
        pk9->ribbon(Ribbon::ChampionSinnoh, ribbon(Ribbon::ChampionSinnoh));
        pk9->ribbon(Ribbon::BestFriends, ribbon(Ribbon::BestFriends));
        pk9->ribbon(Ribbon::Training, ribbon(Ribbon::Training));
        pk9->ribbon(Ribbon::BattlerSkillful, ribbon(Ribbon::BattlerSkillful));
        pk9->ribbon(Ribbon::BattlerExpert, ribbon(Ribbon::BattlerExpert));
        pk9->ribbon(Ribbon::Effort, ribbon(Ribbon::Effort));
        pk9->ribbon(Ribbon::Alert, ribbon(Ribbon::Alert));
        pk9->ribbon(Ribbon::Shock, ribbon(Ribbon::Shock));
        pk9->ribbon(Ribbon::Downcast, ribbon(Ribbon::Downcast));
        pk9->ribbon(Ribbon::Careless, ribbon(Ribbon::Careless));
        pk9->ribbon(Ribbon::Relax, ribbon(Ribbon::Relax));
        pk9->ribbon(Ribbon::Snooze, ribbon(Ribbon::Snooze));
        pk9->ribbon(Ribbon::Smile, ribbon(Ribbon::Smile));
        pk9->ribbon(Ribbon::Gorgeous, ribbon(Ribbon::Gorgeous));
        pk9->ribbon(Ribbon::Royal, ribbon(Ribbon::Royal));
        pk9->ribbon(Ribbon::GorgeousRoyal, ribbon(Ribbon::GorgeousRoyal));
        pk9->ribbon(Ribbon::Artist, ribbon(Ribbon::Artist));
        pk9->ribbon(Ribbon::Footprint, ribbon(Ribbon::Footprint));
        pk9->ribbon(Ribbon::Record, ribbon(Ribbon::Record));
        pk9->ribbon(Ribbon::Legend, ribbon(Ribbon::Legend));
        pk9->ribbon(Ribbon::Country, ribbon(Ribbon::Country));
        pk9->ribbon(Ribbon::National, ribbon(Ribbon::National));
        pk9->ribbon(Ribbon::Earth, ribbon(Ribbon::Earth));
        pk9->ribbon(Ribbon::World, ribbon(Ribbon::World));
        pk9->ribbon(Ribbon::Classic, ribbon(Ribbon::Classic));
        pk9->ribbon(Ribbon::Premier, ribbon(Ribbon::Premier));
        pk9->ribbon(Ribbon::Event, ribbon(Ribbon::Event));
        pk9->ribbon(Ribbon::Birthday, ribbon(Ribbon::Birthday));
        pk9->ribbon(Ribbon::Special, ribbon(Ribbon::Special));
        pk9->ribbon(Ribbon::Souvenir, ribbon(Ribbon::Souvenir));
        pk9->ribbon(Ribbon::Wishing, ribbon(Ribbon::Wishing));
        pk9->ribbon(Ribbon::ChampionBattle, ribbon(Ribbon::ChampionBattle));
        pk9->ribbon(Ribbon::ChampionRegional, ribbon(Ribbon::ChampionRegional));
        pk9->ribbon(Ribbon::ChampionNational, ribbon(Ribbon::ChampionNational));
        pk9->ribbon(Ribbon::ChampionWorld, ribbon(Ribbon::ChampionWorld));
        pk9->ribbon(Ribbon::MemoryContest, ribbon(Ribbon::MemoryContest));
        pk9->ribbon(Ribbon::MemoryBattle, ribbon(Ribbon::MemoryBattle));
        pk9->ribbon(Ribbon::ChampionG6Hoenn, ribbon(Ribbon::ChampionG6Hoenn));
        pk9->ribbon(Ribbon::ContestStar, ribbon(Ribbon::ContestStar));
        pk9->ribbon(Ribbon::MasterCoolness, ribbon(Ribbon::MasterCoolness));
        pk9->ribbon(Ribbon::MasterBeauty, ribbon(Ribbon::MasterBeauty));
        pk9->ribbon(Ribbon::MasterCuteness, ribbon(Ribbon::MasterCuteness));
        pk9->ribbon(Ribbon::MasterCleverness, ribbon(Ribbon::MasterCleverness));
        pk9->ribbon(Ribbon::MasterToughness, ribbon(Ribbon::MasterToughness));
        pk9->ribbon(Ribbon::ChampionAlola, ribbon(Ribbon::ChampionAlola));
        pk9->ribbon(Ribbon::BattleRoyale, ribbon(Ribbon::BattleRoyale));
        pk9->ribbon(Ribbon::BattleTreeGreat, ribbon(Ribbon::BattleTreeGreat));
        pk9->ribbon(Ribbon::BattleTreeMaster, ribbon(Ribbon::BattleTreeMaster));
        pk9->ribbon(Ribbon::ChampionGalar, ribbon(Ribbon::ChampionGalar));
        pk9->ribbon(Ribbon::TowerMaster, ribbon(Ribbon::TowerMaster));
        pk9->ribbon(Ribbon::MasterRank, ribbon(Ribbon::MasterRank));
        pk9->ribbon(Ribbon::MarkLunchtime, ribbon(Ribbon::MarkLunchtime));
        pk9->ribbon(Ribbon::MarkSleepyTime, ribbon(Ribbon::MarkSleepyTime));
        pk9->ribbon(Ribbon::MarkDusk, ribbon(Ribbon::MarkDusk));
        pk9->ribbon(Ribbon::MarkDawn, ribbon(Ribbon::MarkDawn));
        pk9->ribbon(Ribbon::MarkCloudy, ribbon(Ribbon::MarkCloudy));
        pk9->ribbon(Ribbon::MarkRainy, ribbon(Ribbon::MarkRainy));
        pk9->ribbon(Ribbon::MarkStormy, ribbon(Ribbon::MarkStormy));
        pk9->ribbon(Ribbon::MarkSnowy, ribbon(Ribbon::MarkSnowy));
        pk9->ribbon(Ribbon::MarkBlizzard, ribbon(Ribbon::MarkBlizzard));
        pk9->ribbon(Ribbon::MarkDry, ribbon(Ribbon::MarkDry));
        pk9->ribbon(Ribbon::MarkSandstorm, ribbon(Ribbon::MarkSandstorm));
        pk9->ribbon(Ribbon::MarkMisty, ribbon(Ribbon::MarkMisty));
        pk9->ribbon(Ribbon::MarkDestiny, ribbon(Ribbon::MarkDestiny));
        pk9->ribbon(Ribbon::MarkFishing, ribbon(Ribbon::MarkFishing));
        pk9->ribbon(Ribbon::MarkCurry, ribbon(Ribbon::MarkCurry));
        pk9->ribbon(Ribbon::MarkUncommon, ribbon(Ribbon::MarkUncommon));
        pk9->ribbon(Ribbon::MarkRare, ribbon(Ribbon::MarkRare));
        pk9->ribbon(Ribbon::MarkRowdy, ribbon(Ribbon::MarkRowdy));
        pk9->ribbon(Ribbon::MarkAbsentMinded, ribbon(Ribbon::MarkAbsentMinded));
        pk9->ribbon(Ribbon::MarkJittery, ribbon(Ribbon::MarkJittery));
        pk9->ribbon(Ribbon::MarkExcited, ribbon(Ribbon::MarkExcited));
        pk9->ribbon(Ribbon::MarkCharismatic, ribbon(Ribbon::MarkCharismatic));
        pk9->ribbon(Ribbon::MarkCalmness, ribbon(Ribbon::MarkCalmness));
        pk9->ribbon(Ribbon::MarkIntense, ribbon(Ribbon::MarkIntense));
        pk9->ribbon(Ribbon::MarkZonedOut, ribbon(Ribbon::MarkZonedOut));
        pk9->ribbon(Ribbon::MarkJoyful, ribbon(Ribbon::MarkJoyful));
        pk9->ribbon(Ribbon::MarkAngry, ribbon(Ribbon::MarkAngry));
        pk9->ribbon(Ribbon::MarkSmiley, ribbon(Ribbon::MarkSmiley));
        pk9->ribbon(Ribbon::MarkTeary, ribbon(Ribbon::MarkTeary));
        pk9->ribbon(Ribbon::MarkUpbeat, ribbon(Ribbon::MarkUpbeat));
        pk9->ribbon(Ribbon::MarkPeeved, ribbon(Ribbon::MarkPeeved));
        pk9->ribbon(Ribbon::MarkIntellectual, ribbon(Ribbon::MarkIntellectual));
        pk9->ribbon(Ribbon::MarkFerocious, ribbon(Ribbon::MarkFerocious));
        pk9->ribbon(Ribbon::MarkCrafty, ribbon(Ribbon::MarkCrafty));
        pk9->ribbon(Ribbon::MarkScowling, ribbon(Ribbon::MarkScowling));
        pk9->ribbon(Ribbon::MarkKindly, ribbon(Ribbon::MarkKindly));
        pk9->ribbon(Ribbon::MarkFlustered, ribbon(Ribbon::MarkFlustered));
        pk9->ribbon(Ribbon::MarkPumpedUp, ribbon(Ribbon::MarkPumpedUp));
        pk9->ribbon(Ribbon::MarkZeroEnergy, ribbon(Ribbon::MarkZeroEnergy));
        pk9->ribbon(Ribbon::MarkPrideful, ribbon(Ribbon::MarkPrideful));
        pk9->ribbon(Ribbon::MarkUnsure, ribbon(Ribbon::MarkUnsure));
        pk9->ribbon(Ribbon::MarkHumble, ribbon(Ribbon::MarkHumble));
        pk9->ribbon(Ribbon::MarkThorny, ribbon(Ribbon::MarkThorny));
        pk9->ribbon(Ribbon::MarkVigor, ribbon(Ribbon::MarkVigor));
        pk9->ribbon(Ribbon::MarkSlump, ribbon(Ribbon::MarkSlump));

        // PK9-specific: set TeraType from PersonalSV types
        if (!targetIsZA)
        {
            auto* pk9sv = static_cast<PK9*>(pk9.get());
            auto type1  = PersonalSV::type1(pk9->formSpecies());
            u8 tera     = (type1 != Type::Normal) ? u8(type1)
                                                  : u8(PersonalSV::type2(pk9->formSpecies()));
            pk9sv->teraTypeOriginal(tera);
            pk9sv->teraTypeOverride(tera);
        }

        pk9->refreshChecksum();

        return pk9;
    }

    Generation PK8::generation(void) const
    {
        return Generation::EIGHT;
    }

    u32 PK8::encryptionConstant(void) const
    {
        return LittleEndian::convertTo<u32>(data);
    }

    void PK8::encryptionConstant(u32 v)
    {
        LittleEndian::convertFrom<u32>(data, v);
    }

    u16 PK8::sanity(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x04);
    }

    void PK8::sanity(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x04, v);
    }

    u16 PK8::checksum(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x06);
    }

    void PK8::checksum(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x06, v);
    }

    Species PK8::species(void) const
    {
        return Species{LittleEndian::convertTo<u16>(data + 0x08)};
    }

    void PK8::species(Species v)
    {
        LittleEndian::convertFrom<u16>(data + 0x08, u16(v));
    }

    u16 PK8::heldItem(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0A);
    }

    void PK8::heldItem(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0A, v);
    }

    u16 PK8::TID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0C);
    }

    void PK8::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0C, v);
    }

    u16 PK8::SID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0E);
    }

    void PK8::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0E, v);
    }

    u32 PK8::experience(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x10);
    }

    void PK8::experience(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x10, v);
    }

    Ability PK8::ability(void) const
    {
        return Ability{LittleEndian::convertTo<u16>(data + 0x14)};
    }

    void PK8::ability(Ability v)
    {
        LittleEndian::convertFrom<u16>(data + 0x14, u16(v));
    }

    void PK8::setAbility(u8 v)
    {
        u8 abilitynum;

        if (v == 0)
        {
            abilitynum = 1;
        }
        else if (v == 1)
        {
            abilitynum = 2;
        }
        else
        {
            abilitynum = 4;
        }

        abilityNumber(abilitynum);
        ability(abilities(v));
    }

    u8 PK8::abilityNumber(void) const
    {
        return data[0x16] & 0x7;
    }

    void PK8::abilityNumber(u8 v)
    {
        data[0x16] = (data[0x16] & ~7) | (v & 7);
    }

    bool PK8::favorite(void) const
    {
        return (data[0x16] & 8) != 0;
    }

    void PK8::favorite(bool v) const
    {
        data[0x16] = (data[0x16] & ~8) | (v ? 8 : 0);
    }

    bool PK8::canGiga(void) const
    {
        return (data[0x16] & 16) != 0;
    }

    void PK8::canGiga(bool v) const
    {
        data[0x16] = (data[0x16] & ~16) | (v ? 16 : 0);
    }

    u16 PK8::markValue(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18);
    }

    void PK8::markValue(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x18, v);
    }

    u32 PK8::PID(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x1C);
    }

    void PK8::PID(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x1C, v);
    }

    Nature PK8::origNature(void) const
    {
        return Nature{data[0x20]};
    }

    void PK8::origNature(Nature v)
    {
        data[0x20] = u8(v);
    }

    Nature PK8::nature(void) const
    {
        return Nature{data[0x21]};
    }

    void PK8::nature(Nature v)
    {
        data[0x21] = u8(v);
    }

    bool PK8::fatefulEncounter(void) const
    {
        return (data[0x22] & 1) == 1;
    }

    void PK8::fatefulEncounter(bool v)
    {
        data[0x22] = (u8)((data[0x22] & ~0x01) | (v ? 1 : 0));
    }

    /*
    bool PK8::data22flag2(void) const
    {
        return (data[0x22] & 2) == 1;
    }
    void PK8::data22flag2(bool v)
    {
        data[0x22] = (data[0x22] & ~2) | (v ? 2 : 0);
    }
    */

    Gender PK8::gender(void) const
    {
        return Gender{u8((data[0x22] >> 2) & 0x3)};
    }

    void PK8::gender(Gender v)
    {
        data[0x22] = (data[0x22] & ~12) | ((u8(v) & 3) << 2);
    }

    u16 PK8::alternativeForm(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x24);
    }

    void PK8::alternativeForm(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x24, v);
    }

    u16 PK8::ev(Stat ev) const
    {
        return data[0x26 + u8(ev)];
    }

    void PK8::ev(Stat ev, u16 v)
    {
        data[0x26 + u8(ev)] = v;
    }

    u8 PK8::contest(u8 contest) const
    {
        return data[0x2C + contest];
    }

    void PK8::contest(u8 contest, u8 v)
    {
        data[0x2C + contest] = v;
    }

    u8 PK8::pkrs(void) const
    {
        return data[0x32];
    }

    void PK8::pkrs(u8 v)
    {
        data[0x32] = v;
    }

    u8 PK8::pkrsDays(void) const
    {
        return data[0x32] & 0xF;
    }

    void PK8::pkrsDays(u8 v)
    {
        data[0x32] = (data[0x32] & ~0xF) | (v & 0xF);
    }

    u8 PK8::pkrsStrain(void) const
    {
        return data[0x32] >> 4;
    }

    void PK8::pkrsStrain(u8 v)
    {
        data[0x32] = (data[0x2B] & 0xF) | (v << 4);
    }

    bool PK8::hasRibbon(Ribbon ribbon) const
    {
        return OFFSET_OF(ribbon).first != RIBBON_ABSENT;
    }

    bool PK8::ribbon(Ribbon ribbon) const
    {
        auto offset = OFFSET_OF(ribbon);
        if (offset.first != RIBBON_ABSENT)
        {
            return FlagUtil::getFlag(data, offset.first, offset.second);
        }
        return false;
    }

    void PK8::ribbon(Ribbon ribbon, bool v)
    {
        auto offset = OFFSET_OF(ribbon);
        if (offset.first != RIBBON_ABSENT)
        {
            FlagUtil::setFlag(data, offset.first, offset.second, v);
        }
    }

    u8 PK8::ribbonContestCount(void) const
    {
        return data[0x3C];
    }

    void PK8::ribbonContestCount(u8 v)
    {
        data[0x3C] = v;
        if (v)
        {
            // Bitflag for having at least one
            ribbon(Ribbon::MemoryContest, 1);
        }
        else
        {
            // Bitflag for having at least one
            ribbon(Ribbon::MemoryContest, 0);
        }
    }

    u8 PK8::ribbonBattleCount(void) const
    {
        return data[0x3D];
    }

    void PK8::ribbonBattleCount(u8 v)
    {
        data[0x3D] = v;
        if (v)
        {
            // Bitflag for having at least one
            ribbon(Ribbon::MemoryBattle, 1);
        }
        else
        {
            // Bitflag for having at least one
            ribbon(Ribbon::MemoryBattle, 0);
        }
    }

    u8 PK8::height(void) const
    {
        return data[0x50];
    }

    void PK8::height(u8 v)
    {
        data[0x50] = v;
    }

    u8 PK8::weight(void) const
    {
        return data[0x51];
    }

    void PK8::weight(u8 v)
    {
        data[0x51] = v;
    }

    std::string PK8::nickname(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0x58, 13));
    }

    void PK8::nickname(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0x58, 13);
    }

    Move PK8::move(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x72 + m * 2)};
    }

    void PK8::move(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x72 + m * 2, u16(v));
    }

    u8 PK8::PP(u8 m) const
    {
        return data[0x7A + m];
    }

    void PK8::PP(u8 m, u8 v)
    {
        data[0x7A + m] = v;
    }

    u8 PK8::PPUp(u8 m) const
    {
        return data[0x7E + m];
    }

    void PK8::PPUp(u8 m, u8 v)
    {
        data[0x7E + m] = v;
    }

    Move PK8::relearnMove(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x82 + m * 2)};
    }

    void PK8::relearnMove(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x82 + m * 2, u16(v));
    }

    int PK8::partyCurrHP(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x8A);
    }

    void PK8::partyCurrHP(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x8A, v);
    }

    u8 PK8::iv(Stat stat) const
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x8C);
        return (u8)((buffer >> 5 * u8(stat)) & 0x1F);
    }

    void PK8::iv(Stat stat, u8 v)
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x8C);
        buffer    &= ~(0x1F << 5 * u8(stat));
        buffer    |= v << (5 * u8(stat));
        LittleEndian::convertFrom<u32>(data + 0x8C, buffer);
    }

    bool PK8::egg(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x8C) >> 30) & 0x1) == 1;
    }

    void PK8::egg(bool v)
    {
        LittleEndian::convertFrom<u32>(
            data + 0x8C, (u32)((LittleEndian::convertTo<u32>(data + 0x8C) & ~0x40000000) |
                               (u32)(v ? 0x40000000 : 0)));
    }

    bool PK8::nicknamed(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x8C) >> 31) & 0x1) == 1;
    }

    void PK8::nicknamed(bool v)
    {
        LittleEndian::convertFrom<u32>(data + 0x8C,
            (LittleEndian::convertTo<u32>(data + 0x8C) & 0x7FFFFFFF) | (v ? 0x80000000 : 0));
    }

    u8 PK8::dynamaxLevel(void) const
    {
        return data[0x90];
    }

    void PK8::dynamaxLevel(u8 v)
    {
        data[0x90] = v;
    }

    std::string PK8::htName(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0xA8, 13));
    }

    void PK8::htName(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0xA8, 13);
    }

    Gender PK8::htGender(void) const
    {
        return Gender{data[0xC2]};
    }

    void PK8::htGender(Gender v)
    {
        data[0xC2] = u8(v);
    }

    Language PK8::htLanguage(void) const
    {
        return Language(data[0xC3]);
    }

    void PK8::htLanguage(Language lang)
    {
        data[0xC3] = u8(lang);
    }

    PKXHandler PK8::currentHandler(void) const
    {
        return data[0xC4] == 0 ? PKXHandler::OT : PKXHandler::NonOT;
    }

    void PK8::currentHandler(PKXHandler v)
    {
        data[0xC4] = v == PKXHandler::OT ? 0 : 1;
    }

    u16 PK8::htID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0xC6);
    }

    void PK8::htID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0xC6, v);
    }

    u8 PK8::htFriendship(void) const
    {
        return data[0xC8];
    }

    void PK8::htFriendship(u8 v)
    {
        data[0xC8] = v;
    }

    u8 PK8::htIntensity(void) const
    {
        return data[0xC9];
    }

    void PK8::htIntensity(u8 v)
    {
        data[0xC9] = v;
    }

    u8 PK8::htMemory(void) const
    {
        return data[0xCA];
    }

    void PK8::htMemory(u8 v)
    {
        data[0xCA] = v;
    }

    u8 PK8::htFeeling(void) const
    {
        return data[0xCB];
    }

    void PK8::htFeeling(u8 v)
    {
        data[0xCB] = v;
    }

    u16 PK8::htTextVar(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0xCC);
    }

    void PK8::htTextVar(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0xCC, v);
    }

    u8 PK8::fullness(void) const
    {
        return data[0xDC];
    }

    void PK8::fullness(u8 v)
    {
        data[0xDC] = v;
    }

    u8 PK8::enjoyment(void) const
    {
        return data[0xDD];
    }

    void PK8::enjoyment(u8 v)
    {
        data[0xDD] = v;
    }

    GameVersion PK8::version(void) const
    {
        return GameVersion(data[0xDE]);
    }

    void PK8::version(GameVersion v)
    {
        data[0xDE] = u8(v);
    }

    u8 PK8::battleVersion(void) const
    {
        return data[0xDF];
    }

    void PK8::battleVersion(u8 v)
    {
        data[0xDF] = v;
    }

    Language PK8::language(void) const
    {
        return Language(data[0xE2]);
    }

    void PK8::language(Language v)
    {
        data[0xE2] = u8(v);
    }

    u32 PK8::formDuration(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0xE4);
    }

    void PK8::formDuration(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0xE4, v);
    }

    s8 PK8::favRibbon(void) const
    {
        return data[0xE8];
    }

    void PK8::favRibbon(s8 v)
    {
        data[0xE8] = v;
    }

    std::string PK8::otName(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0xF8, 13));
    }

    void PK8::otName(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0xF8, 13);
    }

    u8 PK8::otFriendship(void) const
    {
        return data[0x112];
    }

    void PK8::otFriendship(u8 v)
    {
        data[0x112] = v;
    }

    u8 PK8::otIntensity(void) const
    {
        return data[0x113];
    }

    void PK8::otIntensity(u8 v)
    {
        data[0x113] = v;
    }

    u8 PK8::otMemory(void) const
    {
        return data[0x114];
    }

    void PK8::otMemory(u8 v)
    {
        data[0x114] = v;
    }

    u16 PK8::otTextVar(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x116);
    }

    void PK8::otTextVar(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x116, v);
    }

    u8 PK8::otFeeling(void) const
    {
        return data[0x118];
    }

    void PK8::otFeeling(u8 v)
    {
        data[0x118] = v;
    }

    int PK8::eggYear(void) const
    {
        return 2000 + data[0x119];
    }

    void PK8::eggYear(int v)
    {
        data[0x119] = v - 2000;
    }

    int PK8::eggMonth(void) const
    {
        return data[0x11A];
    }

    void PK8::eggMonth(int v)
    {
        data[0x11A] = v;
    }

    int PK8::eggDay(void) const
    {
        return data[0x11B];
    }

    void PK8::eggDay(int v)
    {
        data[0x11B] = v;
    }

    int PK8::metYear(void) const
    {
        return 2000 + data[0x11C];
    }

    void PK8::metYear(int v)
    {
        data[0x11C] = v - 2000;
    }

    int PK8::metMonth(void) const
    {
        return data[0x11D];
    }

    void PK8::metMonth(int v)
    {
        data[0x11D] = v;
    }

    int PK8::metDay(void) const
    {
        return data[0x11E];
    }

    void PK8::metDay(int v)
    {
        data[0x11E] = v;
    }

    u16 PK8::eggLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x120);
    }

    void PK8::eggLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x120, v);
    }

    u16 PK8::metLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x122);
    }

    void PK8::metLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x122, v);
    }

    Ball PK8::ball(void) const
    {
        return Ball{data[0x124]};
    }

    void PK8::ball(Ball v)
    {
        data[0x124] = u8(v);
    }

    u8 PK8::metLevel(void) const
    {
        return data[0x125] & ~0x80;
    }

    void PK8::metLevel(u8 v)
    {
        data[0x125] = (data[0x125] & 0x80) | v;
    }

    Gender PK8::otGender(void) const
    {
        return Gender{u8(data[0x125] >> 7)};
    }

    void PK8::otGender(Gender v)
    {
        data[0x125] = (data[0x125] & ~0x80) | (u8(v) << 7);
    }

    bool PK8::hyperTrain(Stat stat) const
    {
        return (data[0x126] & (1 << hyperTrainLookup[size_t(stat)])) ==
               1 << hyperTrainLookup[size_t(stat)];
    }

    void PK8::hyperTrain(Stat stat, bool v)
    {
        data[0x126] = (u8)((data[0x126] & ~(1 << hyperTrainLookup[size_t(stat)])) |
                           (v ? 1 << hyperTrainLookup[size_t(stat)] : 0));
    }

    bool PK8::moveRecordFlag(u8 index) const
    {
        return (*(data + (index >> 3)) & (index & 7)) == 1;
    }

    void PK8::moveRecordFlag(u8 index, bool v)
    {
        *(data + (index >> 3)) =
            (*(data + (index >> 3)) & ~(index & 7)) | ((v ? 1 : 0) << (index & 7));
    }

    u64 PK8::homeTracker(void) const
    {
        return LittleEndian::convertTo<u64>(data + 0x135);
    }

    void PK8::homeTracker(u64 v)
    {
        LittleEndian::convertFrom<u64>(data + 0x135, v);
    }

    int PK8::partyStat(Stat stat) const
    {
        if (!isParty())
        {
            return -1;
        }
        return LittleEndian::convertTo<u16>(data + 0x14A + u8(stat) * 2);
    }

    void PK8::partyStat(Stat stat, u16 v)
    {
        if (isParty())
        {
            LittleEndian::convertFrom<u16>(data + 0x14A + u8(stat) * 2, v);
        }
    }

    int PK8::partyLevel() const
    {
        if (!isParty())
        {
            return -1;
        }
        return *(data + 0x148);
    }

    void PK8::partyLevel(u8 v)
    {
        if (isParty())
        {
            *(data + 0x148) = v;
        }
    }

    u16 PK8::dynamaxType(void) const
    {
        if (!isParty())
        {
            return 0;
        }
        return LittleEndian::convertTo<u16>(data + 0x156);
    }

    void PK8::dynamaxType(u16 v)
    {
        if (isParty())
        {
            LittleEndian::convertFrom<u16>(data + 0x156, v);
        }
    }

    void PK8::refreshChecksum(void)
    {
        u16 chk = 0;
        for (size_t i = 8; i < BOX_LENGTH; i += 2)
        {
            chk += LittleEndian::convertTo<u16>(data + i);
        }
        checksum(chk);
    }

    Type PK8::hpType(void) const
    {
        return Type{u8((15 *
                           ((iv(Stat::HP) & 1) + 2 * (iv(Stat::ATK) & 1) + 4 * (iv(Stat::DEF) & 1) +
                               8 * (iv(Stat::SPD) & 1) + 16 * (iv(Stat::SPATK) & 1) +
                               32 * (iv(Stat::SPDEF) & 1)) /
                           63) +
                       1)};
    }

    void PK8::hpType(Type v)
    {
        if (v <= Type::Normal || v >= Type::Fairy)
        {
            return;
        }
        static constexpr u16 hpivs[16][6] = {
            {1, 1, 0, 0, 0, 0}, // Fighting
            {0, 0, 0, 1, 0, 0}, // Flying
            {1, 1, 0, 1, 0, 0}, // Poison
            {1, 1, 1, 1, 0, 0}, // Ground
            {1, 1, 0, 0, 1, 0}, // Rock
            {1, 0, 0, 1, 1, 0}, // Bug
            {1, 0, 1, 1, 1, 0}, // Ghost
            {1, 1, 1, 1, 1, 0}, // Steel
            {1, 0, 1, 0, 0, 1}, // Fire
            {1, 0, 0, 1, 0, 1}, // Water
            {1, 0, 1, 1, 0, 1}, // Grass
            {1, 1, 1, 1, 0, 1}, // Electric
            {1, 0, 1, 0, 1, 1}, // Psychic
            {1, 0, 0, 1, 1, 1}, // Ice
            {1, 0, 1, 1, 1, 1}, // Dragon
            {1, 1, 1, 1, 1, 1}, // Dark
        };

        for (u8 i = 0; i < 6; i++)
        {
            iv(Stat(i), (iv(Stat(i)) & 0x1E) + hpivs[u8(v) - 1][i]);
        }
    }

    u16 PK8::TSV(void) const
    {
        return (TID() ^ SID()) >> 4;
    }

    u16 PK8::PSV(void) const
    {
        return ((PID() >> 16) ^ (PID() & 0xFFFF)) >> 4;
    }

    u8 PK8::level(void) const
    {
        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
        {
            ;
        }
        return i;
    }

    void PK8::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
    }

    bool PK8::shiny(void) const
    {
        return TSV() == PSV();
    }

    void PK8::shiny(bool v)
    {
        PID(PKX::getRandomPID(species(), gender(), version(), nature(), alternativeForm(),
            abilityNumber(), v, TSV(), PID(), generation()));
    }

    u16 PK8::formSpecies(void) const
    {
        u16 tmpSpecies = u16(species());
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

    u16 PK8::statImpl(Stat stat) const
    {
        u16 calc;
        u8 mult = 10, basestat = 0;

        switch (stat)
        {
            case Stat::HP:
                basestat = baseHP();
                break;
            case Stat::ATK:
                basestat = baseAtk();
                break;
            case Stat::DEF:
                basestat = baseDef();
                break;
            case Stat::SPD:
                basestat = baseSpe();
                break;
            case Stat::SPATK:
                basestat = baseSpa();
                break;
            case Stat::SPDEF:
                basestat = baseSpd();
                break;
        }

        if (stat == Stat::HP)
        {
            calc =
                10 + ((2 * basestat) +
                         ((((data[0xDE] >> hyperTrainLookup[u8(stat)]) & 1) == 1) ? 31 : iv(stat)) +
                         ev(stat) / 4 + 100) *
                         level() / 100;
        }
        else
        {
            calc =
                5 + (2 * basestat +
                        ((((data[0xDE] >> hyperTrainLookup[u8(stat)]) & 1) == 1) ? 31 : iv(stat)) +
                        ev(stat) / 4) *
                        level() / 100;
        }
        if (u8(nature()) / 5 + 1 == u8(stat))
        {
            mult++;
        }
        if (u8(nature()) % 5 + 1 == u8(stat))
        {
            mult--;
        }
        return calc * mult / 10;
    }

    void PK8::updatePartyData()
    {
        static constexpr Stat stats[] = {
            Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF};
        for (size_t i = 0; i < 6; i++)
        {
            partyStat(stats[i], stat(stats[i]));
        }
        partyLevel(level());
        partyCurrHP(stat(Stat::HP));
    }
}
