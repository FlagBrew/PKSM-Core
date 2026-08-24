/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2025 Bernardo Giordano, Admiral Fish, piepie62
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

#ifndef SAVERECOGNITION_HPP
#define SAVERECOGNITION_HPP

#include "utils/coretypes.h"
#include <cstddef>
#include <span>

namespace pksm
{
    // The save data a game actually uses, and whatever the tool that produced the file
    // wrapped around it. Emulators append real-time-clock state, flash carts and dumpers
    // round the file up to the size of the chip they read, and every one of them ends up
    // in front of PKSM with a size the games themselves never write.
    struct SaveLayout
    {
        // Offset of the save data within the file. Only formats whose wrapper sits after
        // the data are recognized, so this is always 0 for now; it exists so that a
        // header-carrying format can be added without changing every caller.
        size_t dataOffset = 0;
        // Size of the save data itself: what the game wrote, what PKSM has to parse.
        size_t dataSize = 0;
        // Bytes the wrapper left after the data. PKSM keeps them in the buffer and writes
        // them back untouched, so the emulator that made the file can still read it.
        size_t trailerSize = 0;
        bool recognized    = false;

        explicit operator bool() const { return recognized; }
    };

    // The save sizes the GB, GBC, GBA and DS games write. Anything larger (3DS and Switch
    // saves) only ever reaches PKSM through the console itself, never through an emulator,
    // so it needs no unwrapping.
    namespace SaveSize
    {
        inline constexpr size_t GB_INT = 0x8000;  // international GB/GBC cartridge SRAM
        inline constexpr size_t GB_JPN = 0x10000; // Japanese GB/GBC cartridge SRAM
        inline constexpr size_t GBA    = 0x20000; // GBA flash
        inline constexpr size_t DS     = 0x80000; // DS EEPROM
    }

    // Works out where the save data is inside a file PKSM did not recognize by size alone.
    // Returns an unrecognized layout when the file is not a wrapped save.
    [[nodiscard]] SaveLayout recognizeSaveLayout(std::span<const u8> file);
}

#endif
