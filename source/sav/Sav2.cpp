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

#include "sav/Sav2.hpp"
#include "GameVersion.hpp"
#include "pkx/PK2.hpp"
#include "pkx/PKX.hpp"
#include "utils/crypto.hpp"
#include "utils/endian.hpp"
#include "utils/flagUtil.hpp"
#include "utils/i18n.hpp"
#include "utils/utils.hpp"
#include "wcx/WCX.hpp"
#include <algorithm>
#include <limits>
#include <stdlib.h>

namespace pksm
{
    // the language class and version necessarily need to be found, may as well use them
    Sav2::Sav2(const std::shared_ptr<u8[]>& data, u32 length,
        std::tuple<GameVersion, Language, bool> versionAndLanguage)
        : Sav(data, length)
    {
        versionOfGame = get<0>(versionAndLanguage);
        lang          = get<1>(versionAndLanguage);
        japanese      = lang == Language::JPN;
        korean        = lang == Language::KOR;
        maxPkmInBox   = japanese ? 30 : 20;
        boxSize       = japanese ? 0x54A : 0x450;

        OFS_TID  = 0x2009;
        OFS_NAME = 0x200B;

        if (korean)
        {
            OFS_TIME_PLAYED       = 0x204D;
            OFS_PALETTE           = 0x2065;
            OFS_MONEY             = 0x23D3;
            OFS_BADGES            = 0x23DC;
            OFS_TM_POUCH          = 0x23DE;
            OFS_ITEMS             = 0x2417;
            OFS_KEY_ITEMS         = 0x2441;
            OFS_BALLS             = 0x245C;
            OFS_PC_ITEMS          = 0x2476;
            OFS_CURRENT_BOX_INDEX = 0x26FC;
            OFS_BOX_NAMES         = 0x26FF;
            OFS_PARTY             = 0x28CC;
            OFS_POKEDEX_CAUGHT    = 0x2A8E;
            OFS_POKEDEX_SEEN      = 0x2AAE;
            OFS_CHECKSUM_END      = 0x2DAA;
            OFS_CHECKSUM_ONE      = 0x2DAB;
            OFS_CURRENT_BOX       = 0x2DAE;
            OFS_CHECKSUM_TWO      = 0x7E6B;
            OFS_GENDER            = 0xFFFFFFFF;
        }
        else if (japanese)
        {
            OFS_TIME_PLAYED  = 0x2034;
            OFS_PALETTE      = 0x204C;
            OFS_CURRENT_BOX  = 0x2D10;
            OFS_CHECKSUM_ONE = 0x2D0D;
            OFS_CHECKSUM_TWO = 0x7F0D;
            if (versionOfGame == GameVersion::C)
            {
                OFS_MONEY             = 0x23BE;
                OFS_BADGES            = 0x23C7;
                OFS_TM_POUCH          = 0x23C9;
                OFS_ITEMS             = 0x2402;
                OFS_KEY_ITEMS         = 0x242C;
                OFS_BALLS             = 0x2447;
                OFS_PC_ITEMS          = 0x2461;
                OFS_CURRENT_BOX_INDEX = 0x26E2;
                OFS_BOX_NAMES         = 0x26E5;
                OFS_PARTY             = 0x281A;
                OFS_POKEDEX_CAUGHT    = 0x29AA;
                OFS_POKEDEX_SEEN      = 0x29CA;
                OFS_CHECKSUM_END      = 0x2AE2;
                OFS_GENDER            = 0x8000;
            }
            else
            {
                OFS_MONEY             = 0x23BC;
                OFS_BADGES            = 0x23C5;
                OFS_TM_POUCH          = 0x23C7;
                OFS_ITEMS             = 0x2400;
                OFS_KEY_ITEMS         = 0x242A;
                OFS_BALLS             = 0x2445;
                OFS_PC_ITEMS          = 0x245F;
                OFS_CURRENT_BOX_INDEX = 0x2705;
                OFS_BOX_NAMES         = 0x2708;
                OFS_PARTY             = 0x283E;
                OFS_POKEDEX_CAUGHT    = 0x29CE;
                OFS_POKEDEX_SEEN      = 0x29EE;
                OFS_CHECKSUM_END      = 0x2C8B;
                OFS_GENDER            = 0xFFFFFFFF;
            }
        }
        else
        {
            if (versionOfGame == GameVersion::C)
            {
                OFS_TIME_PLAYED       = 0x2052;
                OFS_PALETTE           = 0x206A;
                OFS_MONEY             = 0x23DC;
                OFS_BADGES            = 0x23E5;
                OFS_TM_POUCH          = 0x23E7;
                OFS_ITEMS             = 0x2420;
                OFS_KEY_ITEMS         = 0x244A;
                OFS_BALLS             = 0x2465;
                OFS_PC_ITEMS          = 0x247F;
                OFS_CURRENT_BOX_INDEX = 0x2700;
                OFS_BOX_NAMES         = 0x2703;
                OFS_PARTY             = 0x2865;
                OFS_POKEDEX_CAUGHT    = 0x2A27;
                OFS_POKEDEX_SEEN      = 0x2A47;
                OFS_CHECKSUM_END      = 0x2B82;
                OFS_CHECKSUM_ONE      = 0x2D0D;
                OFS_CURRENT_BOX       = 0x2D10;
                OFS_GENDER            = 0x3E3D;
                OFS_CHECKSUM_TWO      = 0x7F0D;
            }
            else
            {
                OFS_TIME_PLAYED       = 0x2053;
                OFS_PALETTE           = 0x206B;
                OFS_MONEY             = 0x23DB;
                OFS_BADGES            = 0x23E4;
                OFS_TM_POUCH          = 0x23E6;
                OFS_ITEMS             = 0x241F;
                OFS_KEY_ITEMS         = 0x2449;
                OFS_BALLS             = 0x2464;
                OFS_PC_ITEMS          = 0x247E;
                OFS_CURRENT_BOX_INDEX = 0x2724;
                OFS_BOX_NAMES         = 0x2727;
                OFS_PARTY             = 0x288A;
                OFS_POKEDEX_CAUGHT    = 0x2A4C;
                OFS_POKEDEX_SEEN      = 0x2A6C;
                OFS_CHECKSUM_END      = 0x2D68;
                OFS_CHECKSUM_ONE      = 0x2D69;
                OFS_CURRENT_BOX       = 0x2D6C;
                OFS_CHECKSUM_TWO      = 0x7E6D;
                OFS_GENDER            = 0xFFFFFFFF;
            }
        }

        originalCurrentBox = currentBox();

        if (lang == Language::ENG)
        {
            lang = StringUtils::guessLanguage12(otName());
        }
    }

