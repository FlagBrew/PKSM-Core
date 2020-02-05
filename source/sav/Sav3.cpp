/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2020 Bernardo Giordano, Admiral Fish, piepie62
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

#include "sav/Sav3.hpp"
#include "i18n/i18n.hpp"
#include "pkx/PK3.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/utils.hpp"
#include "wcx/WCX.hpp"
#include <algorithm>

const void Sav3::loadBlocks()
{
    std::array<int, BLOCK_COUNT> o1 = getBlockOrder(data, 0);
    // I removed a length > 0x10000, since length should always be 0x20000 I think that's fine?
    std::array<int, BLOCK_COUNT> o2 = getBlockOrder(data, 0xE000);
    activeSAV                       = getActiveSaveIndex(data, o1, o2);
    blockOrder                      = activeSAV == 0 ? o1 : o2;

    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        unsigned int index = std::find(blockOrder.begin(), blockOrder.end(), i) - blockOrder.begin();
        blockOfs[i]        = index == blockOrder.size() ? -1 /*was int.MinValue*/ : (index * SIZE_BLOCK) + ABO();
    }
}

auto Sav3::getBlockOrder(std::shared_ptr<u8[]> dt, int ofs) -> std::array<int, BLOCK_COUNT>
{
    std::array<int, BLOCK_COUNT> order;
    for (int i = 0; i < BLOCK_COUNT; i++)
        order[i] = Endian::convertTo<s16>(&dt[ofs + (i * SIZE_BLOCK) + 0xFF4]);
    return order;
}

const int Sav3::getActiveSaveIndex(std::shared_ptr<u8[]> dt, std::array<int, BLOCK_COUNT>& blockOrder1, std::array<int, BLOCK_COUNT>& blockOrder2)
{
    int zeroBlock1 = std::find(blockOrder1.begin(), blockOrder1.end(), 0) - blockOrder1.begin();
    int zeroBlock2 = std::find(blockOrder2.begin(), blockOrder2.end(), 0) - blockOrder2.begin();
    if (zeroBlock2 == blockOrder2.size())
        return 0;
    if (zeroBlock1 == blockOrder1.size())
        return 1;
    u32 count1 = Endian::convertTo<u32>(&dt[(zeroBlock1 * SIZE_BLOCK) + 0x0FFC]);
    u32 count2 = Endian::convertTo<u32>(&dt[(zeroBlock2 * SIZE_BLOCK) + 0xEFFC]);
    return count1 > count2 ? 0 : 1;
}

Game Sav3::getVersion(std::shared_ptr<u8[]> dt)
{
    // Get block 0 offset
    std::array<int, BLOCK_COUNT> o1     = getBlockOrder(dt, 0);
    std::array<int, BLOCK_COUNT> o2     = getBlockOrder(dt, 0xE000);
    int activeSAV                       = getActiveSaveIndex(dt, o1, o2);
    std::array<int, BLOCK_COUNT>& order = activeSAV == 0 ? o1 : o2;

    int ABO = activeSAV * SIZE_BLOCK * 0xE;

    int blockOfs0 = ((std::find(order.begin(), order.end(), 0) - order.begin()) * SIZE_BLOCK) + ABO;

    // Get version
    u32 gameCode = Endian::convertTo<u32>(&dt[blockOfs0 + 0xAC]);
    switch (gameCode)
    {
        case 1:
            return Game::FRLG; // fixed value
        case 0:
            return Game::RS; // no battle tower record data
        default:
            // Ruby doesn't set data as far down as Emerald.
            // 00 FF 00 00 00 00 00 00 00 FF 00 00 00 00 00 00
            // ^ byte pattern in Emerald saves, is all zero in Ruby/Sapphire as far as I can tell.
            // Some saves have had data @ 0x550
            if (Endian::convertTo<u64>(&dt[blockOfs0 + 0xEE0]) != 0)
                return Game::E;
            if (Endian::convertTo<u64>(&dt[blockOfs0 + 0xEE8]) != 0)
                return Game::E;
            return Game::RS;
    }
}

