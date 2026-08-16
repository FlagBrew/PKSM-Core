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

#include "wcx/WC9.hpp"
#include "personal/personal.hpp"
#include "utils/endian.hpp"
#include "utils/utils.hpp"

namespace
{
    // WC9 uses 9 language slots (JPN=0, ENG=1, FRA=2, ITA=3, DEU=4, SPA=5, KOR=6, CHS=7, CHT=8)
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

    // WC9/WA9 store ribbons as index bytes.
    // PKSM's Ribbon enum has 48 extra G3/G4 contest entries between index 45 and 94.
    // Mapping: card indices 0-45 == Ribbon 0-45; 46+ == Ribbon (idx + 48)
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
        return 0xFF; // G3/G4 contest ribbons not in RibbonIndex
    }
}

namespace pksm
{
    WC9::WC9(u8* dt)
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

    Generation WC9::generation() const
    {
        return Generation::NINE;
    }

    u16 WC9::ID() const
    {
        return LittleEndian::convertTo<u16>(data + 0x08);
    }

    std::string WC9::title() const
    {
        return title(titleIndex());
    }

    std::string WC9::title(Language lang) const
    {
        (void)lang;
        return "Wonder Card";
    }

    Language WC9::titleIndex() const
    {
        return Language(data[0x15]);
    }

    u8 WC9::type() const
    {
        return data[0x11];
    }

    u8 WC9::flags() const
    {
        return data[0x10];
    }

    bool WC9::multiObtainable() const
    {
        return (flags() & 1) == 0; // WC9: inverted from WC8 (0 = repeatable)
    }

    bool WC9::BP(void) const
    {
        return type() == 3;
    }

    bool WC9::item(void) const
    {
        return type() == 2;
    }

    bool WC9::pokemon(void) const
    {
        return type() == 1;
    }

    bool WC9::clothing(void) const
    {
        return type() == 4;
    }

    u16 WC9::object(void) const
    {
        return object(0);
    }

    u16 WC9::object(int index) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18 + index * 4);
    }

    int WC9::items(void) const
    {
        return numItems;
    }

    u16 WC9::objectQuantity(void) const
    {
        return objectQuantity(0);
    }

    u16 WC9::objectQuantity(int index) const
    {
        return LittleEndian::convertTo<u16>(data + 0x1A + index * 4);
    }

    u8 WC9::PIDType(void) const
    {
        return data[0x240];
    }

    bool WC9::shiny(void) const
    {
        return PIDType() == 2 || PIDType() == 3;
    }

    u16 WC9::TID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x18);
    }

    u16 WC9::SID(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x1A);
    }

    GameVersion WC9::version(void) const
    {
        return GameVersion(LittleEndian::convertTo<u32>(data + 0x1C));
    }

    u32 WC9::encryptionConstant(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x20);
    }

    u32 WC9::PID(void) const
    {
        return LittleEndian::convertTo<u32>(data + 0x24);
    }

    Ball WC9::ball(void) const
    {
        return Ball{u8(LittleEndian::convertTo<u16>(data + 0x224))};
    }

    u16 WC9::heldItem(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x226);
    }

    Move WC9::move(u8 m) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x228 + m * 2)};
    }

    Move WC9::relearnMove(u8 index) const
    {
        return Move{LittleEndian::convertTo<u16>(data + 0x230 + index * 2)};
    }

    Species WC9::species(void) const
    {
        return Species{LittleEndian::convertTo<u16>(data + 0x238)};
    }

    u8 WC9::alternativeForm(void) const
    {
        return data[0x23A];
    }

    Gender WC9::gender(void) const
    {
        return Gender{data[0x23B]};
    }

    u8 WC9::level(void) const
    {
        return data[0x23C];
    }

    bool WC9::egg(void) const
    {
        return data[0x23D] == 1;
    }

    Nature WC9::nature(void) const
    {
        return Nature{data[0x23E]};
    }

    u8 WC9::abilityType(void) const
    {
        return data[0x23F];
    }

    Ability WC9::ability(void) const
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

        return PersonalSV::ability(u16(species()), abilitynum);
    }

    u8 WC9::metLevel(void) const
    {
        return data[0x241];
    }

    u8 WC9::teraType(void) const
    {
        return data[0x242];
    }

    s16 WC9::heightValue(void) const
    {
        return LittleEndian::convertTo<s16>(data + 0x244);
    }

    s16 WC9::weightValue(void) const
    {
        return LittleEndian::convertTo<s16>(data + 0x246);
    }

    u16 WC9::scale(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x0C);
    }

    u8 WC9::ribbonAtIndex(int index) const
    {
        if (index < 0 || index >= 0x20)
        {
            return 0xFF;
        }
        return data[0x248 + index];
    }

    u8 WC9::iv(Stat index) const
    {
        return data[0x268 + u8(index)];
    }

    Gender WC9::otGender(void) const
    {
        return Gender{data[0x26E]};
    }

    u16 WC9::ev(Stat index) const
    {
        return data[0x26F + u8(index)];
    }

    u8 WC9::otIntensity(void) const
    {
        return data[0x275];
    }

    u8 WC9::otMemory(void) const
    {
        return data[0x276];
    }

    u8 WC9::otFeeling(void) const
    {
        return data[0x277];
    }

    u16 WC9::otTextvar(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x278);
    }

    u16 WC9::eggLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x220);
    }

    u16 WC9::metLocation(void) const
    {
        return LittleEndian::convertTo<u16>(data + 0x222);
    }

    bool WC9::hasRibbon(Ribbon rib) const
    {
        u8 idx = toRibbonIndex(rib);
        return idx != 0xFF;
    }

    bool WC9::ribbon(Ribbon rib) const
    {
        u8 idx = toRibbonIndex(rib);
        if (idx == 0xFF)
        {
            return false;
        }
        for (int i = 0; i < 0x20; i++)
        {
            if (data[0x248 + i] == idx)
            {
                return true;
            }
        }
        return false;
    }

    bool WC9::nicknamed(Language lang) const
    {
        return nickname(lang)[0] != 0;
    }

    std::string WC9::nickname(Language lang) const
    {
        return StringUtils::getString(data, 0x28 + langIndex(lang) * 0x1C, 13);
    }

    Language WC9::nicknameLanguage(Language orig) const
    {
        return Language(data[0x28 + langIndex(orig) * 0x1C + 0x1A]);
    }

    std::string WC9::otName(Language lang) const
    {
        return StringUtils::getString(data, 0x124 + langIndex(lang) * 0x1C, 13);
    }

    std::string WC9::nickname() const
    {
        return nickname(Language::ENG);
    }

    std::string WC9::otName() const
    {
        return otName(Language::ENG);
    }

    bool WC9::used() const
    {
        return false;
    }

    u16 WC9::formSpecies(void) const
    {
        u16 tmpSpecies = u16(species());
        u8 form        = alternativeForm();
        u8 formcount   = PersonalSV::formCount(tmpSpecies);

        if (form && form < formcount)
        {
            u16 backSpecies = tmpSpecies;
            tmpSpecies      = PersonalSV::formStatIndex(tmpSpecies);
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
