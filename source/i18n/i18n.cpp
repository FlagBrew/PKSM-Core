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

#include "i18n_internal.hpp"
#include "utils/_map_macro.hpp"
#include <algorithm>
#include <list>

#ifdef _PKSMCORE_EXTRA_LANGUAGES
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT, _PKSMCORE_EXTRA_LANGUAGES
#else
#define LANGUAGES_TO_USE JPN, ENG, FRE, ITA, GER, SPA, KOR, CHS, CHT
#endif

#define MAKE_MAP(lang) ret.emplace(Language::lang, LangState::UNINITIALIZED);
#define TO_STRING_CASE(lang)                                                                                                                         \
    case Language::lang:                                                                                                                             \
    {                                                                                                                                                \
        static const std::string str = #lang;                                                                                                        \
        return str;                                                                                                                                  \
    }
#define TO_IF_STRING(lang)                                                                                                                           \
    if (value == #lang)                                                                                                                              \
        return Language::lang;
#define TO_FOLDER_CASE(lang)                                                                                                                         \
    case Language::lang:                                                                                                                             \
        return toLower(#lang);

namespace
{
    std::string toLower(const std::string& str)
    {
        std::string ret               = str;
        static constexpr char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static constexpr char lower[] = "abcdefghijklmnopqrstuvwxyz";
        for (size_t i = 0; i < ret.size(); i++)
        {
            auto found = std::find(std::begin(upper), std::end(upper), ret[i]);
            if (found != std::end(upper))
            {
                ret[i] = lower[std::distance(std::begin(upper), found)];
            }
        }
        return ret;
    }
}
namespace i18n
{
    std::unordered_map<Language, std::atomic<LangState>> languages = []() {
        std::unordered_map<Language, std::atomic<LangState>> ret;
        MAP(MAKE_MAP, LANGUAGES_TO_USE)
        return ret;
    }();

    std::list<initCallback> initCallbacks = {
        initAbility, initBall, initForm, initGame, initGeo, initType, initItem, initLocation, initMove, initNature, initSpecies};
    std::list<exitCallback> exitCallbacks = {
        exitAbility, exitBall, exitForm, exitGame, exitGeo, exitType, exitItem, exitLocation, exitMove, exitNature, exitSpecies};

    void init(Language lang)
    {
        auto found = languages.find(lang);
        // Should never happen, but might as well check
        if (found == languages.end())
        {
            found = languages.find(Language::ENG);
        }
        if (found->second == LangState::UNINITIALIZED)
        {
            found->second = LangState::INITIALIZING;
            for (auto& callback : initCallbacks)
            {
                callback(lang);
            }
            found->second = LangState::INITIALIZED;
        }
    }

    void exit(void)
    {
        for (auto& lang : languages)
        {
            if (lang.second != LangState::UNINITIALIZED)
            {
                while (lang.second != LangState::INITIALIZED)
                {
                    timespec time = {0, 100000};
                    timespec rem;
                    nanosleep(&time, &rem);
                }

                for (auto& callback : exitCallbacks)
                {
                    callback(lang.first);
                }

                lang.second = LangState::UNINITIALIZED;
            }
        }
    }

    const std::string& langString(Language l)
    {
        static const std::string ENG = "ENG";
        switch (l)
        {
            MAP(TO_STRING_CASE, LANGUAGES_TO_USE)
            default:
                return ENG;
        }
    }

    Language langFromString(const std::string& value)
    {
        MAP(TO_IF_STRING, LANGUAGES_TO_USE)
        return Language::ENG;
    }

    std::string folder(Language lang)
    {
        switch (lang)
        {
            MAP(TO_FOLDER_CASE, LANGUAGES_TO_USE)
            default:
                return "eng";
        }

        return "eng";
    }

    void load(Language lang, const std::string& name, std::vector<std::string>& array)
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

    void addInitCallback(initCallback callback)
    {
        auto i = std::find(initCallbacks.begin(), initCallbacks.end(), callback);
        if (i == initCallbacks.end())
        {
            initCallbacks.emplace_back(callback);
        }
    }

    void removeInitCallback(initCallback callback)
    {
        auto i = std::find(initCallbacks.begin(), initCallbacks.end(), callback);
        while (i != initCallbacks.end())
        {
            initCallbacks.erase(i);
            i = std::find(initCallbacks.begin(), initCallbacks.end(), callback);
        }
    }

    void addExitCallback(exitCallback callback)
    {
        auto i = std::find(exitCallbacks.begin(), exitCallbacks.end(), callback);
        if (i == exitCallbacks.end())
        {
            exitCallbacks.emplace_back(callback);
        }
    }

    void removeExitCallback(exitCallback callback)
    {
        auto i = std::find(exitCallbacks.begin(), exitCallbacks.end(), callback);
        while (i != exitCallbacks.end())
        {
            exitCallbacks.erase(i);
            i = std::find(exitCallbacks.begin(), exitCallbacks.end(), callback);
        }
    }
}