Sav3::Sav3(std::shared_ptr<u8[]> dt) : Sav(dt, 0x20000)
{
    loadBlocks();

    // Japanese games are limited to 5 character OT names; any unused characters are 0xFF.
    // 5 for JP, 7 for INT. There's always 1 terminator, thus we can check 0x6-0x7 being 0xFFFF = INT
    // OT name is stored at the top of the first block.
    japanese = Endian::convertTo<s16>(&data[blockOfs[0] + 0x6]) == 0;

    PokeDex = blockOfs[0] + 0x18;

    initialize();
}

void Sav3::initialize(void)
{
    // Set up PC data buffer
    Box = std::shared_ptr<u8[]>(new u8[SIZE_RESERVED]);

    // Copy chunk to the allocated location
    for (int i = 5; i < BLOCK_COUNT; i++)
    {
        unsigned int blockIndex = std::find(blockOrder.begin(), blockOrder.end(), i) - blockOrder.begin();
        if (blockIndex == blockOrder.size()) // block empty
            continue;
        memcpy(Box.get() + ((i - 5) * 0xF80), data.get() + (blockIndex * SIZE_BLOCK) + ABO(), chunkLength[i]);
    }

    // LoadEReaderBerryData();

    // Sanity Check SeenFlagOffsets -- early saves may not have block 4 initialized yet
    std::vector<int> seenFlagOffsetsTemp;
    for (auto seenFlagOffset : seenFlagOffsets)
    {
        if (seenFlagOffset >= 0)
            seenFlagOffsetsTemp.push_back(seenFlagOffset);
    }
    seenFlagOffsets = seenFlagOffsetsTemp;
}

void Sav3::encrypt(void)
{
    // Copy Box data back
    for (int i = 5; i < BLOCK_COUNT; i++)
    {
        unsigned int blockIndex = std::find(blockOrder.begin(), blockOrder.end(), i) - blockOrder.begin();
        if (blockIndex == blockOrder.size()) // block empty
            continue;
        memcpy(data.get() + (blockIndex * SIZE_BLOCK) + ABO(), Box.get() + ((i - 5) * 0xF80), chunkLength[i]);
    }

    resign();
}

// TODO: Maybe move this elsewhere?
const u16 Sav3::CRC32(u8* dt, int start, int length)
{
    u32 val = 0;
    for (int i = start; i < start + length; i += 4)
        val += Endian::convertTo<u32>(&dt[i]);
    return (u16)(val + (val >> 16));
}

void Sav3::resign(void)
{
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        int ofs   = ABO() + (i * SIZE_BLOCK);
        int index = blockOrder[i];
        if (index == -1)
            continue;
        int len = chunkLength[index];
        u16 chk = CRC32(data.get(), ofs, len);
        Endian::convertFrom<u16>(&data[ofs + 0xFF6], chk);
    }

    // Hall of Fame Checksums
    {
        u16 chk = CRC32(data.get(), 0x1C000, SIZE_BLOCK_USED);
        Endian::convertFrom<u16>(&data[0x1CFF4], chk);
    }
    {
        u16 chk = CRC32(data.get(), 0x1D000, SIZE_BLOCK_USED);
        Endian::convertFrom<u16>(&data[0x1DFF4], chk);
    }
}

u32 Sav3::securityKey(void) const
{
    switch (game)
    {
        case Game::E:
            return Endian::convertTo<u32>(&data[blockOfs[0] + 0xAC]);
        case Game::FRLG:
            return Endian::convertTo<u32>(&data[blockOfs[0] + 0xF20]);
        default:
            return 0u;
    }
}

u16 Sav3::TID(void) const
{
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0xA]);
}
void Sav3::TID(u16 v)
{
    Endian::convertFrom<u16>(&data[blockOfs[0] + 0xA], v);
}

u16 Sav3::SID(void) const
{
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0xC]);
}
void Sav3::SID(u16 v)
{
    Endian::convertFrom<u16>(&data[blockOfs[0] + 0xC], v);
}

u8 Sav3::version(void) const
{
    return game == Game::RS ? 1 : game == Game::E ? 3 : 4;
}
void Sav3::version(u8 v)
{
    (void)v;
}

u8 Sav3::gender(void) const
{
    return data[blockOfs[0] + 8];
}
void Sav3::gender(u8 v)
{
    data[blockOfs[0] + 8] = v;
}

u8 Sav3::subRegion(void) const
{
    return 0;
}
void Sav3::subRegion(u8 v)
{
    (void)v;
}

