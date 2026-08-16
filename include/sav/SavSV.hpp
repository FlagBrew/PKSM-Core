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

#ifndef SAVSV_HPP
#define SAVSV_HPP

#include "sav/Sav8.hpp"

namespace pksm
{
    class SavSV : public Sav8
    {
    private:
        static constexpr u32 KBox       = 0x0D66012C;
        static constexpr u32 KParty     = 0x3AA1A9AD;
        static constexpr u32 KStatus    = 0xE3E89BD1;
        static constexpr u32 KItems     = 0x21C9BD44;
        static constexpr u32 KBoxLayout = 0x19722C89;

        static constexpr u32 KCurrentBox    = 0x017C3CBB;
        static constexpr u32 KBoxesUnlocked = 0x71825204;
        static constexpr u32 KBoxWallpapers = 0x2EB1B190;
        static constexpr u32 KMoney         = 0x4F35D0DD;
        static constexpr u32 KPlayTime      = 0xEDAFF794;
        static constexpr u32 KLeaguePoints  = 0xADB4FE17;
        static constexpr u32 KZukan         = 0x0DEAAEBD;
        static constexpr u32 KZukanT1       = 0xF5D7C0E2;

        // Gym badge event flags
        static constexpr u32 KBadgeElectric = 0x8205ECAD;
        static constexpr u32 KBadgePsychic  = 0x3B819021;
        static constexpr u32 KBadgeGhost    = 0xCDA61DED;
        static constexpr u32 KBadgeIce      = 0x46B6CB30;
        static constexpr u32 KBadgeGrass    = 0xB4C3AFE6;
        static constexpr u32 KBadgeWater    = 0xA803FAAD;
        static constexpr u32 KBadgeBug      = 0x89306FE6;
        static constexpr u32 KBadgeNormal   = 0xF90EFD79;

    public:
        // Base game sizes
        static constexpr size_t SIZE_G9SV_0   = 0x31626F; // 1.0.0 fresh
        static constexpr size_t SIZE_G9SV_0a  = 0x31627C; // 1.0.0 after multiplayer
        static constexpr size_t SIZE_G9SV_1   = 0x319DB3; // 1.0.1 fresh
        static constexpr size_t SIZE_G9SV_1a  = 0x319DC0; // 1.0.1 after multiplayer
        static constexpr size_t SIZE_G9SV_1B  = 0x319DD0; // 1.0.1 -> 1.1.0
        static constexpr size_t SIZE_G9SV_3   = 0x319DC3; // 1.1.0 fresh
        static constexpr size_t SIZE_G9SV_1A  = 0x31A2C0; // 1.0.0 -> 1.0.1
        static constexpr size_t SIZE_G9SV_1Aa = 0x31A2CD; // 1.0.0 -> 1.0.1 -> 1.0.1 after MP
        static constexpr size_t SIZE_G9SV_1Ab =
            0x31A2DD;                                   // 1.0.0 -> 1.0.1 -> 1.0.1 after MP -> 1.1.0
        static constexpr size_t SIZE_G9SV_2 = 0x31A2D0; // 1.0.0 -> 1.1.0
        // 1.2.0 sizes
        static constexpr size_t SIZE_G9SV_3A1 = 0x31CA6F; // 1.0.1 -> 1.1.0 -> 1.2.0
        static constexpr size_t SIZE_G9SV_3B1 = SIZE_G9SV_3A1 - 0xD;
        static constexpr size_t SIZE_G9SV_3P1 = SIZE_G9SV_3B1 + 0x5;
        static constexpr size_t SIZE_G9SV_3G1 = SIZE_G9SV_3A1 + 0x5;
        static constexpr size_t SIZE_G9SV_3A0 = 0x31CF7C; // 1.0.0 -> 1.0.1 -> 1.1.0 -> 1.2.0
        static constexpr size_t SIZE_G9SV_3B0 = SIZE_G9SV_3A0 - 0xD;
        static constexpr size_t SIZE_G9SV_3P0 = SIZE_G9SV_3B0 + 0x5;
        static constexpr size_t SIZE_G9SV_3G0 = SIZE_G9SV_3A0 + 0x5;
        // DLC sizes (ranges due to variable block configurations)
        static constexpr size_t SIZE_G9SV_DLC1_MIN     = 0x4329A0; // Teal Mask min
        static constexpr size_t SIZE_G9SV_DLC1_END     = 0x432A2D; // Teal Mask max + tolerance
        static constexpr size_t SIZE_G9SV_DLC2_MIN     = 0x432EB6; // Indigo Disk min
        static constexpr size_t SIZE_G9SV_DLC2_END     = 0x432F3A; // Indigo Disk max + tolerance
        static constexpr size_t SIZE_G9SV_DLC1_202_MIN = 0x43362E;
        static constexpr size_t SIZE_G9SV_DLC1_202_END = 0x4336BB;
        static constexpr size_t SIZE_G9SV_DLC2_202_MIN = 0x433B44;
        static constexpr size_t SIZE_G9SV_DLC2_202_END = 0x433BC8;
        static constexpr size_t SIZE_G9SV_DLC1_300_MIN = 0x43AD4D;
        static constexpr size_t SIZE_G9SV_DLC1_300_END = 0x43ADDA;
        static constexpr size_t SIZE_G9SV_DLC2_300_MIN = 0x43B263;
        static constexpr size_t SIZE_G9SV_DLC2_300_END = 0x43B2E7;

        SavSV(const std::shared_ptr<u8[]>& dt, size_t length);

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
        [[nodiscard]] u8 badges(void) const override;
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
