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

#include "sav/SavPLA.hpp"
#include "pkx/PA8.hpp"
#include "sav/Item.hpp"
#include "utils/endian.hpp"
#include "utils/i18n.hpp"
#include "utils/utils.hpp"
#include "utils/VersionTables.hpp"
#include <algorithm>
#include <ranges>

namespace pksm
{
    constexpr int normalItems[] = {
        17,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        39,
        41,
        50,
        54,
        72,
        73,
        75,
        80,
        81,
        82,
        83,
        84,
        85,
        90,
        91,
        92,
        107,
        108,
        109,
        110,
        149,
        150,
        151,
        152,
        153,
        154,
        155,
        157,
        158,
        159,
        160,
        161,
        162,
        163,
        164,
        166,
        168,
        233,
        252,
        321,
        322,
        323,
        324,
        325,
        326,
        327,
        583,
        849,

        1125,
        1126,
        1127,
        1128,
        1231,
        1232,
        1233,
        1234,
        1235,
        1236,
        1237,
        1238,
        1239,
        1240,
        1241,
        1242,
        1243,
        1244,
        1245,
        1246,
        1247,
        1248,
        1249,
        1250,
        1251,

        1611,
        1613,
        1614,
        1615,
        1616,
        1617,
        1618,
        1619,
        1620,
        1621,
        1628,
        1630,
        1631,
        1632,
        1633,
        1634,
        1635,
        1636,
        1637,
        1638,
        1651,
        1679,
        1681,
        1682,
        1684,
        1686,
        1687,
        1688,
        1689,
        1690,
        1691,
        1692,
        1693,
        1694,
        1695,
        1696,
        1699,
        1700,
        1701,
        1702,
        1703,
        1704,
        1705,
        1706,
        1707,
        1708,
        1709,
        1710,
        1711,
        1712,
        1713,
        1716,
        1717,
        1720,
        1724,
        1725,
        1726,
        1727,
        1728,
        1732,
        1733,
        1734,
        1735,
        1736,
        1738,
        1739,
        1740,
        1741,
        1742,
        1746,
        1747,
        1748,
        1749,
        1750,
        1754,
        1755,
        1756,
        1757,
        1758,
        1759,
        1760,
        1761,
        1762,
        1764,
        1785,
    };

    constexpr int keyItems[] = {
        111,
        298,
        299,
        300,
        301,
        302,
        303,
        304,
        305,
        306,
        307,
        308,
        309,
        310,
        311,
        312,
        313,
        441,
        455,
        466,
        632,
        638,
        644,
        1608,
        1609,
        1610,
        1612,
        1622,
        1624,
        1625,
        1626,
        1627,
        1629,
        1639,
        1678,
        1721,
        1722,
        1723,
        1737,
        1743,
        1744,
        1745,
        1763,
        1765,
        1766,
        1767,
        1768,
        1769,
        1771,
        1776,
        1777,
        1778,
        1779,
        1780,
        1782,
        1786,
        1787,
        1788,
        1789,
        1790,
        1792,
        1793,
        1794,
        1795,
        1796,
        1797,
        1798,
        1799,
        1800,
        1801,
        1802,
        1803,
        1804,
        1805,
        1806,
        1807,
        1828,
    };

    constexpr int pcItems[] = {
        17,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        39,
        41,
        50,
        54,
        72,
        73,
        75,
        80,
        81,
        82,
        83,
        84,
        85,
        90,
        91,
        92,
        107,
        108,
        109,
        110,
        149,
        150,
        151,
        152,
        153,
        154,
        155,
        157,
        158,
        159,
        160,
        161,
        162,
        163,
        164,
        166,
        168,
        233,
        252,
        321,
        322,
        323,
        324,
        325,
        326,
        327,
        583,
        849,

        1125,
        1126,
        1127,
        1128,
        1231,
        1232,
        1233,
        1234,
        1235,
        1236,
        1237,
        1238,
        1239,
        1240,
        1241,
        1242,
        1243,
        1244,
        1245,
        1246,
        1247,
        1248,
        1249,
        1250,
        1251,

        1611,
        1613,
        1614,
        1615,
        1616,
        1617,
        1618,
        1619,
        1620,
        1621,
        1628,
        1630,
        1631,
        1632,
        1633,
        1634,
        1635,
        1636,
        1637,
        1638,
        1651,
        1679,
        1681,
        1682,
        1684,
        1686,
        1687,
        1688,
        1689,
        1690,
        1691,
        1692,
        1693,
        1694,
        1695,
        1696,
        1699,
        1700,
        1701,
        1702,
        1703,
        1704,
        1705,
        1706,
        1707,
        1708,
        1709,
        1710,
        1711,
        1712,
        1713,
        1716,
        1717,
        1720,
        1724,
        1725,
        1726,
        1727,
        1728,
        1732,
        1733,
        1734,
        1735,
        1736,
        1738,
        1739,
        1740,
        1741,
        1742,
        1746,
        1747,
        1748,
        1749,
        1750,
        1754,
        1755,
        1756,
        1757,
        1758,
        1759,
        1760,
        1761,
        1762,
        1764,
        1785,
    };