u8 Sav3::country(void) const
{
    return 0;
}
void Sav3::country(u8 v)
{
    (void)v;
}

u8 Sav3::consoleRegion(void) const
{
    return 0;
}
void Sav3::consoleRegion(u8 v)
{
    (void)v;
}

Language Sav3::language(void) const
{
    // TODO: Other languages? Is this unused?
    return japanese ? Language::JPN : Language::ENG;
}
void Sav3::language(Language v)
{
    // TODO: Unused?
}

std::string Sav3::otName(void) const
{
    return StringUtils::getString3(data.get(), blockOfs[0], japanese ? 5 : 7, japanese);
}
void Sav3::otName(const std::string& v)
{
    StringUtils::setString3(data.get(), v, blockOfs[0], japanese ? 5 : 7, japanese, japanese ? 5 : 7, 0xFF);
}

u32 Sav3::money(void) const
{
    switch (game)
    {
        case Game::RS:
        case Game::E:
            return Endian::convertTo<u32>(&data[blockOfs[1] + 0x490]) ^ securityKey();
        case Game::FRLG:
            return Endian::convertTo<u32>(&data[blockOfs[1] + 0x290]) ^ securityKey();
        default:
            return int(game);
    }
}
void Sav3::money(u32 v)
{
    switch (game)
    {
        case Game::RS:
        case Game::E:
            Endian::convertFrom<u32>(&data[blockOfs[1] + 0x0490], v ^ securityKey());
        case Game::FRLG:
            Endian::convertFrom<u32>(&data[blockOfs[1] + 0x0290], v ^ securityKey());
        default:
            break;
    }
}

// TODO:? Coins

u32 Sav3::BP(void) const
{
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0xEB8]);
}
void Sav3::BP(u32 v)
{
    if (v > 9999)
    {
        v = 9999;
    }

    Endian::convertFrom<u16>(&data[blockOfs[0] + 0xEB8], v);
}

// TODO:? BPEarned

// TODO:? BerryPowder

bool Sav3::getEventFlag(int flagNumber) const
{
    if (flagNumber >= 8 * (game == Game::E ? 300 : 288))
        return 0;

    int start = eventFlag;
    if (game == Game::FRLG && flagNumber >= 0x500)
    {
        flagNumber -= 0x500;
        start = blockOfs[2];
    }
    return FlagUtil::getFlag(data.get(), start + (flagNumber >> 3), flagNumber & 7);
}

void Sav3::setEventFlag(int flagNumber, bool value)
{
    if (flagNumber >= 8 * (game == Game::E ? 300 : 288))
        return;

    int start = eventFlag;
    if (game == Game::FRLG && flagNumber >= 0x500)
    {
        flagNumber -= 0x500;
        start = blockOfs[2];
    }
    FlagUtil::setFlag(data.get(), start + (flagNumber >> 3), flagNumber & 7, value);
}

u8 Sav3::badges(void) const
{
    int startFlag = 0;
    switch (game)
    {
        case Game::FRLG:
            startFlag = 0x820;
            break;
        case Game::RS:
            startFlag = 0x807;
            break;
        case Game::E:
            startFlag = 0x867;
        default:
            break;
    }

    int ret = 0;
    for (int i = 0; i < 8; i++)
    {
        if (getEventFlag(startFlag + i))
            ret++;
    }

    return ret;
}

u16 Sav3::playedHours(void) const
{
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0xE]);
}
void Sav3::playedHours(u16 v)
{
    Endian::convertFrom<u16>(&data[blockOfs[0] + 0xE], v);
}

u8 Sav3::playedMinutes(void) const
{
    return data[blockOfs[0] + 0x10];
}
void Sav3::playedMinutes(u8 v)
{
    data[blockOfs[0] + 0x10] = v;
}

u8 Sav3::playedSeconds(void) const
{
    return data[blockOfs[0] + 0x11];
}
void Sav3::playedSeconds(u8 v)
{
    data[blockOfs[0] + 0x11] = v;
}

// TODO:? playedFrames, u8 at 0x12

u8 Sav3::currentBox(void) const
{
    return Box[0];
}
void Sav3::currentBox(u8 v)
{
    Box[0] = v;
}

u32 Sav3::boxOffset(u8 box, u8 slot) const
{
    return 4 + (SIZE_STORED * box * 30) + (SIZE_STORED * slot);
}

