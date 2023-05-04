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

#include "pkx/PK1.hpp"
#include "pkx/PK2.hpp"
#include "pkx/PK3.hpp"
#include "pkx/PK4.hpp"
#include "pkx/PK5.hpp"
#include "pkx/PK6.hpp"
#include "pkx/PK7.hpp"
#include "pkx/PK8.hpp"
#include "sav/Sav.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/i18n.hpp"
#include "utils/random.hpp"
#include "utils/utils.hpp"
#include "utils/ValueConverter.hpp"
#include <algorithm>

namespace pksm
{
    PK1::PK1(PrivateConstructor, u8* dt, bool japanese, bool directAccess)
        : PKX(dt, japanese ? JP_LENGTH_WITH_NAMES : INT_LENGTH_WITH_NAMES, directAccess)
    {
        this->japanese = japanese;
        lang           = japanese ? Language::JPN : Language::ENG;
        shiftedData    = data + 3;
    }

    std::unique_ptr<PK2> PK1::convertToG2(Sav&) const
    {
        auto pk2 = PKX::getPKM<Generation::TWO>(
            nullptr, japanese ? PK2::JP_LENGTH_WITH_NAMES : PK2::INT_LENGTH_WITH_NAMES);

        pk2->species(species());
        pk2->TID(TID());
        pk2->experience(experience());
        pk2->egg(false);
        pk2->otFriendship(pk2->baseFriendship());
        pk2->language(language());
        pk2->ev(Stat::HP, ev(Stat::HP));
        pk2->ev(Stat::ATK, ev(Stat::ATK));
        pk2->ev(Stat::DEF, ev(Stat::DEF));
        pk2->ev(Stat::SPD, ev(Stat::SPD));
        pk2->ev(Stat::SPATK, ev(Stat::SPATK));
        pk2->move(0, move(0));
        pk2->move(1, move(1));
        pk2->move(2, move(2));
        pk2->move(3, move(3));
        pk2->PPUp(0, PPUp(0));
        pk2->PPUp(1, PPUp(1));
        pk2->PPUp(2, PPUp(2));
        pk2->PPUp(3, PPUp(3));
        pk2->PP(0, PP(0));
        pk2->PP(1, PP(1));
        pk2->PP(2, PP(2));
        pk2->PP(3, PP(3));
        pk2->iv(Stat::ATK, iv(Stat::ATK));
        pk2->iv(Stat::DEF, iv(Stat::DEF));
        pk2->iv(Stat::SPD, iv(Stat::SPD));
        pk2->iv(Stat::SPATK, iv(Stat::SPATK));
        pk2->otName(otName());
        pk2->nickname(nickname());
        pk2->heldItem2(heldItem2());

        pk2->fixMoves();
        return pk2;
    }

    std::unique_ptr<PK3> PK1::convertToG3(Sav& save) const
    {
        if (auto pk7 = convertToG7(save))
        {
            if (auto pk6 = pk7->convertToG6(save))
            {
                if (auto pk5 = pk6->convertToG5(save))
                {
                    if (auto pk4 = pk5->convertToG4(save))
                    {
                        return pk4->convertToG3(save);
                    }
                }
            }
        }
        return nullptr;
    }

    std::unique_ptr<PK4> PK1::convertToG4(Sav& save) const
    {
        if (auto pk7 = convertToG7(save))
        {
            if (auto pk6 = pk7->convertToG6(save))
            {
                if (auto pk5 = pk6->convertToG5(save))
                {
                    return pk5->convertToG4(save);
                }
            }
        }
        return nullptr;
    }

    std::unique_ptr<PK5> PK1::convertToG5(Sav& save) const
    {
        if (auto pk7 = convertToG7(save))
        {
            if (auto pk6 = pk7->convertToG6(save))
            {
                return pk6->convertToG5(save);
            }
        }
        return nullptr;
    }

    std::unique_ptr<PK6> PK1::convertToG6(Sav& save) const
    {
        if (auto pk7 = convertToG7(save))
        {
            return pk7->convertToG6(save);
        }
        return nullptr;
    }

