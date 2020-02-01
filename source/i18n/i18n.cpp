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

#include "i18n/i18n.hpp"
#include "i18n/LanguageStrings.hpp"
#include <atomic>
#include <unistd.h>

#ifdef _PKSMCORE_EXTRA_LANGUAGES
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT, _PKSMCORE_EXTRA_LANGUAGES
#else
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT
#endif

#include "_map_macro.hpp"
#define MAKE_MAP(lang) ret.emplace(Language::lang, std::make_unique<LangRecord>());
#define TO_STRING_CASE(lang)                                                                                                                         \
    case Language::lang:                                                                                                                             \
    {                                                                                                                                                \
        static const std::string str = #lang;                                                                                                        \
        return str;                                                                                                                                  \
    }
#define TO_IF_STRING(lang)                                                                                                                           \
    if (value == #lang)                                                                                                                              \
        return Language::lang;

namespace
{
    struct LangRecord
    {
        LangRecord() {}
        LanguageStrings* strings     = nullptr;
        std::atomic_flag initialized = ATOMIC_FLAG_INIT;
    };
    std::map<Language, std::unique_ptr<LangRecord>> languages = []() {
        std::map<Language, std::unique_ptr<LangRecord>> ret;
        MAP(MAKE_MAP, LANGUAGES_TO_USE)
        return std::move(ret);
    }();

    LanguageStrings* stringsFor(Language lang)
    {
        auto found = languages.find(lang);
        if (found == languages.end())
        {
            found = languages.find(Language::ENG);
        }
        if (!found->second->initialized.test_and_set())
        {
            found->second->initialized.clear();
            i18n::init(found->first);
        }
        while (!found->second->strings)
        {
            usleep(100);
        }
        return found->second->strings;
    }

    const std::string emptyString                = "";
    const std::vector<std::string> emptyVector   = {};
    const std::map<u16, std::string> emptyU16Map = {};
    const std::map<u8, std::string> emptyU8Map   = {};
}

void i18n::init(Language lang)
{
    auto found = languages.find(lang);
    if (found == languages.end())
    {
        found = languages.find(Language::ENG);
    }
    if (!found->second->initialized.test_and_set())
    {
        found->second->strings = new LanguageStrings(found->first);
    }
}

void i18n::exit(void)
{
    for (auto& lang : languages)
    {
        if (lang.second->initialized.test_and_set())
        {
            while (!lang.second->strings)
            {
                usleep(100);
            }
            delete lang.second->strings;
            lang.second->strings = nullptr;
            lang.second->initialized.clear();
        }
    }
}

const std::string& i18n::langString(Language l)
{
    static const std::string JPN = "JPN";
    static const std::string ENG = "ENG";
    static const std::string FRE = "FRE";
    static const std::string ITA = "ITA";
    static const std::string GER = "GER";
    static const std::string SPA = "SPA";
    static const std::string KOR = "KOR";
    static const std::string CHS = "CHS";
    static const std::string CHT = "CHT";
    switch (l)
    {
        MAP(TO_STRING_CASE, LANGUAGES_TO_USE)
        default:
            return ENG;
    }
}

Language i18n::langFromString(const std::string& value)
{
    MAP(TO_IF_STRING, LANGUAGES_TO_USE)
    return Language::ENG;
}

#ifndef _PKSMCORE_DISABLE_ABILITY_STRINGS
const std::string& i18n::ability(Language lang, u16 val)
{
    return stringsFor(lang)->ability(val);
}
const std::vector<std::string>& i18n::rawAbilities(Language lang)
{
    return stringsFor(lang)->rawAbilities();
}
#endif

#ifndef _PKSMCORE_DISABLE_BALL_STRINGS
const std::string& i18n::ball(Language lang, u8 val)
{
    return stringsFor(lang)->ball(val);
}
#endif

#ifndef _PKSMCORE_DISABLE_FORM_STRINGS
const std::string& i18n::form(Language lang, u16 species, u16 form, Generation generation)
{
    return stringsFor(lang)->form(species, form, generation);
}
#endif

#ifndef _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS
const std::string& i18n::hp(Language lang, u8 val)
{
    return stringsFor(lang)->hp(val);
}
#endif

#ifndef _PKSMCORE_DISABLE_ITEM_STRINGS
const std::string& i18n::item(Language lang, u16 val)
{
    return stringsFor(lang)->item(val);
}
const std::vector<std::string>& i18n::rawItems(Language lang)
{
    return stringsFor(lang)->rawItems();
}
#endif

#ifndef _PKSMCORE_DISABLE_MOVE_STRINGS
const std::string& i18n::move(Language lang, u16 val)
{
    return stringsFor(lang)->move(val);
}
const std::vector<std::string>& i18n::rawMoves(Language lang)
{
    return stringsFor(lang)->rawMoves();
}
#endif

#ifndef _PKSMCORE_DISABLE_NATURE_STRINGS
const std::string& i18n::nature(Language lang, u8 val)
{
    return stringsFor(lang)->nature(val);
}
#endif

#ifndef _PKSMCORE_DISABLE_SPECIES_STRINGS
const std::string& i18n::species(Language lang, u16 val)
{
    return stringsFor(lang)->species(val);
}
const std::vector<std::string>& i18n::rawSpecies(Language lang)
{
    return stringsFor(lang)->rawSpecies();
}
#endif

#ifndef _PKSMCORE_DISABLE_GAME_STRINGS
const std::string& i18n::game(Language lang, u8 v)
{
    return stringsFor(lang)->game(v);
}
size_t i18n::numGameStrings(Language lang)
{
    return stringsFor(lang)->numGameStrings();
}
const std::vector<std::string>& i18n::rawGames(Language lang)
{
    return stringsFor(lang)->rawGames();
}
#endif

#ifndef _PKSMCORE_DISABLE_LOCATION_STRINGS
const std::string& i18n::location(Language lang, u16 v, Generation generation)
{
    return stringsFor(lang)->location(v, generation);
}
const std::string& i18n::location(Language lang, u16 v, u8 originGame)
{
    switch (originGame)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return location(lang, v, Generation::THREE);
        case 7:
        case 8:
        case 10:
        case 11:
        case 12:
            return location(lang, v, Generation::FOUR);
        case 20:
        case 21:
        case 22:
        case 23:
            return location(lang, v, Generation::FIVE);
        case 24:
        case 25:
        case 26:
        case 27:
            return location(lang, v, Generation::SIX);
        case 30:
        case 31:
        case 32:
        case 33:
            return location(lang, v, Generation::SEVEN);
        case 42:
        case 43:
            return location(lang, v, Generation::LGPE);
        case 44:
        case 45:
            return location(lang, v, Generation::EIGHT);
    }
    return emptyString;
}
const std::map<u16, std::string>& i18n::locations(Language lang, Generation g)
{
    return stringsFor(lang)->locations(g);
}
#endif

#ifndef _PKSMCORE_DISABLE_GEO_STRINGS
const std::string& i18n::subregion(Language lang, u8 country, u8 value)
{
    return stringsFor(lang)->subregion(country, value);
}
const std::string& i18n::country(Language lang, u8 value)
{
    return stringsFor(lang)->country(value);
}
const std::map<u8, std::string>& i18n::rawCountries(Language lang)
{
    return stringsFor(lang)->rawCountries();
}
const std::map<u8, std::string>& i18n::rawSubregions(Language lang, u8 country)
{
    return stringsFor(lang)->rawSubregions(country);
}
#endif

#ifndef _PKSMCORE_DISABLE_GUI_STRINGS
const std::string& i18n::localize(Language lang, const std::string& val)
{
    return stringsFor(lang)->localize(val);
}
#endif
