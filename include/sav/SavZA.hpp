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

#ifndef SAVZA_HPP
#define SAVZA_HPP

#include "sav/Sav8.hpp"

namespace pksm
{
    class SavZA : public Sav8
    {
    private:
        static constexpr u32 KBox       = 0x0D66012C;
        static constexpr u32 KParty     = 0x3AA1A9AD;
        static constexpr u32 KStatus    = 0xE3E89BD1;
        static constexpr u32 KItems     = 0x21C9BD44;
        static constexpr u32 KBoxLayout = 0x19722C89;

        static constexpr u32 KCurrentBox         = 0x017C3CBB;
        static constexpr u32 KBoxesUnlocked      = 0x71825204;
        static constexpr u32 KBoxWallpapers      = 0x2EB1B190;
        static constexpr u32 KMoney              = 0x4F35D0DD;
        static constexpr u32 KPlayedSeconds      = 0xCE3AF8F2;
        static constexpr u32 KTicketPointsRoyale = 0x9A730DE1;
        static constexpr u32 KPokedex            = 0x2D87BE5C;

    public:
        static constexpr size_t SIZE_G9ZA_100 = 0x2F3284; // v1.0.0
        static constexpr size_t SIZE_G9ZA_102 = 0x2F3289; // v1.0.2
        static constexpr size_t SIZE_G9ZA_200 = 0x309FA6; // v2.0.0 (Mega Dimension DLC)
        static constexpr size_t SIZE_G9ZA_201 = 0x309FB3; // v2.0.0 + revision

        SavZA(const std::shared_ptr<u8[]>& dt, size_t length);

        [[nodiscard]] Generation generation(void) const override { return Generation::NINE; }

        [[nodiscard]] u16 TID(void) const override;
        void TID(u16 v) override;
        [[nodiscard]] u16 SID(void) const override;
        void SID(u16 v) override;
        [[nodiscard]] GameVersion version(void) const override;
        void version(GameVersion v) override;
        [[nodiscard]] Gender gender(void) const override;
        void gender(Gender v) override;
        [[nodiscard]] std::string otName(void) const override;
        void otName(const std::string_view& v) override;
        [[nodiscard]] Language language(void) const override;
        void language(Language v) override;
        [[nodiscard]] u32 money(void) const override;
        void money(u32 v) override;
        [[nodiscard]] u32 BP(void) const override;
        void BP(u32 v) override;

        [[nodiscard]] u8 badges(void) const override { return 0; } // Z-A has no gym badges

        [[nodiscard]] u16 playedHours(void) const override;
        void playedHours(u16 v) override;
        [[nodiscard]] u8 playedMinutes(void) const override;
        void playedMinutes(u8 v) override;
        [[nodiscard]] u8 playedSeconds(void) const override;
        void playedSeconds(u8 v) override;

        void item(const Item& item, Pouch pouch, u16 slot) override;
        [[nodiscard]] std::unique_ptr<Item> item(Pouch pouch, u16 slot) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, int>, 15> pouches(void) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, std::span<const int>>, 15> validItems(
            void) const override;

        [[nodiscard]] u8 subRegion(void) const override { return 0; }

        void subRegion(u8) override {}

        [[nodiscard]] u8 country(void) const override { return 0; }

        void country(u8) override {}

        [[nodiscard]] u8 consoleRegion(void) const override { return 0; }

        void consoleRegion(u8) override {}

        [[nodiscard]] std::string boxName(u8 box) const override;
        void boxName(u8 box, const std::string_view& name) override;
        [[nodiscard]] u8 boxWallpaper(u8 box) const override;
        void boxWallpaper(u8 box, u8 v) override;

        [[nodiscard]] u8 currentBox(void) const override;
        void currentBox(u8 v) override;
        [[nodiscard]] u8 unlockedBoxes(void) const override;
        void unlockedBoxes(u8 v) override;

        [[nodiscard]] u8 legendBoxUnlockSize(void) const override { return 0; }

        [[nodiscard]] u32 boxOffset(u8 box, u8 slot) const override;
        [[nodiscard]] u32 partyOffset(u8 slot) const override;
        [[nodiscard]] u8 partyCount(void) const override;
        void partyCount(u8 count) override;

        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 slot) const override;
        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 box, u8 slot) const override;
        void pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade) override;
        void pkm(const PKX& pk, u8 slot) override;

        void cryptBoxData(bool crypted) override;

        [[nodiscard]] int maxBoxes(void) const override { return 32; }

        [[nodiscard]] size_t maxWondercards(void) const override { return 1; }

        [[nodiscard]] int currentGiftAmount(void) const override { return 0; }

        void dex(const PKX& pk) override;
        [[nodiscard]] int dexSeen(void) const override;
        [[nodiscard]] int dexCaught(void) const override;

        void mysteryGift(const WCX& wc, int& pos) override;

        [[nodiscard]] std::unique_ptr<WCX> mysteryGift(int) const override { return nullptr; }
    };
}

#endif
