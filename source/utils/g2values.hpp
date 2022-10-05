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

#ifndef G2VALUES_HPP
#define G2VALUES_HPP

#include "utils/coretypes.h"
#include <array>

namespace pksm::internal
{
    // index is Gen II item, value is modern item
    // unlabeled gaps are Teru-sama, a dummy item
    constexpr std::array<u16, 256> g2ToItem = {0, 1, 2, 213, 3, 4, 128, 450, 81, 18, 19, 20, 21, 22,
        23, 24, 25, 26, 17, 78, 79, 41, 82, 83, 84, 128, 45, 46, 47, 48, 256, 49, 50, 60, 85, 257,
        92, 63, 27, 28, 29, 55, 76, 77, 56, 128, 30, 31, 32, 57, 128, 58, 59, 61, 444, 128, 128,
        216, 445, 446, 891, 447, 51, 38, 39, 40, 478, 464, 456, 484, 474, 482, 33, 217, 151, 890,
        237, 244, 149, 153, 152, 245, 221, 156, 150, 485, 86, 87, 222, 487, 128, 223, 486, 488, 224,
        243, 248, 490, 241, 491, 128, 489, 240, 473, 128 /*Pink Bow*/, 259, 228, 246, 242, 157, 88,
        89, 229, 247, 504, 128 /*GS Ball*/, 472, 239, 258, 230, 128, 34, 35, 36, 37, 238, 231, 475,
        481, 128 /*Egg Ticket*/, 479, 90, 91, 476, 480, 128, 128, 128, 249, 43, 232, 128, 128, 233,
        250, 128, 234, 128, 128, 128, 154, 235, 128 /*Berserk Gene*/, 128, 128, 128, 44, 495,
        128 /*Flower Mail*/, 493, 494, 492, 128, 236, 497, 498, 496, 128 /*Normal Box*/,
        128 /*Gorgeous Box*/, 80, 128 /*Polkadot Bow*/, 128, 252, 155, 158, 477, 128, 500, 483, 128,
        128 /*Brick Piece*/, 128 /*Surf Mail*/, 128 /*Litebluemail*/, 128 /*Portraitmail*/,
        128 /*Lovely Mail*/, 128 /*Eon Mail*/, 128 /*Morph Mail*/, 128 /*Bluesky Mail*/,
        128 /*Music Mail*/, 128 /*Mirage Mail*/, 128, 328, 329, 330, 331, 331, 332, 333, 334, 335,
        336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353,
        354, 355, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370,
        371, 372, 373, 374, 375, 376, 377, 420, 421, 422, 423, 424, 425, 426, 427, 128 /*HM09*/,
        128 /*HM10*/, 128 /*HM11*/, 128 /*HM12*/, 128 /*Cancel*/};
}

#endif
