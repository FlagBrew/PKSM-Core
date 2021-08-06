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
    PK1::PK1(PrivateConstructor, u8* dt, bool japanese, bool directAccess)
        : PKX(dt, japanese ? JP_LENGTH_WITH_NAMES : EN_LENGTH_WITH_NAMES, directAccess)
    {
        this->japanese = japanese;
        // TODO: other language detection, which involves characters not used in the English keyboard
        lang = japanese ? Language::JPN : Language::ENG;
        shiftedData = data + 3;
    }

    /*
    std::unique_ptr<PK7> PK1::convertToG7(Sav &sav) const
    {

    }
    */

    std::unique_ptr<PKX> PK1::clone() const
    {
        return PKX::getPKM<Generation::ONE>(const_cast<u8*>(data), japanese ? JP_LENGTH_WITH_NAMES : EN_LENGTH_WITH_NAMES);
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
        return StringUtils::getString1(shiftedData, 44 + (japanese ? 6 : 11), japanese ? 6 : 11, lang);
    }
    void PK1::nickname(const std::string_view& v)
    {
        StringUtils::setString1(shiftedData, v, 44 + (japanese ? 6 : 11), japanese ? 6 : 11, lang, japanese ? 6 : 11);
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
        return StringUtils::getString1(shiftedData, 44, japanese ? 6 : 11, lang);
    }
    void PK1::otName(const std::string_view& v)
    {
        StringUtils::setString1(shiftedData, v, 44, japanese ? 6 : 11, lang, japanese ? 6 : 11);
    }

    u8 PK1::speciesID1() const
    {
        return shiftedData[0];
    }
    void PK1::speciesID1(u8 v)
    {
        data[1] = v;
        shiftedData[0] = v;
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
        return BigEndian::convertTo<u16>(shiftedData + 7);
    }
    void PK1::catchRate(u16 v)
    {
        BigEndian::convertFrom<u16>(shiftedData + 7, v);
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
    u16 PK1::statExperience(Stat se) const
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        return BigEndian::convertTo<u16>(shiftedData + 17 + 2 * u8(se));
    }
    void PK1::statExperience(Stat se, u16 v)
    {
        if (se == Stat::SPDEF) se = Stat::SPATK;
        BigEndian::convertFrom<u16>(shiftedData + 17 + 2 * u8(se), v);
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
                return ((PK1::iv(Stat::ATK) & 0x01) << 3) | ((PK1::iv(Stat::DEF) & 0x01) << 2) | ((PK1::iv(Stat::SPD) & 0x01) << 1) | (PK1::iv(Stat::SPATK) & 0x01);
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
        if (v > 15) v = 15;
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
        std::string target = species().localize(language());
        return nickname() != StringUtils::toUpper(target);
        // TODO: fix the fact that this sucks because this would flag every pokemon that's not Japanese or English
    }
    u8 PK1::level() const
    {
        u8 i      = 1;
        u8 xpType = expType();
        while (experience() >= expTable(i, xpType) && ++i < 100)
            ;
        return i;
    }
    void PK1::level(u8 v)
    {
        experience(expTable(v - 1, expType()));
        boxLevel(v);
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
        u16 EV = u16(std::min(255, int(std::sqrt(statExperience(stat)))) >> 2);
        u16 mid = u16(((2 * (base + iv(stat)) + EV) * level() / 100) + 5);
        if (stat == Stat::HP) return mid + 5 + level();
        return mid;
    }

    int PK1::partyCurrHP() const
    {
        return isParty() ? BigEndian::convertTo<u16>(shiftedData + 1) : -1;
    }
    void PK1::partyCurrHP(u16 v)
    {
        if (isParty()) BigEndian::convertFrom<u16>(shiftedData + 1, v);
    }
    int PK1::partyStat(Stat stat) const
    {
        if (!isParty()) return -1;
        if (stat == Stat::SPDEF) stat = Stat::SPATK;
        return BigEndian::convertTo<u16>(shiftedData + 34 + 2 * u8(stat));
    }
    void PK1::partyStat(Stat stat, u16 v)
    {
        if (isParty()) {
            if (stat == Stat::SPDEF) stat = Stat::SPATK;
            BigEndian::convertFrom<u16>(shiftedData + 34 + 2 * u8(stat), v);
        }
    }
    int PK1::partyLevel() const
    {
        return isParty() ? shiftedData[33] : -1;
    }
    void PK1::partyLevel(u8 v)
    {
        if (isParty()) shiftedData[33] = v;
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
    }

    void PK1::writeG1Types()
    {
        shiftedData[5] = type1() < Type::Steel ? u8(type1()) : (20 + (u8(type1()) - u8(Type::Fire)));
        shiftedData[6] = type2() < Type::Steel ? u8(type2()) : (20 + (u8(type2()) - u8(Type::Fire)));
    }    
}