u32 Sav3::partyOffset(u8 slot) const
{
    return blockOfs[1] + (game == Game::FRLG ? 0x38 : 0x238) + (SIZE_PARTY * slot);
}

std::shared_ptr<PKX> Sav3::pkm(u8 slot) const
{
    return std::make_shared<PK3>(&data[partyOffset(slot)], true);
}
std::shared_ptr<PKX> Sav3::pkm(u8 box, u8 slot) const
{
    return std::make_shared<PK3>(&Box[boxOffset(box, slot)]);
}

void Sav3::pkm(std::shared_ptr<PKX> pk, u8 slot)
{
    if (pk->generation() == Generation::THREE)
    {
        u8 buf[SIZE_PARTY] = {0};
        std::copy(pk->rawData(), pk->rawData() + pk->getLength(), buf);
        std::unique_ptr<PK3> pk3 = std::make_unique<PK3>(buf, true, true);

        if (pk->getLength() != SIZE_PARTY)
        {
            for (int i = 0; i < 6; i++)
            {
                pk3->partyStat(Stat(i), pk3->stat(Stat(i)));
            }
            pk3->partyLevel(pk3->level());
            pk3->partyCurrHP(pk3->stat(Stat::HP));
        }

        pk3->encrypt();
        std::copy(pk3->rawData(), pk3->rawData() + pk3->getLength(), &data[partyOffset(slot)]);
    }
}
void Sav3::pkm(std::shared_ptr<PKX> pk, u8 box, u8 slot, bool applyTrade)
{
    if (pk->generation() == Generation::THREE)
    {
        if (applyTrade)
        {
            trade(pk);
        }

        std::copy(pk->rawData(), pk->rawData() + SIZE_STORED, &Box[boxOffset(box, slot)]);
    }
}

void Sav3::trade(std::shared_ptr<PKX> pk)
{
    (void)pk;
}

std::shared_ptr<PKX> Sav3::emptyPkm() const
{
    return std::make_shared<PK3>();
}

bool Sav3::canSetDex(int species)
{
    if (species <= 0)
        return false;
    if (species > maxSpecies())
        return false;
    if (std::find_if(blockOfs.begin(), blockOfs.end(), [](const int& val) { return val < 0; }) != blockOfs.end())
        return false;
    return true;
}

u32 Sav3::dexPIDUnown(void)
{
    return Endian::convertTo<u32>(&data[PokeDex + 0x4]);
}
void Sav3::dexPIDUnown(u32 v)
{
    Endian::convertFrom<u32>(&data[PokeDex + 0x4], v);
}

u32 Sav3::dexPIDSpinda(void)
{
    return Endian::convertTo<u32>(&data[PokeDex + 0x8]);
}
void Sav3::dexPIDSpinda(u32 v)
{
    Endian::convertFrom<u32>(&data[PokeDex + 0x8], v);
}

void Sav3::dex(std::shared_ptr<PKX> pk)
{
    int species = pk->species();
    if (!canSetDex(species))
        return;

    switch (species)
    {
        case 201: // Unown
            if (!getSeen(species))
                dexPIDUnown(pk->PID());
            break;
        case 327: // Spinda
            if (!getSeen(species))
                dexPIDSpinda(pk->PID());
            break;
    }
    setCaught(species, true);
    setSeen(species, true);
}

bool Sav3::getCaught(int species) const
{
    int bit          = species - 1;
    int ofs          = bit >> 3;
    int caughtOffset = PokeDex + 0x10;
    return FlagUtil::getFlag(data.get(), caughtOffset + ofs, bit & 7);
}

void Sav3::setCaught(int species, bool caught)
{
    int bit          = species - 1;
    int ofs          = bit >> 3;
    int caughtOffset = PokeDex + 0x10;
    FlagUtil::setFlag(data.get(), caughtOffset + ofs, bit & 7, caught);
}

bool Sav3::getSeen(int species) const
{
    int bit        = species - 1;
    int ofs        = bit >> 3;
    int seenOffset = PokeDex + 0x44;
    return FlagUtil::getFlag(data.get(), seenOffset + ofs, bit & 7);
}

