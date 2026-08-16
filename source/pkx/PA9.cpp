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

#include "pkx/PA9.hpp"
#include "pkx/PK8.hpp"
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
            // Byte 0x34
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
            // Byte 0x35
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
            // Byte 0x36
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
            // Byte 0x37
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
            // Byte 0x38
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
            // Byte 0x39
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
            // Byte 0x3A
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
            // Byte 0x3B
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
            // 0x3C-0x3F: memory ribbon counts + padding (not bit flags)
            // Byte 0x40
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
            // Byte 0x41
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
            // Byte 0x42
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
            // Byte 0x43
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
            // Byte 0x44
            case pksm::Ribbon::MarkVigor:
                return {0x44, 0};
            case pksm::Ribbon::MarkSlump:
                return {0x44, 1};
            case pksm::Ribbon::Hisui:
                return {0x44, 2};
            case pksm::Ribbon::TwinklingStar:
                return {0x44, 3};
            case pksm::Ribbon::ChampionPaldea:
                return {0x44, 4};
            case pksm::Ribbon::MarkJumbo:
                return {0x44, 5};
            case pksm::Ribbon::MarkMini:
                return {0x44, 6};
            case pksm::Ribbon::MarkItemfinder:
                return {0x44, 7};
            // Byte 0x45
            case pksm::Ribbon::MarkPartner:
                return {0x45, 0};
            case pksm::Ribbon::MarkGourmand:
                return {0x45, 1};
            case pksm::Ribbon::OnceInALifetime:
                return {0x45, 2};
            case pksm::Ribbon::MarkAlpha:
                return {0x45, 3};
            case pksm::Ribbon::MarkMightiest:
                return {0x45, 4};
            case pksm::Ribbon::MarkTitan:
                return {0x45, 5};
            case pksm::Ribbon::Partner:
                return {0x45, 6};

            default:
                return {RIBBON_ABSENT, 0};
        }
    }
}

namespace pksm
{
    void PA9::encrypt(void)
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

    void PA9::decrypt(void)
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

    bool PA9::isEncrypted() const
    {
        return LittleEndian::convertTo<u16>(data + 0x70) != 0 ||
               LittleEndian::convertTo<u16>(data + 0x110) != 0;
    }

    PA9::PA9(PrivateConstructor, u8* dt, bool party, bool direct)
        : PKX(dt, party ? PARTY_LENGTH : BOX_LENGTH, direct)
    {
        if (isEncrypted())
        {
            decrypt();
        }
    }

    std::unique_ptr<PKX> PA9::clone(void) const
    {
        return PKX::getPKM<PA9>(data, isParty() ? PARTY_LENGTH : BOX_LENGTH);
    }

    std::unique_ptr<PKX> PA9::convertToG9(Sav& save) const
    {
        if (save.version() != GameVersion::ZA)
        {
            // SV target — PA9 and PK9 share identical binary layout, raw-data copy
            auto pk9 = PKX::getPKM<PK9>(data, BOX_LENGTH);
            // Set TeraType from PersonalSV (same pattern as PA8::convertToG9)
            auto type1 = PersonalSV::type1(pk9->formSpecies());
            u8 tera =
                (type1 != Type::Normal) ? u8(type1) : u8(PersonalSV::type2(pk9->formSpecies()));
            pk9->teraTypeOriginal(tera);
            pk9->teraTypeOverride(tera);
            pk9->refreshChecksum();
            return pk9;
        }
        return clone();
    }

    std::unique_ptr<PK8> PA9::convertToG8(Sav& save) const
    {
        // PA9 and PK9 share identical binary layout — reinterpret as PK9 and delegate
        auto asPK9 = PKX::getPKM<PK9>(data, BOX_LENGTH);
        return asPK9->convertToG8(save);
    }

    Generation PA9::generation(void) const
    {
        return Generation::NINE;
    }

    u32 PA9::encryptionConstant(void) const
    {
        return LittleEndian::convertTo<u32>(data);
    }

    void PA9::encryptionConstant(u32 v)
    {
        LittleEndian::convertFrom<u32>(data, v);
    }

