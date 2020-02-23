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

#include "i18n/LanguageStrings.hpp"
#include "utils/io.hpp"
#include "utils/utils.hpp"
#include <stdio.h>

#ifdef _PKSMCORE_EXTRA_LANGUAGES
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT, _PKSMCORE_EXTRA_LANGUAGES
#else
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT
#endif

#include "_map_macro.hpp"
#define TO_FOLDER_CASE(lang)                                                                                                                         \
    case Language::lang:                                                                                                                             \
        return std::string(toLower(#lang));

namespace
{
#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
    nlohmann::json& formJson()
    {
        static nlohmann::json forms;
        static bool first = true;
        if (first)
        {
            FILE* in = fopen(_PKSMCORE_LANG_FOLDER "forms.json", "rt");
            if (in)
            {
                if (!ferror(in))
                {
                    forms = nlohmann::json::parse(in, nullptr, false);
                }
                fclose(in);
            }
            first = false;
        }
        return forms;
    }
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
    std::string subregionFileName(u8 region)
    {
        std::string ret = "/subregions/000.txt";
        for (int pos = 0; pos < 3; pos++)
        {
            char setMe                = '0' + (region % 10);
            ret[ret.size() - 5 - pos] = setMe;
            region /= 10;
        }
        return ret;
    }
#endif

#if __cplusplus > 201703L
    constexpr std::string_view toLower(std::string_view str)
#else
    std::string_view toLower(std::string_view str)
#endif
    {
        char ret[str.size() + 1] = {'\0'};
        char upper[]             = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        char lower[]             = "abcdefghijklmnopqrstuvwxyz";
        for (size_t i = 0; i < str.size(); i++)
        {
            auto found = std::find(std::begin(upper), std::end(upper), str[i]);
            if (found != std::end(upper))
            {
                ret[i] = lower[std::distance(std::begin(upper), found)];
            }
        }
        ret[str.size()] = '\0';
        return std::string_view(ret);
    }
}

std::string LanguageStrings::folder(Language lang)
{
    switch (lang)
    {
        MAP(TO_FOLDER_CASE, LANGUAGES_TO_USE)
        default:
            return "eng";
    }

    return "eng";
}

LanguageStrings::LanguageStrings(Language lang)
{
#ifndef _PKSMCORE_DISABLE_ABILITY_STRINGS
    load(lang, "/abilities.txt", abilities);
#endif

#ifndef _PKSMCORE_DISABLE_BALL_STRINGS
    load(lang, "/balls.txt", balls);
#endif

#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
    load(lang, "/forms.txt", forms);
#endif

#ifndef _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS
    load(lang, "/hp.txt", hps);
#endif

#ifndef _PKSMCORE_DISABLE_ITEM_STRINGS
    load(lang, "/items.txt", items);
#endif

#ifndef _PKSMCORE_DISABLE_MOVE_STRINGS
    load(lang, "/moves.txt", moves);
#endif

#ifndef _PKSMCORE_DISABLE_NATURE_STRINGS
    load(lang, "/natures.txt", natures);
#endif

#ifndef _PKSMCORE_DISABLE_SPECIES_STRINGS
    load(lang, "/species.txt", speciess);
#endif

#ifndef _PKSMCORE_DISABLE_GAME_STRINGS
    load(lang, "/games.txt", games);
#endif

#ifndef _PKSMCORE_DISABLE_LOCATION_STRINGS
    load(lang, "/locations3.txt", locations3);
    load(lang, "/locations4.txt", locations4);
    load(lang, "/locations5.txt", locations5);
    load(lang, "/locations6.txt", locations6);
    load(lang, "/locations7.txt", locations7);
    load(lang, "/locationsLGPE.txt", locationsLGPE);
    load(lang, "/locations8.txt", locations8);
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
    countries = {};
    load(lang, "/countries.txt", countries);
    for (auto i = countries.begin(); i != countries.end(); i++)
    {
        subregions[i->first] = {};
        load(lang, subregionFileName(i->first), subregions[i->first]);
    }
#endif

#ifndef _PKSMCORE_DISABLE_GUI_STRINGS
    load(lang, "/gui.json", gui);
#endif
}

void LanguageStrings::load(Language lang, const std::string& name, std::vector<std::string>& array)
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
                array.emplace_back(tmp.substr(0, tmp.find('\r')));
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

void LanguageStrings::load(Language lang, const std::string& name, nlohmann::json& json)
{
    std::string path = io::exists(_PKSMCORE_LANG_FOLDER + folder(lang) + name) ? _PKSMCORE_LANG_FOLDER + folder(lang) + name
                                                                               : _PKSMCORE_LANG_FOLDER + folder(Language::ENG) + name;

    FILE* values = fopen(path.c_str(), "rt");
    if (values)
    {
        json = nlohmann::json::parse(values, nullptr, false);
        fclose(values);
    }
}

#ifndef _PKSMCORE_DISABLE_ABILITY_STRINGS
const std::string& LanguageStrings::ability(u16 v) const
{
    static std::string badString = "INVALID_ABILITY";
    return v < abilities.size() ? abilities.at(v) : badString;
}
const std::vector<std::string>& LanguageStrings::rawAbilities() const
{
    return abilities;
}
#endif

#ifndef _PKSMCORE_DISABLE_BALL_STRINGS
const std::string& LanguageStrings::ball(u8 v) const
{
    static std::string badString = "INVALID_BALL";
    return v < balls.size() ? balls.at(v) : badString;
}
#endif

#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
const std::string& LanguageStrings::form(u16 species, u16 form, Generation generation) const
{
    std::string sSpecies = std::to_string((int)species);
    if (formJson().contains(sSpecies))
    {
        std::vector<int> formIndices;
        if (formJson()[sSpecies].is_object())
        {
            if (formJson()[sSpecies].find(genToString(generation)) != formJson()[sSpecies].end())
            {
                formIndices = formJson()[sSpecies][genToString(generation)].get<std::vector<int>>();
            }
        }
        else
        {
            formIndices = formJson()[sSpecies].get<std::vector<int>>();
        }

        if (form < formIndices.size())
        {
            size_t formNameIndex = formIndices[form];
            if (formNameIndex < forms.size())
            {
                return forms.at(formNameIndex);
            }
        }
    }
    static std::string badString = "INVALID_FORM";
    return badString;
}
#endif

#ifndef _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS
const std::string& LanguageStrings::hp(u8 v) const
{
    static std::string badString = "INVALID_HP";
    return v < hps.size() ? hps.at(v) : badString;
}
#endif

#ifndef _PKSMCORE_DISABLE_ITEM_STRINGS
const std::string& LanguageStrings::item(u16 v) const
{
    static std::string badString = "INVALID_ITEM";
    return v < items.size() ? items.at(v) : badString;
}
const std::vector<std::string>& LanguageStrings::rawItems() const
{
    return items;
}
#endif

#ifndef _PKSMCORE_DISABLE_MOVE_STRINGS
const std::string& LanguageStrings::move(u16 v) const
{
    static std::string badString = "INVALID_MOVE";
    return v < moves.size() ? moves.at(v) : badString;
}
const std::vector<std::string>& LanguageStrings::rawMoves() const
{
    return moves;
}
#endif

#ifndef _PKSMCORE_DISABLE_NATURE_STRINGS
const std::string& LanguageStrings::nature(u8 v) const
{
    static std::string badString = "INVALID_NATURE";
    return v < natures.size() ? natures.at(v) : badString;
}
#endif

#ifndef _PKSMCORE_DISABLE_SPECIES_STRINGS
const std::string& LanguageStrings::species(u16 v) const
{
    static std::string badString = "INVALID_SPECIES";
    return v < speciess.size() ? speciess.at(v) : badString;
}
const std::vector<std::string>& LanguageStrings::rawSpecies() const
{
    return speciess;
}
#endif

#ifndef _PKSMCORE_DISABLE_GAME_STRINGS
const std::string& LanguageStrings::game(u8 v) const
{
    if (v < games.size() && games.at(v) != "")
    {
        return games.at(v);
    }
    static std::string badString = "INVALID_GAME";
    return badString;
}
size_t LanguageStrings::numGameStrings() const
{
    return games.size();
}
const std::vector<std::string>& LanguageStrings::rawGames() const
{
    return games;
}
#endif

#ifndef _PKSMCORE_DISABLE_LOCATION_STRINGS
const std::string& LanguageStrings::location(u16 v, Generation generation) const
{
    std::map<u16, std::string>::const_iterator i;
    switch (generation)
    {
        case Generation::THREE:
            if ((i = locations3.find(v)) != locations3.end())
            {
                return i->second;
            }
            break;
        case Generation::FOUR:
            if ((i = locations4.find(v)) != locations4.end())
            {
                return i->second;
            }
            break;
        case Generation::FIVE:
            if ((i = locations5.find(v)) != locations5.end())
            {
                return i->second;
            }
            break;
        case Generation::SIX:
            if ((i = locations6.find(v)) != locations6.end())
            {
                return i->second;
            }
            break;
        case Generation::SEVEN:
            if ((i = locations7.find(v)) != locations7.end())
            {
                return i->second;
            }
            break;
        case Generation::LGPE:
            if ((i = locationsLGPE.find(v)) != locationsLGPE.end())
            {
                return i->second;
            }
            break;
        case Generation::EIGHT:
            if ((i = locations8.find(v)) != locations8.end())
            {
                return i->second;
            }
            break;
        case Generation::UNUSED:
            break;
    }
    static std::string badString = "INVALID_LOCATION";
    return badString;
}
const std::map<u16, std::string>& LanguageStrings::locations(Generation g) const
{
    static std::map<u16, std::string> emptyMap;
    switch (g)
    {
        case Generation::THREE:
            return locations3;
        case Generation::FOUR:
            return locations4;
        case Generation::FIVE:
            return locations5;
        case Generation::SIX:
            return locations6;
        case Generation::SEVEN:
            return locations7;
        case Generation::LGPE:
            return locationsLGPE;
        case Generation::EIGHT:
            return locations8;
        case Generation::UNUSED:
            return emptyMap;
    }
    return emptyMap;
}
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
const std::string& LanguageStrings::subregion(u8 country, u8 v) const
{
    auto i = subregions.find(country);
    if (i != subregions.end())
    {
        auto j = i->second.find(v);
        if (j != i->second.end())
        {
            return j->second;
        }
        static std::string badString = "INVALID_SUBREGION";
        return badString;
    }
    static std::string badString = "INVALID_COUNTRY";
    return badString;
}
const std::string& LanguageStrings::country(u8 v) const
{
    auto i = countries.find(v);
    if (i != countries.end())
    {
        return i->second;
    }
    static std::string badString = "INVALID_COUNTRY";
    return badString;
}
const std::map<u8, std::string>& LanguageStrings::rawCountries() const
{
    return countries;
}
const std::map<u8, std::string>& LanguageStrings::rawSubregions(u8 country) const
{
    static std::map<u8, std::string> emptyMap;
    auto i = subregions.find(country);
    if (i == subregions.end())
    {
        return emptyMap;
    }
    return i->second;
}
#endif

#ifndef _PKSMCORE_DISABLE_GUI_STRINGS
const std::string& LanguageStrings::localize(const std::string& v) const
{
    if (!gui.contains(v))
    {
        const_cast<nlohmann::json&>(gui)[v] = "MISSING: " + v;
    }
    return gui.at(v).get_ref<const std::string&>();
}
#endif
