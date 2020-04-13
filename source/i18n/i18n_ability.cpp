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

#include "enums/Ability.hpp"
#include "i18n_internal.hpp"

namespace i18n
{
    std::unordered_map<Language, std::vector<std::string>> abilities;

    void initAbility(Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/abilities.txt", vec);
        abilities.insert_or_assign(lang, std::move(vec));
    }

    void exitAbility(Language lang) { abilities.erase(lang); }

    const std::string& ability(Language lang, Ability val)
    {
        checkInitialized(lang);
        if (abilities.contains(lang))
        {
            if (size_t(val) < abilities[lang].size())
            {
                return abilities[lang][size_t(val)];
            }
        }
        return emptyString;
    }

    const std::vector<std::string>& rawAbilities(Language lang)
    {
        checkInitialized(lang);
        if (abilities.contains(lang))
        {
            return abilities[lang];
        }
        return emptyVector;
    }
}

const std::string& Ability_impl::localize(Language lang) const
{
    return i18n::ability(lang, *this);
}