    std::unique_ptr<PK7> PK1::convertToG7(Sav& save) const
    {
        auto pk7 = PKX::getPKM<Generation::SEVEN>(nullptr, PK7::BOX_LENGTH);

        pk7->encryptionConstant(randomNumber(0, 0xFFFFFFFF));
        pk7->PID(randomNumber(0, 0xFFFFFFFF));
        pk7->species(species());
        pk7->TID(TID());
        pk7->level(level());
        pk7->metLevel(level());
        pk7->nature(nature());
        pk7->ball(ball());
        pk7->metDate(Date::today());
        pk7->version(version());
        pk7->move(0, move(0));
        pk7->move(1, move(1));
        pk7->move(2, move(2));
        pk7->move(3, move(3));
        pk7->PPUp(0, PPUp(0));
        pk7->PPUp(1, PPUp(1));
        pk7->PPUp(2, PPUp(2));
        pk7->PPUp(3, PPUp(3));
        pk7->metLocation(0x753D);
        pk7->gender(gender());
        pk7->nicknamed(false);
        pk7->otName(japanese ? StringUtils::fixJapaneseNameTransporter(transferOT(save.language()))
                             : transferOT(save.language()));

        pk7->currentHandler(PKXHandler::NonOT);
        pk7->htName(save.otName());
        pk7->htGender(save.gender());

        pk7->consoleRegion(save.consoleRegion());
        pk7->country(save.country());
        pk7->region(save.subRegion());
        pk7->geoCountry(0, save.country());
        pk7->geoRegion(0, save.subRegion());

        pk7->healPP();
        pk7->language(save.language());
        pk7->nickname(species().localize(pk7->language()));

        pk7->otFriendship(PersonalSMUSUM::baseFriendship(u16(species())));
        pk7->htFriendship(pk7->otFriendship());

        std::array<Stat, 6> stats = {
            Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF};
        std::shuffle(stats.begin(), stats.end(), UniformRandomBitGenerator{});

        u8 perfectStats = 3;
        if (species() == Species::Mew)
        {
            perfectStats = 5;
        }

        for (const auto& s : stats | std::views::take(perfectStats))
        {
            pk7->iv(s, 31);
        }
        for (const auto& s : stats | std::views::drop(perfectStats))
        {
            pk7->iv(s, randomNumber(0, 31));
        }

        if (pk7->shiny() && !shiny())
        {
            pk7->PID(pk7->PID() ^ 0x10000000);
        }
        else if (!pk7->shiny() && shiny())
        {
            pk7->PID(u32(((pk7->TID() ^ (pk7->PID() & 0xFFFF)) << 16) | (pk7->PID() & 0xFFFF)));
        }

        // always has hidden ability unless it doesn't exist
        switch (species())
        {
            case Species::Gastly:
            case Species::Haunter:
            case Species::Gengar:
            case Species::Koffing:
            case Species::Weezing:
            case Species::Mew:
                pk7->setAbility(0);
                break;
            default:
                pk7->setAbility(2);
        }

        if (species() == Species::Mew)
        {
            pk7->fatefulEncounter(true);
        }
        else if (nicknamed())
        {
            pk7->nicknamed(true);
            pk7->nickname(japanese ? StringUtils::fixJapaneseNameTransporter(nicknameTransporter())
                                   : nicknameTransporter());
        }

        pk7->htMemory(4);
        pk7->htTextVar(0);
        pk7->htIntensity(1);
        pk7->htFeeling(randomNumber(0, 9));

        pk7->fixMoves();
        pk7->refreshChecksum();
        return pk7;
    }

    std::unique_ptr<PK8> PK1::convertToG8(Sav& save) const
    {
        if (auto pk7 = convertToG7(save))
        {
            return pk7->convertToG8(save);
        }
        return nullptr;
    }

    std::unique_ptr<PKX> PK1::clone() const
    {
        return PKX::getPKM<Generation::ONE>(
            data, japanese ? JP_LENGTH_WITH_NAMES : INT_LENGTH_WITH_NAMES);
    }

    u16 PK1::TID() const
    {
        return BigEndian::convertTo<u16>(shiftedData + 12);
    }

    void PK1::TID(u16 v)
    {
        BigEndian::convertFrom<u16>(shiftedData + 12, v);
    }

