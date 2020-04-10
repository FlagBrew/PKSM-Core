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

namespace i18n
{
    std::atomic<LangState> jsonState = LangState::UNINITIALIZED;
    nlohmann::json formJson;

    std::unordered_map<Language, std::vector<std::string>> formss;

    void initForm(Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/forms.txt", vec);
        formss.insert_or_assign(lang, std::move(vec));

        if (jsonState == LangState::UNINITIALIZED)
        {
            jsonState = LangState::INITIALIZING;
            FILE* in  = fopen(_PKSMCORE_LANG_FOLDER "forms.json", "rt");
            if (in)
            {
                if (!ferror(in))
                {
                    formJson = nlohmann::json::parse(in, nullptr, false);
                }
                fclose(in);
            }
            jsonState = LangState::INITIALIZED;
        }
    }

    void exitForm(Language lang) { formss.erase(lang); }

    /**
     * Form JSON format:
     * {
     *   "<species number>": [
     *     {
     *       "<version number>": string_index
     *     },
     *     string_index
     *   }
     * }
     *
     * Integers in the species number array are interpreted as raw string indexes, and will always be translated. Objects will have the version
     *numbers compared with the GameVersion passed into i18n::form(s) and the string will only be translated if there is a match.
     **/

    const std::string& form(Language lang, GameVersion version, u16 species, u16 form)
    {
        checkInitialized(lang);
        auto stringsIt = formss.find(lang);
        if (stringsIt != formss.end())
        {
            std::string sSpecies = std::to_string((int)species);
            auto speciesIt       = formJson.find(sSpecies);
            if (speciesIt != formJson.end() && form < speciesIt->size())
            {
                const auto& formVal = (*speciesIt)[form];

                int index = -1;
                if (formVal.is_object())
                {
                    std::string sVersion = std::to_string((int)version);
                    auto versionIt       = formVal.find(sVersion);
                    if (versionIt != formVal.end())
                    {
                        index = versionIt->get<int>();
                    }
                    else
                    {
                        sVersion       = "G" + (std::string)((Generation)version);
                        auto versionIt = formVal.find(sVersion);
                        if (versionIt != formVal.end())
                        {
                            index = versionIt->get<int>();
                        }
                    }
                }
                else
                {
                    index = formVal.get<int>();
                }

                if (index != -1 && (size_t)index < stringsIt->second.size())
                {
                    return stringsIt->second[index];
                }
            }
        }
        return emptyString;
    }

    std::vector<std::string> forms(Language lang, GameVersion version, u16 species)
    {
        checkInitialized(lang);
        auto stringsIt = formss.find(lang);
        if (stringsIt != formss.end())
        {
            std::string sSpecies = std::to_string((int)species);
            auto speciesIt       = formJson.find(sSpecies);
            if (speciesIt != formJson.end())
            {
                std::vector<std::string> ret;
                for (const auto& formVal : *speciesIt)
                {
                    int index = -1;
                    if (formVal.is_object())
                    {
                        std::string sVersion = std::to_string((int)version);
                        auto versionIt       = formVal.find(sVersion);
                        if (versionIt != formVal.end())
                        {
                            index = versionIt->get<int>();
                        }
                    }
                    else
                    {
                        index = formVal.get<int>();
                    }

                    if (index != -1 && (size_t)index < stringsIt->second.size())
                    {
                        ret.push_back(stringsIt->second[index]);
                    }
                    else
                    {
                        ret.push_back("");
                    }
                }
                return ret;
            }
        }
        return {};
    }
}
