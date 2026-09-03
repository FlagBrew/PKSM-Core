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

#include "sav/SavZA.hpp"
#include "pkx/PA9.hpp"
#include "sav/Item.hpp"
#include "utils/endian.hpp"
#include "utils/i18n.hpp"
#include "utils/random.hpp"
#include "utils/utils.hpp"
#include "wcx/WA9.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ranges>

namespace
{
    // From SAV9ZA.cs: SIZE_BOXSLOT = SIZE_9PARTY + GapBoxSlot = 0x158 + 0x40 = 0x198
    constexpr u32 BOX_SLOT_SIZE = pksm::PA9::PARTY_LENGTH + 0x40;
    // From Party9a.cs: SlotSize = SIZE_9PARTY + 0x40 + 0x48 = 0x158 + 0x88 = 0x1E0
    constexpr u32 PARTY_SLOT_SIZE = pksm::PA9::PARTY_LENGTH + 0x88;
    // From SAV6.LongStringLength = 0x22 (34 bytes, 17 chars UTF-16LE)
    constexpr int BOX_NAME_BYTES = 0x22;
    constexpr int BOX_NAME_CHARS = BOX_NAME_BYTES / 2;
    // Item9a size
    constexpr int ITEM_SIZE = 16;
    constexpr int DONUT_SIZE = 0x48;

    // PokeDexEntry9a: 0x84 bytes per species
    constexpr int DEX_ENTRY_SIZE_ZA = 0x84;

    // Gen 9 species converter: national dex ID -> internal ID
    constexpr int FIRST_UNALIGNED_9  = 917;
    constexpr int8_t speciesTable9[] = {
        1,
        1,
        1,
        1,
        33,
        33,
        33,
        21,
        21,
        44,
        44,
        7,
        7,
        7,
        29,
        31,
        31,
        31,
        68,
        68,
        68,
        2,
        2,
        17,
        17,
        30,
        30,
        24,
        24,
        28,
        28,
        58,
        58,
        12,
        -13,
        -13,
        -31,
        -31,
        -29,
        -29,
        43,
        43,
        43,
        -31,
        -31,
        -3,
        -30,
        -30,
        -23,
        -23,
        -14,
        -24,
        -3,
        -3,
        -47,
        -47,
        -12,
        -27,
        -27,
        -44,
        -46,
        -26,
        31,
        29,
        -53,
        -65,
        25,
        -6,
        -3,
        -7,
        -4,
        -4,
        -8,
        -4,
        1,
        -3,
        -3,
        -6,
        -4,
        -47,
        -47,
        -47,
        -23,
        -23,
        -5,
        -7,
        -9,
        -7,
        -20,
        -13,
        -9,
        -9,
        -29,
        -23,
        1,
        12,
        12,
        0,
        0,
        0,
        -6,
        5,
        -6,
        -3,
        -3,
        -2,
        -4,
        -3,
        -3,
    };

    u16 getInternal9(u16 species)
    {
        int shift = species - FIRST_UNALIGNED_9;
        if (shift < 0 || shift >= (int)std::size(speciesTable9))
        {
            return species;
        }
        return u16(species + speciesTable9[shift]);
    }

    int getDexLangFlag(int lang)
    {
        if (lang > 10 || lang == 6 || lang <= 0)
        {
            return 0;
        }
        return lang >= 7 ? lang - 2 : lang - 1;
    }

    // Item lists from ItemStorage9ZA.cs
    constexpr int medicineItems[] = {
        17,
        18,
        19,
        20,
        21,
        22,
        23,
        24,
        25,
        26,
        27,
        28,
        29,
        30,
        31,
        32,
        33,
        708,
        2684,
    };