    // Crafting recipes live in their own pouch as regular item ids
    constexpr int recipeItems[] = {
        1640, 1641, 1642, 1643, 1644, 1646, 1647, 1648, 1649,
        1650, 1652, 1653, 1654, 1655, 1656, 1657, 1658, 1659,
        1660, 1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669,
        1670, 1671, 1673, 1674, 1675, 1676, 1677,
        1729, 1730, 1731,
        1751, 1752, 1753,
        1783, 1784,
    };

    std::span<const int> itemListForPouch(pksm::Sav::Pouch pouch)
    {
        using P = pksm::Sav::Pouch;
        switch (pouch)
        {
            case P::NormalItem:
                return normalItems;
            case P::KeyItem:
                return keyItems;
            case P::PCItem:
                return pcItems;
            default:
                return {};
        }
    }

    SavPLA::SavPLA(const std::shared_ptr<u8[]>& dt, size_t length) : Sav8(dt, length)
    {
        game      = Game::PLA;
        Box       = KBox;
        Party     = KParty;
        Status    = KStatus;
        PokeDex   = KZukan;
        Items     = KItems;
        BoxLayout = KBoxLayout;
    }

    u16 SavPLA::TID(void) const
    {
        return LittleEndian::convertTo<u16>(getBlock(Status)->decryptedData() + 0x10);
    }