    std::tuple<GameVersion, Language, bool> Sav2::getVersion(const std::shared_ptr<u8[]>& dt)
    {
        GameVersion returnVersion = GameVersion::INVALID;
        Language returnLanguage   = Language::None;
        bool saveFound            = false;

        if (validList(dt, 0x288A, 20) && validList(dt, 0x2D6C, 20))
        {
            returnVersion  = GameVersion::GD;
            returnLanguage = Language::ENG; // as well as all other languages not enumerated
            saveFound      = true;
        }
        else if (validList(dt, 0x2865, 20) && validList(dt, 0x2D10, 20))
        {
            returnVersion  = GameVersion::C;
            returnLanguage = Language::ENG;
            saveFound      = true;
        }
        else if (validList(dt, 0x2D10, 30))
        {
            if (validList(dt, 0x283E, 30))
            {
                returnVersion  = GameVersion::GD;
                returnLanguage = Language::JPN;
                saveFound      = true;
            }
            else if (validList(dt, 0x281A, 30))
            {
                returnVersion  = GameVersion::C;
                returnLanguage = Language::JPN;
                saveFound      = true;
            }
        }
        else if (validList(dt, 0x28CC, 20) && validList(dt, 0x2DAE, 20))
        {
            returnVersion  = GameVersion::GD;
            returnLanguage = Language::KOR;
            saveFound      = true;
        }
        // there is no KOR crystal

        return {returnVersion, returnLanguage, saveFound};
    }

    bool Sav2::validList(const std::shared_ptr<u8[]>& dt, size_t ofs, u8 slot)
    {
        return (dt[ofs] <= 30) && (dt[ofs + 1 + dt[ofs]] == 0xFF);
    }

    // max length of string + terminator
    u8 Sav2::nameLength() const
    {
        return japanese ? 6 : 11;
    }

    u8 Sav2::PK2Length(void) const
    {
        return japanese ? PK2::JP_LENGTH_WITH_NAMES : PK2::INT_LENGTH_WITH_NAMES;
    }

    void Sav2::fixBoxes()
    {
        // Poor man's bubble sort-like thing
        for (int i = 0; i < maxBoxes(); i++)
        {
            for (int j = maxPkmInBox - 1; j > 0; j--)
            {
                auto checkPKM = pkm(i, j);
                if (checkPKM->species() == Species::None)
                {
                    continue;
                }
                auto prevPKM = pkm(i, j - 1);
                if (checkPKM->species() != Species::None && prevPKM->species() == Species::None)
                {
                    pkm(*checkPKM, i, j - 1, false);
                    pkm(*prevPKM, i, j, false);
                    j = maxPkmInBox; // reset loop
                }
            }
            fixBox(i);
        }
    }