void Sav3::setSeen(int species, bool seen)
{
    int bit = species - 1;
    int ofs = bit >> 3;

    for (int o : seenFlagOffsets)
        FlagUtil::setFlag(data.get(), o + ofs, bit & 7, seen);
}

int Sav3::dexSeen(void) const
{
    int ret = 0;
    for (int i = 1; i < maxSpecies(); i++)
    {
        if (getSeen(i))
            ret++;
    }

    return ret;
}

int Sav3::dexCaught(void) const
{
    int ret = 0;
    for (int i = 1; i < maxSpecies(); i++)
    {
        if (getCaught(i))
            ret++;
    }

    return ret;
}

// Unused
int Sav3::emptyGiftLocation(void) const
{
    return 0;
}

// Unused
std::vector<Sav::giftData> Sav3::currentGifts(void) const
{
    return std::vector<giftData>();
}

// Unused
void Sav3::mysteryGift(WCX& wc, int& pos) {}

// Unused
std::unique_ptr<WCX> Sav3::mysteryGift(int pos) const
{
    return nullptr;
}

void Sav3::cryptBoxData(bool crypted)
{
    for (u8 box = 0; box < maxBoxes(); box++)
    {
        for (u8 slot = 0; slot < 30; slot++)
        {
            std::unique_ptr<PKX> pk3 = std::make_unique<PK3>(&Box[boxOffset(box, slot)], false, true);
            if (!crypted)
            {
                pk3->encrypt();
            }
        }
    }
}

std::string Sav3::boxName(u8 box) const
{
    return StringUtils::getString3(Box.get(), boxOffset(maxBoxes(), 0) + (box * 9), 9, japanese);
}
void Sav3::boxName(u8 box, const std::string& v)
{
    return StringUtils::setString3(Box.get(), v, boxOffset(maxBoxes(), 0) + (box * 9), 8, japanese, 9);
}

// Note: This is needed for pkmn-chest, but not in PKSM's core currently
u8 Sav3::boxWallpaper(u8 box) const
{
    int offset = boxOffset(maxBoxes(), 0);
    offset += (maxBoxes() * 0x9) + box;
    return Box[offset];
}
void Sav3::boxWallpaper(u8 box, u8 v)
{
    int offset = boxOffset(maxBoxes(), 0);
    offset += (maxBoxes() * 0x9) + box;
    Box[offset] = v;
}

u8 Sav3::partyCount(void) const
{
    return data[blockOfs[1] + (game == Game::FRLG ? 0x34 : 0x234)];
}
void Sav3::partyCount(u8 v)
{
    data[blockOfs[1] + (game == Game::FRLG ? 0x34 : 0x234)] = v;
}

const std::set<int>& Sav3::availableItems(void) const
{
    if (items.empty())
    {
        fill_set(items, 0, maxItem());
    }
    return items;
}

const std::set<int>& Sav3::availableMoves(void) const
{
    if (moves.empty())
    {
        fill_set(moves, 0, maxMove());
    }
    return moves;
}

const std::set<int>& Sav3::availableSpecies(void) const
{
    if (species.empty())
    {
        fill_set(species, 1, maxSpecies());
    }
    return species;
}

const std::set<int>& Sav3::availableAbilities(void) const
{
    if (abilities.empty())
    {
        fill_set(abilities, 1, maxAbility());
    }
    return abilities;
}

const std::set<int>& Sav3::availableBalls(void) const
{
    if (balls.empty())
    {
        fill_set(balls, 1, maxBall());
    }
    return balls;
}