    void SavPLA::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(getBlock(Status)->decryptedData() + 0x10, v);
    }

    u16 SavPLA::SID(void) const
    {
        return LittleEndian::convertTo<u16>(getBlock(Status)->decryptedData() + 0x12);
    }

    void SavPLA::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(getBlock(Status)->decryptedData() + 0x12, v);
    }

    GameVersion SavPLA::version(void) const
    {
        return GameVersion(getBlock(Status)->decryptedData()[0x14]);
    }

    void SavPLA::version(GameVersion v)
    {
        getBlock(Status)->decryptedData()[0x14] = u8(v);
    }

    Gender SavPLA::gender(void) const
    {
        return Gender{getBlock(Status)->decryptedData()[0x15]};
    }

    void SavPLA::gender(Gender v)
    {
        getBlock(Status)->decryptedData()[0x15] = u8(v);
    }

    Language SavPLA::language(void) const
    {
        return Language(getBlock(Status)->decryptedData()[0x17]);
    }

    void SavPLA::language(Language v)
    {
        getBlock(Status)->decryptedData()[0x17] = u8(v);
    }

    std::string SavPLA::otName(void) const
    {
        return StringUtils::getString(getBlock(Status)->decryptedData(), 0x20, 13);
    }

    void SavPLA::otName(const std::string_view& v)
    {
        StringUtils::setString(getBlock(Status)->decryptedData(), v, 0x20, 13);
    }

    u32 SavPLA::money(void) const
    {
        return LittleEndian::convertTo<u32>(getBlock(KMoney)->decryptedData());
    }

    void SavPLA::money(u32 v)
    {
        LittleEndian::convertFrom<u32>(getBlock(KMoney)->decryptedData(), v);
    }

    u16 SavPLA::playedHours(void) const
    {
        // The block stores u16 hours followed by minute and second bytes.
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return 0;
        }
        return LittleEndian::convertTo<u16>(block->decryptedData());
    }

    void SavPLA::playedHours(u16 v)
    {
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return;
        }
        LittleEndian::convertFrom<u16>(block->decryptedData(), v);
    }

    u8 SavPLA::playedMinutes(void) const
    {
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[2];
    }

    void SavPLA::playedMinutes(u8 v)
    {
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return;
        }
        block->decryptedData()[2] = v;
    }

    u8 SavPLA::playedSeconds(void) const
    {
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[3];
    }

    void SavPLA::playedSeconds(u8 v)
    {
        auto block = getBlock(0xC4FA7C8C);
        if (!block)
        {
            return;
        }
        block->decryptedData()[3] = v;
    }

    void SavPLA::item(const Item& item, Pouch pouch, u16 slot)
    {
        auto write = item.bytes();
        switch (pouch)
        {
            case Pouch::NormalItem:
                std::copy(write.begin(), write.end(), getBlock(KItems)->decryptedData() + 4 * slot);
                break;
            case Pouch::KeyItem:
                std::copy(
                    write.begin(), write.end(), getBlock(KItemsKey)->decryptedData() + 4 * slot);
                break;
            case Pouch::PCItem:
                std::copy(
                    write.begin(), write.end(), getBlock(KItemsStored)->decryptedData() + 4 * slot);
                break;
            case Pouch::Recipe:
                std::copy(
                    write.begin(), write.end(), getBlock(KItemsRecipe)->decryptedData() + 4 * slot);
                break;
            default:
                break;
        }
    }

    std::unique_ptr<Item> SavPLA::item(Pouch pouch, u16 slot) const
    {
        switch (pouch)
        {
            case Pouch::NormalItem:
                return std::make_unique<Item8a>(getBlock(KItems)->decryptedData() + 4 * slot);
            case Pouch::KeyItem:
                return std::make_unique<Item8a>(getBlock(KItemsKey)->decryptedData() + 4 * slot);
            case Pouch::PCItem:
                return std::make_unique<Item8a>(getBlock(KItemsStored)->decryptedData() + 4 * slot);
            case Pouch::Recipe:
                return std::make_unique<Item8a>(getBlock(KItemsRecipe)->decryptedData() + 4 * slot);
            default:
                return std::make_unique<Item8a>();
        }
    }

    SmallVector<std::pair<Sav::Pouch, int>, 15> SavPLA::pouches(void) const
    {
        return {
            std::pair{Pouch::NormalItem, 675},
            std::pair{Pouch::KeyItem,    100},
            std::pair{Pouch::PCItem,     180},
            std::pair{Pouch::Recipe,     70 },
        };
    }

    SmallVector<std::pair<Sav::Pouch, std::span<const int>>, 15> SavPLA::validItems(void) const
    {
        return {
            std::pair{Pouch::NormalItem, std::span<const int>(normalItems)},
            std::pair{Pouch::KeyItem,    std::span<const int>(keyItems)   },
            std::pair{Pouch::PCItem,     std::span<const int>(pcItems)    },
            std::pair{Pouch::Recipe,     std::span<const int>(recipeItems)},
        };
    }

    u8 SavPLA::currentBox() const
    {
        auto block = getBlock(KCurrentBox);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[0];
    }

    void SavPLA::currentBox(u8 box)
    {
        auto block = getBlock(KCurrentBox);
        if (!block)
        {
            return;
        }
        block->decryptedData()[0] = box;
    }

    u8 SavPLA::unlockedBoxes() const
    {
        auto block = getBlock(KBoxesUnlocked);
        if (!block)
        {
            return maxBoxes();
        }
        return block->decryptedData()[0];
    }

    void SavPLA::unlockedBoxes(u8 v)
    {
        auto block = getBlock(KBoxesUnlocked);
        if (!block)
        {
            return;
        }
        block->decryptedData()[0] = v;
    }

    std::string SavPLA::boxName(u8 box) const
    {
        return StringUtils::getString(getBlock(BoxLayout)->decryptedData(), box * 0x22, 17);
    }

    void SavPLA::boxName(u8 box, const std::string_view& name)
    {
        StringUtils::setString(getBlock(BoxLayout)->decryptedData(), name, box * 0x22, 17);
    }

    u8 SavPLA::boxWallpaper(u8 box) const
    {
        auto block = getBlock(KBoxWallpapers);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[box];
    }

    void SavPLA::boxWallpaper(u8 box, u8 v)
    {
        auto block = getBlock(KBoxWallpapers);
        if (!block)
        {
            return;
        }
        block->decryptedData()[box] = v;
    }

    u32 SavPLA::boxOffset(u8 box, u8 slot) const
    {
        return PA8::BOX_LENGTH * slot + PA8::BOX_LENGTH * 30 * box;
    }

    u32 SavPLA::partyOffset(u8 slot) const
    {
        return PA8::PARTY_LENGTH * slot;
    }

    u8 SavPLA::partyCount(void) const
    {
        return getBlock(Party)->decryptedData()[PA8::PARTY_LENGTH * 6];
    }

    void SavPLA::partyCount(u8 count)
    {
        getBlock(Party)->decryptedData()[PA8::PARTY_LENGTH * 6] = count;
    }

    std::unique_ptr<PKX> SavPLA::pkm(u8 slot) const
    {
        u32 offset = partyOffset(slot);
        return PKX::getPKM<PA8>(getBlock(Party)->decryptedData() + offset, PA8::PARTY_LENGTH);
    }

    std::unique_ptr<PKX> SavPLA::pkm(u8 box, u8 slot) const
    {
        u32 offset = boxOffset(box, slot);
        return PKX::getPKM<PA8>(getBlock(Box)->decryptedData() + offset, PA8::BOX_LENGTH);
    }

    void SavPLA::pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade)
    {
        if (pk.getLength() == PA8::PARTY_LENGTH || pk.getLength() == PA8::BOX_LENGTH)
        {
            auto pa8 = pk.partyClone();
            if (applyTrade)
            {
                trade(*pa8);
            }
            // Box slots hold 0x168 bytes, stored encrypted; the game decrypts on read, so
            // plaintext becomes a Bad Egg
            pa8->encrypt();
            std::ranges::copy(pa8->rawData().subspan(0, PA8::BOX_LENGTH),
                getBlock(Box)->decryptedData() + boxOffset(box, slot));
        }
    }

    void SavPLA::pkm(const PKX& pk, u8 slot)
    {
        if (pk.getLength() == PA8::PARTY_LENGTH || pk.getLength() == PA8::BOX_LENGTH)
        {
            auto pa8 = pk.partyClone();
            pa8->encrypt();
            std::ranges::copy(pa8->rawData(), getBlock(Party)->decryptedData() + partyOffset(slot));
        }
    }

    void SavPLA::cryptBoxData(bool crypted)
    {
        for (u8 box = 0; box < maxBoxes(); box++)
        {
            for (u8 slot = 0; slot < 30; slot++)
            {
                std::unique_ptr<PKX> pa8 = PKX::getPKM<PA8>(
                    getBlock(Box)->decryptedData() + boxOffset(box, slot), PA8::BOX_LENGTH, true);
                if (!crypted)
                {
                    pa8->encrypt();
                }
            }
        }
    }

    // PLA Pokedex: PokedexSaveData in KZukan block
    // Research entries start at offset 0x70, each 0x58 bytes, indexed by species ID (0-980)
    // Entry layout: Flags(u32)@0x00, ResearchRate(u16)@0x08, NumObtained(u16)@0x0A,
    // SelectedForm(u8)@0x50

    namespace
    {
        constexpr size_t DEX_RESEARCH_OFFSET     = 0x70;
        constexpr size_t DEX_RESEARCH_ENTRY_SIZE = 0x58;
        constexpr int DEX_MAX_SPECIES            = 981;
    }

    void SavPLA::dex(const PKX& pk)
    {
        if (pk.egg())
        {
            return;
        }

        auto block = getBlock(KZukan);
        if (!block)
        {
            return;
        }

        u16 species = u16(pk.species());
        if (species >= DEX_MAX_SPECIES)
        {
            return;
        }

        u8* entry =
            block->decryptedData() + DEX_RESEARCH_OFFSET + species * DEX_RESEARCH_ENTRY_SIZE;

        // Set HasEverBeenUpdated flag (bit 0)
        u32 flags = LittleEndian::convertTo<u32>(entry + 0x00);
        flags    |= 1u;
        LittleEndian::convertFrom<u32>(entry + 0x00, flags);

        // Increment NumObtained
        u16 numObtained = LittleEndian::convertTo<u16>(entry + 0x0A);
        if (numObtained < 0xFFFF)
        {
            LittleEndian::convertFrom<u16>(entry + 0x0A, u16(numObtained + 1));
        }

        // Set SelectedForm
        entry[0x50] = pk.alternativeForm();
    }

    int SavPLA::dexSeen(void) const
    {
        auto block = getBlock(KZukan);
        if (!block)
        {
            return 0;
        }

        u8* data  = block->decryptedData();
        int count = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 species = u16(spec);
            if (species >= DEX_MAX_SPECIES)
            {
                continue;
            }
            u8* entry = data + DEX_RESEARCH_OFFSET + species * DEX_RESEARCH_ENTRY_SIZE;
            u32 flags = LittleEndian::convertTo<u32>(entry + 0x00);
            if (flags & 1u) // HasEverBeenUpdated
            {
                count++;
            }
        }
        return count;
    }

    int SavPLA::dexCaught(void) const
    {
        auto block = getBlock(KZukan);
        if (!block)
        {
            return 0;
        }

        u8* data  = block->decryptedData();
        int count = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 species = u16(spec);
            if (species >= DEX_MAX_SPECIES)
            {
                continue;
            }
            u8* entry       = data + DEX_RESEARCH_OFFSET + species * DEX_RESEARCH_ENTRY_SIZE;
            u16 numObtained = LittleEndian::convertTo<u16>(entry + 0x0A);
            if (numObtained > 0)
            {
                count++;
            }
        }
        return count;
    }
}