    void Sav2::finishEditing()
    {
        // we just pretend the secondary data copy doesn't exist, it's never used as long as we get
        // the checksum for the primary copy right
        fixBoxes();
        fixParty();
        fixItemLists();
        std::copy(&data[OFS_CURRENT_BOX], &data[OFS_CURRENT_BOX] + boxSize,
            &data[boxStart(originalCurrentBox, false)]);
        if (currentBox() != originalCurrentBox)
        {
            std::copy(&data[boxStart(currentBox())], &data[boxStart(currentBox())] + boxSize,
                &data[OFS_CURRENT_BOX]);
        }

        // no, i don't know why only the checksum is little-endian. also idk why PKHeX destroys the
        // checksum for the secondary data copy
        u16 checksum = crypto::bytewiseSum16({&data[OFS_TID], OFS_CHECKSUM_END - OFS_TID + 1});
        LittleEndian::convertFrom<u16>(&data[OFS_CHECKSUM_ONE], checksum);
        LittleEndian::convertFrom<u16>(&data[OFS_CHECKSUM_TWO], checksum);

        originalCurrentBox = currentBox();
    }

    u16 Sav2::TID() const
    {
        return BigEndian::convertTo<u16>(&data[OFS_TID]);
    }

    void Sav2::TID(u16 v)
    {
        BigEndian::convertFrom<u16>(&data[OFS_TID], v);
    }

    GameVersion Sav2::version() const
    {
        return versionOfGame;
    }

    void Sav2::version(GameVersion v)
    {
        if ((v == GameVersion::C) ^ (versionOfGame != GameVersion::C))
        {
            versionOfGame = v;
        }
    }

    Gender Sav2::gender() const
    {
        if (versionOfGame == GameVersion::C)
        {
            if (data[OFS_GENDER] > 1)
            {
                return Gender::Male;
            }
            return Gender{data[OFS_GENDER]};
        }
        return Gender::Male;
    }

    void Sav2::gender(Gender v)
    {
        if (versionOfGame == GameVersion::C)
        {
            if (v <= Gender::Female)
            {
                data[OFS_GENDER] = u8(v);
            }

            // this is the palette used to draw the player, always 0 (red) for male and 1 (blue)
            // for female. it's present and 0 in Gold and Silver
            data[OFS_PALETTE] = u8(v);
        }
    }

    Language Sav2::language() const
    {
        return lang;
    }

    void Sav2::language(Language v)
    {
        if (((lang == Language::JPN) == (v == Language::JPN)) &&
            ((lang == Language::KOR) == (v == Language::KOR)))
        {
            lang = v;
        }
    }

    std::string Sav2::otName() const
    {
        return StringUtils::getString2(
            data.get(), OFS_NAME, japanese ? 6 : (korean ? 11 : 8), lang);
    }

    void Sav2::otName(const std::string_view& v)
    {
        StringUtils::setString2(data.get(), v, OFS_NAME, japanese ? 6 : (korean ? 11 : 8), lang);
    }

    // yay, they stopped using BCD
    u32 Sav2::money() const
    {
        return BigEndian::convertTo<u32>(&data[OFS_MONEY]) >> 8;
    }

    void Sav2::money(u32 v)
    {
        if (v > 999999)
        {
            v = 999999;
        }
        data[OFS_MONEY]     = v >> 16;
        data[OFS_MONEY + 1] = (v >> 8) & 0x00FF;
        data[OFS_MONEY + 2] = v & 0x0000FF;
    }

    u8 Sav2::badges() const
    {
        u8 sum         = 0;
        u16 badgeFlags = BigEndian::convertTo<u16>(&data[OFS_BADGES]);
        for (int i = 0; i < 16; i++)
        {
            sum += (badgeFlags & (1 << i)) >> i;
        }
        return sum;
    }

    u16 Sav2::playedHours() const
    {
        return BigEndian::convertTo<u16>(&data[OFS_TIME_PLAYED]);
    }

    void Sav2::playedHours(u16 v)
    {
        BigEndian::convertFrom<u16>(&data[OFS_TIME_PLAYED], v);
    }

    u8 Sav2::playedMinutes() const
    {
        return data[OFS_TIME_PLAYED + 2];
    }

    void Sav2::playedMinutes(u8 v)
    {
        data[OFS_TIME_PLAYED + 2] = v;
    }

    u8 Sav2::playedSeconds() const
    {
        return data[OFS_TIME_PLAYED + 3];
    }

    void Sav2::playedSeconds(u8 v)
    {
        data[OFS_TIME_PLAYED + 3] = v;
    }

    u8 Sav2::currentBox() const
    {
        return data[OFS_CURRENT_BOX_INDEX] & 0x7F;
    }

