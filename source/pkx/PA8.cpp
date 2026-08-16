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

#include "pkx/PA8.hpp"
#include "pkx/PA9.hpp"
#include "pkx/PK8.hpp"
#include "pkx/PK9.hpp"
#include "sav/Sav.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/utils.hpp"

namespace pksm
{
    void PA8::encrypt(void)
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

    void PA8::decrypt(void)
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

    bool PA8::isEncrypted() const
    {
        return LittleEndian::convertTo<u16>(data + 0x78) != 0 ||
               LittleEndian::convertTo<u16>(data + 0x128) != 0;
    }

    PA8::PA8(PrivateConstructor, u8* dt, bool party, bool direct)
        : PKX(dt, party ? PARTY_LENGTH : BOX_LENGTH, direct)
    {
        if (isEncrypted())
        {
            decrypt();
        }
    }

    std::unique_ptr<PKX> PA8::clone(void) const
    {
        return PKX::getPKM<PA8>(data, isParty() ? PARTY_LENGTH : BOX_LENGTH);
    }

    std::unique_ptr<PKX> PA8::convertToG9(Sav& save) const
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
        pk9->nature(nature());

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

        // PA8 ribbon support is stubbed; no ribbons are awarded in PLA

        // PK9-specific: set TeraType from PersonalSV types
        if (!targetIsZA)
        {
            auto* pk9sv = static_cast<PK9*>(pk9.get());
            auto type1  = PersonalSV::type1(pk9->formSpecies());
            u8 tera =
                (type1 != Type::Normal) ? u8(type1) : u8(PersonalSV::type2(pk9->formSpecies()));
            pk9sv->teraTypeOriginal(tera);
            pk9sv->teraTypeOverride(tera);
        }

        pk9->refreshChecksum();