    std::string PK1::nickname() const
    {
        return StringUtils::getString1(
            shiftedData, 44 + (japanese ? 6 : 11), japanese ? 6 : 11, lang);
    }

    std::string PK1::nicknameTransporter() const
    {
        return StringUtils::getString1(
            shiftedData, 44 + (japanese ? 6 : 11), japanese ? 6 : 11, lang, true);
    }

    void PK1::nickname(const std::string_view& v)
    {
        StringUtils::setString1(
            shiftedData, v, 44 + (japanese ? 6 : 11), japanese ? 6 : 11, lang, japanese ? 6 : 11);
    }

    Language PK1::language() const
    {
        return lang;
    }

    void PK1::language(Language v)
    {
        // data is too different to allow converting anything between japanese and another language
        if (lang != Language::JPN && v != Language::JPN)
        {
            lang = v;
        }
    }

    std::string PK1::otName() const
    {
        return StringUtils::getString1(shiftedData, 44, japanese ? 6 : 11, lang);
    }

    void PK1::otName(const std::string_view& v)
    {
        StringUtils::setString1(shiftedData, v, 44, japanese ? 6 : 11, lang, japanese ? 6 : 11);
    }

    std::string PK1::transferOT(Language newLang) const
    {
        return StringUtils::getString1(
            shiftedData, 44, japanese ? 6 : 11, shiftedData[44] == 0x5D ? newLang : lang, true);
    }

    u8 PK1::speciesID1() const
    {
        return shiftedData[0];
    }

    void PK1::speciesID1(u8 v)
    {
        data[1]        = v;
        shiftedData[0] = v;
        writeG1Types();

        // do this now rather than never
        data[0] = 1;
        data[2] = 0xFF;
    }

    Species PK1::species() const
    {
        return SpeciesConverter::g1ToNational(speciesID1());
    }

    void PK1::species(Species v)
    {
        speciesID1(SpeciesConverter::nationalToG1(v));
    }

    // yes, Gen I pokemon data stores catch rate, and yes it's actually important for Gen II
    // importing
    u8 PK1::catchRate() const
    {
        return shiftedData[7];
    }

    void PK1::catchRate(u8 v)
    {
        shiftedData[7] = v;
    }

    // experience is actually 3 bytes
    u32 PK1::experience() const
    {
        return BigEndian::convertTo<u32>(shiftedData + 14) >> 8;
    }

    void PK1::experience(u32 v)
    {
        shiftedData[14] = v >> 16;
        shiftedData[15] = (v >> 8) & 0x00FF;
        shiftedData[16] = v & 0x0000FF;
    }

    u16 PK1::ev(Stat ev) const
    {
        if (ev == Stat::SPDEF)
        {
            ev = Stat::SPATK;
        }
        return BigEndian::convertTo<u16>(shiftedData + 17 + 2 * u8(ev));
    }

    void PK1::ev(Stat ev, u16 v)
    {
        if (ev == Stat::SPDEF)
        {
            ev = Stat::SPATK;
        }
        BigEndian::convertFrom<u16>(shiftedData + 17 + 2 * u8(ev), v);
    }

    Move PK1::move(u8 move) const
    {
        return Move{shiftedData[8 + move]};
    }

    void PK1::move(u8 move, Move v)
    {
        shiftedData[8 + move] = u8(v);
    }

    u8 PK1::PP(u8 move) const
    {
        return u8(shiftedData[29 + move] & 0x3F);
    }

    void PK1::PP(u8 move, u8 v)
    {
        shiftedData[29 + move] = u8((shiftedData[29 + move] & 0xC0) | (v & 0x3F));
    }

    u8 PK1::PPUp(u8 move) const
    {
        return shiftedData[29 + move] >> 6;
    }

    void PK1::PPUp(u8 move, u8 v)
    {
        shiftedData[29 + move] = u8((v & 0xC0) | (shiftedData[29 + move] & 0x3F));
    }

