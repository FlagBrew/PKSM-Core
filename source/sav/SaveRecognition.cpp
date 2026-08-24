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

#include "sav/SaveRecognition.hpp"
#include <algorithm>
#include <array>

namespace pksm
{
    namespace
    {
        // Biggest first: a file padded up from a DS save has to be read as a DS save, not
        // as the GBA save that also fits inside it.
        constexpr std::array<size_t, 4> KNOWN_SIZES = {
            SaveSize::DS, SaveSize::GBA, SaveSize::GB_JPN, SaveSize::GB_INT};

        [[nodiscard]] constexpr bool isKnownSize(size_t size)
        {
            return std::ranges::find(KNOWN_SIZES, size) != KNOWN_SIZES.end();
        }

        // DeSmuME appends a fixed-size trailer. It records the data size itself, but the
        // layout of that record has changed across DeSmuME versions, so the size is taken
        // from the file instead: a trailer this specific plus a save size the games
        // actually write is already unambiguous.
        constexpr size_t DESMUME_TRAILER = 0x7A;

        [[nodiscard]] SaveLayout desmume(std::span<const u8> file)
        {
            if (file.size() <= DESMUME_TRAILER)
            {
                return {};
            }
            const size_t dataSize = file.size() - DESMUME_TRAILER;
            if (!isKnownSize(dataSize))
            {
                return {};
            }
            return {.dataOffset = 0,
                .dataSize       = dataSize,
                .trailerSize    = DESMUME_TRAILER,
                .recognized     = true};
        }

        // GB, GBC and GBA emulators keep their real-time-clock state after the save data,
        // and every one of them sizes that block differently. The sizes all land in the
        // same small window, so the window is what gets accepted rather than the handful
        // of exact totals PKSM used to know about.
        constexpr size_t RTC_TRAILER_MIN = 0x0C;
        constexpr size_t RTC_TRAILER_MAX = 0x30;

        [[nodiscard]] constexpr bool isPlausibleRtcTrailer(size_t size)
        {
            if (size == 0x07) // lesserkuma/FlashGBX past v2.0
            {
                return true;
            }
            if (size % 2 != 0)
            {
                return false;
            }
            return size >= RTC_TRAILER_MIN && size <= RTC_TRAILER_MAX;
        }

        [[nodiscard]] SaveLayout realTimeClock(std::span<const u8> file)
        {
            const size_t trailerSize = file.size() & 0x3F;
            if (!isPlausibleRtcTrailer(trailerSize))
            {
                return {};
            }
            const size_t dataSize = file.size() - trailerSize;
            // The DS games have no real-time clock trailer; only the handheld formats
            // whose emulators write one are eligible.
            if (dataSize != SaveSize::GB_INT && dataSize != SaveSize::GB_JPN &&
                dataSize != SaveSize::GBA)
            {
                return {};
            }
            return {.dataOffset = 0,
                .dataSize       = dataSize,
                .trailerSize    = trailerSize,
                .recognized     = true};
        }

        // Flash carts and dumpers hand back the whole chip, so a save can arrive followed
        // by however much unwritten space the chip had left. Unwritten flash reads back as
        // a single repeated byte, which is what makes this safe to trim: a file whose tail
        // holds anything else is not padding and is left alone.
        [[nodiscard]] SaveLayout padding(std::span<const u8> file)
        {
            for (const size_t dataSize : KNOWN_SIZES)
            {
                if (dataSize >= file.size())
                {
                    continue;
                }
                const auto tail = file.subspan(dataSize);
                const u8 filler = tail.front();
                if (filler != 0x00 && filler != 0xFF)
                {
                    continue;
                }
                if (!std::ranges::all_of(tail, [filler](u8 byte) { return byte == filler; }))
                {
                    continue;
                }
                // A file that is filler all the way through is unwritten space, not a
                // save that happens to sit at the front of some.
                const auto data = file.first(dataSize);
                if (std::ranges::all_of(data, [filler](u8 byte) { return byte == filler; }))
                {
                    continue;
                }
                return {.dataOffset = 0,
                    .dataSize       = dataSize,
                    .trailerSize    = file.size() - dataSize,
                    .recognized     = true};
            }
            return {};
        }
    }

    SaveLayout recognizeSaveLayout(std::span<const u8> file)
    {
        if (file.empty())
        {
            return {};
        }

        if (const auto layout = desmume(file))
        {
            return layout;
        }
        if (const auto layout = realTimeClock(file))
        {
            return layout;
        }
        return padding(file);
    }
}
