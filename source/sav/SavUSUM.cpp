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

#include "sav/SavUSUM.hpp"
#include "memecrypto.h"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include <algorithm>

namespace pksm
{
    SavUSUM::SavUSUM(const std::shared_ptr<u8[]>& dt) : Sav7(dt, 0x6CC00)
    {
        game = Game::USUM;

        TrainerCard          = 0x1400;
        Misc                 = 0x4400;
        PlayTime             = 0x41000;
        LastViewedBox        = 0x51E3;
        Box                  = 0x5200;
        Party                = 0x1600;
        PokeDex              = 0x2C00;
        PokeDexLanguageFlags = 0x3150;
        WondercardFlags      = 0x66200;
        WondercardData       = 0x66300;
        PCLayout             = 0x4C00;
        PouchHeldItem        = 0x0;
        PouchKeyItem         = 0x6AC;
        PouchTMHM            = 0x9C4;
        PouchMedicine        = 0xB74;
        PouchBerry           = 0xC64;
        PouchZCrystals       = 0xD70;
        BattleItems          = 0xDFC;
    }

    void SavUSUM::resign(void)
    {
        static constexpr u8 blockCount = 39;
        static constexpr u32 csoff     = 0x6CA1A;

        for (u8 i = 0; i < blockCount; i++)
        {
            // Clear memecrypto data
            if (LittleEndian::convertTo<u16>(&data[csoff + i * 8 - 2]) == 36)
            {
                std::fill_n(&data[chkofs[i] + 0x100], 0x80, 0);
            }
            LittleEndian::convertFrom<u16>(
                &data[csoff + i * 8], pksm::crypto::crc16({&data[chkofs[i]], chklen[i]}));
        }

        const u32 checksumTableOffset = 0x6CA00;
        const u32 checksumTableLength = 0x150;
        const u32 memecryptoOffset    = 0x6C100;

        auto hash = crypto::sha256({&data[checksumTableOffset], checksumTableLength});

        u8 decryptedSignature[0x80];
        reverseCrypt(&data[memecryptoOffset], decryptedSignature);
        std::copy(hash.begin(), hash.end(), decryptedSignature);

        memecrypto_sign(decryptedSignature, &data[memecryptoOffset], 0x80);
    }

    int SavUSUM::dexFormIndex(int species, int formct, int start) const
    {
        int formindex = start;
        int f         = 0;
        for (u8 i = 0; i < 246; i += 2)
        {
            if (formtable[i] == species)
            {
                break;
            }
            f         = formtable[i + 1];
            formindex += f - 1;
        }
        return f > formct ? -1 : formindex;
    }

    int SavUSUM::dexFormCount(int species) const
    {
        for (u8 i = 0; i < 246; i += 2)
        {
            if (formtable[i] == species)
            {
                return formtable[i + 1];
            }
        }
        return 0;
    }

    SmallVector<std::pair<Sav::Pouch, std::span<const int>>, 15> SavUSUM::validItems() const
    {
        static constexpr std::array NormalItem = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            16, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
            79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 99, 100, 101, 102, 103,
            104, 105, 106, 107, 108, 109, 110, 111, 112, 116, 117, 118, 119, 135, 136, 137, 213,
            214, 215, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
            232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
            249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265,
            266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282,
            283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
            300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316,
            317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 492, 493, 494, 495, 496, 497,
            498, 499, 534, 535, 537, 538, 539, 540, 541, 542, 543, 544, 545, 546, 547, 548, 549,
            550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561, 562, 563, 564, 571, 572,
            573, 576, 577, 580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 639, 640, 644,
            646, 647, 648, 649, 650, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667,
            668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684,
            685, 699, 704, 710, 711, 715, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762,
            763, 764, 767, 768, 769, 770, 795, 796, 844, 846, 849, 851, 853, 854, 855, 856, 879,
            880, 881, 882, 883, 884, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915,
            916, 917, 918, 919, 920};
        static constexpr std::array KeyItem    = {216, 440, 465, 466, 628, 629, 631, 632, 638, 705,
               706, 765, 773, 797, 841, 842, 843, 845, 847, 850, 857, 858, 860, 933, 934, 935, 936,
               937, 938, 939, 940, 941, 942, 943, 944, 945, 946, 947, 948};
        static constexpr std::array TM = {328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338,
            339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355,
            356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372,
            373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389,
            390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406,
            407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 618, 619, 620, 690,
            691, 692, 693, 694};
        static constexpr std::array Medicine = {17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
            52, 53, 54, 65, 66, 67, 134, 504, 565, 566, 567, 568, 569, 570, 591, 645, 708, 709,
            852};
        static constexpr std::array Berry = {149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
            160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
            177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193,
            194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
            211, 212, 686, 687, 688};
        static constexpr std::array ZCrystals  = {807, 808, 809, 810, 811, 812, 813, 814, 815, 816,
             817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833,
             834, 835, 927, 928, 929, 930, 931, 932};
        static constexpr std::array RotomPower = {
            949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959};

        return {
            std::pair{Pouch::NormalItem, std::span{NormalItem}},
            std::pair{Pouch::KeyItem,    std::span{KeyItem}   },
            std::pair{Pouch::TM,         std::span{TM}        },
            std::pair{Pouch::Medicine,   std::span{Medicine}  },
            std::pair{Pouch::Berry,      std::span{Berry}     },
            std::pair{Pouch::ZCrystals,  std::span{ZCrystals} },
            std::pair{Pouch::RotomPower, std::span{RotomPower}}
        };
    }
}
