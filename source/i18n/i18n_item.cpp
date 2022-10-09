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

#include "i18n_internal.hpp"

namespace i18n
{
    std::unordered_map<pksm::Language, std::vector<std::string>> items = std::invoke(
        []
        {
            std::unordered_map<pksm::Language, std::vector<std::string>> ret;
            MAP(MAKE_GENERIC_LANGMAP, LANGUAGES_TO_USE)
            return ret;
        });
    std::unordered_map<pksm::Language, std::vector<std::string>> items1 = std::invoke(
        []
        {
            std::unordered_map<pksm::Language, std::vector<std::string>> ret;
            MAP(MAKE_GENERIC_LANGMAP, LANGUAGES_TO_USE)
            return ret;
        });
    std::unordered_map<pksm::Language, std::vector<std::string>> items2 = std::invoke(
        []
        {
            std::unordered_map<pksm::Language, std::vector<std::string>> ret;
            MAP(MAKE_GENERIC_LANGMAP, LANGUAGES_TO_USE)
            return ret;
        });
    std::unordered_map<pksm::Language, std::vector<std::string>> items3 = std::invoke(
        []
        {
            std::unordered_map<pksm::Language, std::vector<std::string>> ret;
            MAP(MAKE_GENERIC_LANGMAP, LANGUAGES_TO_USE)
            return ret;
        });

    void initItem(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/items.txt", vec);

        // HM07 & HM08
        vec[426] = vec[425].substr(0, vec[425].size() - 1) + '7';
        vec[427] = vec[425].substr(0, vec[425].size() - 1) + '8';
        items.insert_or_assign(lang, std::move(vec));
    }

    void initItem1(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/items1.txt", vec);
        items1.insert_or_assign(lang, std::move(vec));
    }

    void initItem2(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/items2.txt", vec);
        items2.insert_or_assign(lang, std::move(vec));
    }

    void initItem3(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/items3.txt", vec);
        items3.insert_or_assign(lang, std::move(vec));
    }

    void exitItem(pksm::Language lang)
    {
        items[lang].clear();
    }

    void exitItem1(pksm::Language lang)
    {
        items1[lang].clear();
    }

    void exitItem2(pksm::Language lang)
    {
        items2[lang].clear();
    }

    void exitItem3(pksm::Language lang)
    {
        items3[lang].clear();
    }

    const std::string& item(pksm::Language lang, u16 val)
    {
        checkInitialized(lang);
        if (val < items[lang].size())
        {
            return items[lang][val];
        }
        return emptyString;
    }

    const std::string& item1(pksm::Language lang, u8 val)
    {
        checkInitialized(lang);
        if (val < items1[lang].size())
        {
            return items1[lang][val];
        }
        return emptyString;
    }

    const std::string& item2(pksm::Language lang, u8 val)
    {
        checkInitialized(lang);
        if (val < items2[lang].size())
        {
            return items2[lang][val];
        }
        return emptyString;
    }

    const std::string& item3(pksm::Language lang, u16 val)
    {
        checkInitialized(lang);
        if (val < items3[lang].size())
        {
            return items3[lang][val];
        }
        return emptyString;
    }

    const std::vector<std::string>& rawItems(pksm::Language lang)
    {
        checkInitialized(lang);
        return items[lang];
    }

    const std::vector<std::string>& rawItems1(pksm::Language lang)
    {
        checkInitialized(lang);
        return items1[lang];
    }

    const std::vector<std::string>& rawItems2(pksm::Language lang)
    {
        checkInitialized(lang);
        return items2[lang];
    }

    const std::vector<std::string>& rawItems3(pksm::Language lang)
    {
        checkInitialized(lang);
        return items3[lang];
    }
}
