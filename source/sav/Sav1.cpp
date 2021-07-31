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

#include "sav/Sav1.hpp"
#include "pkx/PK1.hpp"
#include "pkx/PKX.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/i18n.hpp"
#include "utils/utils.hpp"
#include "gui.hpp"
#include "wcx/WCX.hpp"
#include <algorithm>
#include <limits>
#include <stdlib.h>

namespace pksm
{
    Sav::Game Sav1::getVersion(std::shared_ptr<u8[]> dt)
    {
        // for now it doesn't matter, the only difference is Pikachu's friendship and Pikachu surf
        // score
        return Game::RGB;
    }
    // ctrl-v the fixparty code, boxes are expected to be continuous

    void Sav1::fixBoxes()
    {
        // Poor man's bubble sort-like thing
        for (int i = 0; i < maxBoxes(); i++)
        {
            int numPkm = maxPkmInBox;
            for (int j = maxPkmInBox - 1; j > 0; j--)
            {
                auto checkPKM = pkm(i, j);
                if (checkPKM->species() == Species::None)
                {
                    numPkm--;
                    continue;
                }
                auto prevPKM = pkm(i, j - 1);
                if (checkPKM->species() != Species::None && prevPKM->species() == Species::None)
                {
                    pkm(*checkPKM, i, j - 1, false);
                    pkm(*prevPKM, i, j, false);
                    numPkm = maxPkmInBox;
                    j      = maxPkmInBox; // reset loop
                }
            }
            boxCount(i, numPkm);
        }
    }

    void Sav1::initialize()
    {
        // these are unused padding bytes for box data (current and 0 respectively). the difference
        // in international is location of boxes
        // japanese    = (data[0x2EF4] == 0xFF) && (data[0x304C] == 0xFF);
        japanese    = false;
        maxPkmInBox = japanese ? 30 : 20;
        OFS_PARTY   = japanese ? 0x2ED5 : 0x2F2C;
    }
    u8 Sav1::calculateChecksum(const u8* data, size_t len)
    {
        u8 state     = 255;
        size_t index = 0;
        while (index < len)
        {
            state -= data[index];
            index++;
        }
        return state;
    }
    void Sav1::finishEditing()
    {
        // TODO: clean up this ternary crap
        if (playedHours() == 255 && playedMinutes() == 59 && playedSeconds() == 59)
        {
            data[japanese ? 0x2CA4 : 0x2CF1] = 59;  // frame count
            data[japanese ? 0x2CA1 : 0x2CEE] = 255; // maxed out timer
        }
        else
        {
            data[japanese ? 0x2CA1 : 0x2CEE] = 0;
        }
        // fixBoxes();
        for (int box = 0; box < maxBoxes(); box++)
        {
            if (box < (maxBoxes()/2))
            {
                data[(japanese ? 0x54A9 : 0x5A4D) + box] =
                    calculateChecksum(&data[boxDataStart(box)], boxSize());
            }
            else
            {
                data[(japanese ? 0x74A9 : 0x7A4D) + box] =
                    calculateChecksum(&data[boxDataStart(box)], boxSize());
            }
        }
        std::copy(&data[boxStart(currentBox())], &data[boxStart(currentBox())] + boxSize(),
            &data[japanese ? 0x302D : 0x30C0]);
        data[japanese ? 0x3594 : 0x3523] =
            calculateChecksum(&data[0x2598], japanese ? 0xFFC : 0xF8B);
        data[japanese ? 0x54A8 : 0x5A4C] =
            calculateChecksum(&data[0x4000], japanese ? 0x14A8 : 0x1A4C);
        data[japanese ? 0x74A8 : 0x7A4C] =
            calculateChecksum(&data[0x6000], japanese ? 0x14A8 : 0x1A4C);
    }
    u16 Sav1::TID() const { return BigEndian::convertTo<u16>(&data[japanese ? 0x25FB : 0x2605]); }
    void Sav1::TID(u16 v) { BigEndian::convertFrom<u16>(&data[japanese ? 0x25FB : 0x2605], v); }
    Language Sav1::language() const
    {
        return japanese ? Language::JPN : Language::ENG;
        // [French or German] can also be detected by umlaut characters, which aren't in the other
        // keyboards. And obviously when finding umlauts German would be the first guess. meh later
    }
    void Sav1::language(Language v) {}
    std::string Sav1::otName() const
    {
        return StringUtils::getString1(data.get(), 0x2598, japanese ? 6 : 11, japanese);
    }
    void Sav1::otName(const std::string_view& v)
    {
        StringUtils::setString1(
            data.get(), v, 0x2598, japanese ? 6 : 11, japanese, japanese ? 6 : 11, 0);
    }