    constexpr int ballItems[] = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        492,
        493,
        494,
        495,
        496,
        497,
        498,
        499,
        576,
        851,
    };

    constexpr int otherItems[] = {
        45,
        46,
        47,
        48,
        49,
        50,
        52,
        80,
        81,
        82,
        83,
        84,
        85,
        103,
        107,
        108,
        109,
        116,
        117,
        118,
        119,
        214,
        217,
        218,
        221,
        222,
        230,
        231,
        232,
        233,
        234,
        236,
        237,
        238,
        239,
        240,
        241,
        242,
        243,
        244,
        245,
        246,
        247,
        248,
        249,
        250,
        251,
        252,
        253,
        258,
        259,
        266,
        267,
        268,
        270,
        275,
        289,
        290,
        291,
        292,
        293,
        294,
        296,
        324,
        534,
        535,
        537,
        538,
        540,
        564,
        565,
        566,
        567,
        568,
        569,
        570,
        639,
        640,
        646,
        647,
        710,
        711,
        795,
        796,
        849,
        1124,
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
        1582,
        1592,
        1691,
        1861,
        2344,
        2401,
        2558,
        2618,
        2137,
        2619,
    };

    constexpr int treasureItems[] = {
        86,
        88,
        89,
        92,
        571,
        581,
        582,
    };

    constexpr int keyItems[] = {
        632,
        700,
        765,
        847,
        1278,
        2588,
        2589,
        2590,
        2591,
        2592,
        2595,
        2596,
        2597,
        2598,
        2599,
        2600,
        2601,
        2602,
        2603,
        2604,
        2605,
        2606,
        2607,
        2608,
        2609,
        2610,
        2611,
        2612,
        2613,
        2620,
        2621,
        2622,
        2623,
        2624,
        2625,
        2626,
        2627,
        2628,
        2629,
        2630,
        2631,
        2632,
        2633,
        2634,
    };

    constexpr int berryItems[] = {
        149,
        150,
        151,
        152,
        153,
        155,
        156,
        157,
        158,
        169,
        170,
        171,
        172,
        173,
        174,
        184,
        185,
        186,
        187,
        188,
        189,
        190,
        191,
        192,
        193,
        194,
        195,
        196,
        197,
        198,
        199,
        200,
        686,
        2651,
        2652,
        2653,
        2654,
        2655,
        2656,
        2657,
        2658,
        2659,
        2660,
        2661,
        2662,
        2663,
        2664,
        2665,
        2666,
        2667,
        2668,
        2669,
        2670,
        2671,
        2672,
        2673,
        2674,
        2675,
        2676,
        2677,
        2678,
        2679,
        2680,
        2681,
        2682,
        2683,
    };

    constexpr int tmItems[] = {
        328,
        329,
        330,
        331,
        332,
        333,
        334,
        335,
        336,
        337,
        338,
        339,
        340,
        341,
        342,
        343,
        344,
        345,
        346,
        347,
        348,
        349,
        350,
        351,
        352,
        353,
        354,
        355,
        356,
        357,
        358,
        359,
        360,
        361,
        362,
        363,
        364,
        365,
        366,
        367,
        368,
        369,
        370,
        371,
        372,
        373,
        374,
        375,
        376,
        377,
        378,
        379,
        380,
        381,
        382,
        383,
        384,
        385,
        386,
        387,
        388,
        389,
        390,
        391,
        392,
        393,
        394,
        395,
        396,
        397,
        398,
        399,
        400,
        401,
        402,
        403,
        404,
        405,
        406,
        407,
        408,
        409,
        410,
        411,
        412,
        413,
        414,
        415,
        416,
        417,
        418,
        419,
        618,
        619,
        620,
        690,
        691,
        692,
        693,
        2160,
        2162,
        2163,
        2164,
        2165,
        2166,
        2167,
        2168,
        2169,
        2170,
        2171,
        2172,
        2173,
        2174,
        2175,
        2176,
        2177,
        2178,
        2179,
        2180,
        2181,
        2182,
        2183,
        2184,
        2185,
        2186,
        2187,
        2188,
        2189,
        2190,
        2191,
        2192,
        2193,
        2194,
        2195,
        2196,
        2197,
        2198,
        2199,
        2200,
        2201,
        2202,
        2203,
        2204,
        2205,
        2206,
        2207,
        2208,
        2209,
        2210,
        2211,
        2212,
        2213,
        2214,
        2215,
        2216,
        2217,
        2218,
        2219,
        2220,
        2221,
    };

    constexpr int megaStoneItems[] = {
        656,
        657,
        658,
        659,
        660,
        661,
        662,
        663,
        664,
        665,
        666,
        667,
        668,
        669,
        670,
        671,
        672,
        673,
        674,
        675,
        676,
        677,
        678,
        679,
        680,
        681,
        682,
        683,
        684,
        685,
        752,
        753,
        754,
        755,
        756,
        757,
        758,
        759,
        760,
        761,
        762,
        763,
        764,
        767,
        768,
        769,
        770,
        2559,
        2560,
        2561,
        2562,
        2563,
        2564,
        2565,
        2566,
        2567,
        2568,
        2569,
        2570,
        2571,
        2572,
        2573,
        2574,
        2575,
        2576,
        2577,
        2578,
        2579,
        2580,
        2581,
        2582,
        2583,
        2584,
        2585,
        2586,
        2587,
        2635,
        2636,
        2637,
        2638,
        2639,
        2640,
        2641,
        2642,
        2643,
        2644,
        2645,
        2646,
        2647,
        2648,
        2649,
        2650,
    };

    std::span<const int> itemListForPouch(pksm::Sav::Pouch pouch)
    {
        using P = pksm::Sav::Pouch;
        switch (pouch)
        {
            case P::Medicine:
                return medicineItems;
            case P::Ball:
                return ballItems;
            case P::NormalItem:
                return otherItems;
            case P::Treasure:
                return treasureItems;
            case P::KeyItem:
                return keyItems;
            case P::Berry:
                return berryItems;
            case P::TM:
                return tmItems;
            case P::MegaStones:
                return megaStoneItems;
            default:
                return {};
        }
    }
}