    void Sav2::currentBox(u8 v)
    {
        data[OFS_CURRENT_BOX_INDEX] = (data[OFS_CURRENT_BOX_INDEX] & 0x80) | (v & 0x7F);
    }

    u32 Sav2::boxOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (slot * PK2::BOX_LENGTH);
    }

    u32 Sav2::boxOtNameOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (maxPkmInBox * PK2::BOX_LENGTH) + (slot * nameLength());
    }

    u32 Sav2::boxNicknameOffset(u8 box, u8 slot) const
    {
        return boxDataStart(box) + (maxPkmInBox * PK2::BOX_LENGTH) +
               ((maxPkmInBox + slot) * nameLength());
    }

    u32 Sav2::partyOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (slot * PK2::PARTY_LENGTH);
    }

    u32 Sav2::partyOtNameOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (6 * PK2::PARTY_LENGTH) + (slot * nameLength());
    }

    u32 Sav2::partyNicknameOffset(u8 slot) const
    {
        return OFS_PARTY + 8 + (6 * PK2::PARTY_LENGTH) + ((6 + slot) * nameLength());
    }

    u32 Sav2::boxStart(u8 box, bool obeyCurrentBoxMechanics) const
    {
        if (box == originalCurrentBox && obeyCurrentBoxMechanics)
        {
            return OFS_CURRENT_BOX;
        }
        if (!japanese)
        {
            if (box < maxBoxes() / 2)
            {
                return 0x4000 + (box * boxSize);
            }
            box -= maxBoxes() / 2;
            return 0x6000 + (box * boxSize);
        }
        // huh
        else
        {
            if (box < 6)
            {
                return 0x4000 + (box * boxSize);
            }
            box -= 6;
            return 0x6000 + (box * boxSize);
        }
    }

    u32 Sav2::boxDataStart(u8 box, bool obeyCurrentBoxMechanics) const
    {
        return boxStart(box, obeyCurrentBoxMechanics) + maxPkmInBox + 2;
    }

    // the PK1 and PK2 formats used by the community start with magic bytes, the second being
    // species
    std::unique_ptr<PKX> Sav2::pkm(u8 slot) const
    {
        if (slot >= partyCount())
        {
            return emptyPkm();
        }
        // using the larger of the two sizes to not dynamically allocate
        u8 buffer[PK2::INT_LENGTH_WITH_NAMES] = {0x01, data[OFS_PARTY + 1 + slot], 0xFF};

        std::copy(
            &data[partyOffset(slot)], &data[partyOffset(slot)] + PK2::PARTY_LENGTH, buffer + 3);
        std::copy(&data[partyOtNameOffset(slot)], &data[partyOtNameOffset(slot)] + nameLength(),
            buffer + 3 + PK2::PARTY_LENGTH);
        std::copy(&data[partyNicknameOffset(slot)], &data[partyNicknameOffset(slot)] + nameLength(),
            buffer + 3 + PK2::PARTY_LENGTH + nameLength());

        StringUtils::gbStringFailsafe(buffer, 3 + PK2::PARTY_LENGTH, nameLength());
        StringUtils::gbStringFailsafe(buffer, 3 + PK2::PARTY_LENGTH + nameLength(), nameLength());

        auto pk2 = PKX::getPKM<Generation::TWO>(buffer, PK2Length());

        // has to be done now, since you can't do it in the PK2 initializer because of the nullptr
        // case
        if (language() == Language::KOR)
        {
            pk2->languageOverrideLimits(Language::KOR);
        }
        else if (language() != Language::JPN)
        {
            pk2->language(StringUtils::guessLanguage12(pk2->nickname()));
        }

        return pk2;
    }

    std::unique_ptr<PKX> Sav2::pkm(u8 box, u8 slot) const
    {
        if (slot >= maxPkmInBox || slot >= boxCount(box))
        {
            return emptyPkm();
        }

        u8 buffer[PK2::INT_LENGTH_WITH_NAMES] = {0x01, data[boxStart(box) + 1 + slot], 0xFF};

        std::copy(
            &data[boxOffset(box, slot)], &data[boxOffset(box, slot)] + PK2::BOX_LENGTH, buffer + 3);
        std::copy(&data[boxOtNameOffset(box, slot)],
            &data[boxOtNameOffset(box, slot)] + nameLength(), buffer + 3 + PK2::PARTY_LENGTH);
        std::copy(&data[boxNicknameOffset(box, slot)],
            &data[boxNicknameOffset(box, slot)] + nameLength(),
            buffer + 3 + PK2::PARTY_LENGTH + nameLength());

        StringUtils::gbStringFailsafe(buffer, 3 + PK2::PARTY_LENGTH, nameLength());
        StringUtils::gbStringFailsafe(buffer, 3 + PK2::PARTY_LENGTH + nameLength(), nameLength());

        auto pk2 = PKX::getPKM<Generation::TWO>(buffer, PK2Length());
        pk2->updatePartyData();

        if (language() == Language::KOR)
        {
            pk2->language(Language::KOR);
        }
        else if (language() != Language::JPN)
        {
            pk2->language(StringUtils::guessLanguage12(pk2->nickname()));
        }

        return pk2;
    }

    void Sav2::pkm(const PKX& pk, u8 slot)
    {
        if (pk.generation() == Generation::TWO)
        {
            if (slot >= partyCount())
            {
                if (slot > partyCount())
                {
                    pkm(*emptyPkm(), slot - 1);
                }
                partyCount(slot + 1);
            }

            auto pk2 = pk.partyClone();

            std::ranges::copy(
                pk2->rawData().subspan(3, PK2::PARTY_LENGTH), &data[partyOffset(slot)]);

            // korean has a page for INT characters
            if (((language() == Language::JPN) != (pk2->language() == Language::JPN)) ||
                ((pk2->language() == Language::KOR) && (language() != Language::KOR)))
            {
                StringUtils::setString2(&data[partyNicknameOffset(slot)],
                    StringUtils::toUpper(pk2->species().localize(language())), 0, nameLength(),
                    language());

                // check if it's the trade ot byte
                if (pk2->rawData()[3 + PK2::PARTY_LENGTH] == 0x5D)
                {
                    data[partyOtNameOffset(slot)]     = 0x5D;
                    data[partyOtNameOffset(slot) + 1] = 0x50;
                }
                else
                {
                    StringUtils::setString2(&data[partyOtNameOffset(slot)],
                        StringUtils::getTradeOT(language()), 0, nameLength(), language());
                }
            }
            else
            {
                std::ranges::copy(pk2->rawData().subspan(3 + PK2::PARTY_LENGTH, nameLength()),
                    &data[partyOtNameOffset(slot)]);
                std::ranges::copy(
                    pk2->rawData().subspan(3 + PK2::PARTY_LENGTH + nameLength(), nameLength()),
                    &data[partyNicknameOffset(slot)]);
            }

            data[OFS_PARTY + 1 + slot] = pk2->rawData()[1];
        }
    }

    void Sav2::pkm(const PKX& pk, u8 box, u8 slot, bool applyTrade)
    {
        if (slot >= maxPkmInBox)
        {
            return;
        }
        if (pk.generation() == Generation::TWO)
        {
            auto pk2 = pk.clone(); // note that partyClone and clone are equivalent
            if (applyTrade)
            {
                trade(*pk2);
            }

            if (slot >= boxCount(box))
            {
                if (slot > boxCount(box))
                {
                    pkm(*emptyPkm(), box, slot - 1, false);
                }
                boxCount(box, slot + 1);
            }

            std::ranges::copy(
                pk2->rawData().subspan(3, PK2::BOX_LENGTH), &data[boxOffset(box, slot)]);

            if (((language() == Language::JPN) != (pk2->language() == Language::JPN)) ||
                ((pk2->language() == Language::KOR) && (language() != Language::KOR)))
            {
                StringUtils::setString2(&data[boxNicknameOffset(box, slot)],
                    StringUtils::toUpper(pk2->species().localize(language())), 0, nameLength(),
                    language());

                if (pk2->rawData()[3 + PK2::BOX_LENGTH] == 0x5D)
                {
                    data[boxOtNameOffset(box, slot)]     = 0x5D;
                    data[boxOtNameOffset(box, slot) + 1] = 0x50;
                }
                else
                {
                    StringUtils::setString2(&data[boxOtNameOffset(box, slot)],
                        StringUtils::toUpper(StringUtils::getTradeOT(language())), 0, nameLength(),
                        language());
                }
            }
            else
            {
                std::ranges::copy(pk2->rawData().subspan(3 + PK2::PARTY_LENGTH, nameLength()),
                    &data[boxOtNameOffset(box, slot)]);
                std::ranges::copy(
                    pk2->rawData().subspan(3 + PK2::PARTY_LENGTH + nameLength(), nameLength()),
                    &data[boxNicknameOffset(box, slot)]);
            }

            data[boxStart(box) + 1 + slot] = pk2->rawData()[1];
        }
    }

    std::unique_ptr<PKX> Sav2::emptyPkm() const
    {
        return PKX::getPKM<Generation::TWO>(nullptr, PK2Length());
    }

    void Sav2::dex(const PKX& pk)
    {
        if (!(availableSpecies().count(pk.species()) > 0))
        {
            return;
        }
        setCaught(pk.species(), true);
        setSeen(pk.species(), true);
    }

    bool Sav2::getCaught(Species species) const
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        return FlagUtil::getFlag(data.get() + OFS_POKEDEX_CAUGHT, ofs, flag & 7);
    }

    void Sav2::setCaught(Species species, bool caught)
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        FlagUtil::setFlag(data.get() + OFS_POKEDEX_CAUGHT, ofs, flag & 7, caught);

        if (species == Species::Unown)
        {
            // set the form of Unown seen in the Pokedex to A
            if (data[OFS_POKEDEX_SEEN + 0x1F + 28] == 0)
            {
                data[OFS_POKEDEX_SEEN + 0x1F + 28] = 1;
            }

            // set all the caught flags for Unown forms (allegedly to prevent crash)
            for (int i = 1; i <= 26; i++)
            {
                data[OFS_POKEDEX_SEEN + 0x1F + i] = i;
            }
        }
    }

    bool Sav2::getSeen(Species species) const
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        return FlagUtil::getFlag(data.get() + OFS_POKEDEX_SEEN, ofs, flag & 7);
    }

    void Sav2::setSeen(Species species, bool seen)
    {
        int flag = u8(species) - 1;
        int ofs  = flag >> 3;
        FlagUtil::setFlag(data.get() + OFS_POKEDEX_SEEN, ofs, flag & 7, seen);

        if (species == Species::Unown && data[OFS_POKEDEX_SEEN + 0x1F + 28] == 0)
        {
            // set the form of Unown seen in the Pokedex to A
            data[OFS_POKEDEX_SEEN + 0x1F + 28] = 1; // A
        }
    }

    int Sav2::dexSeen() const
    {
        return std::count_if(availableSpecies().begin(), availableSpecies().end(),
            [this](const auto& spec) { return getSeen(spec); });
    }

    int Sav2::dexCaught() const
    {
        return std::count_if(availableSpecies().begin(), availableSpecies().end(),
            [this](const auto& spec) { return getCaught(spec); });
    }

    std::string Sav2::boxName(u8 box) const
    {
        int boxNameLength = korean ? 17 : 9;
        return StringUtils::getString2(
            data.get(), OFS_BOX_NAMES + (box * boxNameLength), boxNameLength, lang);
    }

    void Sav2::boxName(u8 box, const std::string_view& name)
    {
        int boxNameLength = korean ? 17 : 9;
        StringUtils::setString2(data.get(), name, OFS_BOX_NAMES + (box * boxNameLength),
            boxNameLength, lang, boxNameLength);
    }

    u8 Sav2::partyCount() const
    {
        return data[OFS_PARTY];
    }

    void Sav2::partyCount(u8 count)
    {
        data[OFS_PARTY] = count;
    }

    u8 Sav2::boxCount(u8 box) const
    {
        return data[boxStart(box)];
    }

    void Sav2::boxCount(u8 box, u8 count)
    {
        data[boxStart(box)] = count;
    }

    void Sav2::fixBox(u8 box)
    {
        u8 count = 0;
        while (count < maxPkmInBox)
        {
            if (pkm(box, count)->species() == Species::None)
            {
                break;
            }

            if (pkm(box, count)->egg())
            {
                data[boxStart(box) + 1 + count] = 0xFD;
            }
            else
            {
                data[boxStart(box) + 1 + count] = data[boxOffset(box, count)];
            }

            count++;
        }
        data[boxStart(box) + 1 + count] = 0xFF;
        data[boxStart(box)]             = count;
    }

    void Sav2::fixParty()
    {
        Sav::fixParty();
        u8 count = 0;
        while (count < 6)
        {
            if (pkm(count)->species() == Species::None)
            {
                break;
            }

            if (pkm(count)->egg())
            {
                data[OFS_PARTY + 1 + count] = 0xFD;
            }
            else
            {
                data[OFS_PARTY + 1 + count] = data[partyOffset(count)];
            }

            count++;
        }
        data[OFS_PARTY + 1 + count] = 0xFF;
        data[OFS_PARTY]             = count;
    }

    int Sav2::maxSlot() const
    {
        return maxBoxes() * maxPkmInBox;
    }

    int Sav2::maxBoxes() const
    {
        return japanese ? 9 : 14;
    }

    void Sav2::item(const Item& tItem, Pouch pouch, u16 slot)
    {
        if (slot >= pouchEntryCount(pouch))
        {
            pouchEntryCount(pouch, slot + 1);
        }
        Item2 item = static_cast<Item2>(tItem);
        auto write = item.bytes();
        int index  = 0; // for TMs
        switch (pouch)
        {
            case Pouch::TM:
                while (tmItems2()[index] != write[0])
                {
                    index++;
                }
                data[OFS_TM_POUCH + index] = write[1];
                break;
            case Pouch::NormalItem:
                std::copy(write.begin(), write.end(), &data[OFS_ITEMS + 1 + (slot * 2)]);
                break;
            case Pouch::KeyItem:
                data[OFS_KEY_ITEMS + 1 + slot] = write[0];
                break;
            case Pouch::Ball:
                std::copy(write.begin(), write.end(), &data[OFS_BALLS + 1 + (slot * 2)]);
                break;
            case Pouch::PCItem:
                std::copy(write.begin(), write.end(), &data[OFS_PC_ITEMS + 1 + (slot * 2)]);
                break;
            default:
                return;
        }
    }

    std::unique_ptr<Item> Sav2::item(Pouch pouch, u16 slot) const
    {
        if (slot >= pouchEntryCount(pouch))
        {
            return std::make_unique<Item2>(nullptr);
        }
        std::unique_ptr<Item2> returnVal;
        std::array<u8, 2> itemData;
        switch (pouch)
        {
            case Pouch::TM:
                // apparently they store the counts of the TMs
                itemData[0] = tmItems2()[slot];
                itemData[1] = data[OFS_TM_POUCH + slot];
                returnVal   = std::make_unique<Item2>(itemData.data());
                break;
            case Pouch::NormalItem:
                returnVal = std::make_unique<Item2>(&data[OFS_ITEMS + 1 + (slot * 2)]);
                break;
            case Pouch::KeyItem:
                itemData[0] = data[OFS_KEY_ITEMS + 1 + slot];
                itemData[1] = 1;
                returnVal   = std::make_unique<Item2>(itemData.data());
                break;
            case Pouch::Ball:
                returnVal = std::make_unique<Item2>(&data[OFS_BALLS + 1 + (slot * 2)]);
                break;
            case Pouch::PCItem:
                returnVal = std::make_unique<Item2>(&data[OFS_PC_ITEMS + 1 + (slot * 2)]);
                break;
            default:
                return nullptr;
        }
        // 0xFF is a list terminator. In a normal game state it will be in an ID slot.
        if (returnVal->id2() == 0xFF)
        {
            return std::make_unique<Item2>(nullptr);
        }
        return returnVal;
    }

    SmallVector<std::pair<Sav::Pouch, int>, 15> Sav2::pouches() const
    {
        return {
            std::pair{Pouch::TM,         57},
            std::pair{Pouch::NormalItem, 20},
            std::pair{Pouch::KeyItem,    26},
            std::pair{Pouch::Ball,       12},
            std::pair{Pouch::PCItem,     50}
        };
    }

    std::span<const int> Sav2::tmItems2() const
    {
        for (const auto& i : validItems2())
        {
            if (i.first == Pouch::TM)
            {
                return i.second;
            }
        }

        return {};
    }

    SmallVector<std::pair<pksm::Sav::Pouch, std::span<const int>>, 15> Sav2::validItems2() const
    {
        static constexpr std::array validItems = {// TMs
            191, 192, 193, 194, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208,
            209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 221, 222, 223, 224, 225, 226,
            227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
            244, 245, 246, 247, 248, 249,

            // Normal
            3, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 46, 47, 48, 49, 51, 52, 53, 57,
            60, 62, 63, 64, 65, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
            89, 91, 92, 93, 94, 95, 96, 97, 98, 99, 101, 102, 103, 104, 105, 106, 107, 108, 109,
            110, 111, 112, 113, 114, 117, 118, 119, 121, 122, 123, 124, 125, 126, 131, 132, 138,
            139, 140, 143, 144, 146, 150, 151, 152, 156, 158, 163, 167, 168, 169, 170, 172, 173,
            174, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,

            // Ball
            1, 2, 4, 5, 157, 159, 160, 161, 164, 165, 166,

            // Key
            7, 54, 55, 58, 59, 61, 66, 67, 68, 69, 71, 127, 128, 130, 133, 134, 175, 178,

            // Key for Crystal
            70, 115, 116, 129};

        return {
            std::pair{Pouch::TM,         std::span{validItems.begin(), validItems.begin() + 57}                                   },
            std::pair{
                      Pouch::NormalItem, std::span{validItems.begin() + 57, validItems.begin() + 188}                             },
            std::pair{Pouch::Ball,       std::span{validItems.begin() + 188, validItems.begin() + 199}                            },
            std::pair{
                      Pouch::KeyItem,    std::span{validItems.begin() + 199,
                                    validItems.end() - (version() == GameVersion::C ? 0 : 4)}},
            std::pair{
                      Pouch::PCItem,     std::span{validItems.begin(),
                                   validItems.end() - (version() == GameVersion::C ? 0 : 4)}  }
        };
    }

    SmallVector<std::pair<pksm::Sav::Pouch, std::span<const int>>, 15> Sav2::validItems() const
    {
        static constexpr std::array validItems = {// TMs
            328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344,
            345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361,
            362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 420,
            421, 422, 423, 424, 425, 426,

            // Normal
            213, 81, 18, 19, 20, 21, 22, 23, 24, 25, 26, 17, 78, 79, 41, 82, 83, 84, 45, 46, 47, 48,
            256, 49, 50, 60, 85, 257, 92, 63, 27, 28, 29, 55, 76, 77, 56, 30, 31, 32, 57, 58, 59,
            61, 216, 891, 51, 38, 39, 40, 33, 217, 151, 890, 237, 244, 149, 153, 152, 245, 221, 156,
            150, 485, 86, 87, 222, 487, 223, 486, 488, 224, 243, 248, 490, 241, 491, 489, 240, 473,
            259, 228, 246, 242, 157, 88, 89, 229, 247, 504, 239, 258, 230, 34, 35, 36, 37, 238, 231,
            90, 91, 249, 43, 232, 233, 250, 234, 154, 235, 44, 236, 80, 252, 155, 158,

            // Ball
            1, 2, 3, 4, 495, 493, 494, 492, 497, 498, 496,

            // Key
            450, 444, 445, 446, 447, 478, 464, 456, 484, 482, 475, 481, 479, 476, 480, 477, 483,

            // Key for crystal
            474, 472};

        return {
            std::pair{Pouch::TM,         std::span{validItems.begin(), validItems.begin() + 57}                                   },
            std::pair{
                      Pouch::NormalItem, std::span{validItems.begin() + 57, validItems.begin() + 172}                             },
            std::pair{Pouch::Ball,       std::span{validItems.begin() + 172, validItems.begin() + 183}                            },
            std::pair{
                      Pouch::KeyItem,    std::span{validItems.begin() + 183,
                                    validItems.end() - (version() == GameVersion::C ? 0 : 2)}},
            std::pair{
                      Pouch::PCItem,     std::span{validItems.begin(),
                                   validItems.end() - (version() == GameVersion::C ? 0 : 2)}  }
        };
    }

    u8 Sav2::pouchEntryCount(Pouch pouch) const
    {
        switch (pouch)
        {
            // the TM/HM pocket is a bytefield for all of the TMs and HMs
            case Pouch::TM:
                return 255;
            case Pouch::NormalItem:
                return data[OFS_ITEMS] > 20 ? 0 : data[OFS_ITEMS];
            case Pouch::KeyItem:
                return data[OFS_KEY_ITEMS] > 26 ? 0 : data[OFS_KEY_ITEMS];
            case Pouch::Ball:
                return data[OFS_BALLS] > 12 ? 0 : data[OFS_BALLS];
            case Pouch::PCItem:
                return data[OFS_PC_ITEMS] > 50 ? 0 : data[OFS_PC_ITEMS];
            default:
                return 0;
        }
    }

    void Sav2::pouchEntryCount(Pouch pouch, u8 v)
    {
        switch (pouch)
        {
            case Pouch::NormalItem:
                data[OFS_ITEMS] = v;
                break;
            case Pouch::KeyItem:
                data[OFS_KEY_ITEMS] = v;
                break;
            case Pouch::Ball:
                data[OFS_BALLS] = v;
                break;
            case Pouch::PCItem:
                data[OFS_PC_ITEMS] = v;
                break;
            case Pouch::TM:
            default:
                return;
        }
    }

    void Sav2::fixItemLists()
    {
        // the TM pouch has neither a terminator nor a count

        u8 count = 0;
        while (count < 20 && item(Pouch::NormalItem, count)->id() != 0)
        {
            count++;
        }
        pouchEntryCount(Pouch::NormalItem, count);
        data[OFS_ITEMS + 1 + (count * 2)] = 0xFF;

        count = 0;
        while (count < 26 && item(Pouch::KeyItem, count)->id() != 0)
        {
            count++;
        }
        pouchEntryCount(Pouch::KeyItem, count);
        data[OFS_KEY_ITEMS + 1 + count] = 0xFF;

        count = 0;
        while (count < 12 && item(Pouch::Ball, count)->id() != 0)
        {
            count++;
        }
        pouchEntryCount(Pouch::Ball, count);
        data[OFS_BALLS + 1 + (count * 2)] = 0xFF;

        count = 0;
        while (count < 50 && item(Pouch::PCItem, count)->id() != 0)
        {
            count++;
        }
        pouchEntryCount(Pouch::PCItem, count);
        data[OFS_PC_ITEMS + 1 + (count * 2)] = 0xFF;
    }
}
