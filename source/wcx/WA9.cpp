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

#include "wcx/WA9.hpp"
#include "personal/personal.hpp"
#include "utils/endian.hpp"
#include "utils/utils.hpp"

namespace
{
    // WA9 uses 9 language slots, same indexing as WC9/WC8
    int langIndex(pksm::Language lang)
    {
        if (lang > pksm::Language::UNUSED)
        {
            return u8(lang) - 2;
        }
        else if (lang == pksm::Language::UNUSED)
        {
            return 2 - 1; // English
        }
        else
        {
            return u8(lang) - 1;
        }
    }

    // Same RibbonIndex mapping as WC9
    u8 toRibbonIndex(pksm::Ribbon rib)
    {
        u32 val = u32(rib);
        if (val <= 45)
        {
            return u8(val);
        }
        if (val >= 94)
        {
            return u8(val - 48);
        }
        return 0xFF;
    }
}

namespace pksm
{
    WA9::WA9(u8* dt)
    {
        std::copy(dt, dt + length, data);
        for (int i = 0; i < 6; i++)
        {
            if (object(i) != 0)
            {
                numItems++;
            }
            else
            {
                break;
            }
        }
    }

    Generation WA9::generation() const
    {
        return Generation::NINE;
    }

    u16 WA9::ID() const
    {
        return LittleEndian::convertTo<u16>(data + 0x08);
    }

    std::string WA9::title() const
    {
        return title(titleIndex());
    }

    std::string WA9::title(Language lang) const
    {
        (void)lang;
        return "Wonder Card";
    }

    Language WA9::titleIndex() const
    {
        return Language(data[0x15]);
    }

    u8 WA9::type() const
    {
        return data[0x11];
    }

    u8 WA9::flags() const
    {
        return data[0x10];
    }

    bool WA9::multiObtainable() const
    {
        return (flags() & 1) == 0;
    }

    bool WA9::BP(void) const
    {
        return type() == 3;
    }

    bool WA9::item(void) const
    {
        return type() == 2;
    }

    bool WA9::pokemon(void) const
    {
        return type() == 1;
    }

    bool WA9::clothing(void) const
    {
        return type() == 4;
    }

    u16 WA9::object(void) const
    {
        return object(0);
    }

    u16 WA9::object(int index) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18 + index * 4);
    }

    int WA9::items(void) const
    {
        return numItems;
    }

    u16 WA9::objectQuantity(void) const
    {
        return objectQuantity(0);
    }

    u16 WA9::objectQuantity(int index) const
    {
        return LittleEndian::convertTo<u16>(data + 0x1A + index * 4);
    }

    u8 WA9::PIDType(void) const
    {
        return data[0x278];
    }

    bool WA9::shiny(void) const
    {
        return PIDType() == 2 || PIDType() == 3;
    }

    u16 WA9::TID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18);
    }

    u16 WA9::SID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x1A);
    }

    GameVersion WA9::version(void) const
    {
        return GameVersion(LittleEndian::convertTo<u32>(data + 0x1C));
    }

    u32 WA9::encryptionConstant(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x20);
    }

    u32 WA9::PID(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x24);
    }

    Ball WA9::ball(void) const
    {
        return Ball{u8(LittleEndian::convertTo<u16>(data + 0x25C))};
    }

    u16 WA9::heldItem(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x25E);
    }

    Move WA9::move(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x260 + m * 2)};
    }

    Move WA9::relearnMove(u8 index) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x268 + index * 2)};
    }

    Species WA9::species(void) const
    {
        return Species{LittleEndian::convertTo<u16>(data + 0x270)};
    }

    u8 WA9::alternativeForm(void) const
    {
        return data[0x272];
    }

    Gender WA9::gender(void) const
    {
        return Gender{data[0x273]};
    }

    u8 WA9::level(void) const
    {
        return data[0x274];
    }

    bool WA9::egg(void) const
    {
        return data[0x275] == 1;
    }

    Nature WA9::nature(void) const
    {
        return Nature{data[0x276]};
    }

    u8 WA9::abilityType(void) const
    {
        return data[0x277];
    }

    Ability WA9::ability(void) const
    {
        u8 abilitynum, type = abilityType();

        if (type == 2 || type == 4)
        {
            abilitynum = 2;
        }
        else
        {
            abilitynum = 0;
        }

        return PersonalZA::ability(u16(species()), abilitynum);
    }

    u8 WA9::metLevel(void) const
    {
        return data[0x279];
    }

    bool WA9::isAlpha(void) const
    {
        return data[0x2AE] != 0;
    }

    u16 WA9::scale(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x2AC);
    }

    u8 WA9::ribbonAtIndex(int index) const
    {
        if (index < 0 || index >= 0x20)
        {
            return 0xFF;
        }
        return data[0x27A + index];
    }

    u8 WA9::iv(Stat index) const
    {
        return data[0x29A + u8(index)];
    }

    Gender WA9::otGender(void) const
    {
        return Gender{data[0x2A0]};
    }

    u16 WA9::ev(Stat index) const
    {
        return data[0x2A1 + u8(index)];
    }

    u8 WA9::otIntensity(void) const
    {
        return data[0x2A7];
    }

    u8 WA9::otMemory(void) const
    {
        return data[0x2A8];
    }

    u8 WA9::otFeeling(void) const
    {
        return data[0x2A9];
    }

    u16 WA9::otTextvar(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x2AA);
    }

    u16 WA9::eggLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x258);
    }

    u16 WA9::metLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x25A);
    }

    bool WA9::hasRibbon(Ribbon rib) const
    {
        u8 idx = toRibbonIndex(rib);
        return idx != 0xFF;
    }

    bool WA9::ribbon(Ribbon rib) const
    {
        u8 idx = toRibbonIndex(rib);
        if (idx == 0xFF)
        {
            return false;
        }
        for (int i = 0; i < 0x20; i++)
        {
            if (data[0x27A + i] == idx)
            {
                return true;
            }
        }
        return false;
    }

    bool WA9::nicknamed(Language lang) const
    {
        return nickname(lang)[0] != 0;
    }

    std::string WA9::nickname(Language lang) const
    {
        return StringUtils::getString(data, 0x28 + langIndex(lang) * 0x1C, 13);
    }

    Language WA9::nicknameLanguage(Language orig) const
    {
        return Language(data[0x28 + langIndex(orig) * 0x1C + 0x1A]);
    }

    std::string WA9::otName(Language lang) const
    {
        return StringUtils::getString(data, 0x140 + langIndex(lang) * 0x1C, 13);
    }

    std::string WA9::nickname() const
    {
        return nickname(Language::ENG);
    }

    std::string WA9::otName() const
    {
        return otName(Language::ENG);
    }

    bool WA9::used() const
    {
        return false;
    }

    u16 WA9::formSpecies(void) const
    {
        u16 tmpSpecies = u16(species());
        u8 form        = alternativeForm();
        u8 formcount   = PersonalZA::formCount(tmpSpecies);

        if (form && form < formcount)
        {
            u16 backSpecies = tmpSpecies;
            tmpSpecies      = PersonalZA::formStatIndex(tmpSpecies);
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
}