namespace pksm
{
    SavZA::SavZA(const std::shared_ptr<u8[]>& dt, size_t length) : Sav8(dt, length)
    {
        game      = Game::ZA;
        Box       = KBox;
        Party     = KParty;
        Status    = KStatus;
        Items     = KItems;
        BoxLayout = KBoxLayout;
    }

    u16 SavZA::TID(void) const
    {
        return LittleEndian::convertTo<u16>(getBlock(Status)->decryptedData() + 0x00);
    }

    void SavZA::TID(u16 v)
    {
        LittleEndian::convertFrom<u16>(getBlock(Status)->decryptedData() + 0x00, v);
    }

    u16 SavZA::SID(void) const
    {
        return LittleEndian::convertTo<u16>(getBlock(Status)->decryptedData() + 0x02);
    }

    void SavZA::SID(u16 v)
    {
        LittleEndian::convertFrom<u16>(getBlock(Status)->decryptedData() + 0x02, v);
    }

    GameVersion SavZA::version(void) const
    {
        return GameVersion(getBlock(Status)->decryptedData()[0x04]);
    }

    void SavZA::version(GameVersion v)
    {
        getBlock(Status)->decryptedData()[0x04] = u8(v);
    }

    Gender SavZA::gender(void) const
    {
        return Gender{getBlock(Status)->decryptedData()[0x05]};
    }

    void SavZA::gender(Gender v)
    {
        getBlock(Status)->decryptedData()[0x05] = u8(v);
    }

    Language SavZA::language(void) const
    {
        return Language(getBlock(Status)->decryptedData()[0x07]);
    }

    void SavZA::language(Language v)
    {
        getBlock(Status)->decryptedData()[0x07] = u8(v);
    }

    std::string SavZA::otName(void) const
    {
        // MyStatus9a: OT at 0x10, 0x1A bytes (13 chars UTF-16LE)
        return StringUtils::getString(getBlock(Status)->decryptedData(), 0x10, 13);
    }

    void SavZA::otName(const std::string_view& v)
    {
        StringUtils::setString(getBlock(Status)->decryptedData(), v, 0x10, 13);
    }

    u32 SavZA::money(void) const
    {
        return LittleEndian::convertTo<u32>(getBlock(KMoney)->decryptedData());
    }

    void SavZA::money(u32 v)
    {
        LittleEndian::convertFrom<u32>(getBlock(KMoney)->decryptedData(), v);
    }

    u32 SavZA::BP(void) const
    {
        auto block = getBlock(KTicketPointsRoyale);
        if (!block)
        {
            return 0;
        }
        return LittleEndian::convertTo<u32>(block->decryptedData());
    }

    void SavZA::BP(u32 v)
    {
        auto block = getBlock(KTicketPointsRoyale);
        if (!block)
        {
            return;
        }
        LittleEndian::convertFrom<u32>(block->decryptedData(), v);
    }

