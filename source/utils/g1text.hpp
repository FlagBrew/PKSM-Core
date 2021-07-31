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

#ifndef G1TEXT_HPP
#define G1TEXT_HPP

#include "utils/coretypes.h"
#include <map>

namespace pksm::internal
{
    char16_t tradeOTChar = u'ඞ'; // a character not expected to appear in native names, maybe it'll surprise a joker ;P

    std::map<u8, char16_t> G1_EN = {{0x50, u'\0'}, {0x5D, tradeOTChar},
        {0x7F, u' '}, {0x80, u'A'}, {0x81, u'B'}, {0x82, u'C'}, {0x83, u'D'}, {0x84, u'E'}, {0x85, u'F'},
        {0x86, u'G'}, {0x87, u'H'}, {0x88, u'I'}, {0x89, u'J'}, {0x8A, u'K'}, {0x8B, u'L'}, {0x8C, u'M'},
        {0x8D, u'N'}, {0x8E, u'O'}, {0x8F, u'P'}, {0x90, u'Q'}, {0x91, u'R'}, {0x92, u'S'}, {0x93, u'T'},
        {0x94, u'U'}, {0x95, u'V'}, {0x96, u'W'}, {0x97, u'X'}, {0x98, u'Y'}, {0x99, u'Z'}, {0x9A, u'('},
        {0x9B, u')'}, {0x9C, u':'}, {0x9D, u';'}, {0x9E, u'['}, {0x9F, u']'}, {0xA0, u'a'}, {0xA1, u'b'},
        {0xA2, u'c'}, {0xA3, u'd'}, {0xA4, u'e'}, {0xA5, u'f'}, {0xA6, u'g'}, {0xA7, u'h'}, {0xA8, u'i'},
        {0xA9, u'j'}, {0xAA, u'k'}, {0xAB, u'l'}, {0xAC, u'm'}, {0xAD, u'n'}, {0xAE, u'o'}, {0xAF, u'p'},
        {0xB0, u'q'}, {0xB1, u'r'}, {0xB2, u's'}, {0xB3, u't'}, {0xB4, u'u'}, {0xB5, u'v'}, {0xB6, u'w'},
        {0xB7, u'x'}, {0xB8, u'y'}, {0xB9, u'z'},

        // unused characters
        {0xBA, u'à'}, {0xBB, u'è'}, {0xBC, u'é'}, {0xBD, u'ù'}, {0xBE, u'À'},
        {0xBF, u'Á'}, // Used in Spanish FALCÁN in-game trade, inaccessible from keyboard

        {0xC0, u'Ä'}, {0xC1, u'Ö'}, {0xC2, u'Ü'}, {0xC3, u'ä'}, {0xC4, u'ö'}, {0xC5, u'ü'},

        // unused characters
        {0xC6, u'È'}, {0xC7, u'É'}, {0xC8, u'Ì'},
        {0xC9, u'Í'}, // Used in Spanish MANÍA in-game trade, inaccessible from keyboard
        {0xCA, u'Ñ'}, {0xCB, u'Ò'}, {0xCC, u'Ó'}, {0xCD, u'Ù'}, {0xCE, u'Ú'}, {0xCF, u'á'}, {0xD0, u'ì'},
        {0xD1, u'í'}, {0xD2, u'ñ'}, {0xD3, u'ò'}, {0xD4, u'ó'}, {0xD5, u'ú'},

        {0xE0, u'’'}, {0xE1, u'{'},                           /* Pk */
        {0xE2, u'}'},                                         /* Mn */
        {0xE3, u'-'}, {0xE6, u'?'}, {0xE7, u'!'}, {0xE8, u'.'}, // Alias decimal point to .
        {0xEF, u'♂'}, {0xF1, u'×'}, {0xF2, u'.'}, {0xF3, u'/'}, {0xF4, u','}, {0xF5, u'♀'}, {0xF6, u'0'},
        {0xF7, u'1'}, {0xF8, u'2'}, {0xF9, u'3'}, {0xFA, u'4'}, {0xFB, u'5'}, {0xFC, u'6'}, {0xFD, u'7'},
        {0xFE, u'8'}, {0xFF, u'9'}};