    u8 PK1::iv(Stat iv) const
    {
        switch (iv)
        {
            case Stat::HP:
                return ((PK1::iv(Stat::ATK) & 0x01) << 3) | ((PK1::iv(Stat::DEF) & 0x01) << 2) |
                       ((PK1::iv(Stat::SPD) & 0x01) << 1) | (PK1::iv(Stat::SPATK) & 0x01);
            case Stat::ATK:
                return (shiftedData[0x1B] & 0xF0) >> 4;
            case Stat::DEF:
                return shiftedData[0x1B] & 0x0F;
            case Stat::SPD:
                return (shiftedData[0x1C] & 0xF0) >> 4;
            case Stat::SPATK:
            case Stat::SPDEF:
                return shiftedData[0x1C] & 0x0F;
            default:
                return 0;
        }
    }

    void PK1::iv(Stat iv, u8 v)
    {
        if (v > 15)
        {
            v = 15;
        }
        switch (iv)
        {
            case Stat::ATK:
                shiftedData[0x1B] = ((v & 0x0F) << 4) | (shiftedData[0x1B] & 0x0F);
                break;
            case Stat::DEF:
                shiftedData[0x1B] = (shiftedData[0x1B] & 0xF0) | (v & 0x0F);
                break;
            case Stat::SPD:
                shiftedData[0x1C] = ((v & 0x0F) << 4) | (shiftedData[0x1C] & 0x0F);
                break;
            case Stat::SPATK:
            case Stat::SPDEF:
                shiftedData[0x1C] = (shiftedData[0x1C] & 0xF0) | (v & 0x0F);
                break;
            case Stat::HP:
            default:
                return;
        }
    }