    u16 SavZA::playedHours(void) const
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return 0;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        return (u16)(secs / 3600);
    }

    void SavZA::playedHours(u16 v)
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        double curMinSec = std::fmod(secs, 3600);
        double newSecs   = (double)v * 3600.0 + curMinSec;
        std::memcpy(block->decryptedData(), &newSecs, sizeof(double));
    }

    u8 SavZA::playedMinutes(void) const
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return 0;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        return (u8)(std::fmod(secs, 3600) / 60);
    }

    void SavZA::playedMinutes(u8 v)
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        double hours   = std::floor(secs / 3600);
        double curSec  = std::fmod(secs, 60);
        double newSecs = hours * 3600.0 + (double)v * 60.0 + curSec;
        std::memcpy(block->decryptedData(), &newSecs, sizeof(double));
    }

    u8 SavZA::playedSeconds(void) const
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return 0;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        return (u8)(std::fmod(secs, 60));
    }

    void SavZA::playedSeconds(u8 v)
    {
        auto block = getBlock(KPlayedSeconds);
        if (!block)
        {
            return;
        }
        double secs;
        std::memcpy(&secs, block->decryptedData(), sizeof(double));
        double hoursMin = secs - std::fmod(secs, 60);
        double newSecs  = hoursMin + (double)v;
        std::memcpy(block->decryptedData(), &newSecs, sizeof(double));
    }

    // Items: Z-A uses index-based storage (3000 slots x 16 bytes, indexed by item ID)
    // Format per slot: [Pouch:u32][Count:u32][Flags:u32][Padding:u32]
    void SavZA::item(const Item& item, Pouch pouch, u16 slot)
    {
        auto items = itemListForPouch(pouch);
        if (slot >= items.size())
        {
            return;
        }
        u16 itemId    = static_cast<u16>(items[slot]);
        u8* blockData = getBlock(Items)->decryptedData();
        u32 offset    = itemId * ITEM_SIZE;
        u32 count     = item.count();
        // Write count at offset + 4
        LittleEndian::convertFrom<u32>(blockData + offset + 4, count);
    }

    std::unique_ptr<Item> SavZA::item(Pouch pouch, u16 slot) const
    {
        auto items = itemListForPouch(pouch);
        if (slot >= items.size())
        {
            return std::make_unique<Item9a>();
        }
        u16 itemId    = static_cast<u16>(items[slot]);
        u8* blockData = getBlock(Items)->decryptedData();
        u32 offset    = itemId * ITEM_SIZE;
        return std::make_unique<Item9a>(blockData + offset, itemId);
    }

    // Donut slot: [Made:u64 ms since 1970][Stars:u8][LevelBoost:u8][Id:u16][Calories:u16]
    // [NameBerry:u16][Berries:8 x u16][Made:u64 1900 epoch][Flavor hashes:3 x u64][Reserved:u64]
    bool SavZA::hasDonuts(void) const
    {
        return getBlock(KDonuts) != nullptr;
    }

    std::vector<SavZA::Donut> SavZA::donuts(void) const
    {
        std::vector<Donut> ret;
        auto block = getBlock(KDonuts);
        if (!block)
        {
            return ret;
        }
        for (u16 slot = 0; slot < DONUT_SLOTS; slot++)
        {
            const u8* data = block->decryptedData() + slot * DONUT_SIZE;
            if (LittleEndian::convertTo<u64>(data) == 0) // never made
            {
                continue;
            }
            ret.push_back({LittleEndian::convertTo<u16>(data + 0x0A), data[0x08], data[0x09],
                LittleEndian::convertTo<u16>(data + 0x0C), LittleEndian::convertTo<u16>(data + 0x0E)});
        }
        return ret;
    }

    SmallVector<std::pair<Sav::Pouch, int>, 15> SavZA::pouches(void) const
    {
        return {
            std::pair{Pouch::Medicine,   int(std::size(medicineItems)) },
            std::pair{Pouch::Ball,       int(std::size(ballItems))     },
            std::pair{Pouch::Berry,      int(std::size(berryItems))    },
            std::pair{Pouch::NormalItem, int(std::size(otherItems))    },
            std::pair{Pouch::TM,         int(std::size(tmItems))       },
            std::pair{Pouch::MegaStones, int(std::size(megaStoneItems))},
            std::pair{Pouch::Treasure,   int(std::size(treasureItems)) },
            std::pair{Pouch::KeyItem,    int(std::size(keyItems))      },
        };
    }

    SmallVector<std::pair<Sav::Pouch, std::span<const int>>, 15> SavZA::validItems(void) const
    {
        return {
            std::pair{Pouch::Medicine,   std::span<const int>(medicineItems) },
            std::pair{Pouch::Ball,       std::span<const int>(ballItems)     },
            std::pair{Pouch::Berry,      std::span<const int>(berryItems)    },
            std::pair{Pouch::NormalItem, std::span<const int>(otherItems)    },
            std::pair{Pouch::TM,         std::span<const int>(tmItems)       },
            std::pair{Pouch::MegaStones, std::span<const int>(megaStoneItems)},
            std::pair{Pouch::Treasure,   std::span<const int>(treasureItems) },
            std::pair{Pouch::KeyItem,    std::span<const int>(keyItems)      },
        };
    }

    u8 SavZA::currentBox() const
    {
        auto block = getBlock(KCurrentBox);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[0];
    }

    void SavZA::currentBox(u8 box)
    {
        auto block = getBlock(KCurrentBox);
        if (!block)
        {
            return;
        }
        block->decryptedData()[0] = box;
    }

    u8 SavZA::unlockedBoxes() const
    {
        auto block = getBlock(KBoxesUnlocked);
        if (!block)
        {
            return maxBoxes();
        }
        return block->decryptedData()[0];
    }

    void SavZA::unlockedBoxes(u8 v)
    {
        auto block = getBlock(KBoxesUnlocked);
        if (!block)
        {
            return;
        }
        block->decryptedData()[0] = v;
    }

    std::string SavZA::boxName(u8 box) const
    {
        // BoxLayout9a: SAV6.LongStringLength (0x22) bytes per box, 17 chars UTF-16LE
        return StringUtils::getString(
            getBlock(BoxLayout)->decryptedData(), box * BOX_NAME_BYTES, BOX_NAME_CHARS);
    }

    void SavZA::boxName(u8 box, const std::string_view& name)
    {
        StringUtils::setString(
            getBlock(BoxLayout)->decryptedData(), name, box * BOX_NAME_BYTES, BOX_NAME_CHARS);
    }

    u8 SavZA::boxWallpaper(u8 box) const
    {
        auto block = getBlock(KBoxWallpapers);
        if (!block)
        {
            return 0;
        }
        return block->decryptedData()[box];
    }

    void SavZA::boxWallpaper(u8 box, u8 v)
    {
        auto block = getBlock(KBoxWallpapers);
        if (!block)
        {
            return;
        }
        block->decryptedData()[box] = v;
    }

    // From SAV9ZA.cs:
    // SIZE_BOXSLOT = SIZE_9PARTY + GapBoxSlot = 0x158 + 0x40 = 0x198
    // GetBoxOffset(box) = Box + (SIZE_BOXSLOT * box * 30)
    // Slot within box: slot * SIZE_BOXSLOT
    u32 SavZA::boxOffset(u8 box, u8 slot) const
    {
        return BOX_SLOT_SIZE * slot + BOX_SLOT_SIZE * 30 * box;
    }

    // From SAV9ZA.cs:
    // GetPartyOffset(slot) = Party + ((SIZE_PARTY + GapPartySlot) * slot)
    // GapPartySlot = 0x88
    u32 SavZA::partyOffset(u8 slot) const
    {
        return PARTY_SLOT_SIZE * slot;
    }

    // Party9a.cs computes party count by iteration (no stored count byte)
    u8 SavZA::partyCount(void) const
    {
        u8* blockData = getBlock(Party)->decryptedData();
        for (u8 i = 0; i < 6; i++)
        {
            u32 offset  = PARTY_SLOT_SIZE * i;
            u16 species = LittleEndian::convertTo<u16>(blockData + offset + 0x08);
            if (species == 0)
            {
                return i;
            }
        }
        return 6;
    }

    void SavZA::partyCount(u8 count)
    {
        // Clear slots after the count to maintain consistency
        u8* blockData = getBlock(Party)->decryptedData();
        for (u8 i = count; i < 6; i++)
        {
            u32 offset = PARTY_SLOT_SIZE * i;
            std::memset(blockData + offset, 0, PA9::PARTY_LENGTH);
        }
    }

    std::unique_ptr<PKX> SavZA::pkm(u8 slot) const
    {
        u32 offset = partyOffset(slot);
        return PKX::getPKM<PA9>(getBlock(Party)->decryptedData() + offset, PA9::PARTY_LENGTH);
    }

    // Box slots store party-length data (from GetDataForBox: EncryptedPartyData + gap)
    std::unique_ptr<PKX> SavZA::pkm(u8 box, u8 slot) const
    {
        u32 offset = boxOffset(box, slot);
        return PKX::getPKM<PA9>(getBlock(Box)->decryptedData() + offset, PA9::PARTY_LENGTH);
    }

    void SavZA::pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade)
    {
        if (pk.getLength() == PA9::PARTY_LENGTH || pk.getLength() == PA9::BOX_LENGTH)
        {
            auto pa9 = pk.partyClone();
            if (applyTrade)
            {
                trade(*pa9);
            }
            // Box slots are stored encrypted; the game decrypts on read, so plaintext becomes a Bad Egg
            pa9->encrypt();
            std::ranges::copy(pa9->rawData().subspan(0, PA9::PARTY_LENGTH),
                getBlock(Box)->decryptedData() + boxOffset(box, slot));
        }
    }

    void SavZA::pkm(const PKX& pk, u8 slot)
    {
        if (pk.getLength() == PA9::PARTY_LENGTH || pk.getLength() == PA9::BOX_LENGTH)
        {
            auto pa9 = pk.partyClone();
            pa9->encrypt();
            std::ranges::copy(pa9->rawData(), getBlock(Party)->decryptedData() + partyOffset(slot));
        }
    }

    void SavZA::cryptBoxData(bool crypted)
    {
        for (u8 box = 0; box < maxBoxes(); box++)
        {
            for (u8 slot = 0; slot < 30; slot++)
            {
                std::unique_ptr<PKX> pa9 = PKX::getPKM<PA9>(
                    getBlock(Box)->decryptedData() + boxOffset(box, slot), PA9::PARTY_LENGTH, true);
                if (!crypted)
                {
                    pa9->encrypt();
                }
            }
        }
    }

    // Pokedex: Z-A uses PokeDexEntry9a (0x84 bytes per species), indexed by internal species ID
    void SavZA::dex(const PKX& pk)
    {
        if (pk.egg())
        {
            return;
        }
        auto block = getBlock(KPokedex);
        if (!block)
        {
            return;
        }

        u16 species  = u16(pk.species());
        u16 internal = getInternal9(species);
        u8* entry    = block->decryptedData() + internal * DEX_ENTRY_SIZE_ZA;

        u8 form = pk.alternativeForm();

        // Form captured (u32 at 0x00)
        u32 caughtFlags = LittleEndian::convertTo<u32>(entry + 0x00);
        caughtFlags    |= (1u << form);
        LittleEndian::convertFrom<u32>(entry + 0x00, caughtFlags);

        // Form seen (u32 at 0x04)
        u32 seenFlags = LittleEndian::convertTo<u32>(entry + 0x04);
        seenFlags    |= (1u << form);
        LittleEndian::convertFrom<u32>(entry + 0x04, seenFlags);

        // Language obtained (u16 at 0x08)
        u16 langFlags = LittleEndian::convertTo<u16>(entry + 0x08);
        langFlags    |= u16(1u << getDexLangFlag(u8(pk.language())));
        langFlags    |= u16(1u << getDexLangFlag(u8(language())));
        LittleEndian::convertFrom<u16>(entry + 0x08, langFlags);

        // New flag (0x0A)
        entry[0x0A] = 1;

        // Gender seen (0x0B)
        entry[0x0B] |= u8(1u << u8(pk.gender()));

        // Shiny seen per form (u32 at 0x0C)
        if (pk.shiny())
        {
            u32 shinyFlags = LittleEndian::convertTo<u32>(entry + 0x0C);
            shinyFlags    |= (1u << form);
            LittleEndian::convertFrom<u32>(entry + 0x0C, shinyFlags);
        }

        // Display form, gender, shiny (0x5A, 0x5B, 0x5C)
        entry[0x5A] = form;
        entry[0x5B] = u8(pk.gender());
        if (pk.shiny())
        {
            entry[0x5C] = 1;
        }
    }

    int SavZA::dexSeen(void) const
    {
        auto block = getBlock(KPokedex);
        if (!block)
        {
            return 0;
        }
        u8* data  = block->decryptedData();
        int count = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 internal = getInternal9(u16(spec));
            u32 seenFlags =
                LittleEndian::convertTo<u32>(data + internal * DEX_ENTRY_SIZE_ZA + 0x04);
            if (seenFlags != 0)
            {
                count++;
            }
        }
        return count;
    }

    int SavZA::dexCaught(void) const
    {
        auto block = getBlock(KPokedex);
        if (!block)
        {
            return 0;
        }
        u8* data  = block->decryptedData();
        int count = 0;
        for (const auto& spec : availableSpecies())
        {
            u16 internal    = getInternal9(u16(spec));
            u32 caughtFlags = LittleEndian::convertTo<u32>(data + internal * DEX_ENTRY_SIZE_ZA);
            if (caughtFlags != 0)
            {
                count++;
            }
        }
        return count;
    }

    void SavZA::mysteryGift(const WCX& wc, int&)
    {
        if (wc.generation() != Generation::NINE || wc.extension() != ".wa9")
        {
            return;
        }

        const WA9& wa9 = static_cast<const WA9&>(wc);
        if (wa9.pokemon())
        {
            int injectPosition = 0;
            for (injectPosition = 0; injectPosition < maxSlot(); injectPosition++)
            {
                if (pkm(injectPosition / 30, injectPosition % 30)->species() == Species::None)
                {
                    break;
                }
            }

            if (injectPosition == maxSlot())
            {
                return;
            }

            auto pa9 = PKX::getPKM<PA9>(nullptr, PA9::BOX_LENGTH);

            pa9->encryptionConstant(wa9.encryptionConstant() ? wa9.encryptionConstant()
                                                             : pksm::randomNumber(0, 0xFFFFFFFF));
            pa9->TID(wa9.TID());
            pa9->SID(wa9.SID());
            pa9->species(wa9.species());
            pa9->alternativeForm(wa9.alternativeForm());
            pa9->level(wa9.level() ? wa9.level() : pksm::randomNumber(1, 100));
            pa9->ball(wa9.ball() ? wa9.ball() : Ball::Poke);
            pa9->metLevel(wa9.metLevel() ? wa9.metLevel() : pa9->level());
            pa9->heldItem(wa9.heldItem());

            for (size_t move = 0; move < 4; move++)
            {
                pa9->move(move, wa9.move(move));
                pa9->relearnMove(move, wa9.relearnMove(move));
            }

            pa9->version(wa9.version() != GameVersion::INVALID ? wa9.version() : version());

            std::string wcOT = wa9.otName(language());
            if (wcOT.empty())
            {
                pa9->otName(otName());
                pa9->otGender(gender());
            }
            else
            {
                pa9->otName(wcOT);
                pa9->otGender(wa9.otGender() < Gender::Genderless ? wa9.otGender() : gender());
                pa9->currentHandler(PKXHandler::NonOT);
            }

            pa9->fatefulEncounter(true);

            for (Stat stat : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF})
            {
                pa9->ev(stat, wa9.ev(stat));
            }

            pa9->metLocation(wa9.metLocation());
            pa9->eggLocation(wa9.eggLocation());

            if (wa9.otGender() >= Gender::Genderless)
            {
                pa9->TID(TID());
                pa9->SID(SID());
            }

            if (pa9->species() == Species::Meowstic)
            {
                pa9->alternativeForm(u8(pa9->gender()));
            }

            pa9->metDate(Date::today());

            Language nickLang = wa9.nicknameLanguage(language());
            if (nickLang != Language(0))
            {
                pa9->language(nickLang);
            }
            else
            {
                pa9->language(language());
            }

            pa9->nicknamed(wa9.nicknamed(pa9->language()));
            pa9->nickname(pa9->nicknamed() ? wa9.nickname(pa9->language())
                                           : pa9->species().localize(pa9->language()));

            // Ribbon transfer: WA9 stores ribbon index bytes (0xFF = none)
            for (int i = 0; i < 0x20; i++)
            {
                u8 ribIdx = wa9.ribbonAtIndex(i);
                if (ribIdx == 0xFF)
                {
                    continue;
                }
                Ribbon rib = ribIdx <= 45 ? Ribbon{u32(ribIdx)} : Ribbon{u32(ribIdx) + 48};
                pa9->ribbon(rib, true);
            }

            if (wa9.egg())
            {
                pa9->eggDate(Date::today());
                pa9->nickname(i18n::species(pa9->language(), Species::None));
                pa9->nicknamed(true);
            }

            pa9->currentFriendship(pa9->baseFriendship());

            // Alpha
            static_cast<PA9*>(pa9.get())->isAlpha(wa9.isAlpha());

            pa9->nature(wa9.nature() == Nature::INVALID ? Nature{u8(pksm::randomNumber(0, 24))}
                                                        : wa9.nature());
            pa9->gender(PKX::genderFromRatio(pksm::randomNumber(0, 0xFFFFFFFF), pa9->genderType()));

            // Ability
            switch (wa9.abilityType())
            {
                case 0:
                case 1:
                case 2:
                    pa9->setAbility(wa9.abilityType());
                    break;
                case 3:
                case 4:
                    pa9->setAbility(pksm::randomNumber(0, wa9.abilityType() - 2));
                    break;
            }

            // PID
            switch (wa9.PIDType())
            {
                case 0: // Fixed value
                    pa9->PID(wa9.PID());
                    break;
                case 1: // Random
                    pa9->PID(pksm::randomNumber(0, 0xFFFFFFFF));
                    break;
                case 2: // Force shiny
                case 3:
                    pa9->PID(pksm::randomNumber(0, 0xFFFFFFFF));
                    pa9->PID(((pa9->TID() ^ pa9->SID() ^ (pa9->PID() & 0xFFFF) ^ 1) << 16) |
                             (pa9->PID() & 0xFFFF));
                    break;
                case 4: // Fixed PID value
                    pa9->PID(wa9.PID());
                    break;
                default: // Never shiny
                    pa9->PID(pksm::randomNumber(0, 0xFFFFFFFF));
                    pa9->shiny(false);
                    break;
            }

            // IVs
            int numPerfectIVs = 0;
            for (Stat stat : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF})
            {
                if (wa9.iv(stat) - 0xFC < 3)
                {
                    numPerfectIVs = wa9.iv(stat) - 0xFB;
                    break;
                }
            }
            for (int iv = 0; iv < numPerfectIVs; iv++)
            {
                Stat setMeTo31 = Stat(pksm::randomNumber(0, 5));
                while (pa9->iv(setMeTo31) == 31)
                {
                    setMeTo31 = Stat(pksm::randomNumber(0, 5));
                }
                pa9->iv(setMeTo31, 31);
            }
            for (Stat stat : {Stat::HP, Stat::ATK, Stat::DEF, Stat::SPD, Stat::SPATK, Stat::SPDEF})
            {
                if (pa9->iv(stat) != 31)
                {
                    pa9->iv(stat, pksm::randomNumber(0, 31));
                }
            }

            pa9->refreshChecksum();

            pkm(*pa9, injectPosition / 30, injectPosition % 30, false);
        }
        else if (wa9.item())
        {
            auto valid  = validItems();
            auto limits = pouches();
            for (int itemNum = 0; itemNum < wa9.items(); itemNum++)
            {
                bool currentSet = false;
                for (size_t pouch = 0; pouch < limits.size(); pouch++)
                {
                    auto validPouch = std::ranges::find_if(valid,
                        [&](const auto& i)
                        {
                            return i.first == limits[pouch].first;
                        })->second;
                    if (!currentSet && std::binary_search(validPouch.begin(), validPouch.end(),
                                           wa9.object(itemNum)))
                    {
                        for (int slot = 0; slot < limits[pouch].second; slot++)
                        {
                            auto occupying = item(limits[pouch].first, slot);
                            if (occupying->id() == 0)
                            {
                                occupying->id(wa9.object(itemNum));
                                occupying->count(wa9.objectQuantity(itemNum));
                                item(*occupying, limits[pouch].first, slot);
                                currentSet = true;
                                break;
                            }
                            else if (occupying->id() == wa9.object(itemNum) &&
                                     limits[pouch].first != Pouch::TM)
                            {
                                occupying->count(occupying->count() + 1);
                                item(*occupying, limits[pouch].first, slot);
                                currentSet = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