    std::map<u8, char16_t> G1_JP = {{0x05, u'ガ'}, {0x06, u'ギ'}, {0x07, u'グ'},
        {0x08, u'ゲ'}, {0x09, u'ゴ'}, {0x0A, u'ザ'}, {0x0B, u'ジ'}, {0x0C, u'ズ'}, {0x0D, u'ゼ'},
        {0x0E, u'ゾ'}, {0x0F, u'ダ'}, {0x10, u'ヂ'}, {0x11, u'ヅ'}, {0x12, u'デ'}, {0x13, u'ド'},
        {0x19, u'バ'}, {0x1A, u'ビ'}, {0x1B, u'ブ'}, {0x1C, u'ボ'}, {0x26, u'が'}, {0x27, u'ぎ'},
        {0x28, u'ぐ'}, {0x29, u'げ'}, {0x2A, u'ご'}, {0x2B, u'ざ'}, {0x2C, u'じ'}, {0x2D, u'ず'},
        {0x2E, u'ぜ'}, {0x2F, u'ぞ'}, {0x30, u'だ'}, {0x31, u'ぢ'}, {0x32, u'づ'}, {0x33, u'で'},
        {0x34, u'ど'}, {0x3A, u'ば'}, {0x3B, u'び'}, {0x3C, u'ぶ'}, {0x3D, u'ベ'}, {0x3E, u'ぼ'},
        {0x40, u'パ'}, {0x41, u'ピ'}, {0x42, u'プ'}, {0x43, u'ポ'}, {0x44, u'ぱ'}, {0x45, u'ぴ'},
        {0x46, u'ぷ'}, {0x47, u'ペ'}, {0x48, u'ぽ'}, {0x50, u'\0'}, {0x5D, tradeOTChar},
        {0x7F, u' '}, {0x80, u'ア'}, {0x81, u'イ'}, {0x82, u'ウ'}, {0x83, u'エ'}, {0x84, u'オ'},
        {0x85, u'カ'}, {0x86, u'キ'}, {0x87, u'ク'}, {0x88, u'ケ'}, {0x89, u'コ'}, {0x8A, u'サ'},
        {0x8B, u'シ'}, {0x8C, u'ス'}, {0x8D, u'セ'}, {0x8E, u'ソ'}, {0x8F, u'タ'}, {0x90, u'チ'},
        {0x91, u'ツ'}, {0x92, u'テ'}, {0x93, u'ト'}, {0x94, u'ナ'}, {0x95, u'ニ'}, {0x96, u'ヌ'},
        {0x97, u'ネ'}, {0x98, u'ノ'}, {0x99, u'ハ'}, {0x9A, u'ヒ'}, {0x9B, u'フ'}, {0x9C, u'ホ'},
        {0x9D, u'マ'}, {0x9E, u'ミ'}, {0x9F, u'ム'}, {0xA0, u'メ'}, {0xA1, u'モ'}, {0xA2, u'ヤ'},
        {0xA3, u'ユ'}, {0xA4, u'ヨ'}, {0xA5, u'ラ'}, {0xA6, u'ル'}, {0xA7, u'レ'}, {0xA8, u'ロ'},
        {0xA9, u'ワ'}, {0xAA, u'ヲ'}, {0xAB, u'ン'}, {0xAC, u'ッ'}, {0xAD, u'ャ'}, {0xAE, u'ュ'},
        {0xAF, u'ョ'}, {0xB0, u'ィ'}, {0xB1, u'あ'}, {0xB2, u'い'}, {0xB3, u'う'}, {0xB4, u'え'},
        {0xB5, u'お'}, {0xB6, u'か'}, {0xB7, u'き'}, {0xB8, u'く'}, {0xB9, u'け'}, {0xBA, u'こ'},
        {0xBB, u'さ'}, {0xBC, u'し'}, {0xBD, u'す'}, {0xBE, u'せ'}, {0xBF, u'そ'}, {0xC0, u'た'},
        {0xC1, u'ち'}, {0xC2, u'つ'}, {0xC3, u'て'}, {0xC4, u'と'}, {0xC5, u'な'}, {0xC6, u'に'},
        {0xC7, u'ぬ'}, {0xC8, u'ね'}, {0xC9, u'の'}, {0xCA, u'は'}, {0xCB, u'ひ'}, {0xCC, u'ふ'},
        {0xCD, u'へ'}, {0xCE, u'ほ'}, {0xCF, u'ま'}, {0xD0, u'み'}, {0xD1, u'む'}, {0xD2, u'め'},
        {0xD3, u'も'}, {0xD4, u'や'}, {0xD5, u'ゆ'}, {0xD6, u'よ'}, {0xD7, u'ら'}, {0xD8, u'リ'},
        {0xD9, u'る'}, {0xDA, u'れ'}, {0xDB, u'ろ'}, {0xDC, u'わ'}, {0xDD, u'を'}, {0xDE, u'ん'},
        {0xDF, u'っ'}, {0xE0, u'ゃ'}, {0xE1, u'ゅ'}, {0xE2, u'ょ'}, {0xE3, u'ー'}, {0xE6, u'?'},
        {0xE7, u'!'}, {0xE9, u'ァ'}, {0xEA, u'ゥ'}, {0xEB, u'ェ'}, {0xEF, u'♂'}, {0xF4, u'ォ'},
        {0xF5, u'♀'}, {0xF6, u'０'}, {0xF7, u'１'}, {0xF8, u'２'}, {0xF9, u'３'}, {0xFA, u'４'},
        {0xFB, u'５'}, {0xFC, u'６'}, {0xFD, u'７'}, {0xFE, u'８'}, {0xFF, u'９'}};
}

#endif
