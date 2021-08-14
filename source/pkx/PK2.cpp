/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2021 Bernardo Giordano, Admiral Fish, piepie62
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

#include "gui/gui.hpp"
#include "pkx/PK1.hpp"
#include "pkx/PK2.hpp"
#include "pkx/PK3.hpp"
#include "pkx/PK4.hpp"
#include "pkx/PK5.hpp"
#include "pkx/PK6.hpp"
#include "pkx/PK7.hpp"
#include "pkx/PK8.hpp"
#include "utils/ValueConverter.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/i18n.hpp"
#include "utils/random.hpp"
#include "utils/utils.hpp"
#include <algorithm>

namespace pksm
{
    PK2::PK2(PrivateConstructor, u8* dt, bool japanese, bool directAccess)
        : PKX(dt, japanese ? JP_LENGTH_WITH_NAMES : INT_LENGTH_WITH_NAMES, directAccess)
    {
        this->japanese = japanese;

        /*
        // detects korean by looking at the first byte of OT name to see if it's a korean codepoint page index
        if (!japanese)
        {
            for (int i = 0; i < 5; i++)
            {
                if (dt[PARTY_LENGTH + (2 * i)] <= 0xB)
                {
                    korean = true;
                    break;
                }
            }
        }

        // TODO: other language detection, which involves characters not used in the English keyboard
        lang = japanese ? Language::JPN : (korean ? Language::KOR : Language::ENG);
        */
       
        lang = japanese ? Language::JPN : Language::ENG;

        shiftedData = data + 3;
    }

    GameVersion PK2::version() const
    {
        return catchData() == 0 ? versionOfGame : GameVersion::C;
    }
    void PK2::version(GameVersion v)
    {
        if ((version() == GameVersion::C) ^ (v != GameVersion::C))
        {
            versionOfGame = v;
        }
    }

    std::unique_ptr<PKX> PK2::clone() const
    {
        return PKX::getPKM<Generation::TWO>(const_cast<u8*>(data), japanese ? JP_LENGTH_WITH_NAMES : INT_LENGTH_WITH_NAMES);
    }

    u8 PK2::currentFriendship() const
    {
        return otFriendship();
    }
    void PK2::currentFriendship(u8 v)
    {
        otFriendship(v);
    }

    u16 PK2::TID() const
    {
        return BigEndian::convertTo<u16>(shiftedData + 6);
    }
    void PK2::TID(u16 v)
    {
        BigEndian::convertFrom<u16>(shiftedData + 6, v);
    }

    std::string PK2::nickname() const
    {
        return StringUtils::getString2(shiftedData, 48 + (japanese ? 6 : 11), japanese ? 6 : 11, lang);
    }
    void PK2::nickname(const std::string_view& v)
    {
        StringUtils::setString2(shiftedData, v, 48 + (japanese ? 6 : 11), japanese ? 6 : 11, lang, japanese ? 6 : 11);
    }

    Language PK2::language() const
    {
        return lang;
    }
    void PK2::language(Language v)
    {
        if (((lang == Language::JPN) ^ (v != Language::JPN)) && ((lang == Language::KOR) ^ (v != Language::KOR)))
        {
            lang = v;
        }
    }

    std::string PK2::otName() const
    {
        return StringUtils::getString2(shiftedData, 48, japanese ? 6 : 11, lang);
    }
    void PK2::otName(const std::string_view& v)
    {
        StringUtils::setString2(shiftedData, v, 48, japanese ? 6 : 11, lang, japanese ? 6 : 11);
    }

    Species PK2::species() const
    {
        return Species{shiftedData[0]};
    }
    void PK2::species(Species v)
    {
        shiftedData[0] = u8(v);
        if (!egg()) data[1] = u8(v);
    }

