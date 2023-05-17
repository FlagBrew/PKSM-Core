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

#ifndef SAVSWSH_HPP
#define SAVSWSH_HPP

#include "sav/Sav8.hpp"

namespace pksm
{
    class SavSWSH : public Sav8
    {
    private:
        int ArmorDex, CrownDex;
        static constexpr std::array<u16, 25> gigaSpecies = {6, 12, 25, 52, 68, 94, 99, 131, 133,
            143, 569, 823, 826, 834, 839, 841, 842, 844, 851, 858, 861, 869, 879, 884, 890};

    public:
        static constexpr size_t SIZE_G8SWSH    = 0x1716B3; // 1.0
        static constexpr size_t SIZE_G8SWSH_1  = 0x17195E; // 1.0 -> 1.1
        static constexpr size_t SIZE_G8SWSH_2  = 0x180B19; // 1.0 -> 1.1 -> 1.2
        static constexpr size_t SIZE_G8SWSH_2B = 0x180AD0; // 1.0 -> 1.2
        static constexpr size_t SIZE_G8SWSH_3  = 0x1876B1; // 1.0 -> 1.1 -> 1.2 -> 1.3
        static constexpr size_t SIZE_G8SWSH_3A = 0x187693; // 1.0 -> 1.1 -> 1.3
        static constexpr size_t SIZE_G8SWSH_3B = 0x187668; // 1.0 -> 1.2 -> 1.3
        static constexpr size_t SIZE_G8SWSH_3C = 0x18764A; // 1.0 -> 1.3

        SavSWSH(const std::shared_ptr<u8[]>& dt, size_t length);

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
        [[nodiscard]] std::string jerseyNum(void) const;
        void jerseyNum(const std::string_view& v);
        [[nodiscard]] u32 money(void) const override;
        void money(u32 v) override;
        [[nodiscard]] u32 BP(void) const override;
        void BP(u32 v) override;
        [[nodiscard]] u8 badges(void) const override;
        [[nodiscard]] u16 playedHours(void) const override;
        void playedHours(u16 v) override;
        [[nodiscard]] u8 playedMinutes(void) const override;
        void playedMinutes(u8 v) override;
        [[nodiscard]] u8 playedSeconds(void) const override;
        void playedSeconds(u8 v) override;
        // TODO: last saved time

        void item(const Item& item, Pouch pouch, u16 slot) override;
        [[nodiscard]] std::unique_ptr<Item> item(Pouch pouch, u16 slot) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, int>, 15> pouches(void) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, std::span<const int>>, 15> validItems(
            void) const override;

        [[nodiscard]] u8 subRegion(void) const override { return 0; } // Not applicable

        void subRegion(u8) override {} // Not applicable

        [[nodiscard]] u8 country(void) const override { return 0; } // Not applicable

        void country(u8) override {} // Not applicable

        [[nodiscard]] u8 consoleRegion(void) const override { return 0; } // Not applicable

        void consoleRegion(u8) override {} // Not applicable

        [[nodiscard]] std::string boxName(u8 box) const override;
        void boxName(u8 box, const std::string_view& name) override;
        [[nodiscard]] u8 boxWallpaper(u8 box) const override;
        void boxWallpaper(u8 box, u8 v) override;

        [[nodiscard]] u8 currentBox(void) const override;
        void currentBox(u8) override;

        // TODO: CHECK WHERE THIS IS ACTUALLY STORED
        [[nodiscard]] u8 unlockedBoxes(void) const override { return maxBoxes(); }

        void unlockedBoxes(u8 v) override {}

        [[nodiscard]] u8 legendBoxUnlockSize(void) const override { return 0; }

        [[nodiscard]] u32 boxOffset(u8 box, u8 slot) const override;
        [[nodiscard]] u32 partyOffset(u8 slot) const override;
        [[nodiscard]] u8 partyCount(void) const override;
        void partyCount(u8 count) override;

        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 slot) const override;
        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 box, u8 slot) const override;

        // NOTICE: this sets a pkx into the savefile, not a ekx
        // that's because PKSM works with decrypted boxes and
        // crypts them back during resigning
        void pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade) override;
        void pkm(const PKX& pk, u8 slot) override;

        void cryptBoxData(bool crypted) override;

        void dex(const PKX& pk) override;
        [[nodiscard]] int dexSeen(void) const override;
        [[nodiscard]] int dexCaught(void) const override;

        // Check whether gifts are stored, or whether some disgusting record system is used
        void mysteryGift(const WCX& wc, int& pos) override;
        [[nodiscard]] std::unique_ptr<WCX> mysteryGift(int) const override;
    };
}

#endif
