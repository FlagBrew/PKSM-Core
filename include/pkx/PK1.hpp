/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2021 Bernardo Giordano, Admiral Fish, piepie62
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

#ifndef PK1_HPP
#define PK1_HPP

#include "personal/personal.hpp"
#include "pkx/PK3.hpp"
#include "pkx/PK4.hpp"
#include "pkx/PK5.hpp"
#include "pkx/PK6.hpp"
#include "pkx/PK7.hpp"
#include "pkx/PK8.hpp"
#include "pkx/PKX.hpp"

namespace pksm
{
    class PK1 : public PKX
    {
    protected:
        GameVersion versionOfGame = GameVersion::RD; //not even PKHeX tries to do better

    private:
        // TODO: below
        bool japanese = false;
        Language lang;
        [[nodiscard]] int eggYear(void) const override { return 1900; }
        void eggYear(int) override {}
        [[nodiscard]] int eggMonth(void) const override { return 1; }
        void eggMonth(int) override {}
        [[nodiscard]] int eggDay(void) const override { return 1; }
        void eggDay(int) override {}
        [[nodiscard]] int metYear(void) const override { return 1900; }
        void metYear(int) override {}
        [[nodiscard]] int metMonth(void) const override { return 1; }
        void metMonth(int) override {}
        [[nodiscard]] int metDay(void) const override { return 1; }
        void metDay(int) override {}

    public:
        // frustratingly, nickname and ot name are not stored contiguously
        static constexpr size_t BOX_LENGTH              = 33;
        static constexpr size_t PARTY_LENGTH            = 44;
        static constexpr size_t BOX_LENGTH_WITH_NAMES   = 44;
        static constexpr size_t PARTY_LENGTH_WITH_NAMES = 55;
        static constexpr Species FORMAT_SPECIES_LIMIT = Species::Mew;

        PK1(PrivateConstructor, u8* dt, bool party = false, bool directAccess = false);

        [[nodiscard]] std::string_view extension() const override { return ".pk1"; }

        // TODO: below, especially Gen VII
        [[nodiscard]] std::unique_ptr<PK4> convertToG4(Sav& save) const override { return nullptr; }
        [[nodiscard]] std::unique_ptr<PK5> convertToG5(Sav& save) const override { return nullptr; }
        [[nodiscard]] std::unique_ptr<PK6> convertToG6(Sav& save) const override { return nullptr; }
        [[nodiscard]] std::unique_ptr<PK7> convertToG7(Sav& save) const override { return nullptr; }
        // std::unique_ptr<PB7> convertToLGPE(Sav& save) const override { return nullptr; }
        [[nodiscard]] std::unique_ptr<PK8> convertToG8(Sav& save) const override { return nullptr; }

        [[nodiscard]] std::unique_ptr<PKX> clone(void) const override;

        [[nodiscard]] Generation generation(void) const override { return Generation::ONE; }
        [[nodiscard]] bool isParty(void) const override { return getLength() == PARTY_LENGTH; }
        void decrypt(void) override {}
        void encrypt(void) override {}
        [[nodiscard]] bool isEncrypted(void) const override { return false; }

        [[nodiscard]] u32 encryptionConstant(void) const override { return 0; }
        void encryptionConstant(u32 v) override {}
        // friendship is only a thing for Pikachu in Yellow, and not even PKHeX bothers
        [[nodiscard]] u8 currentFriendship(void) const override { return 0; }
        void currentFriendship(u8 v) override {}
        [[nodiscard]] u8 currentHandler(void) const override { return 0; }
        void currentHandler(u8) override {}
        [[nodiscard]] u8 abilityNumber(void) const override { return 0; }
        void abilityNumber(u8 v) override {}
        void setAbility(u8 abilityNumber) override {}

