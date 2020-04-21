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

#ifndef I18N_HPP
#define I18N_HPP

#include "enums/Ability.hpp"
#include "enums/Ball.hpp"
#include "enums/GameVersion.hpp"
#include "enums/Generation.hpp"
#include "enums/Language.hpp"
#include "enums/Nature.hpp"
#include "enums/Ribbon.hpp"
#include "enums/Species.hpp"
#include "enums/Type.hpp"
#include "utils/coretypes.h"
#include <map>
#include <string>
#include <vector>

namespace i18n
{
    using initCallback = void (*)(Language);
    using exitCallback = void (*)(Language);
    void addInitCallback(initCallback callback);
    void removeInitCallback(initCallback callback);
    void addExitCallback(exitCallback callback);
    void removeExitCallback(exitCallback callback);

    inline void addCallbacks(initCallback init, exitCallback exit)
    {
        addInitCallback(init);
        addExitCallback(exit);
    }
    inline void removeCallbacks(initCallback init, exitCallback exit)
    {
        removeInitCallback(init);
        removeExitCallback(exit);
    }

    // Calls the callbacks that have been registered with addInitCallback in a thread-safe manner
    // NOTE: default callbacks include all init functions in this file
    void init(Language lang);
    // Calls the callbacks that have been registered with addExitCallback in a thread-safe manner for all languages that have been initialized
    // NOTE: default callbacks include all exit functions in this file
    void exit(void);

    const std::string& langString(Language l);
    Language langFromString(const std::string& value);

    void initAbility(Language lang);
    void exitAbility(Language lang);
    const std::string& ability(Language lang, Ability value);
    const std::vector<std::string>& rawAbilities(Language lang);

    void initBall(Language lang);
    void exitBall(Language lang);
    const std::string& ball(Language lang, Ball value);
    const std::vector<std::string>& rawBalls(Language lang);

    void initForm(Language lang);
    void exitForm(Language lang);
    const std::string& form(Language lang, GameVersion version, Species species, u8 form);
    std::vector<std::string> forms(Language lang, GameVersion version, Species species);
    // No good raw interface for this

    void initGame(Language lang);
    void exitGame(Language lang);
    const std::string& game(Language lang, GameVersion value);
    const std::vector<std::string>& rawGames(Language lang);

    void initItem(Language lang);
    void exitItem(Language lang);
    const std::string& item(Language lang, u16 value);
    const std::vector<std::string>& rawItems(Language lang);

    void initMove(Language lang);
    void exitMove(Language lang);
    const std::string& move(Language lang, u16 value);
    const std::vector<std::string>& rawMoves(Language lang);

    void initNature(Language lang);
    void exitNature(Language lang);
    const std::string& nature(Language lang, Nature value);
    const std::vector<std::string>& rawNatures(Language lang);

    void initRibbon(Language lang);
    void exitRibbon(Language lang);
    const std::string& ribbon(Language lang, Ribbon value);
    const std::vector<std::string>& rawRibbons(Language lang);

    // Note: several functions require this to function properly.
    // A nonexhaustive list includes PK3::nicknamed, PK4::convertToPK3, PK5::convertToPK6, SavLGPE::mysteryGift, and SavSWSH::mysteryGift
    void initSpecies(Language lang);
    void exitSpecies(Language lang);
    const std::string& species(Language lang, Species value);
    const std::vector<std::string>& rawSpecies(Language lang);

    void initType(Language lang);
    void exitType(Language lang);
    const std::string& type(Language lang, Type value);
    const std::vector<std::string>& rawType(Language lang);

    void initLocation(Language lang);
    void exitLocation(Language lang);
    const std::string& location(Language lang, Generation generation, u16 value);
    const std::map<u16, std::string>& rawLocations(Language lang, Generation g);

    void initGeo(Language lang);
    void exitGeo(Language lang);
    const std::string& subregion(Language lang, u8 country, u8 value);
    const std::map<u8, std::string>& rawSubregions(Language lang, u8 country);
    const std::string& country(Language lang, u8 value);
    const std::map<u8, std::string>& rawCountries(Language lang);
};

#endif
