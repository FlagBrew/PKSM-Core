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

#include "sav/Sav8.hpp"
#include "pkx/PK8.hpp"
#include <algorithm>

Sav8::Sav8(std::shared_ptr<u8[]> dt, size_t length) : Sav(dt, length)
{
    swshcrypto_applyXor(dt, length);
    blocks = swshcrypto_getBlockList(dt, length);
}

std::shared_ptr<SCBlock> Sav8::getBlock(u32 key) const
{
    int min = 0, mid = 0, max = blocks.size();
    while (min <= max)
    {
        mid = min + (max - min) / 2;
        if (blocks[mid]->key() == key)
        {
            return blocks[mid];
        }
        if (blocks[mid]->key() < key)
        {
            min = mid + 1;
        }
        else
        {
            max = mid - 1;
        }
    }

    return nullptr;
}

std::unique_ptr<PKX> Sav8::emptyPkm() const
{
    return PKX::getPKM<Generation::EIGHT>(nullptr);
}

const std::set<int>& Sav8::availableItems(void) const
{
    if (items.empty())
    {
        fill_set_consecutive(items, 0, maxItem());
    }
    return items;
}

const std::set<int>& Sav8::availableMoves(void) const
{
    if (moves.empty())
    {
        fill_set_consecutive(moves, 0, maxMove());
    }
    return moves;
}

const std::set<Species>& Sav8::availableSpecies(void) const
{
    if (species.empty())
    {
        fill_set_consecutive<Species>(species, Species::Bulbasaur, maxSpecies());
    }
    return species;
}

const std::set<Ability>& Sav8::availableAbilities(void) const
{
    if (abilities.empty())
    {
        fill_set_consecutive<Ability>(abilities, Ability::Stench, maxAbility());
    }
    return abilities;
}

const std::set<::Ball>& Sav8::availableBalls(void) const
{
    if (balls.empty())
    {
        fill_set_consecutive<::Ball>(balls, Ball::Master, maxBall());
    }
    return balls;
}

void Sav8::trade(PKX& pk, const Date& date) const
{
    if (pk.egg())
    {
        if (pk.otName() != otName() || pk.TID() != TID() || pk.SID() != SID() || pk.gender() != gender())
        {
            pk.metLocation(30002);
            pk.metDate(date);
        }
    }
    else
    {
        if (pk.otName() != otName() || pk.TID() != TID() || pk.SID() != SID() || pk.gender() != gender())
        {
            pk.currentHandler(0);
        }
        else
        {
            pk.currentHandler(1);
            ((PK8&)pk).htName(otName());
            ((PK8&)pk).currentFriendship(pk.baseFriendship());
            ((PK8&)pk).htGender(gender());
            ((PK8&)pk).htLanguage(language());
        }
    }
}

void Sav8::finishEditing()
{
    if (!encrypted)
    {
        for (auto& block : blocks)
        {
            block->encrypt();
        }

        swshcrypto_applyXor(data, length);
        swshcrypto_sign(data, length);
    }
}

void Sav8::beginEditing()
{
    if (encrypted)
    {
        swshcrypto_applyXor(data, length);
    }

    // I could decrypt every block here, but why not just let them be done on the fly via the functions that need them?
}