    // why did they use BCD
    u32 Sav1::money() const
    {
        u32 bcdValue = (BigEndian::convertTo<u16>(&data[japanese ? 0x25EE : 0x25F3]) << 8) |
                       data[japanese ? 0x25F0 : 0x25F5];
        return (((bcdValue & 0xF00000) >> 20) * 100000) + (((bcdValue & 0x0F0000) >> 16) * 10000) +
               (((bcdValue & 0x00F000) >> 12) * 1000) + (((bcdValue & 0x000F00) >> 8) * 100) +
               (((bcdValue & 0x0000F0) >> 4) * 10) + (bcdValue & 0x00000F);
    }
    void Sav1::money(u32 v)
    {
        u32 result = 0;
        v %= 1000000;
        result |= (v / 100000) << 20;
        result |= ((v / 10000) % 10) << 16;
        result |= ((v / 1000) % 10) << 12;
        result |= ((v / 100) % 10) << 8;
        result |= ((v / 10) % 10) << 4;
        result |= v % 10;
        BigEndian::convertFrom<u16>(
            &data[japanese ? 0x25EE : 0x25F3], u16((result & 0xFFFF00) >> 8));
        data[japanese ? 0x25F0 : 0x25F5] = u8(result & 0x0000FF);
    }
    u8 Sav1::badges() const
    {
        u8 sum        = 0;
        u8 badgeFlags = data[japanese ? 0x25F8 : 0x2602];
        for (int i = 0; i < 8; i++)
        {
            sum += (badgeFlags & (1 << i)) >> i;
        }
        return sum;
    }
    u16 Sav1::playedHours() const { return u16(data[japanese ? 0x2CA0 : 0x2CED]); }
    void Sav1::playedHours(u16 v) { data[japanese ? 0x2CA0 : 0x2CED] = u8(v); }
    u8 Sav1::playedMinutes() const { return data[japanese ? 0x2CA2 : 0x2CEF]; }
    void Sav1::playedMinutes(u8 v) { data[japanese ? 0x2CA2 : 0x2CEF] = v; }
    u8 Sav1::playedSeconds() const { return data[japanese ? 0x2CA3 : 0x2CF0]; }
    void Sav1::playedSeconds(u8 v) { data[japanese ? 0x2CA3 : 0x2CF0] = v; }