        [[nodiscard]] u32 PID(void) const override { return 0; }
        void PID(u32 v) override {}
        [[nodiscard]] u16 TID(void) const override;
        void TID(u16 v) override;
        [[nodiscard]] u16 SID(void) const override { return 0; }
        void SID(u16 v) override {}
        [[nodiscard]] std::string nickname(void) const override;
        void nickname(const std::string_view& v) override;
        [[nodiscard]] Language language(void) const override;
        void language(Language v) override;
        // [[nodiscard]] bool flagIsBadEgg(void) const;
        // void flagIsBadEgg(bool v);
        // [[nodiscard]] bool flagHasSpecies(void) const;
        // void flagHasSpecies(bool v);
        // [[nodiscard]] bool flagIsEgg(void) const;
        // void flagIsEgg(bool v);
        [[nodiscard]] std::string otName(void) const override;
        void otName(const std::string_view& v) override;
        [[nodiscard]] u16 markValue(void) const override { return 0; }
        void markValue(u16 v) override {}
        [[nodiscard]] u16 checksum(void) const override { return 0; }
        void checksum(u16 v) override {}
        [[nodiscard]] u16 sanity(void) const override { return 0; }
        void sanity(u16 v) override {}

        [[nodiscard]] u8 speciesID1(void) const;
        void speciesID1(u8 v);
        [[nodiscard]] Species species(void) const override;
        void species(Species v) override;
        [[nodiscard]] u16 catchRate(void) const;
        void catchRate(u16 v);
        [[nodiscard]] u16 heldItem(void) const override { return 0; }
        void heldItem(u16 v) override {}
        void heldItem(const Item& item) override {}
        [[nodiscard]] u32 experience(void) const override;
        void experience(u32 v) override;
        [[nodiscard]] u8 otFriendship(void) const override { return 0; }
        void otFriendship(u8 v) override {}
        [[nodiscard]] Ability ability(void) const override { return Ability::None; }
        void ability(Ability v) override {}
        // [[nodiscard]] bool abilityBit(void) const;
        // void abilityBit(bool v);

        // EVs in Gen I are u16s
        [[nodiscard]] u8 ev(Stat ev) const override { return 0; }
        void ev(Stat ev, u8 v) override {}
        [[nodiscard]] u16 statExperience(Stat se) const;
        void statExperience(Stat se, u16 v);

        [[nodiscard]] u8 contest(u8 contest) const override { return 0; }
        void contest(u8 contest, u8 v) override {}
        [[nodiscard]] bool hasRibbon(Ribbon rib) const override { return false; }
        [[nodiscard]] bool ribbon(Ribbon rib) const override { return false; }
        void ribbon(Ribbon rib, bool v) override {}
        // [[nodiscard]] u8 contestRibbonCount(u8 contest) const;
        // void contestRibbonCount(u8 contest, u8 count);

        [[nodiscard]] Move move(u8 move) const override;
        void move(u8 move, Move v) override;
        [[nodiscard]] Move relearnMove(u8) const override { return Move::None; }
        void relearnMove(u8, Move) override {}
        [[nodiscard]] u8 PP(u8 move) const override;
        void PP(u8 move, u8 v) override;
        [[nodiscard]] u8 PPUp(u8 move) const override;
        void PPUp(u8 move, u8 v) override;
        [[nodiscard]] u8 iv(Stat iv) const override;
        void iv(Stat iv, u8 v) override;
        [[nodiscard]] bool egg(void) const override { return false; }
        void egg(bool v) override {}
        [[nodiscard]] bool nicknamed(void) const override;
        void nicknamed(bool v) override {}
        [[nodiscard]] bool fatefulEncounter(void) const override { return false; }
        void fatefulEncounter(bool v) override {}
        [[nodiscard]] Gender gender(void) const override { return Gender::Genderless; }
        void gender(Gender g) override {}
        [[nodiscard]] u16 alternativeForm(void) const override { return 0; }
        void alternativeForm(u16 v) override {}
        // u8 shinyLeaf(void) const;
        // void shinyLeaf(u8 v);
        [[nodiscard]] Nature nature(void) const override { return Nature::Bashful; }
        void nature(Nature v) override {}
        // bool hiddenAbility(void) const;
        // void hiddenAbility(bool v);