    u16 PA9::sanity(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x04);
    }

    void PA9::sanity(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x04, v);
    }

    u16 PA9::checksum(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x06);
    }

    void PA9::checksum(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x06, v);
    }

    Species PA9::species(void) const
    {
        return Species{LittleEndian::convertTo<u16>(data + 0x08)};
    }

    void PA9::species(Species v)
    {
        LittleEndian::convertFrom<u16>(data + 0x08, u16(v));
    }

    u16 PA9::heldItem(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0A);
    }

    void PA9::heldItem(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0A, v);
    }

    u16 PA9::TID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0C);
    }

    void PA9::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0C, v);
    }

    u16 PA9::SID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0E);
    }

    void PA9::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0E, v);
    }

    u32 PA9::experience(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x10);
    }

    void PA9::experience(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x10, v);
    }

    Ability PA9::ability(void) const
    {
        return Ability{LittleEndian::convertTo<u16>(data + 0x14)};
    }

    void PA9::ability(Ability v)
    {
        LittleEndian::convertFrom<u16>(data + 0x14, u16(v));
    }

    void PA9::setAbility(u8 v)
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

    u8 PA9::abilityNumber(void) const
    {
        return data[0x16] & 0x7;
    }

    void PA9::abilityNumber(u8 v)
    {
        data[0x16] = (data[0x16] & ~7) | (v & 7);
    }

    u16 PA9::markValue(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18);
    }

    void PA9::markValue(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x18, v);
    }

    u32 PA9::PID(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x1C);
    }

    void PA9::PID(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x1C, v);
    }

    Nature PA9::nature(void) const
    {
        return Nature{data[0x20]};
    }

    void PA9::nature(Nature v)
    {
        data[0x20] = u8(v);
        data[0x21] = u8(v);
    }

    bool PA9::fatefulEncounter(void) const
    {
        return (data[0x22] & 1) == 1;
    }

    void PA9::fatefulEncounter(bool v)
    {
        data[0x22] = (u8)((data[0x22] & ~0x01) | (v ? 1 : 0));
    }

    Gender PA9::gender(void) const
    {
        return Gender{u8((data[0x22] >> 2) & 0x3)};
    }

    void PA9::gender(Gender v)
    {
        data[0x22] = (data[0x22] & ~12) | ((u8(v) & 3) << 2);
    }

    bool PA9::isAlpha(void) const
    {
        return data[0x23] != 0;
    }

    void PA9::isAlpha(bool v)
    {
        data[0x23] = v ? 1 : 0;
    }

    u16 PA9::alternativeForm(void) const
    {
        return data[0x24];
    }

    void PA9::alternativeForm(u16 v)
    {
        data[0x24] = v;
    }

    u16 PA9::ev(Stat ev) const
    {
        return data[0x26 + u8(ev)];
    }

    void PA9::ev(Stat ev, u16 v)
    {
        data[0x26 + u8(ev)] = v;
    }

    u8 PA9::contest(u8 contest) const
    {
        return data[0x2C + contest];
    }

    void PA9::contest(u8 contest, u8 v)
    {
        data[0x2C + contest] = v;
    }

    u8 PA9::pkrs(void) const
    {
        return data[0x32];
    }

    void PA9::pkrs(u8 v)
    {
        data[0x32] = v;
    }

    u8 PA9::pkrsDays(void) const
    {
        return data[0x32] & 0xF;
    }

    void PA9::pkrsDays(u8 v)
    {
        data[0x32] = (data[0x32] & ~0xF) | (v & 0xF);
    }

    u8 PA9::pkrsStrain(void) const
    {
        return data[0x32] >> 4;
    }

    void PA9::pkrsStrain(u8 v)
    {
        data[0x32] = (data[0x32] & 0xF) | (v << 4);
    }

    bool PA9::hasRibbon(Ribbon ribbon) const
    {
        return OFFSET_OF(ribbon).first != RIBBON_ABSENT;
    }

    bool PA9::ribbon(Ribbon ribbon) const
    {
        auto offset = OFFSET_OF(ribbon);
        if (offset.first != RIBBON_ABSENT)
        {
            return FlagUtil::getFlag(data, offset.first, offset.second);
        }
        return false;
    }

    void PA9::ribbon(Ribbon ribbon, bool v)
    {
        auto offset = OFFSET_OF(ribbon);
        if (offset.first != RIBBON_ABSENT)
        {
            FlagUtil::setFlag(data, offset.first, offset.second, v);
        }
    }

    std::string PA9::nickname(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0x58, 13));
    }

    void PA9::nickname(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0x58, 13);
    }

    Move PA9::move(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x72 + m * 2)};
    }

    void PA9::move(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x72 + m * 2, u16(v));
    }

    u8 PA9::PP(u8 m) const
    {
        return data[0x7A + m];
    }

    void PA9::PP(u8 m, u8 v)
    {
        data[0x7A + m] = v;
    }

    u8 PA9::PPUp(u8 m) const
    {
        return data[0x7E + m];
    }

    void PA9::PPUp(u8 m, u8 v)
    {
        data[0x7E + m] = v;
    }

    Move PA9::relearnMove(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x82 + m * 2)};
    }

    void PA9::relearnMove(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x82 + m * 2, u16(v));
    }

    int PA9::partyCurrHP(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x8A);
    }

    void PA9::partyCurrHP(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x8A, v);
    }

    u8 PA9::iv(Stat stat) const
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x8C);
        return (u8)((buffer >> 5 * u8(stat)) & 0x1F);
    }

    void PA9::iv(Stat stat, u8 v)
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x8C);
        buffer    &= ~(0x1F << 5 * u8(stat));
        buffer    |= v << (5 * u8(stat));
        LittleEndian::convertFrom<u32>(data + 0x8C, buffer);
    }

    bool PA9::egg(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x8C) >> 30) & 0x1) == 1;
    }

    void PA9::egg(bool v)
    {
        LittleEndian::convertFrom<u32>(
            data + 0x8C, (u32)((LittleEndian::convertTo<u32>(data + 0x8C) & ~0x40000000) |
                               (u32)(v ? 0x40000000 : 0)));
    }

    bool PA9::nicknamed(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x8C) >> 31) & 0x1) == 1;
    }

    void PA9::nicknamed(bool v)
    {
        LittleEndian::convertFrom<u32>(data + 0x8C,
            (LittleEndian::convertTo<u32>(data + 0x8C) & 0x7FFFFFFF) | (v ? 0x80000000 : 0));
    }

    bool PA9::hyperTrain(Stat stat) const
    {
        return (data[0x126] & (1 << u8(stat))) != 0;
    }

    void PA9::hyperTrain(Stat stat, bool v)
    {
        data[0x126] = (data[0x126] & ~(1 << u8(stat))) | (v ? (1 << u8(stat)) : 0);
    }

    PKXHandler PA9::currentHandler(void) const
    {
        return data[0xC4] == 0 ? PKXHandler::OT : PKXHandler::NonOT;
    }

    void PA9::currentHandler(PKXHandler v)
    {
        data[0xC4] = v == PKXHandler::OT ? 0 : 1;
    }

    std::string PA9::otName(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0xF8, 13));
    }

    void PA9::otName(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0xF8, 13);
    }

    u8 PA9::htFriendship(void) const
    {
        return data[0xC8];
    }

    void PA9::htFriendship(u8 v)
    {
        data[0xC8] = v;
    }

    u8 PA9::otFriendship(void) const
    {
        return data[0x112];
    }

    void PA9::otFriendship(u8 v)
    {
        data[0x112] = v;
    }

    u16 PA9::eggLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x120);
    }

    void PA9::eggLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x120, v);
    }

    u16 PA9::metLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x122);
    }

    void PA9::metLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x122, v);
    }

    Ball PA9::ball(void) const
    {
        return Ball{data[0x124]};
    }

    void PA9::ball(Ball v)
    {
        data[0x124] = u8(v);
    }

    u8 PA9::metLevel(void) const
    {
        return data[0x125] & ~0x80;
    }

    void PA9::metLevel(u8 v)
    {
        data[0x125] = (data[0x125] & 0x80) | v;
    }

    Gender PA9::otGender(void) const
    {
        return Gender{u8(data[0x125] >> 7)};
    }

    void PA9::otGender(Gender v)
    {
        data[0x125] = (data[0x125] & ~0x80) | (u8(v) << 7);
    }

    GameVersion PA9::version(void) const
    {
        return GameVersion(data[0xCE]);
    }

    void PA9::version(GameVersion v)
    {
        data[0xCE] = u8(v);
    }

    Language PA9::language(void) const
    {
        return Language(data[0xD5]);
    }

    void PA9::language(Language v)
    {
        data[0xD5] = u8(v);
    }

    int PA9::partyStat(Stat stat) const
    {
        if (!isParty())
        {
            return -1;
        }
        return LittleEndian::convertTo<u16>(data + 0x14A + u8(stat) * 2);
    }

    void PA9::partyStat(Stat stat, u16 v)
    {
        if (isParty())
        {
            LittleEndian::convertFrom<u16>(data + 0x14A + u8(stat) * 2, v);
        }
    }

    int PA9::partyLevel() const
    {
        if (!isParty())
        {
            return -1;
        }
        return *(data + 0x148);
    }

    void PA9::partyLevel(u8 v)
    {
        if (isParty())
        {
            *(data + 0x148) = v;
        }
    }

    void PA9::updatePartyData(void)
    {
        if (!isParty())
        {
            return;
        }

        partyLevel(level());
        partyCurrHP(stat(Stat::HP));
        for (Stat s : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF})
        {
            partyStat(s, stat(s));
        }
    }

    void PA9::refreshChecksum(void)
    {
        u16 chk = 0;
        for (size_t i = 8; i < BOX_LENGTH; i += 2)
        {
            chk += LittleEndian::convertTo<u16>(data + i);
        }
        checksum(chk);
    }

    Type PA9::hpType(void) const
    {
        return Type{u8((15 *
                           ((iv(Stat::HP) & 1) + 2 * (iv(Stat::ATK) & 1) + 4 * (iv(Stat::DEF) & 1) +
                               8 * (iv(Stat::SPD) & 1) + 16 * (iv(Stat::SPATK) & 1) +
                               32 * (iv(Stat::SPDEF) & 1)) /
                           63) +
                       1)};
    }

    void PA9::hpType(Type) {}

    u16 PA9::TSV(void) const
    {
        return (TID() ^ SID()) >> 4;
    }

    u16 PA9::PSV(void) const
    {
        return ((PID() >> 16) ^ (PID() & 0xFFFF)) >> 4;
    }

    u8 PA9::level(void) const
    {
        if (isParty())
        {
            return data[0x148];
        }

        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
        {
            ;
        }
        return i;
    }

    void PA9::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
        if (isParty())
        {
            data[0x148] = v;
        }
    }

    bool PA9::shiny(void) const
    {
        return PSV() == TSV();
    }

    void PA9::shiny(bool) {}

    u16 PA9::formSpecies(void) const
    {
        return u16(species());
    }

    u16 PA9::statImpl(Stat stat) const
    {
        // No full stat calculation support; for party data this is sufficient for now.
        switch (stat)
        {
            case Stat::HP:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x14A) : 0;
            case Stat::ATK:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x14C) : 0;
            case Stat::DEF:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x14E) : 0;
            case Stat::SPD:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x150) : 0;
            case Stat::SPATK:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x152) : 0;
            case Stat::SPDEF:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x154) : 0;
        }
        return 0;
    }

    int PA9::eggYear(void) const
    {
        return 2000 + data[0x119];
    }

    void PA9::eggYear(int v)
    {
        data[0x119] = v - 2000;
    }

    int PA9::eggMonth(void) const
    {
        return data[0x11A];
    }

    void PA9::eggMonth(int v)
    {
        data[0x11A] = v;
    }

    int PA9::eggDay(void) const
    {
        return data[0x11B];
    }

    void PA9::eggDay(int v)
    {
        data[0x11B] = v;
    }

    int PA9::metYear(void) const
    {
        return 2000 + data[0x11C];
    }

    void PA9::metYear(int v)
    {
        data[0x11C] = v - 2000;
    }

    int PA9::metMonth(void) const
    {
        return data[0x11D];
    }

    void PA9::metMonth(int v)
    {
        data[0x11D] = v;
    }

    int PA9::metDay(void) const
    {
        return data[0x11E];
    }

    void PA9::metDay(int v)
    {
        data[0x11E] = v;
    }
}
