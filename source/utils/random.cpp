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

#include "utils/random.hpp"
#include "utils/DateTime.hpp"
#include <random>

#ifndef _PKSMCORE_CONFIGURED
#include "PKSMCORE_CONFIG.h"
#endif

namespace
{
#ifdef _PKSMCORE_DISABLE_THREAD_SAFE_RANDOM
    std::mt19937 randomNumbers;
    bool seeded = false;
#else
    thread_local std::mt19937 randomNumbers;
    thread_local bool seeded = false;
#endif
}

u32 pksm::randomNumber()
{
    if (!seeded)
    {
        DateTime now = DateTime::now();
        // Dumb, but it works
        // clang-format off
        seedRand(now.year() * 365 * 24 * 60 * 60 +
                 now.day() * 24 * 60 * 60 +
                 now.hour() * 60 * 60 +
                 now.minute() * 60 +
                 now.second());
        // clang-format on
    }

    return pksm::randomNumber();
}

void pksm::seedRand(u32 seed)
{
    randomNumbers.seed(seed);
    seeded = true;
}
