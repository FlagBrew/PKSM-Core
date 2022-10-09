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

#ifndef WC8_HPP
#define WC8_HPP

#include "enums/GameVersion.hpp"
#include "wcx/WCX.hpp"

namespace pksm
{
    class WC8 : public WCX
    {
    private:
        [[nodiscard]] int year(void) const override { return 1900; }

        [[nodiscard]] int month(void) const override { return 1; }

        [[nodiscard]] int day(void) const override { return 1; }

        void year(int) override {}

        void month(int) override {}

        void day(int) override {}

    protected:
        u8 data[0x2D0];
        int numItems = 0;

    public:
        static constexpr u16 length = 0x2D0;

        explicit WC8(u8* dt);

        [[nodiscard]] std::string_view extension(void) const override { return ".wc8"; }

        [[nodiscard]] Generation generation(void) const override;

        [[nodiscard]] bool bean(void) const override { return false; }

        [[nodiscard]] bool BP(void) const override;
        [[nodiscard]] bool item(void) const override;
        [[nodiscard]] bool pokemon(void) const override;
        [[nodiscard]] bool clothing(void) const;

        [[nodiscard]] bool power(void) const override { return false; }

        [[nodiscard]] std::string title(void) const override;
        [[nodiscard]] std::string title(Language lang) const;
        [[nodiscard]] Language titleIndex(void) const;
        [[nodiscard]] u8 type(void) const override;
        [[nodiscard]] u16 ID(void) const override;
        [[nodiscard]] u16 object(void) const override;
        [[nodiscard]] u16 objectQuantity(void) const;
        // Multiple-item cards
        [[nodiscard]] u16 object(int index) const;
        [[nodiscard]] u16 objectQuantity(int index) const;
        [[nodiscard]] int items(void) const;
        [[nodiscard]] bool multiObtainable(void) const override;
        [[nodiscard]] u8 flags(void) const override;

        [[nodiscard]] u8 cardLocation(void) const override { return 0; }

        [[nodiscard]] bool used(void) const override;

        // Pokemon properties
        [[nodiscard]] Move move(u8 index) const override;
        [[nodiscard]] Move relearnMove(u8 index) const;
        [[nodiscard]] Species species(void) const override;
        [[nodiscard]] u8 alternativeForm(void) const override;
        [[nodiscard]] Gender gender(void) const override;
        [[nodiscard]] u8 level(void) const override;
        [[nodiscard]] bool egg(void) const override;
        [[nodiscard]] Nature nature(void) const override;
        [[nodiscard]] u8 abilityType(void) const override;
        [[nodiscard]] u8 PIDType(void) const override;
        [[nodiscard]] u8 metLevel(void) const override;
        [[nodiscard]] u8 dynamaxLevel(void) const;
        [[nodiscard]] bool canGigantamax(void) const;
        [[nodiscard]] u8 ribbonValue(int index) const;
        [[nodiscard]] u8 iv(Stat index) const override;
        [[nodiscard]] Gender otGender(void) const;
        [[nodiscard]] u16 ev(Stat index) const;
        [[nodiscard]] bool shiny(void) const override;
        [[nodiscard]] Ability ability(void) const override;
        [[nodiscard]] Ball ball(void) const override;
        [[nodiscard]] u32 encryptionConstant(void) const;
        [[nodiscard]] u16 heldItem(void) const override;

        [[nodiscard]] Language language(void) const override
        {
            return Language::UNUSED;
        } // Apparently all WCs are now multi-language?

        [[nodiscard]] std::string nickname(void) const override;
        [[nodiscard]] std::string nickname(Language lang) const;
        [[nodiscard]] Language nicknameLanguage(Language orig) const;
        [[nodiscard]] bool nicknamed(Language lang) const;
        [[nodiscard]] GameVersion version(void) const;
        [[nodiscard]] std::string otName(void) const override;
        [[nodiscard]] std::string otName(Language lang) const;
        [[nodiscard]] u8 otIntensity(void) const;
        [[nodiscard]] u8 otMemory(void) const;
        [[nodiscard]] u16 otTextvar(void) const;
        [[nodiscard]] u8 otFeeling(void) const;
        [[nodiscard]] u16 TID(void) const override;
        [[nodiscard]] u16 SID(void) const override;
        [[nodiscard]] u32 PID(void) const override;
        [[nodiscard]] u16 eggLocation(void) const override;
        [[nodiscard]] u16 metLocation(void) const override;

        [[nodiscard]] u8 contest(u8) const override { return 0; }

        [[nodiscard]] bool hasRibbon(Ribbon rib) const override;
        [[nodiscard]] bool ribbon(Ribbon rib) const override;

        [[nodiscard]] u16 formSpecies(void) const override;

        [[nodiscard]] int size(void) const override { return length; }

        [[nodiscard]] const u8* rawData(void) const override { return data; }
    };
}

#endif
