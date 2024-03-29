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

#include "utils/ValueConverter.hpp"
#include "enums/Species.hpp"
#include "g1values.hpp"
#include "g2values.hpp"
#include "g3values.hpp"
#include <algorithm>

namespace pksm
{
    Species SpeciesConverter::g1ToNational(u8 v)
    {
        if (v < internal::g1ToSpecies.size())
        {
            return Species{internal::g1ToSpecies[v]};
        }
        return Species::None;
    }

    u8 SpeciesConverter::nationalToG1(Species v)
    {
        if (u16(v) < internal::speciesToG1.size())
        {
            return internal::speciesToG1[u16(v)];
        }
        return 0;
    }

    Species SpeciesConverter::g3ToNational(u16 v)
    {
        if (v < internal::g3ToSpecies.size())
        {
            return Species{internal::g3ToSpecies[v]};
        }
        return Species::None;
    }

    u16 SpeciesConverter::nationalToG3(Species v)
    {
        if (u16(v) < internal::speciesToG3.size())
        {
            return internal::speciesToG3[u16(v)];
        }
        return 0;
    }

    u16 ItemConverter::g1ToNational(u8 v)
    {
        //"v < internal::g1ToItem.size()" is always true due to a size of 256
        return internal::g1ToItem[v];
    }

    u8 ItemConverter::nationalToG1(u16 v)
    {
        if (v == 0)
        {
            return 0;
        }

        auto it = std::find(internal::g1ToItem.begin(), internal::g1ToItem.end(), v);
        if (it == internal::g1ToItem.end())
        {
            return 0;
        }
        return std::distance(internal::g1ToItem.begin(), it);
    }

    u16 ItemConverter::g2ToNational(u8 v)
    {
        //"v < internal::g2ToItem.size()" is always true due to a size of 256
        return internal::g2ToItem[v];
    }

    u8 ItemConverter::nationalToG2(u16 v)
    {
        if (v == 0)
        {
            return 0;
        }

        auto it = std::find(internal::g2ToItem.begin(), internal::g2ToItem.end(), v);
        if (it == internal::g2ToItem.end())
        {
            return 0;
        }
        return std::distance(internal::g2ToItem.begin(), it);
    }

    u16 ItemConverter::g3ToNational(u16 v)
    {
        if (v < internal::g3ToItem.size())
        {
            return internal::g3ToItem[v];
        }
        return 0;
    }

    u16 ItemConverter::nationalToG3(u16 v)
    {
        if (v == 0)
        {
            return 0;
        }

        auto it = std::find(internal::g3ToItem.begin(), internal::g3ToItem.end(), v);
        if (it == internal::g3ToItem.end())
        {
            return 0;
        }
        return std::distance(internal::g3ToItem.begin(), it);
    }
}
