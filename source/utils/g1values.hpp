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

#ifndef G1VALUES_HPP
#define G1VALUES_HPP

#include "utils/coretypes.h"
#include <array>

namespace pksm::internal
{
    // index is Pokedex species (= modern species), value is internal species
    constexpr std::array<u8, 152> speciesToG1 = {0, 153, 9, 154, 176, 178, 180, 177, 179, 28, 123,
        124, 125, 112, 113, 114, 36, 150, 151, 165, 166, 5, 35, 108, 45, 84, 85, 96, 97, 15, 168,
        16, 3, 167, 7, 4, 142, 82, 83, 100, 101, 107, 130, 185, 186, 187, 109, 46, 65, 119, 59, 118,
        77, 144, 47, 128, 57, 117, 33, 20, 71, 110, 111, 148, 38, 149, 106, 41, 126, 188, 189, 190,
        24, 155, 169, 39, 49, 163, 164, 37, 8, 173, 54, 64, 70, 116, 58, 120, 13, 136, 23, 139, 25,
        147, 14, 34, 48, 129, 78, 138, 6, 141, 12, 10, 17, 145, 43, 44, 11, 55, 143, 18, 1, 40, 30,
        2, 92, 93, 157, 158, 27, 152, 42, 26, 72, 53, 51, 29, 60, 133, 22, 19, 76, 102, 105, 104,
        103, 170, 98, 99, 90, 91, 171, 132, 74, 75, 73, 88, 89, 66, 131, 21};

    // index is internal species, value is Pokedex species (= modern species)
    constexpr std::array<u8, 191> g1ToSpecies = {0, 112, 115, 32, 35, 21, 100, 34, 80, 2, 103, 108,
        102, 88, 94, 29, 31, 104, 111, 131, 59, 151, 130, 90, 72, 92, 123, 120, 9, 127, 114, 0, 0,
        58, 95, 22, 16, 79, 64, 75, 113, 67, 122, 106, 107, 24, 47, 54, 96, 76, 0, 126, 0, 125, 82,
        109, 0, 56, 86, 50, 128, 0, 0, 0, 83, 48, 149, 0, 0, 0, 84, 60, 124, 146, 144, 145, 132, 52,
        98, 0, 0, 0, 37, 38, 25, 26, 0, 0, 147, 148, 140, 141, 116, 117, 0, 0, 27, 28, 138, 139, 39,
        40, 133, 136, 135, 134, 66, 41, 23, 46, 61, 62, 13, 14, 15, 0, 85, 57, 51, 49, 87, 0, 0, 10,
        11, 12, 68, 0, 55, 97, 42, 150, 143, 129, 0, 0, 89, 0, 99, 91, 0, 101, 36, 110, 53, 105, 0,
        93, 63, 65, 17, 18, 121, 1, 3, 73, 0, 118, 119, 0, 0, 0, 0, 77, 78, 19, 20, 33, 30, 74, 137,
        142, 0, 81, 0, 0, 4, 7, 5, 8, 6, 0, 0, 0, 0, 43, 44, 45, 69, 70, 71};

    // index is Gen I item, value is modern item
    constexpr std::array<u16, 256> g1ToItem = {0, 1, 2, 3, 4, 442, 450, 128, 5, 128 /*Pokedex*/, 81,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 17, 128, 128, 128, 128, 128, 128, 128, 128 /*badges*/,
        78, 79, 103, 82, 83, 84, 45, 46, 47, 48, 49, 50, 102, 101, 872, 128,
        128 /*Bicycle Voucher*/, 60, 85, 876, 92, 128 /*dysfunctional "PP Up"*/, 63, 27, 28, 29, 55,
        76, 77, 56, 128 /*Coin*/, 30, 31, 32, 873, 877, 57, 58, 59, 61 /*X Special*/, 444, 875,
        471 /*Itemfinder*/, 874, 651, 878, 216 /*Exp. All*/, 445, 446, 447, 51, 38, 39, 40, 41, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
        128, 128, 128 /*gap*/, 420, 421, 422, 423, 424, 328, 329, 330, 331, 332, 333, 334, 335, 336,
        337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354,
        355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372,
        373, 374, 375, 376, 377, 378, 379, 380, 381, 382};
}

#endif