void Sav3::item(const Item& item, Pouch pouch, u16 slot)
{
    Item3 inject = (Item3)item;
    if (pouch != Pouch::PCItem)
        inject.count(inject.count() ^ (u16)securityKey());
    auto write = inject.bytes();
    switch (pouch)
    {
        case NormalItem:
            std::copy(write.begin(), write.end(), &data[OFS_PouchHeldItem + (slot * 4)]);
            break;
        case KeyItem:
            std::copy(write.begin(), write.end(), &data[OFS_PouchKeyItem + (slot * 4)]);
            break;
        case Ball:
            std::copy(write.begin(), write.end(), &data[OFS_PouchBalls + (slot * 4)]);
            break;
        case TM:
            std::copy(write.begin(), write.end(), &data[OFS_PouchTMHM + (slot * 4)]);
            break;
        case Berry:
            std::copy(write.begin(), write.end(), &data[OFS_PouchBerry + (slot * 4)]);
            break;
        case PCItem:
            std::copy(write.begin(), write.end(), &data[OFS_PCItem + (slot * 4)]);
            break;
        default:
            return;
    }
}
std::unique_ptr<Item> Sav3::item(Pouch pouch, u16 slot) const
{
    switch (pouch)
    {
        case NormalItem:
            return std::make_unique<Item3>(&data[OFS_PouchHeldItem + (slot * 4)], securityKey());
        case KeyItem:
            return std::make_unique<Item3>(&data[OFS_PouchKeyItem + (slot * 4)], securityKey());
        case Ball:
            return std::make_unique<Item3>(&data[OFS_PouchBalls + (slot * 4)], securityKey());
        case TM:
            return std::make_unique<Item3>(&data[OFS_PouchTMHM + (slot * 4)], securityKey());
        case Berry:
            return std::make_unique<Item3>(&data[OFS_PouchBerry + (slot * 4)], securityKey());
        case PCItem:
            return std::make_unique<Item3>(&data[OFS_PCItem + (slot * 4)]);
        default:
            return nullptr;
    }
}

std::vector<std::pair<Sav::Pouch, int>> Sav3::pouches(void) const
{
    // TODO: Is this the right thing for the int?
    return {{NormalItem, (OFS_PouchKeyItem - OFS_PouchHeldItem) / 4}, {KeyItem, (OFS_PouchBalls - OFS_PouchKeyItem) / 4},
        {Ball, (OFS_PouchTMHM - OFS_PouchBalls) / 4}, {TM, (OFS_PouchBerry - OFS_PouchTMHM) / 4}, {Berry, game == Game::FRLG ? 43 : 46},
        {PCItem, (OFS_PouchHeldItem - OFS_PCItem) / 4}};
}

std::string Sav3::pouchName(Language lang, Pouch pouch) const
{
    switch (pouch)
    {
        case NormalItem:
            return i18n::localize(lang, "ITEMS");
        case KeyItem:
            return i18n::localize(lang, "KEY_ITEMS");
        case Ball:
            return i18n::localize(lang, "BALLS");
        case TM:
            return i18n::localize(lang, "TMHM");
        case Berry:
            return i18n::localize(lang, "BERRIES");
        case PCItem:
            return i18n::localize(lang, "PC_ITEMS");
        default:
            return "";
    }
}

u16 Sav3::rtcInitialDay(void) const
{
    if (game == Game::FRLG)
        return 0;
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0x98]);
}
void Sav3::rtcInitialDay(u16 v)
{
    if (game == Game::FRLG)
        return;
    Endian::convertFrom<u16>(&data[blockOfs[0] + 0x98], v);
}

u8 Sav3::rtcInitialHour(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0x98 + 2];
}
void Sav3::rtcInitialHour(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0x98 + 2] = v;
}

u8 Sav3::rtcInitialMinute(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0x98 + 3];
}
void Sav3::rtcInitialMinute(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0x98 + 3] = v;
}

u8 Sav3::rtcInitialSecond(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0x98 + 4];
}
void Sav3::rtcInitialSecond(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0x98 + 4] = v;
}

u16 Sav3::rtcElapsedDay(void) const
{
    if (game == Game::FRLG)
        return 0;
    return Endian::convertTo<u16>(&data[blockOfs[0] + 0xA0]);
}
void Sav3::rtcElapsedDay(u16 v)
{
    if (game == Game::FRLG)
        return;
    Endian::convertFrom<u16>(&data[blockOfs[0] + 0xA0], v);
}

u8 Sav3::rtcElapsedHour(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0xA0 + 2];
}
void Sav3::rtcElapsedHour(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0xA0 + 2] = v;
}

u8 Sav3::rtcElapsedMinute(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0xA0 + 3];
}
void Sav3::rtcElapsedMinute(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0xA0 + 3] = v;
}

u8 Sav3::rtcElapsedSecond(void) const
{
    if (game == Game::FRLG)
        return 0;
    return data[blockOfs[0] + 0xA0 + 4];
}
void Sav3::rtcElapsedSecond(u8 v)
{
    if (game == Game::FRLG)
        return;
    data[blockOfs[0] + 0xA0 + 4] = v;
}