        return pk9;
    }

    std::unique_ptr<PK8> PA8::convertToG8(Sav& save) const
    {
        auto pk8 = PKX::getPKM<PK8>(nullptr, PK8::BOX_LENGTH);

        pk8->encryptionConstant(encryptionConstant());
        pk8->species(species());
        pk8->TID(TID());
        pk8->SID(SID());
        pk8->experience(experience());
        pk8->PID(PID());

        if (ability() == PersonalSWSH::ability(formSpecies(), abilityNumber() >> 1))
        {
            pk8->setAbility(abilityNumber() >> 1);
        }
        else
        {
            pk8->ability(ability());
            pk8->abilityNumber(abilityNumber());
        }

        pk8->language(language());
        pk8->heldItem(heldItem());
        pk8->markValue(markValue());

        for (Stat stat : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPATK, Stat::SPDEF, Stat::SPD})
        {
            pk8->ev(stat, ev(stat));
            pk8->iv(stat, iv(stat));
            pk8->hyperTrain(stat, hyperTrain(stat));
        }

        for (size_t i = 0; i < 4; i++)
        {
            pk8->move(i, move(i));
            pk8->PPUp(i, PPUp(i));
            pk8->PP(i, PP(i));
            pk8->relearnMove(i, relearnMove(i));
        }

        pk8->egg(egg());
        pk8->nicknamed(nicknamed());
        pk8->nickname(nickname());
        pk8->fatefulEncounter(fatefulEncounter());
        pk8->gender(gender());
        pk8->otGender(otGender());
        pk8->alternativeForm(alternativeForm());
        pk8->nature(nature());
        pk8->origNature(nature());

        pk8->version(version());
        pk8->otName(otName());
        pk8->otFriendship(otFriendship());
        pk8->metDate(metDate());
        pk8->eggDate(eggDate());
        pk8->metLocation(metLocation());
        pk8->eggLocation(eggLocation());
        pk8->ball(ball());
        pk8->metLevel(metLevel());
        pk8->currentHandler(currentHandler());
        pk8->htFriendship(htFriendship());

        pk8->pkrsStrain(pkrsStrain());
        pk8->pkrsDays(pkrsDays());

        for (size_t i = 0; i < 6; i++)
        {
            pk8->contest(i, contest(i));
        }

        // PA8 ribbon support is stubbed; no ribbons are awarded in PLA

        pk8->refreshChecksum();

        return pk8;
    }

    Generation PA8::generation(void) const
    {
        return Generation::EIGHT;
    }

    u32 PA8::encryptionConstant(void) const
    {
        return LittleEndian::convertTo<u32>(data);
    }

    void PA8::encryptionConstant(u32 v)
    {
        LittleEndian::convertFrom<u32>(data, v);
    }

    u16 PA8::sanity(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x04);
    }

    void PA8::sanity(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x04, v);
    }

    u16 PA8::checksum(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x06);
    }

    void PA8::checksum(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x06, v);
    }

    Species PA8::species(void) const
    {
        return Species{LittleEndian::convertTo<u16>(data + 0x08)};
    }

    void PA8::species(Species v)
    {
        LittleEndian::convertFrom<u16>(data + 0x08, u16(v));
    }

    u16 PA8::heldItem(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0A);
    }

    void PA8::heldItem(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0A, v);
    }

    u16 PA8::TID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0C);
    }

    void PA8::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0C, v);
    }

    u16 PA8::SID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0E);
    }

    void PA8::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x0E, v);
    }

    u32 PA8::experience(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x10);
    }

    void PA8::experience(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x10, v);
    }

    Ability PA8::ability(void) const
    {
        return Ability{LittleEndian::convertTo<u16>(data + 0x14)};
    }

    void PA8::ability(Ability v)
    {
        LittleEndian::convertFrom<u16>(data + 0x14, u16(v));
    }

    void PA8::setAbility(u8 v)
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

    u8 PA8::abilityNumber(void) const
    {
        return data[0x16] & 0x7;
    }

    void PA8::abilityNumber(u8 v)
    {
        data[0x16] = (data[0x16] & ~7) | (v & 7);
    }

    u16 PA8::markValue(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18);
    }

    void PA8::markValue(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x18, v);
    }

    u32 PA8::PID(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x1C);
    }

    void PA8::PID(u32 v)
    {
        LittleEndian::convertFrom<u32>(data + 0x1C, v);
    }

    Nature PA8::nature(void) const
    {
        return Nature{data[0x20]};
    }

    void PA8::nature(Nature v)
    {
        data[0x20] = u8(v);
        data[0x21] = u8(v);
    }

    bool PA8::fatefulEncounter(void) const
    {
        return (data[0x22] & 1) == 1;
    }

    void PA8::fatefulEncounter(bool v)
    {
        data[0x22] = (u8)((data[0x22] & ~0x01) | (v ? 1 : 0));
    }

    Gender PA8::gender(void) const
    {
        return Gender{u8((data[0x22] >> 2) & 0x3)};
    }

    void PA8::gender(Gender v)
    {
        data[0x22] = (data[0x22] & ~12) | ((u8(v) & 3) << 2);
    }

    u16 PA8::alternativeForm(void) const
    {
        return data[0x24];
    }

    void PA8::alternativeForm(u16 v)
    {
        data[0x24] = v;
    }

    u16 PA8::ev(Stat ev) const
    {
        return data[0x26 + u8(ev)];
    }

    void PA8::ev(Stat ev, u16 v)
    {
        data[0x26 + u8(ev)] = v;
    }

    u8 PA8::contest(u8 contest) const
    {
        return data[0x2C + contest];
    }

    void PA8::contest(u8 contest, u8 v)
    {
        data[0x2C + contest] = v;
    }

    u8 PA8::pkrs(void) const
    {
        return data[0x32];
    }

    void PA8::pkrs(u8 v)
    {
        data[0x32] = v;
    }

    u8 PA8::pkrsDays(void) const
    {
        return data[0x32] & 0xF;
    }

    void PA8::pkrsDays(u8 v)
    {
        data[0x32] = (data[0x32] & ~0xF) | (v & 0xF);
    }

    u8 PA8::pkrsStrain(void) const
    {
        return data[0x32] >> 4;
    }

    void PA8::pkrsStrain(u8 v)
    {
        data[0x32] = (data[0x2B] & 0xF) | (v << 4);
    }

    bool PA8::hasRibbon(Ribbon) const
    {
        return false;
    }

    bool PA8::ribbon(Ribbon) const
    {
        return false;
    }

    void PA8::ribbon(Ribbon, bool) {}

    std::string PA8::nickname(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0x60, 13));
    }

    void PA8::nickname(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0x60, 13);
    }

    Move PA8::move(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x54 + m * 2)};
    }

    void PA8::move(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x54 + m * 2, u16(v));
    }

    u8 PA8::PP(u8 m) const
    {
        return data[0x5C + m];
    }

    void PA8::PP(u8 m, u8 v)
    {
        data[0x5C + m] = v;
    }

    u8 PA8::PPUp(u8 m) const
    {
        return data[0x86 + m];
    }

    void PA8::PPUp(u8 m, u8 v)
    {
        data[0x86 + m] = v;
    }

    Move PA8::relearnMove(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x8A + m * 2)};
    }

    void PA8::relearnMove(u8 m, Move v)
    {
        LittleEndian::convertFrom<u16>(data + 0x8A + m * 2, u16(v));
    }

    int PA8::partyCurrHP(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x92);
    }

    void PA8::partyCurrHP(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x92, v);
    }

    u8 PA8::iv(Stat stat) const
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x94);
        return (u8)((buffer >> 5 * u8(stat)) & 0x1F);
    }

    void PA8::iv(Stat stat, u8 v)
    {
        u32 buffer = LittleEndian::convertTo<u32>(data + 0x94);
        buffer    &= ~(0x1F << 5 * u8(stat));
        buffer    |= v << (5 * u8(stat));
        LittleEndian::convertFrom<u32>(data + 0x94, buffer);
    }

    bool PA8::egg(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x94) >> 30) & 0x1) == 1;
    }

    void PA8::egg(bool v)
    {
        LittleEndian::convertFrom<u32>(
            data + 0x94, (u32)((LittleEndian::convertTo<u32>(data + 0x94) & ~0x40000000) |
                               (u32)(v ? 0x40000000 : 0)));
    }

    bool PA8::nicknamed(void) const
    {
        return ((LittleEndian::convertTo<u32>(data + 0x94) >> 31) & 0x1) == 1;
    }

    void PA8::nicknamed(bool v)
    {
        LittleEndian::convertFrom<u32>(data + 0x94,
            (LittleEndian::convertTo<u32>(data + 0x94) & 0x7FFFFFFF) | (v ? 0x80000000 : 0));
    }

    bool PA8::hyperTrain(Stat) const
    {
        return false;
    }

    void PA8::hyperTrain(Stat, bool) {}

    PKXHandler PA8::currentHandler(void) const
    {
        return data[0xD4] == 0 ? PKXHandler::OT : PKXHandler::NonOT;
    }

    void PA8::currentHandler(PKXHandler v)
    {
        data[0xD4] = v == PKXHandler::OT ? 0 : 1;
    }

    std::string PA8::otName(void) const
    {
        return StringUtils::transString67(StringUtils::getString(data, 0x110, 13));
    }

    void PA8::otName(const std::string_view& v)
    {
        StringUtils::setString(data, StringUtils::transString67(v), 0x110, 13);
    }

    u8 PA8::htFriendship(void) const
    {
        return data[0xD8];
    }

    void PA8::htFriendship(u8 v)
    {
        data[0xD8] = v;
    }

    u8 PA8::otFriendship(void) const
    {
        return data[0x12A];
    }

    void PA8::otFriendship(u8 v)
    {
        data[0x12A] = v;
    }

    u16 PA8::eggLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x144);
    }

    void PA8::eggLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x144, v);
    }

    u16 PA8::metLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x146);
    }

    void PA8::metLocation(u16 v)
    {
        LittleEndian::convertFrom<u16>(data + 0x146, v);
    }

    Ball PA8::ball(void) const
    {
        return Ball{data[0x148]};
    }

    void PA8::ball(Ball v)
    {
        data[0x148] = u8(v);
    }

    u8 PA8::metLevel(void) const
    {
        return data[0x149] & ~0x80;
    }

    void PA8::metLevel(u8 v)
    {
        data[0x149] = (data[0x149] & 0x80) | v;
    }

    Gender PA8::otGender(void) const
    {
        return Gender{u8(data[0x149] >> 7)};
    }

    void PA8::otGender(Gender v)
    {
        data[0x149] = (data[0x149] & ~0x80) | (u8(v) << 7);
    }

    GameVersion PA8::version(void) const
    {
        return GameVersion(data[0xEE]);
    }

    void PA8::version(GameVersion v)
    {
        data[0xEE] = u8(v);
    }

    Language PA8::language(void) const
    {
        return Language(data[0xF2]);
    }

    void PA8::language(Language v)
    {
        data[0xF2] = u8(v);
    }

    int PA8::partyStat(Stat stat) const
    {
        if (!isParty())
        {
            return -1;
        }
        return LittleEndian::convertTo<u16>(data + 0x16A + u8(stat) * 2);
    }

    void PA8::partyStat(Stat stat, u16 v)
    {
        if (isParty())
        {
            LittleEndian::convertFrom<u16>(data + 0x16A + u8(stat) * 2, v);
        }
    }

    int PA8::partyLevel() const
    {
        if (!isParty())
        {
            return -1;
        }
        return *(data + 0x168);
    }

    void PA8::partyLevel(u8 v)
    {
        if (isParty())
        {
            *(data + 0x168) = v;
        }
    }

    void PA8::updatePartyData(void)
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

    void PA8::refreshChecksum(void)
    {
        u16 chk = 0;
        for (size_t i = 8; i < BOX_LENGTH; i += 2)
        {
            chk += LittleEndian::convertTo<u16>(data + i);
        }
        checksum(chk);
    }

    Type PA8::hpType(void) const
    {
        return Type{u8((15 *
                           ((iv(Stat::HP) & 1) + 2 * (iv(Stat::ATK) & 1) + 4 * (iv(Stat::DEF) & 1) +
                               8 * (iv(Stat::SPD) & 1) + 16 * (iv(Stat::SPATK) & 1) +
                               32 * (iv(Stat::SPDEF) & 1)) /
                           63) +
                       1)};
    }

    void PA8::hpType(Type) {}

    u16 PA8::TSV(void) const
    {
        return (TID() ^ SID()) >> 4;
    }

    u16 PA8::PSV(void) const
    {
        return ((PID() >> 16) ^ (PID() & 0xFFFF)) >> 4;
    }

    u8 PA8::level(void) const
    {
        if (isParty())
        {
            return data[0x168];
        }

        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
        {
            ;
        }
        return i;
    }

    void PA8::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
        if (isParty())
        {
            data[0x168] = v;
        }
    }

    bool PA8::shiny(void) const
    {
        return PSV() == TSV();
    }

    void PA8::shiny(bool) {}

    u16 PA8::formSpecies(void) const
    {
        return u16(species());
    }

    u16 PA8::statImpl(Stat stat) const
    {
        // No full stat calculation support; for party data this is sufficient for now.
        switch (stat)
        {
            case Stat::HP:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x16A) : 0;
            case Stat::ATK:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x16C) : 0;
            case Stat::DEF:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x16E) : 0;
            case Stat::SPD:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x170) : 0;
            case Stat::SPATK:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x172) : 0;
            case Stat::SPDEF:
                return isParty() ? LittleEndian::convertTo<u16>(data + 0x174) : 0;
        }
        return 0;
    }

    int PA8::eggYear(void) const
    {
        return 2000 + data[0x131];
    }

    void PA8::eggYear(int v)
    {
        data[0x131] = v - 2000;
    }

    int PA8::eggMonth(void) const
    {
        return data[0x132];
    }

    void PA8::eggMonth(int v)
    {
        data[0x132] = v;
    }

    int PA8::eggDay(void) const
    {
        return data[0x133];
    }

    void PA8::eggDay(int v)
    {
        data[0x133] = v;
    }

    int PA8::metYear(void) const
    {
        return 2000 + data[0x134];
    }

    void PA8::metYear(int v)
    {
        data[0x134] = v - 2000;
    }

    int PA8::metMonth(void) const
    {
        return data[0x135];
    }

    void PA8::metMonth(int v)
    {
        data[0x135] = v;
    }

    int PA8::metDay(void) const
    {
        return data[0x136];
    }

    void PA8::metDay(int v)
    {
        data[0x136] = v;
    }
}