    bool PK1::nicknamed() const
    {
        if (japanese)
        {
            std::string target = species().localize(language());
            return nickname() != StringUtils::toUpper(target);
        }
        else
        {
            for (u8 i = u8(Language::ENG); i <= u8(Language::SPA); i++)
            {
                if (i != u8(Language::UNUSED))
                {
                    std::string target = species().localize(Language{i});
                    if (nickname() == StringUtils::toUpper(target))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    Gender PK1::gender() const
    {
        switch (genderType())
        {
            case 255:
                return Gender::Genderless;
            case 254:
                return Gender::Female;
            case 0:
                return Gender::Male;
            default:
                return iv(Stat::ATK) > (genderType() >> 4) ? Gender::Male : Gender::Female;
        }
    }

    void PK1::gender(Gender g)
    {
        switch (genderType())
        {
            case 255:
            case 254:
            case 0:
                return;
            default:
                iv(Stat::ATK,
                    (g == Gender::Female) ? (genderType() >> 4) : ((genderType() >> 4) + 1));
        }
    }

    Nature PK1::nature() const
    {
        return Nature{u8(experience() % 25)};
    }

    void PK1::nature(Nature v)
    {
        experience(experience() - (experience() % 25) + u8(v));
    }

    Type PK1::hpType() const
    {
        return Type{u8((((iv(Stat::ATK) & 0x3) << 2) | (iv(Stat::DEF) & 0x3)) + 1)};
    }

    void PK1::hpType(Type v)
    {
        if (v <= Type::Normal || v >= Type::Fairy)
        {
            return;
        }

        u8 noNormal = u8(v) - 1;
        noNormal    %= 16; // just in case
        iv(Stat::ATK, (iv(Stat::ATK) & 0xC) | (noNormal >> 2));
        iv(Stat::DEF, (iv(Stat::DEF) & 0xC) | (noNormal & 0x3));
    }

    u8 PK1::level() const
    {
        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
        {
            ;
        }
        return i;
    }

    void PK1::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
        boxLevel(v);
    }

    // shininess doesn't exist until Gen II, but the GUI is probably gonna use colored sprites, so
    // why not
    bool PK1::shiny() const
    {
        return (iv(Stat::DEF) == 10) && (iv(Stat::SPD) == 10) && (iv(Stat::SPATK) == 10) &&
               (((iv(Stat::ATK) & 0x02) >> 1) == 1);
    }

    void PK1::shiny(bool v)
    {
        if (v)
        {
            iv(Stat::DEF, 10);
            iv(Stat::SPD, 10);
            iv(Stat::SPATK, 10);
        }
        iv(Stat::ATK, v ? 7 : 8);
    }

    u16 PK1::formSpecies() const
    {
        return u16(species());
    }

    u16 PK1::statImpl(Stat stat) const
    {
        u16 base;
        switch (stat)
        {
            case Stat::HP:
                base = baseHP();
                break;
            case Stat::ATK:
                base = baseAtk();
                break;
            case Stat::DEF:
                base = baseDef();
                break;
            // WHO NAMED THE STAT SPD AND THE METHOD SPE?!
            case Stat::SPD:
                base = baseSpe();
                break;
            case Stat::SPATK:
            case Stat::SPDEF:
                base = baseSpa();
                break;
            default:
                base = 0;
        }
        u16 EV  = u16(std::min(255, int(std::ceil(std::sqrt(ev(stat))))) >> 2);
        u16 mid = u16(((2 * (base + iv(stat)) + EV) * level() / 100) + 5);
        if (stat == Stat::HP)
        {
            return mid + 5 + level();
        }
        return mid;
    }

    int PK1::partyCurrHP() const
    {
        return isParty() ? BigEndian::convertTo<u16>(shiftedData + 1) : -1;
    }

    void PK1::partyCurrHP(u16 v)
    {
        if (isParty())
        {
            BigEndian::convertFrom<u16>(shiftedData + 1, v);
        }
    }

    int PK1::partyStat(Stat stat) const
    {
        if (!isParty())
        {
            return -1;
        }
        if (stat == Stat::SPDEF)
        {
            stat = Stat::SPATK;
        }
        return BigEndian::convertTo<u16>(shiftedData + 34 + 2 * u8(stat));
    }

    void PK1::partyStat(Stat stat, u16 v)
    {
        if (isParty())
        {
            if (stat == Stat::SPDEF)
            {
                stat = Stat::SPATK;
            }
            BigEndian::convertFrom<u16>(shiftedData + 34 + 2 * u8(stat), v);
        }
    }

    int PK1::partyLevel() const
    {
        return isParty() ? shiftedData[33] : -1;
    }

    void PK1::partyLevel(u8 v)
    {
        if (isParty())
        {
            shiftedData[33] = v;
        }
    }

    int PK1::boxLevel() const
    {
        return shiftedData[3];
    }

    void PK1::boxLevel(u8 v)
    {
        shiftedData[3] = v;
    }

    void PK1::updatePartyData()
    {
        partyLevel(level());
        for (u8 pstat = u8(Stat::HP); pstat != u8(Stat::SPDEF); pstat++)
        {
            partyStat(static_cast<Stat>(pstat), stat(static_cast<Stat>(pstat)));
        }
        partyCurrHP(stat(Stat::HP));
    }

    void PK1::writeG1Types()
    {
        static constexpr auto convType = [](Type t) -> u8
        {
            switch (t)
            {
                case Type::Normal:
                case Type::Fighting:
                case Type::Flying:
                case Type::Poison:
                case Type::Ground:
                case Type::Rock:
                    return u8(t);
                case Type::Bug:
                case Type::Ghost:
                case Type::Steel:
                    return u8(t) + 1;
                default:
                    return u8(t) + 11;
            }
        };
        shiftedData[5] = convType(type1());
        shiftedData[6] = convType(type2());
    }

    // gen 2 stuff because this is a gen 2 construct
    u8 PK1::heldItem2() const
    {
        switch (catchRate())
        {
            case 0x19:       // Teru-sama
                return 0x92; // Leftovers
            case 0x2D:       // Teru-sama
                return 0x53; // Bitter Berry
            case 0x32:       // Teru-sama
                return 0xAE; // Gold Berry
            case 0x5A:       // Teru-sama
            case 0x64:       // Teru-sama
            case 0x78:       // Teru-sama
            case 0x7F:       // Card Key
            case 0xBE:       // Teru-sama
            case 0xFF:       // Cancel
                return 0xAD; // Berry
            default:
                return catchRate();
        }
    }

    void PK1::heldItem2(u8 v)
    {
        catchRate(v);
    }

    u16 PK1::heldItem() const
    {
        return ItemConverter::g2ToNational(heldItem2());
    }

    void PK1::heldItem(u16 v)
    {
        catchRate(ItemConverter::nationalToG2(v));
    }

    void PK1::heldItem(const Item& item)
    {
        if (item.generation() == Generation::TWO)
        {
            heldItem2((static_cast<const Item2&>(item)).id2());
        }
        else
        {
            heldItem(item.id());
        }
    }
}
