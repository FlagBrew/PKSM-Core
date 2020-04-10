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
    std::unordered_map<Language, std::vector<std::string>> hps;

    void initHP(Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/hp.txt", vec);
        hps.insert_or_assign(lang, std::move(vec));
    }

    void exitHP(Language lang) { hps.erase(lang); }

    const std::string& hp(Language lang, u8 val)
    {
        checkInitialized(lang);
        if (hps.contains(lang))
        {
            if (val < hps[lang].size())
            {
                return hps[lang][val];
            }
        }
        return emptyString;
    }

    const std::vector<std::string>& rawHPs(Language lang)
    {
        checkInitialized(lang);
        if (hps.contains(lang))
        {
            return hps[lang];
        }
        return emptyVector;
    }
}
