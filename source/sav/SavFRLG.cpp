/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2020 Bernardo Giordano, Admiral Fish, piepie62, Pk11
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

#include "sav/SavFRLG.hpp"

SavFRLG::SavFRLG(std::shared_ptr<u8[]> dt) : Sav3(dt)
{
    game = Game::FRLG;

    seenFlagOffsets = std::vector<int>({ PokeDex + 0x44, blockOfs[1] + 0x5F8, blockOfs[4] + 0xB98 });

    OFS_PCItem = blockOfs[1] + 0x0298;
    OFS_PouchHeldItem = blockOfs[1] + 0x0310;
    OFS_PouchKeyItem = blockOfs[1] + 0x03B8;
    OFS_PouchBalls = blockOfs[1] + 0x0430;
    OFS_PouchTMHM = blockOfs[1] + 0x0464;
    OFS_PouchBerry = blockOfs[1] + 0x054C;
    eventFlag = blockOfs[1] + 0xEE0;
    // EventConst = blockOfs[2] + 0x80;
    // DaycareOffset = blockOfs[4] + 0x100;
}

std::map<Sav::Pouch, std::vector<int>> SavFRLG::validItems() const
{
    std::map<Sav::Pouch, std::vector<int>> items = {
        {NormalItem, {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 65, 66, 67, 68, 69, 43, 44, 70, 71, 72, 73, 74, 75, 45, 46, 47, 48, 49, 50, 51, 52, 53, 55, 56, 57, 58, 59, 60, 61, 63, 64, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264}},
        {Ball, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {KeyItem, {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}},
        {TM, {328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377}},
        {Berry, {149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 201, 202, 203, 204, 205, 206, 207, 208}},
        {PCItem, {}}
    };

    // PC can hold any item
    items[PCItem].insert(items[PCItem].end(), items[NormalItem].begin(), items[NormalItem].end());
    items[PCItem].insert(items[PCItem].end(), items[Ball].begin(), items[Ball].end());
    items[PCItem].insert(items[PCItem].end(), items[KeyItem].begin(), items[KeyItem].end());
    items[PCItem].insert(items[PCItem].end(), items[TM].begin(), items[TM].end());
    items[PCItem].insert(items[PCItem].end(), items[Berry].begin(), items[Berry].end());

    return items;
}
