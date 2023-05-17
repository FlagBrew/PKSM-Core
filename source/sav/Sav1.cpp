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

#include "sav/Sav1.hpp"
#include "pkx/PK1.hpp"
#include "pkx/PKX.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/i18n.hpp"
#include "utils/utils.hpp"
#include "wcx/WCX.hpp"
#include <algorithm>
#include <limits>
#include <stdlib.h>

namespace pksm
{
    Sav1::Sav1(const std::shared_ptr<u8[]>& data, u32 length) : Sav(data, length)
    {
        // checks if two boxes are valid
        japanese = ((data[0x2ED5] <= 30) && (data[0x2ED5 + 1 + data[0x2ED5]] == 0xFF)) &&
                   ((data[0x302D] <= 30) && (data[0x302D + 1 + data[0x302D]] == 0xFF));

        lang = japanese ? Language::JPN : StringUtils::guessLanguage12(otName());

        maxPkmInBox    = japanese ? 30 : 20;
        boxSize        = japanese ? 0x566 : 0x462;
        mainDataLength = japanese ? 0xFFC : 0xF8B;
        bankBoxesSize  = boxSize * (japanese ? 4 : 6);

        OFS_DEX_CAUGHT        = japanese ? 0x259E : 0x25A3;
        OFS_DEX_SEEN          = japanese ? 0x25B1 : 0x25B6;
        OFS_BAG               = japanese ? 0x25C4 : 0x25C9;
        OFS_MONEY             = japanese ? 0x25EE : 0x25F3;
        OFS_BADGES            = japanese ? 0x25F8 : 0x2602;
        OFS_TID               = japanese ? 0x25FB : 0x2605;
        OFS_PC_ITEMS          = japanese ? 0x27DC : 0x27E6;
        OFS_CURRENT_BOX_INDEX = japanese ? 0x2842 : 0x284C;
        OFS_HOURS             = japanese ? 0x2CA0 : 0x2CED;
        OFS_PARTY             = japanese ? 0x2ED5 : 0x2F2C;
        OFS_CURRENT_BOX       = japanese ? 0x302D : 0x30C0;
        OFS_MAIN_DATA_SUM     = japanese ? 0x3594 : 0x3523;

        OFS_BANK2_BOX_SUMS = 0x4000 + bankBoxesSize;
        OFS_BANK3_BOX_SUMS = 0x6000 + bankBoxesSize;

        originalCurrentBox = currentBox();
    }

    Sav::Game Sav1::getVersion(const std::shared_ptr<u8[]>& dt)
    {
        // for now it doesn't matter, the only difference is Pikachu's friendship and Pikachu surf
        // score
        return Game::RGB;
    }

    // max length of string + terminator
    u8 Sav1::nameLength() const
    {
        return japanese ? 6 : 11;
    }

    u8 Sav1::PK1Length() const
    {
        return japanese ? PK1::JP_LENGTH_WITH_NAMES : PK1::INT_LENGTH_WITH_NAMES;
    }