    u8 PK2::heldItem2() const
    {
        return shiftedData[1];
    }
    void PK2::heldItem2(u8 v)
    {
        shiftedData[1] = v;
    }
    u16 PK2::heldItem() const
    {
        return ItemConverter::g2ToNational(shiftedData[1]);
    }
    void PK2::heldItem(u16 v)
    {
        shiftedData[1] = ItemConverter::nationalToG2(v);
    }
    void PK2::heldItem(const Item& item)
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
    u32 PK2::experience() const
    {
        return BigEndian::convertTo<u32>(shiftedData + 8) >> 8;
    }
    void PK2::experience(u32 v)
    {
        shiftedData[8] = v >> 16;
        shiftedData[9] = (v >> 8) & 0x00FF;
        shiftedData[10] = v & 0x0000FF;
    }
    u8 PK2::otFriendship() const
    {
        return shiftedData[27];
    }
    void PK2::otFriendship(u8 v)
    {
        shiftedData[27] = v;
    }
    u16 PK2::statExperience(Stat se) const
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        return BigEndian::convertTo<u16>(shiftedData + 11 + 2 * u8(se));
    }
    void PK2::statExperience(Stat se, u16 v)
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        BigEndian::convertFrom<u16>(shiftedData + 11 + 2 * u8(se), v);
    }

    Move PK2::move(u8 move) const
    {
        return Move{shiftedData[2 + move]};
    }
    void PK2::move(u8 move, Move v)
    {
        shiftedData[2 + move] = u8(v);
    }

    u8 PK2::PP(u8 move) const
    {
        return u8(shiftedData[23 + move] & 0x3F);
    }
    void PK2::PP(u8 move, u8 v)
    {
        shiftedData[23 + move] = u8((shiftedData[23 + move] & 0xC0) | (v & 0x3F));
    }
    u8 PK2::PPUp(u8 move) const
    {
        return shiftedData[23 + move] >> 6;
    }
    void PK2::PPUp(u8 move, u8 v)
    {
        shiftedData[23 + move] = u8((v & 0xC0) | (shiftedData[23 + move] & 0x3F));
    }
    u8 PK2::iv(Stat iv) const
    {
        switch (iv)
        {
            case Stat::HP:
                return ((PK2::iv(Stat::ATK) & 0x01) << 3) | ((PK2::iv(Stat::DEF) & 0x01) << 2) | ((PK2::iv(Stat::SPD) & 0x01) << 1) | (PK2::iv(Stat::SPATK) & 0x01);
            case Stat::ATK:
                return (shiftedData[0x15] & 0xF0) >> 4;
            case Stat::DEF:
                return shiftedData[0x15] & 0x0F;
            case Stat::SPD:
                return (shiftedData[0x16] & 0xF0) >> 4;
            case Stat::SPATK:
            case Stat::SPDEF:
                return shiftedData[0x16] & 0x0F;
            default:
                return 0;
        }
    }
    void PK2::iv(Stat iv, u8 v)
    {
        if (v > 15) v = 15;
        switch (iv)
        {
            case Stat::ATK:
                shiftedData[0x15] = ((v & 0x0F) << 4) | (shiftedData[0x15] & 0x0F);
                break;
            case Stat::DEF:
                shiftedData[0x15] = (shiftedData[0x15] & 0xF0) | (v & 0x0F);
                break;
            case Stat::SPD:
                shiftedData[0x16] = ((v & 0x0F) << 4) | (shiftedData[0x16] & 0x0F);
                break;
            case Stat::SPATK:
            case Stat::SPDEF:
                shiftedData[0x16] = (shiftedData[0x16] & 0xF0) | (v & 0x0F);
                break;
            case Stat::HP:
            default:
                return;
        }
    }
    bool PK2::egg() const
    {
        return data[1] == 0xFD;
    }
    void PK2::egg(bool v)
    {
        data[1] = v ? 0xFD : u8(species());
    }
    bool PK2::nicknamed() const
    {
        std::string target = species().localize(language());
        return nickname() != StringUtils::toUpper(target);
        // TODO: fix the fact that this sucks because this would flag every pokemon that's not JP, EN, or KOR
    }
    Gender PK2::gender() const
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
                return (iv(Stat::ATK) << 4) < genderType() ? Gender::Female : Gender::Male;
        }
    }
    void PK2::gender(Gender g)
    {
        switch (genderType())
        {
            case 255:
            case 254:
            case 0:
                return;
            default:
                iv(Stat::ATK, g ? (genderType() >> 4) : ((genderType() >> 4) - 1));
        }
    }
    u16 PK2::alternativeForm() const
    {
        if (species() == Species::Unown)
        {
            return ((((iv(Stat::ATK) >> 1) & 0x03) << 6) | (((iv(Stat::DEF) >> 1) & 0x03) << 4) | (((iv(Stat::SPD) >> 1) & 0x03) << 2) | ((iv(Stat::SPATK) >> 1) & 0x03)) / 10;
        }
        return 0;
    }
    void PK2::alternativeForm(u16 v)
    {
        //TODO
    }

    // this data is generated in Crystal, preserved in Gold and Silver
    u16 PK2::catchData() const
    {
        return BigEndian::convertTo<u16>(shiftedData + 29);
    }
    void PK2::catchData(u16 v)
    {
        BigEndian::convertFrom<u16>(shiftedData + 29, v);
    }

    u16 PK2::metLocation() const
    {
        return catchData() & 0x7F;
    }
    void PK2::metLocation(u16 v)
    {
        catchData((catchData() & 0xFF80) | (v & 0x7F));
    }
    u8 PK2::pkrs() const
    {
        return shiftedData[28];
    }
    void PK2::pkrs(u8 v)
    {
        shiftedData[28] = v;
    }
    u8 PK2::pkrsDays() const
    {
        return shiftedData[28] & 0xF;
    }
    void PK2::pkrsDays(u8 v)
    {
        shiftedData[28] = (shiftedData[28] & 0xF0) | v;
    }
    u8 PK2::pkrsStrain() const
    {
        return shiftedData[28] >> 4;
    }
    void PK2::pkrsStrain(u8 v)
    {
        shiftedData[28] = (v << 4) | (shiftedData[28] & 0x0F);
    }
    u8 PK2::metLevel() const
    {
        return (catchData() >> 8) & 0x3F;
    }
    void PK2::metLevel(u8 v)
    {
        catchData((catchData() & 0xC0FF) | ((v & 0x3F) << 8));
    }
    Gender PK2::otGender() const
    {
        return Gender{u8((catchData() & 0x80) >> 7)}; 
    }
    void PK2::otGender(Gender v)
    {
        if (u8(v) > 1) v = Gender::Male;
        catchData((catchData() & 0xFF7F) | (u8(v) << 7));
    }

    // 1 is morning, 2 is day, 3 is night, 0 is for PK2s not generated in Crystal
    u8 PK2::metTime() const
    {
        return catchData() >> 14;
    }
    void PK2::metTime(u8 v)
    {
        catchData((catchData() & 0x3FFF) | ((v & 0x03) << 14));
    }

    Type PK2::hpType() const
    {
        return Type{u8((((iv(Stat::ATK) & 0x3) << 2) | (iv(Stat::DEF) & 0x3)) + 1)};
    }
    void PK2::hpType(Type v)
    {
        //TODO
    }

    u8 PK2::level() const
    {
        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
            ;
        return i;
    }
    void PK2::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
    }
    bool PK2::shiny() const
    {
        return (ev(Stat::DEF) == 10) && (ev(Stat::SPD) == 10) && (ev(Stat::SPATK) == 10) && (((ev(Stat::ATK) & 0x02) >> 1) == 1);
    }
    void PK2::shiny(bool v)
    {
        if (v)
        {
            iv(Stat::DEF, 10);
            iv(Stat::SPD, 10);
            iv(Stat::SPATK, 10);
        }
        iv(Stat::ATK, v ? 7 : 8);
    }
    u16 PK2::formSpecies() const
    {
        return u16(species());
    }
    u16 PK2::stat(Stat stat) const
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
            case Stat::SPD:
                base = baseSpe();
                break;
            case Stat::SPATK:
                base = baseSpa();
                break;
            case Stat::SPDEF:
                base = baseSpd();
                break;
            default:
                base = 0;
        }
        u16 EV = u16(std::min(255, int(std::sqrt(statExperience(stat)))) >> 2);
        u16 mid = u16(((2 * (base + iv(stat)) + EV) * level() / 100) + 5);
        if (stat == Stat::HP) return mid + 5 + level();
        return mid;
    }

    int PK2::partyCurrHP() const
    {
        return isParty() ? BigEndian::convertTo<u16>(shiftedData + 34) : -1;
    }
    void PK2::partyCurrHP(u16 v)
    {
        if (isParty()) BigEndian::convertFrom<u16>(shiftedData + 34, v);
    }
    int PK2::partyStat(Stat stat) const
    {
        return isParty() ? BigEndian::convertTo<u16>(shiftedData + 36 + 2 * u8(stat)) : -1;
    }
    void PK2::partyStat(Stat stat, u16 v)
    {
        if (isParty()) BigEndian::convertFrom<u16>(shiftedData + 36 + 2 * u8(stat), v);
    }
    int PK2::partyLevel() const
    {
        return isParty() ? shiftedData[31] : -1;
    }
    void PK2::partyLevel(u8 v)
    {
        if (isParty()) shiftedData[31] = v;
    }
    void PK2::updatePartyData()
    {
        partyLevel(level());
        for (u8 pstat = u8(Stat::HP); pstat <= u8(Stat::SPDEF); pstat++)
        {
            partyStat(static_cast<Stat>(pstat), stat(static_cast<Stat>(pstat)));
        }
    }
}