    u8 Sav1::currentBox() const { return data[japanese ? 0x2842 : 0x284C] & 0x7F; }
    void Sav1::currentBox(u8 v)
    {
        data[japanese ? 0x2842 : 0x284C] = 0x80 | (v & 0x7F);
    }
    u32 Sav1::boxOffset(u8 box, u8 slot) const
    {
        Gui::warn(std::to_string(boxDataStart(box) + (slot * PK1::BOX_LENGTH)));
        return boxDataStart(box) + (slot * PK1::BOX_LENGTH);
    }
    u32 Sav1::partyOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (slot * PK1::PARTY_LENGTH);
    }

    u32 Sav1::boxStart(u8 box) const
    {
        if (box < maxBoxes() / 2)
        {
            return 0x4000 + (box * boxSize());
        }
        box -= maxBoxes() / 2;
        return 0x6000 + (box * boxSize());
    }
    u32 Sav1::boxDataStart(u8 box) const { return boxStart(box) + (japanese ? 32 : 22); }

    // these will need modified to have the name information included, since names are not
    // contiguous with other data and PK1 includes names. good enough to boot with placeholders
    std::unique_ptr<PKX> Sav1::pkm(u8 slot) const
    {
        return PKX::getPKM<Generation::ONE>(&data[partyOffset(slot)], true);
    }
    std::unique_ptr<PKX> Sav1::pkm(u8 box, u8 slot) const
    {
        if (slot >= maxPkmInBox)
        {
            return PKX::getPKM<Generation::ONE>(nullptr);
        }
        return PKX::getPKM<Generation::ONE>(&data[boxOffset(box, slot)]);
    }
    void Sav1::pkm(const PKX& pk, u8 slot)
    {
        // this and the other one will need Gen II handling
        if (pk.generation() == Generation::ONE)
        {
            auto pk1 = pk.partyClone();
            std::copy(pk1->rawData(), pk1->rawData() + PK1::PARTY_LENGTH, &data[partyOffset(slot)]);
        }
    }
    void Sav1::pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade)
    {
        if (slot >= maxPkmInBox)
        {
            return;
        }
        if (pk.generation() == Generation::ONE)
        {
            auto pk1 = pk.clone();
            if (applyTrade)
            {
                trade(*pk1);
            }
            std::copy(
                pk1->rawData(), pk1->rawData() + PK1::BOX_LENGTH, &data[boxOffset(box, slot)]);
        }
    }

    void Sav1::trade(PKX& pk, const Date& date) const
    {
        // TODO
    }
    std::unique_ptr<PKX> Sav1::emptyPkm() const { return PKX::getPKM<Generation::ONE>(nullptr); }

    void Sav1::dex(const PKX& pk)
    {
        if (!(availableSpecies().count(pk.species()) > 0) || pk.egg())
            return;
        setCaught(pk.species(), true);
        setSeen(pk.species(), true);
    }
    bool Sav1::getCaught(Species species) const
    {
        int byteOffset = (japanese ? 0x259E : 0x25A3) + (u8(species) / 8);
        u8 bitIndex    = 7 - (u8(species) % 8);
        return FlagUtil::getFlag(data.get(), byteOffset, bitIndex);
    }
    void Sav1::setCaught(Species species, bool caught)
    {
        int byteOffset = (japanese ? 0x259E : 0x25A3) + (u8(species) / 8);
        u8 bitIndex    = 7 - (u8(species) % 8);
        FlagUtil::setFlag(data.get(), byteOffset, bitIndex, caught);
    }
    bool Sav1::getSeen(Species species) const
    {
        int byteOffset = (japanese ? 0x25B1 : 0x25B6) + (u8(species) / 8);
        u8 bitIndex    = 7 - (u8(species) % 8);
        return FlagUtil::getFlag(data.get(), byteOffset, bitIndex);
    }
    void Sav1::setSeen(Species species, bool seen)
    {
        int byteOffset = (japanese ? 0x25B1 : 0x25B6) + (u8(species) / 8);
        u8 bitIndex    = 7 - (u8(species) % 8);
        FlagUtil::setFlag(data.get(), byteOffset, bitIndex, seen);
    }
    int Sav1::dexSeen() const
    {
        return std::count_if(availableSpecies().begin(), availableSpecies().end(),
            [this](const auto& spec) { return getSeen(spec); });
    }
    int Sav1::dexCaught() const
    {
        return std::count_if(availableSpecies().begin(), availableSpecies().end(),
            [this](const auto& spec) { return getCaught(spec); });
    }
    u8 Sav1::partyCount() const { return data[japanese ? 0x2ED5 : 0x2F2C]; }
    void Sav1::partyCount(u8 count) { data[japanese ? 0x2ED5 : 0x2F2C] = count; }
    u8 Sav1::boxCount(u8 box) const
    {
        return data[boxStart(box)];
    }
    void Sav1::boxCount(u8 box, u8 count)
    {
        data[boxStart(box)] = count;
    }

    int Sav1::maxBoxes() const { return japanese ? 8 : 12; }
    int Sav1::boxSize() const { return japanese ? 0x52A : 0x462; }

    void Sav1::item(const Item& tItem, Pouch pouch, u16 slot)
    {
        if (slot < (pouch == Pouch::NormalItem ? 20 : 50))
        {
            Item1 item = static_cast<Item1>(tItem);
            auto write = item.bytes();
            switch (pouch)
            {
                case Pouch::NormalItem:
                    std::copy(write.begin(), write.end(),
                        &data[(japanese ? 0x25C4 : 0x25C9) + 1 + (slot * 2)]);
                    break;
                case Pouch::PCItem:
                    std::copy(write.begin(), write.end(),
                        &data[(japanese ? 0x27DC : 0x27E6) + 1 + (slot * 2)]);
                    break;
                default:
                    return;
            }
        }
    }
    std::unique_ptr<Item> Sav1::item(Pouch pouch, u16 slot) const
    {
        switch (pouch)
        {
            case Pouch::NormalItem:
                return std::make_unique<Item1>(&data[(japanese ? 0x25C4 : 0x25C9) + (slot * 2)]);
            case Pouch::PCItem:
                return std::make_unique<Item1>(&data[(japanese ? 0x27DC : 0x27E6) + (slot * 2)]);
            default:
                return nullptr;
        }
    }
    std::vector<std::pair<Sav::Pouch, int>> Sav1::pouches() const
    {
        return {{Pouch::NormalItem, 20}, {Pouch::PCItem, 50}};
    }
    std::map<Sav::Pouch, std::vector<int>> Sav1::validItems1() const
    {
        std::map<Sav::Pouch, std::vector<int>> items = {
            {Pouch::NormalItem,
                {0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 29, 30, 31, 32,
                    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 51, 52, 53, 54,
                    55, 56, 57, 58, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
                    76, 77, 78, 79, 80, 81, 82, 83, 196, 197, 198, 199, 200, 201, 202, 203, 204,
                    205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
                    221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
                    237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250}},
            {Pouch::PCItem, {}}};
        items[Pouch::PCItem].insert(items[Pouch::PCItem].end(), items[Pouch::NormalItem].begin(),
            items[Pouch::NormalItem].end());
        return items;
    }
    std::map<Sav::Pouch, std::vector<int>> Sav1::validItems() const
    {
        std::map<Sav::Pouch, std::vector<int>> items = {
            {Pouch::NormalItem,
                {0, 1, 2, 3, 4, 442, 450, 81, 18, 19, 20, 21, 22, 23, 24, 25, 26, 17, 78, 79, 103,
                    82, 83, 84, 45, 46, 47, 48, 49, 50, 102, 101, 872, 128, 60, 85, 876, 92, 63, 27,
                    28, 29, 55, 76, 77, 56, 30, 31, 32, 873, 877, 57, 58, 59, 61, 444, 875, 471,
                    874, 651, 878, 216, 445, 446, 447, 51, 38, 39, 40, 41, 420, 421, 422, 423, 424,
                    328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343,
                    344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
                    360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375,
                    376, 377}},
            {Pouch::PCItem, {}}};
        items[Pouch::PCItem].insert(items[Pouch::PCItem].end(), items[Pouch::NormalItem].begin(),
            items[Pouch::NormalItem].end());
        return items;
    }
}