    // ctrl-v the fixparty code, boxes are expected to be contiguous
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
            fixBox(i);
        }
    }

    void Sav1::finishEditing()
    {
        if (playedHours() == 255 && playedMinutes() == 59 && playedSeconds() == 59)
        {
            data[OFS_HOURS + 4] = 59;  // frame count
            data[OFS_HOURS + 1] = 255; // maxed out timer
        }
        else
        {
            data[OFS_HOURS + 1] = 0;
        }
        fixBoxes();
        fixParty();
        fixItemLists();
        std::copy(&data[OFS_CURRENT_BOX], &data[OFS_CURRENT_BOX] + boxSize,
            &data[boxStart(originalCurrentBox, false)]);
        if (currentBox() != originalCurrentBox)
        {
            std::copy(&data[boxStart(currentBox())], &data[boxStart(currentBox())] + boxSize,
                &data[OFS_CURRENT_BOX]);
        }
        for (int box = 0; box < maxBoxes(); box++)
        {
            if (box < (maxBoxes() / 2))
            {
                data[OFS_BANK2_BOX_SUMS + 1 + box] =
                    crypto::diff8({&data[boxDataStart(box, false)], boxSize});
            }
            else
            {
                data[OFS_BANK3_BOX_SUMS + 1 + (box - (maxBoxes() / 2))] =
                    crypto::diff8({&data[boxDataStart(box, false)], boxSize});
            }
        }
        data[OFS_MAIN_DATA_SUM]  = crypto::diff8({&data[0x2598], mainDataLength});
        data[OFS_BANK2_BOX_SUMS] = crypto::diff8({&data[0x4000], bankBoxesSize});
        data[OFS_BANK3_BOX_SUMS] = crypto::diff8({&data[0x6000], bankBoxesSize});

        originalCurrentBox = currentBox();
    }

    u16 Sav1::TID() const
    {
        return BigEndian::convertTo<u16>(&data[OFS_TID]);
    }

    void Sav1::TID(u16 v)
    {
        BigEndian::convertFrom<u16>(&data[OFS_TID], v);
    }

    Language Sav1::language() const
    {
        return lang;
    }

    void Sav1::language(Language v)
    {
        if (lang != Language::JPN && v != Language::JPN)
        {
            lang = v;
        }
    }

    // INT has space for 10 characters + terminator, but the in-game keyboard only permits 7
    std::string Sav1::otName() const
    {
        return StringUtils::getString1(data.get(), 0x2598, japanese ? 6 : 8, lang);
    }

    void Sav1::otName(const std::string_view& v)
    {
        StringUtils::setString1(data.get(), v, 0x2598, japanese ? 6 : 8, lang);
    }

    // why did they use BCD
    u32 Sav1::money() const
    {
        return BigEndian::BCDtoUInteger<u32, 3>(&data[OFS_MONEY]);
    }

    void Sav1::money(u32 v)
    {
        BigEndian::UIntegerToBCD<u32, 3>(&data[OFS_MONEY], v);
    }

    u8 Sav1::badges() const
    {
        u8 sum        = 0;
        u8 badgeFlags = data[OFS_BADGES];
        for (int i = 0; i < 8; i++)
        {
            sum += (badgeFlags & (1 << i)) >> i;
        }
        return sum;
    }

    u16 Sav1::playedHours() const
    {
        return u16(data[OFS_HOURS]);
    }

    void Sav1::playedHours(u16 v)
    {
        data[OFS_HOURS] = u8(v);
    }

    u8 Sav1::playedMinutes() const
    {
        return data[OFS_HOURS + 2];
    }

    void Sav1::playedMinutes(u8 v)
    {
        data[OFS_HOURS + 2] = v;
    }

    u8 Sav1::playedSeconds() const
    {
        return data[OFS_HOURS + 3];
    }

    void Sav1::playedSeconds(u8 v)
    {
        data[OFS_HOURS + 3] = v;
    }

    u8 Sav1::currentBox() const
    {
        return data[OFS_CURRENT_BOX_INDEX] & 0x7F;
    }

    void Sav1::currentBox(u8 v)
    {
        data[OFS_CURRENT_BOX_INDEX] = (data[OFS_CURRENT_BOX_INDEX] & 0x80) | (v & 0x7F);
    }

    u32 Sav1::boxOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (slot * PK1::BOX_LENGTH);
    }

    u32 Sav1::boxOtNameOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (maxPkmInBox * PK1::BOX_LENGTH) + (slot * nameLength());
    }

    u32 Sav1::boxNicknameOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (maxPkmInBox * PK1::BOX_LENGTH) +
               ((maxPkmInBox + slot) * nameLength());
    }

    u32 Sav1::partyOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (slot * PK1::PARTY_LENGTH);
    }

    u32 Sav1::partyOtNameOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (6 * PK1::PARTY_LENGTH) + (slot * nameLength());
    }

    u32 Sav1::partyNicknameOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (6 * PK1::PARTY_LENGTH) + ((6 + slot) * nameLength());
    }

    u32 Sav1::boxStart(u8 box, bool obeyCurrentBoxMechanics) const
    {
        if (box == originalCurrentBox && obeyCurrentBoxMechanics)
        {
            return OFS_CURRENT_BOX;
        }
        if (box < maxBoxes() / 2)
        {
            return 0x4000 + (box * boxSize);
        }
        box -= maxBoxes() / 2;
        return 0x6000 + (box * boxSize);
    }

    u32 Sav1::boxDataStart(u8 box, bool obeyCurrentBoxMechanics) const
    {
        return boxStart(box, obeyCurrentBoxMechanics) + maxPkmInBox + 2;
    }

    // the PK1 and PK2 formats used by the community start with magic bytes, the second being
    // species
    std::unique_ptr<PKX> Sav1::pkm(u8 slot) const
    {
        if (slot >= partyCount())
        {
            return emptyPkm();
        }
        // using the larger of the two sizes to not dynamically allocate
        u8 buffer[PK1::INT_LENGTH_WITH_NAMES] = {0x01, data[partyOffset(slot)], 0xFF};

        std::copy(
            &data[partyOffset(slot)], &data[partyOffset(slot)] + PK1::PARTY_LENGTH, buffer + 3);
        std::copy(&data[partyOtNameOffset(slot)], &data[partyOtNameOffset(slot)] + nameLength(),
            buffer + 3 + PK1::PARTY_LENGTH);
        std::copy(&data[partyNicknameOffset(slot)], &data[partyNicknameOffset(slot)] + nameLength(),
            buffer + 3 + PK1::PARTY_LENGTH + nameLength());

        StringUtils::gbStringFailsafe(buffer, 3 + PK1::PARTY_LENGTH, nameLength());
        StringUtils::gbStringFailsafe(buffer, 3 + PK1::PARTY_LENGTH + nameLength(), nameLength());

        auto pk1 = PKX::getPKM<Generation::ONE>(buffer, PK1Length());
        if (language() != Language::JPN)
        {
            pk1->language(StringUtils::guessLanguage12(pk1->nickname()));
        }

        return pk1;
    }

    std::unique_ptr<PKX> Sav1::pkm(u8 box, u8 slot) const
    {
        if (slot >= maxPkmInBox || slot >= boxCount(box))
        {
            return emptyPkm();
        }

        u8 buffer[PK1::INT_LENGTH_WITH_NAMES] = {0x01, data[boxOffset(box, slot)], 0xFF};

        std::copy(
            &data[boxOffset(box, slot)], &data[boxOffset(box, slot)] + PK1::BOX_LENGTH, buffer + 3);
        std::copy(&data[boxOtNameOffset(box, slot)],
            &data[boxOtNameOffset(box, slot)] + nameLength(), buffer + 3 + PK1::PARTY_LENGTH);
        std::copy(&data[boxNicknameOffset(box, slot)],
            &data[boxNicknameOffset(box, slot)] + nameLength(),
            buffer + 3 + PK1::PARTY_LENGTH + nameLength());

        StringUtils::gbStringFailsafe(buffer, 3 + PK1::PARTY_LENGTH, nameLength());
        StringUtils::gbStringFailsafe(buffer, 3 + PK1::PARTY_LENGTH + nameLength(), nameLength());

        auto pk1 = PKX::getPKM<Generation::ONE>(buffer, PK1Length());
        pk1->updatePartyData();
        if (language() != Language::JPN)
        {
            pk1->language(StringUtils::guessLanguage12(pk1->nickname()));
        }

        return pk1;
    }

    void Sav1::pkm(const PKX& pk, u8 slot)
    {
        if (pk.generation() == Generation::ONE)
        {
            if (slot >= partyCount())
            {
                if (slot > partyCount())
                {
                    pkm(*emptyPkm(), slot - 1);
                }
                partyCount(slot + 1);
            }

            auto pk1 = pk.partyClone();

            std::ranges::copy(
                pk1->rawData().subspan(3, PK1::PARTY_LENGTH), &data[partyOffset(slot)]);

            if ((pk.language() == Language::JPN) != (language() == Language::JPN))
            {
                StringUtils::setString1(&data[partyNicknameOffset(slot)],
                    StringUtils::toUpper(pk1->species().localize(language())), 0, nameLength(),
                    language());

                // check if it's the trade ot byte
                if (pk1->rawData()[3 + PK1::PARTY_LENGTH] == 0x5D)
                {
                    data[partyOtNameOffset(slot)]     = 0x5D;
                    data[partyOtNameOffset(slot) + 1] = 0x50;
                }
                else
                {
                    StringUtils::setString1(&data[partyOtNameOffset(slot)],
                        StringUtils::getTradeOT(language()), 0, nameLength(), language());
                }
            }
            else
            {
                std::ranges::copy(pk1->rawData().subspan(3 + PK1::PARTY_LENGTH, nameLength()),
                    &data[partyOtNameOffset(slot)]);
                std::ranges::copy(
                    pk1->rawData().subspan(3 + PK1::PARTY_LENGTH + nameLength(), nameLength()),
                    &data[partyNicknameOffset(slot)]);
            }
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
            auto pk1 = pk.clone(); // note that partyClone and clone are equivalent
            if (applyTrade)
            {
                trade(*pk1);
            }
            static_cast<PK1*>(pk1.get())->boxLevel(pk1->level());

            if (slot >= boxCount(box))
            {
                if (slot > boxCount(box))
                {
                    pkm(*emptyPkm(), box, slot - 1, false);
                }
                boxCount(box, slot + 1);
            }

            std::ranges::copy(
                pk1->rawData().subspan(3, PK1::BOX_LENGTH), &data[boxOffset(box, slot)]);

            if ((pk.language() == Language::JPN) != (language() == Language::JPN))
            {
                StringUtils::setString1(&data[boxNicknameOffset(box, slot)],
                    StringUtils::toUpper(pk1->species().localize(language())), 0, nameLength(),
                    language());

                if (pk1->rawData()[3 + PK1::BOX_LENGTH] == 0x5D)
                {
                    data[boxOtNameOffset(box, slot)]     = 0x5D;
                    data[boxOtNameOffset(box, slot) + 1] = 0x50;
                }
                else
                {
                    StringUtils::setString1(&data[boxOtNameOffset(box, slot)],
                        StringUtils::toUpper(StringUtils::getTradeOT(language())), 0, nameLength(),
                        language());
                }
            }
            else
            {
                std::ranges::copy(pk1->rawData().subspan(3 + PK1::PARTY_LENGTH, nameLength()),
                    &data[boxOtNameOffset(box, slot)]);
                std::ranges::copy(
                    pk1->rawData().subspan(3 + PK1::PARTY_LENGTH + nameLength(), nameLength()),
                    &data[boxNicknameOffset(box, slot)]);
            }
        }
    }

    std::unique_ptr<PKX> Sav1::emptyPkm() const
    {
        return PKX::getPKM<Generation::ONE>(nullptr, PK1Length());
    }

    void Sav1::dex(const PKX& pk)
    {
        if (!(availableSpecies().count(pk.species()) > 0))
        {
            return;
        }
        setCaught(pk.species(), true);
        setSeen(pk.species(), true);
    }

    bool Sav1::getCaught(Species species) const
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        return FlagUtil::getFlag(data.get() + OFS_DEX_CAUGHT, ofs, flag & 7);
    }

    void Sav1::setCaught(Species species, bool caught)
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        FlagUtil::setFlag(data.get() + OFS_DEX_CAUGHT, ofs, flag & 7, caught);
    }

    bool Sav1::getSeen(Species species) const
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        return FlagUtil::getFlag(data.get() + OFS_DEX_SEEN, ofs, flag & 7);
    }

    void Sav1::setSeen(Species species, bool seen)
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        FlagUtil::setFlag(data.get() + OFS_DEX_SEEN, ofs, flag & 7, seen);
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

    u8 Sav1::partyCount() const
    {
        return data[OFS_PARTY];
    }

    void Sav1::partyCount(u8 count)
    {
        data[OFS_PARTY] = count;
    }

    u8 Sav1::boxCount(u8 box) const
    {
        return data[boxStart(box)];
    }

    void Sav1::boxCount(u8 box, u8 count)
    {
        data[boxStart(box)] = count;
    }

    void Sav1::fixBox(u8 box)
    {
        u8 count = 0;
        while (pkm(box, count)->species() != Species::None)
        {
            // sets the species1 in a list that the game uses for speed
            data[boxStart(box) + 1 + count] = data[boxOffset(box, count)];
            count++;
        }
        data[boxStart(box) + 1 + count] = 0xFF;
        data[boxStart(box)]             = count;
    }

    void Sav1::fixParty()
    {
        Sav::fixParty();
        u8 count = 0;
        while (pkm(count)->species() != Species::None && count < 6)
        {
            data[OFS_PARTY + 1 + count] = data[partyOffset(count)];
            count++;
        }
        data[OFS_PARTY + 1 + count] = 0xFF;
        data[OFS_PARTY]             = count;
    }

    int Sav1::maxSlot() const
    {
        return maxBoxes() * maxPkmInBox;
    }

    int Sav1::maxBoxes() const
    {
        return japanese ? 8 : 12;
    }

    void Sav1::item(const Item& tItem, Pouch pouch, u16 slot)
    {
        if (slot < (pouch == Pouch::NormalItem ? 20 : 50))
        {
            Item1 item = static_cast<Item1>(tItem);
            auto write = item.bytes();
            switch (pouch)
            {
                case Pouch::NormalItem:
                    std::copy(write.begin(), write.end(), &data[OFS_BAG + 1 + (slot * 2)]);
                    break;
                case Pouch::PCItem:
                    std::copy(write.begin(), write.end(), &data[OFS_PC_ITEMS + 1 + (slot * 2)]);
                    break;
                default:
                    return;
            }
        }
    }

    std::unique_ptr<Item> Sav1::item(Pouch pouch, u16 slot) const
    {
        std::unique_ptr<Item1> returnVal;
        switch (pouch)
        {
            case Pouch::NormalItem:
                returnVal = std::make_unique<Item1>(&data[OFS_BAG + 1 + (slot * 2)]);
                break;
            case Pouch::PCItem:
                returnVal = std::make_unique<Item1>(&data[OFS_PC_ITEMS + 1 + (slot * 2)]);
                break;
            default:
                return nullptr;
        }
        // 0xFF is a list terminator. In a normal game state it will be in an ID slot.
        if (returnVal->id1() == 0xFF)
        {
            return std::make_unique<Item1>(nullptr);
        }
        return returnVal;
    }

    SmallVector<std::pair<Sav::Pouch, int>, 15> Sav1::pouches() const
    {
        return {
            std::pair{Pouch::NormalItem, 20},
            std::pair{Pouch::PCItem,     50}
        };
    }

    SmallVector<std::pair<pksm::Sav::Pouch, std::span<const int>>, 15> Sav1::validItems1() const
    {
        static constexpr std::array validItems = {1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 15, 16, 17,
            18, 19, 20, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48,
            49, 51, 52, 53, 54, 55, 56, 57, 58, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
            73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 196, 197, 198, 199, 200, 201, 202, 203, 204,
            205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221,
            222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
            239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250};

        return {
            std::pair{Pouch::NormalItem, std::span{validItems}},
            std::pair{Pouch::PCItem,     std::span{validItems}}
        };
    }

    SmallVector<std::pair<pksm::Sav::Pouch, std::span<const int>>, 15> Sav1::validItems() const
    {
        static constexpr std::array validItems = {1, 2, 3, 4, 442, 450, 81, 18, 19, 20, 21, 22, 23,
            24, 25, 26, 17, 78, 79, 103, 82, 83, 84, 45, 46, 47, 48, 49, 50, 102, 101, 872, 60, 85,
            876, 92, 63, 27, 28, 29, 55, 76, 77, 56, 30, 31, 32, 873, 877, 57, 58, 59, 61, 444, 875,
            471, 874, 651, 878, 216, 445, 446, 447, 51, 38, 39, 40, 41, 420, 421, 422, 423, 424,
            328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344,
            345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361,
            362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377};

        return {
            std::pair{Pouch::NormalItem, std::span{validItems}},
            std::pair{Pouch::PCItem,     std::span{validItems}}
        };
    }

    void Sav1::fixItemLists()
    {
        u8 count = 0;
        while (count < 20 && item(Pouch::NormalItem, count)->id() != 0)
        {
            count++;
        }
        data[OFS_BAG]                   = count;
        data[OFS_BAG + 1 + (count * 2)] = 0xFF;

        count = 0;
        while (count < 50 && item(Pouch::PCItem, count)->id() != 0)
        {
            count++;
        }
        data[OFS_PC_ITEMS]                   = count;
        data[OFS_PC_ITEMS + 1 + (count * 2)] = 0xFF;
    }
}
