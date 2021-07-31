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

#include "pkx/PK1.hpp"
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
    // TODO: bang head on wall until i figure out how to put in strings
    PK1::PK1(PrivateConstructor, u8* dt, bool party, bool directAccess)
        : PKX(dt, party ? PARTY_LENGTH : BOX_LENGTH, directAccess)
    {
        lang = japanese ? Language::JPN : Language::ENG;
    }

    /*
    std::unique_ptr<PK7> PK1::convertToG7(Sav &sav) const
    {

    }
    */

    std::unique_ptr<PKX> PK1::clone() const
    {
        return PKX::getPKM<Generation::ONE>(const_cast<u8*>(data), isParty());
    }

    u16 PK1::TID() const
    {
        return BigEndian::convertTo<u16>(data + 12);
    }
    void PK1::TID(u16 v)
    {
        BigEndian::convertFrom<u16>(data + 12, v);
    }
    std::string PK1::nickname() const
    {
        return "filler";
    }
    void PK1::nickname(const std::string_view& v)
    {

    }
    Language PK1::language() const
    {
        return lang;
    }
    void PK1::language(Language v)
    {
        // save data is too different to allow converting anything between japanese and another language
        if (lang != Language::JPN && v != Language::JPN)
        {
            lang = v;
        }
    }
    std::string PK1::otName() const
    {
        return "filler";
    }
    void PK1::otName(const std::string_view& v)
    {

    }

    u8 PK1::speciesID1() const
    {
        return data[0];
    }
    void PK1::speciesID1(u8 v)
    {
        data[0] = v;
        writeG1Types();
    }

    Species PK1::species() const
    {
        return SpeciesConverter::g1ToNational(speciesID1());
    }
    void PK1::species(Species v)
    {
        speciesID1(SpeciesConverter::nationalToG1(v));
    }
    // yes, Gen I pokemon data stores catch rate, and yes it's actually important for Gen II importing
    u16 PK1::catchRate() const
    {
        return BigEndian::convertTo<u16>(data + 7);
    }
    void PK1::catchRate(u16 v)
    {
        BigEndian::convertFrom<u16>(data + 7, v);
    }
    // experience is actually 3 bytes
    u32 PK1::experience() const
    {
        return (BigEndian::convertTo<u16>(data + 14) << 8) | data[16];
    }
    void PK1::experience(u32 v)
    {
        BigEndian::convertFrom<u16>(data + 14, (u16)(v >> 8));
        data[16] = u8(v);
    }
    u16 PK1::statExperience(Stat se) const
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        return BigEndian::convertTo<u16>(data + 17 + 2 * u8(se));
    }
    void PK1::statExperience(Stat se, u16 v)
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        BigEndian::convertFrom<u16>(data + 17 + 2 * u8(se), v);
    }

    Move PK1::move(u8 move) const
    {
        return Move{data[8 + move]};
    }
    void PK1::move(u8 move, Move v)
    {
        data[8 + move] = u8(v);
    }
    u8 PK1::PP(u8 move) const
    {
        return u8(data[29 + move] & 0x3F);
    }
    void PK1::PP(u8 move, u8 v)
    {
        data[29 + move] = u8((data[29 + move] & 0xC0) | (v & 0x3F));
    }
    u8 PK1::PPUp(u8 move) const
    {
        return data[29 + move] >> 6;
    }
    void PK1::PPUp(u8 move, u8 v)
    {
        data[29 + move] = u8((v & 0xC0) | (data[29 + move] & 0x3F));
    }
    u8 PK1::iv(Stat iv) const
    {
        if (iv == Stat::HP) {
            return (data[27] & 0x10 >> 1) | (data[27] & 0x01 << 2) | (data[28] & 0x10 >> 3) | (data[28] & 0x01);
        }
        if (iv == Stat::SPDEF) iv = Stat::SPATK;
        return u8((BigEndian::convertTo<u16>(data + 27) >> 4 * (u8(iv) - 1)) & 0x0F);
    }
    void PK1::iv(Stat iv, u8 v)
    {
        if (iv == Stat::HP) return;
        if (iv == Stat::SPDEF) iv = Stat::SPATK;
        data[27 + (u8(iv) - 1)/2] = u8((v & 0x0F) << (((u8(iv) - 1) % 2) ? 0 : 4));
    }
    bool PK1::nicknamed() const
    {
        std::string target = species().localize(language());
        return nickname() != StringUtils::toUpper(target);
        // TODO: fix the fact that this sucks because this would flag every pokemon that's not Japanese or English
    }
    u8 PK1::level() const
    {
        u8 i = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100);
        return i;
    }
    void PK1::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
    }
    bool PK1::shiny() const
    {
        u8 atkEV = ev(Stat::ATK);
        return (ev(Stat::DEF) == 10) && (ev(Stat::SPD) == 10) && (ev(Stat::SPATK) == 10) && (
            atkEV == 2 || atkEV == 3 || atkEV == 6 || atkEV == 7 || atkEV == 10 || atkEV == 11 || atkEV == 14 || atkEV == 15);
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
    u16 PK1::stat(Stat stat) const
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
                base = baseSpd();
                break;
            case Stat::SPATK:
            case Stat::SPDEF:
                base = baseSpa();
                break;
            default:
                base = 0;
        }
        u16 partOne = u16((((base + iv(stat)) * 2 + u16(sqrt(statExperience(stat)) / 4)) * level()) / 100);
        if (stat == Stat::HP) return partOne + level() + 10;
        return partOne + 5;
    }

    int PK1::partyCurrHP() const
    {
        return isParty() ? BigEndian::convertTo<u16>(data + 1) : -1;
    }
    void PK1::partyCurrHP(u16 v)
    {
        if (isParty()) BigEndian::convertFrom<u16>(data + 1, v);
    }
    int PK1::partyStat(Stat stat) const
    {
        if (!isParty()) return -1;
        if (stat == Stat::SPDEF) stat = Stat::SPATK;
        return BigEndian::convertTo<u16>(data + 34 + 2 * u8(stat));
    }
    void PK1::partyStat(Stat stat, u16 v)
    {
        if (isParty()) {
            if (stat == Stat::SPDEF) stat = Stat::SPATK;
            BigEndian::convertFrom<u16>(data + 34 + 2 * u8(stat), v);
        }
    }
    int PK1::partyLevel() const
    {
        return isParty() ? data[34] : -1;
    }
    void PK1::partyLevel(u8 v)
    {
        if (isParty()) data[34] = v;
    }
    void PK1::updatePartyData()
    {
        partyLevel(level());
        for (u8 pstat = u8(Stat::HP); pstat != u8(Stat::SPDEF); pstat++)
        {
            partyStat(static_cast<Stat>(pstat), stat(static_cast<Stat>(pstat)));
        }
    }

    void PK1::writeG1Types()
    {
        data[5] = type1() < Type::Steel ? u8(type1()) : (20 + (u8(type1()) - u8(Type::Fire)));
        data[6] = type2() < Type::Steel ? u8(type2()) : (20 + (u8(type2()) - u8(Type::Fire)));
    }    
}

