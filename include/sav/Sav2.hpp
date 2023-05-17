/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2022 Bernardo Giordano, Admiral Fish, piepie62, Pk11
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

#ifndef SAV2_HPP
#define SAV2_HPP

#include "personal/personal.hpp"
#include "sav/Sav.hpp"
#include <array>

namespace pksm
{
    class Sav2 : public Sav
    {
    protected:
        bool japanese;
        bool korean;
        Language lang;
        GameVersion versionOfGame;
        u8 maxPkmInBox, originalCurrentBox;
        u32 boxSize, OFS_PARTY, OFS_PC_ITEMS, OFS_CURRENT_BOX, OFS_CURRENT_BOX_INDEX, OFS_GENDER,
            OFS_BADGES, OFS_POKEDEX_CAUGHT, OFS_POKEDEX_SEEN, OFS_BOX_NAMES, OFS_TM_POUCH,
            OFS_ITEMS, OFS_KEY_ITEMS, OFS_BALLS, OFS_CHECKSUM_ONE, OFS_MONEY, OFS_CHECKSUM_TWO,
            OFS_CHECKSUM_END, OFS_TIME_PLAYED, OFS_PALETTE, OFS_TID, OFS_NAME;

        void fixBoxes(void);

        [[nodiscard]] bool getCaught(Species species) const;
        void setCaught(Species species, bool caught);
        [[nodiscard]] bool getSeen(Species species) const;
        void setSeen(Species species, bool seen);

        [[nodiscard]] static u16 calculateChecksum(u8* start, const u8* end);

        std::span<const int> tmItems2() const;

    public:
        Sav2(const std::shared_ptr<u8[]>& data, u32 length,
            std::tuple<GameVersion, Language, bool> versionAndLanguage);

        [[nodiscard]] static std::tuple<GameVersion, Language, bool> getVersion(
            const std::shared_ptr<u8[]>& dt);
        [[nodiscard]] static bool validList(const std::shared_ptr<u8[]>& dt, size_t ofs, u8 slot);

        void finishEditing(void) override;

        void beginEditing(void) override {}

        [[nodiscard]] u16 TID(void) const override;
        void TID(u16 v) override;

        [[nodiscard]] u16 SID(void) const override { return 0; }

        void SID(u16 v) override {}

        [[nodiscard]] GameVersion version(void) const override;
        void version(GameVersion v) override;
        [[nodiscard]] Gender gender(void) const override;
        void gender(Gender v) override;

        [[nodiscard]] u8 subRegion(void) const override { return 0; }

        void subRegion(u8) override {}

        [[nodiscard]] u8 country(void) const override { return 0; }

        void country(u8) override {}

        [[nodiscard]] u8 consoleRegion(void) const override { return 0; }

        void consoleRegion(u8) override {}

        [[nodiscard]] Language language(void) const override;
        void language(Language v) override;
        [[nodiscard]] std::string otName(void) const override;
        void otName(const std::string_view& v) override;
        [[nodiscard]] u32 money(void) const override;
        void money(u32 v) override;

        [[nodiscard]] u32 BP(void) const override { return 0; }

        void BP(u32 v) override {}

        [[nodiscard]] u8 badges(void) const override;
        [[nodiscard]] u16 playedHours(void) const override;
        void playedHours(u16 v) override;
        [[nodiscard]] u8 playedMinutes(void) const override;
        void playedMinutes(u8 v) override;
        [[nodiscard]] u8 playedSeconds(void) const override;
        void playedSeconds(u8 v) override;

        [[nodiscard]] u8 currentBox(void) const override;
        void currentBox(u8 v) override;

        [[nodiscard]] u8 unlockedBoxes(void) const override { return maxBoxes(); }

        void unlockedBoxes(u8) override {}

        [[nodiscard]] u8 legendBoxUnlockSize(void) const override { return 0; }

        [[nodiscard]] u32 boxOffset(u8 box, u8 slot) const override;
        [[nodiscard]] u32 boxNicknameOffset(u8 box, u8 slot) const;
        [[nodiscard]] u32 boxOtNameOffset(u8 box, u8 slot) const;
        [[nodiscard]] u32 partyOffset(u8 slot) const override;
        [[nodiscard]] u32 partyNicknameOffset(u8 slot) const;
        [[nodiscard]] u32 partyOtNameOffset(u8 slot) const;
        [[nodiscard]] u32 boxStart(u8 box, bool obeyCurrentBoxMechanics = true) const;
        [[nodiscard]] u32 boxDataStart(u8 box, bool obeyCurrentBoxMechanics = true) const;
        [[nodiscard]] u8 nameLength(void) const;
        [[nodiscard]] u8 PK2Length(void) const;

        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 slot) const override;
        [[nodiscard]] std::unique_ptr<PKX> pkm(u8 box, u8 slot) const override;

        void pkm(const PKX& pk, u8 slot) override;
        void pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade) override;

        void trade(PKX& pk, const Date& date = Date::today()) const override {}

        [[nodiscard]] std::unique_ptr<PKX> emptyPkm(void) const override;

        void dex(const PKX& pk) override;
        [[nodiscard]] int dexSeen(void) const override;
        [[nodiscard]] int dexCaught(void) const override;

        [[nodiscard]] int currentGiftAmount(void) const override { return 0; }

        void mysteryGift(const WCX&, int&) override {}

        [[nodiscard]] std::unique_ptr<WCX> mysteryGift(int pos) const override { return nullptr; }

        void cryptBoxData(bool crypted) override {}

        [[nodiscard]] std::string boxName(u8 box) const override;
        void boxName(u8 box, const std::string_view& name) override;

        [[nodiscard]] u8 boxWallpaper(u8 box) const override { return 0; }

        void boxWallpaper(u8 box, u8 v) override {}

        [[nodiscard]] u8 partyCount(void) const override;
        [[nodiscard]] u8 boxCount(u8 box) const;
        void partyCount(u8 count) override;
        void boxCount(u8 box, u8 count);
        void fixBox(u8 box);
        void fixParty(void) override;

        [[nodiscard]] int maxSlot(void) const override;
        [[nodiscard]] int maxBoxes(void) const override;

        [[nodiscard]] size_t maxWondercards(void) const override { return 0; }

        [[nodiscard]] Generation generation(void) const override { return Generation::TWO; }

        void item(const Item& tItem, Pouch pouch, u16 slot) override;
        [[nodiscard]] std::unique_ptr<Item> item(Pouch pouch, u16 slot) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, int>, 15> pouches(void) const override;
        [[nodiscard]] SmallVector<std::pair<Pouch, std::span<const int>>, 15> validItems(
            void) const override;

        // Gen II Item IDs
        [[nodiscard]] SmallVector<std::pair<Pouch, std::span<const int>>, 15> validItems2(
            void) const;

        // apparently weird stuff happens if you ignore this
        [[nodiscard]] u8 pouchEntryCount(Pouch pouch) const;
        void pouchEntryCount(Pouch pouch, u8 v);

        void fixItemLists(void);
    };
}

#endif