        [[nodiscard]] bool hyperTrain(Stat) const override { return false; }
        void hyperTrain(Stat, bool) override {}

        [[nodiscard]] GameVersion version(void) const override { return versionOfGame; }
        void version(GameVersion v) override { versionOfGame = v; }

        [[nodiscard]] u16 eggLocation(void) const override { return 0; }
        void eggLocation(u16) override {}
        [[nodiscard]] u16 metLocation(void) const override { return 0; }
        void metLocation(u16 v) override {}
        [[nodiscard]] u8 pkrs(void) const override { return 0; }
        void pkrs(u8 v) override {}
        [[nodiscard]] u8 pkrsDays(void) const override { return 0; }
        void pkrsDays(u8 v) override {}
        [[nodiscard]] u8 pkrsStrain(void) const override { return 0; }
        void pkrsStrain(u8 v) override {}
        [[nodiscard]] Ball ball(void) const override { return Ball::Poke; }
        void ball(Ball v) override {}
        [[nodiscard]] u8 metLevel(void) const override { return 0; }
        void metLevel(u8 v) override {}
        [[nodiscard]] Gender otGender(void) const override { return Gender::Male; }
        void otGender(Gender v) override {}
        // [[nodiscard]] u8 encounterType(void) const;
        // void encounterType(u8 v);
        // [[nodiscard]] u8 characteristic(void) const;

        void refreshChecksum(void) override {}
        [[nodiscard]] Type hpType(void) const override { return Type::Normal; }
        void hpType(Type v) override {}
        [[nodiscard]] u16 TSV(void) const override { return 0; }
        [[nodiscard]] u16 PSV(void) const override { return 0; }
        [[nodiscard]] u8 level(void) const override;
        void level(u8 v) override;
        [[nodiscard]] bool shiny(void) const override;
        void shiny(bool v) override;
        [[nodiscard]] u16 formSpecies(void) const override;
        [[nodiscard]] u16 stat(Stat stat) const override;

        [[nodiscard]] int partyCurrHP(void) const override;
        void partyCurrHP(u16 v) override;
        [[nodiscard]] int partyStat(Stat stat) const override;
        void partyStat(Stat stat, u16 v) override;
        [[nodiscard]] int partyLevel() const override;
        void partyLevel(u8 v) override;
        void updatePartyData(void) override;

        void writeG1Types(void);

        [[nodiscard]] inline u8 baseHP(void) const override
        {
            return PersonalRGBY::baseHP(u8(species()));
        }
        [[nodiscard]] inline u8 baseAtk(void) const override
        {
            return PersonalRGBY::baseAtk(u8(species()));
        }
        [[nodiscard]] inline u8 baseDef(void) const override
        {
            return PersonalRGBY::baseDef(u8(species()));
        }
        [[nodiscard]] inline u8 baseSpe(void) const override
        {
            return PersonalRGBY::baseSpe(u8(species()));
        }
        [[nodiscard]] inline u8 baseSpa(void) const override
        {
            return PersonalRGBY::baseSpad(u8(species()));
        }
        [[nodiscard]] inline u8 baseSpd(void) const override
        {
            return PersonalRGBY::baseSpad(u8(species()));
        }
        [[nodiscard]] inline Type type1(void) const override
        {
            return PersonalRGBY::type1(u8(species()));
        }
        [[nodiscard]] inline Type type2(void) const override
        {
            return PersonalRGBY::type2(u8(species()));
        }
        [[nodiscard]] inline u8 genderType(void) const override
        {
            return 0;
        }
        [[nodiscard]] inline u8 baseFriendship(void) const override
        {
            return 0;
        }
        [[nodiscard]] inline u8 baseCatchRate(void) const
        {
            return PersonalRGBY::catchRate(u8(species()));
        }
        [[nodiscard]] inline u8 expType(void) const override
        {
            return PersonalRGBY::expType(u8(species()));
        }
        [[nodiscard]] inline Ability abilities(u8 n) const override
        {
            return Ability::None;
        }
        [[nodiscard]] inline u16 formStatIndex(void) const override { return 0; }
    };
}

#endif
