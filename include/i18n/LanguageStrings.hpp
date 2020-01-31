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

#ifndef LANGUAGESTRINGS_HPP
#define LANGUAGESTRINGS_HPP

#include "PKSMCORE_CONFIG.h"

#include "Language.hpp"
#include "nlohmann/json.hpp"
#include "utils/coretypes.h"
#include "utils/generation.hpp"
#include "utils/io.hpp"
#include <algorithm>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

class LanguageStrings
{
protected:
#ifndef _PKSMCORE_DISABLE_ABILITY_STRINGS
    std::vector<std::string> abilities;
#endif

#ifndef _PKSMCORE_DISABLE_BALL_STRINGS
    std::vector<std::string> balls;
#endif

#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
    std::vector<std::string> forms;
#endif

#ifndef _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS
    std::vector<std::string> hps;
#endif

#ifndef _PKSMCORE_DISABLE_ITEM_STRINGS
    std::vector<std::string> items;
#endif

#ifndef _PKSMCORE_DISABLE_MOVE_STRINGS
    std::vector<std::string> moves;
#endif

#ifndef _PKSMCORE_DISABLE_NATURE_STRINGS
    std::vector<std::string> natures;
#endif

#ifndef _PKSMCORE_DISABLE_SPECIES_STRINGS
    std::vector<std::string> speciess;
#endif

#ifndef _PKSMCORE_DISABLE_GAME_STRINGS
    std::vector<std::string> games;
#endif

#ifndef _PKSMCORE_DISABLE_LOCATION_STRINGS
    std::map<u16, std::string> locations4;
    std::map<u16, std::string> locations5;
    std::map<u16, std::string> locations6;
    std::map<u16, std::string> locations7;
    std::map<u16, std::string> locationsLGPE;
    std::map<u16, std::string> locations8;
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
    std::map<u8, std::string> countries;
    std::map<u8, std::map<u8, std::string>> subregions;
#endif

#ifndef _PKSMCORE_DISABLE_GUI_STRINGS
    nlohmann::json gui;
#endif

    static void load(Language lang, const std::string& name, std::vector<std::string>& array);
    template <typename T>
    static void load(Language lang, const std::string& name, std::map<T, std::string>& map)
    {
        std::string path = io::exists(_PKSMCORE_LANG_FOLDER + folder(lang) + name) ? _PKSMCORE_LANG_FOLDER + folder(lang) + name
                                                                                   : _PKSMCORE_LANG_FOLDER + folder(Language::ENG) + name;

        std::string tmp;
        FILE* values = fopen(path.c_str(), "rt");
        if (values)
        {
            if (ferror(values))
            {
                fclose(values);
                return;
            }
            char* data  = (char*)malloc(128);
            size_t size = 0;
            while (!feof(values) && !ferror(values))
            {
                size = std::max(size, (size_t)128);
                if (_PKSMCORE_GETLINE_FUNC(&data, &size, values) >= 0)
                {
                    tmp = std::string(data);
                    tmp = tmp.substr(0, tmp.find('\n'));
                    // 0 automatically deduces the base: 0x prefix makes it hexadecimal, 0 prefix makes it octal
                    T val    = std::stoi(tmp.substr(0, tmp.find('|')), 0, 0);
                    map[val] = tmp.substr(0, tmp.find('\r')).substr(tmp.find('|') + 1);
                }
                else
                {
                    break;
                }
            }
            fclose(values);
            free(data);
        }
    }
    static void load(Language lang, const std::string& name, nlohmann::json& json);

public:
    LanguageStrings(Language lang);
    static std::string folder(Language lang);

#ifndef _PKSMCORE_DISABLE_ABILITY_STRINGS
    const std::string& ability(u16 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_BALL_STRINGS
    const std::string& ball(u8 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
    const std::string& form(u16 species, u16 form, Generation generation) const;
#endif

#ifndef _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS
    const std::string& hp(u8 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_ITEM_STRINGS
    const std::vector<std::string>& rawItems() const;
    const std::string& item(u16 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_MOVE_STRINGS
    const std::vector<std::string>& rawMoves() const;
    const std::string& move(u16 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_NATURE_STRINGS
    const std::string& nature(u8 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_SPECIES_STRINGS
    const std::string& species(u16 v) const;
#endif

#ifndef _PKSMCORE_DISABLE_GAME_STRINGS
    const std::string& game(u8 v) const;
    size_t numGameStrings() const;
#endif

#ifndef _PKSMCORE_DISABLE_LOCATION_STRINGS
    const std::map<u16, std::string>& locations(Generation g) const;
    const std::string& location(u16 v, Generation generation) const;
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
    const std::string& subregion(u8 country, u8 v) const;
    const std::string& country(u8 v) const;
    const std::map<u8, std::string>& rawCountries() const;
    const std::map<u8, std::string>& rawSubregions(u8 country) const;
#endif

#ifndef _PKSMCORE_DISABLE_GUI_STRINGS
    const std::string& localize(const std::string& v) const;
#endif
};

#endif
