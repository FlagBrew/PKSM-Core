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

#include "enums/Generation.hpp"
#include "i18n_internal.hpp"

namespace i18n
{
    struct Locations
    {
        std::map<u16, std::string> locations3;
        std::map<u16, std::string> locations4;
        std::map<u16, std::string> locations5;
        std::map<u16, std::string> locations6;
        std::map<u16, std::string> locations7;
        std::map<u16, std::string> locationsLGPE;
        std::map<u16, std::string> locations8;
    };

    std::unordered_map<Language, Locations> locationss;

    void initLocation(Language lang)
    {
        Locations tmp;
        load(lang, "/locations3.txt", tmp.locations3);
        load(lang, "/locations4.txt", tmp.locations4);
        load(lang, "/locations5.txt", tmp.locations5);
        load(lang, "/locations6.txt", tmp.locations6);
        load(lang, "/locations7.txt", tmp.locations7);
        load(lang, "/locationsLGPE.txt", tmp.locationsLGPE);
        load(lang, "/locations8.txt", tmp.locations8);
        locationss.insert_or_assign(lang, std::move(tmp));
    }

    void exitLocation(Language lang) { locationss.erase(lang); }

    const std::string& location(Language lang, Generation gen, u16 v)
    {
        checkInitialized(lang);
        if (locationss.contains(lang))
        {
            switch (gen)
            {
                case Generation::THREE:
                    if (locationss[lang].locations3.contains(v))
                    {
                        return locationss[lang].locations3[v];
                    }
                    break;
                case Generation::FOUR:
                    if (locationss[lang].locations4.contains(v))
                    {
                        return locationss[lang].locations4[v];
                    }
                    break;
                case Generation::FIVE:
                    if (locationss[lang].locations5.contains(v))
                    {
                        return locationss[lang].locations5[v];
                    }
                    break;
                case Generation::SIX:
                    if (locationss[lang].locations6.contains(v))
                    {
                        return locationss[lang].locations6[v];
                    }
                    break;
                case Generation::SEVEN:
                    if (locationss[lang].locations7.contains(v))
                    {
                        return locationss[lang].locations7[v];
                    }
                    break;
                case Generation::LGPE:
                    if (locationss[lang].locationsLGPE.contains(v))
                    {
                        return locationss[lang].locationsLGPE[v];
                    }
                    break;
                case Generation::EIGHT:
                    if (locationss[lang].locations8.contains(v))
                    {
                        return locationss[lang].locations8[v];
                    }
                    break;
                case Generation::UNUSED:
                    break;
            }
        }
        return emptyString;
    }

    const std::map<u16, std::string>& rawLocations(Language lang, Generation g)
    {
        checkInitialized(lang);
        if (locationss.contains(lang))
        {
            switch (g)
            {
                case Generation::THREE:
                    return locationss[lang].locations3;
                case Generation::FOUR:
                    return locationss[lang].locations4;
                case Generation::FIVE:
                    return locationss[lang].locations5;
                case Generation::SIX:
                    return locationss[lang].locations6;
                case Generation::SEVEN:
                    return locationss[lang].locations7;
                case Generation::LGPE:
                    return locationss[lang].locationsLGPE;
                case Generation::EIGHT:
                    return locationss[lang].locations8;
                case Generation::UNUSED:
                    break;
            }
        }
        return emptyU16Map;
    }
}
