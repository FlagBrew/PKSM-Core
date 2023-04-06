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

#ifndef PK2_HPP
#define PK2_HPP

#include "personal/personal.hpp"
#include "pkx/PKX.hpp"

namespace pksm
{
    class PK2 : public PKX
    {
    private:
        u8* shiftedData;
        Language lang;
        GameVersion versionOfGame = GameVersion::GD;
        bool japanese;
        bool korean = false;

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
        static constexpr size_t JP_LENGTH_WITH_NAMES  = 63;
        static constexpr size_t INT_LENGTH_WITH_NAMES = 73;

        static constexpr size_t BOX_LENGTH   = 32;
        static constexpr size_t PARTY_LENGTH = 48;

        static constexpr Species FORMAT_SPECIES_LIMIT = Species::Celebi;

        PK2(PrivateConstructor, u8* dt, bool japanese = false, bool directAccess = false);

        [[nodiscard]] std::string_view extension() const override { return ".pk2"; }

        [[nodiscard]] std::unique_ptr<PK1> convertToG1(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK3> convertToG3(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK4> convertToG4(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK5> convertToG5(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK6> convertToG6(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK7> convertToG7(Sav& save) const override;
        // [[nodiscard]] std::unique_ptr<PB7> convertToLGPE(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK8> convertToG8(Sav& save) const override;

        [[nodiscard]] std::unique_ptr<PKX> clone(void) const override;

        [[nodiscard]] Generation generation(void) const override { return Generation::TWO; }

        [[nodiscard]] bool isParty(void) const override { return true; }

        void decrypt(void) override {}

        void encrypt(void) override {}

        [[nodiscard]] bool isEncrypted(void) const override { return false; }

        [[nodiscard]] u32 encryptionConstant(void) const override { return 0; }

        void encryptionConstant(u32 v) override {}

        [[nodiscard]] PKXHandler currentHandler(void) const override { return PKXHandler::OT; }

        void currentHandler(PKXHandler) override {}

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
        [[nodiscard]] std::string nicknameTransporter(void) const;
        void nickname(const std::string_view& v) override;
        [[nodiscard]] Language language(void) const override;
        void language(Language v) override;
        void languageOverrideLimits(Language v);
        [[nodiscard]] std::string otName(void) const override;
        void otName(const std::string_view& v) override;
        [[nodiscard]] std::string transferOT(Language lang) const;

        [[nodiscard]] u16 markValue(void) const override { return 0; }

        void markValue(u16 v) override {}

        [[nodiscard]] u16 checksum(void) const override { return 0; }

        void checksum(u16 v) override {}

        [[nodiscard]] u16 sanity(void) const override { return 0; }

        void sanity(u16 v) override {}

        [[nodiscard]] Species species(void) const override;
        void species(Species v) override;
        [[nodiscard]] u8 heldItem2(void) const;
        void heldItem2(u8 v);
        [[nodiscard]] u16 heldItem(void) const override;
        void heldItem(u16 v) override;
        void heldItem(const Item& item) override;
        [[nodiscard]] u32 experience(void) const override;
        void experience(u32 v) override;
        [[nodiscard]] u8 otFriendship(void) const override;
        void otFriendship(u8 v) override;

        [[nodiscard]] u8 htFriendship(void) const override { return 0; }

        void htFriendship(u8 v) override {}

        [[nodiscard]] Ability ability(void) const override { return Ability::None; }

        void ability(Ability v) override {}

        // EVs in Gen I and II are u16s
        [[nodiscard]] u16 ev(Stat ev) const override;
        void ev(Stat ev, u16 v) override;

        [[nodiscard]] u8 contest(u8) const override { return 0; }

        void contest(u8 contest, u8 v) override {}

        [[nodiscard]] bool hasRibbon(Ribbon) const override { return false; }

        [[nodiscard]] bool ribbon(Ribbon) const override { return false; }

        void ribbon(Ribbon rib, bool v) override {}

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
        [[nodiscard]] bool egg(void) const override;
        void egg(bool v) override;
        [[nodiscard]] bool nicknamed(void) const override;

        void nicknamed(bool v) override {}

        [[nodiscard]] bool fatefulEncounter(void) const override { return false; }

        void fatefulEncounter(bool v) override {}

        [[nodiscard]] Gender gender(void) const override;
        void gender(Gender g) override;
        [[nodiscard]] u16 alternativeForm(void) const override;
        void alternativeForm(u16 v) override;
        [[nodiscard]] Nature nature(void) const override;
        void nature(Nature v) override;

        [[nodiscard]] bool hyperTrain(Stat) const override { return false; }

        void hyperTrain(Stat, bool) override {}

        [[nodiscard]] GameVersion version(void) const override;
        void version(GameVersion v) override;

        [[nodiscard]] u16 catchData(void) const;
        void catchData(u16 v);

        [[nodiscard]] u16 eggLocation(void) const override { return 0; }

        void eggLocation(u16) override {}

        [[nodiscard]] u16 metLocation(void) const override;
        void metLocation(u16 v) override;
        [[nodiscard]] u8 pkrs(void) const override;
        void pkrs(u8 v) override;
        [[nodiscard]] u8 pkrsDays(void) const override;
        void pkrsDays(u8 v) override;
        [[nodiscard]] u8 pkrsStrain(void) const override;
        void pkrsStrain(u8 v) override;

        [[nodiscard]] Ball ball(void) const override { return Ball::Poke; }

        void ball(Ball v) override {}

        [[nodiscard]] u8 metLevel(void) const override;
        void metLevel(u8 v) override;
        [[nodiscard]] Gender otGender(void) const override;
        void otGender(Gender v) override;

        // 1: Morning, 2: Day, 3: Night
        [[nodiscard]] u8 metTime() const;
        void metTime(u8 v);

        void refreshChecksum(void) override {}

        [[nodiscard]] Type hpType(void) const override;
        void hpType(Type v) override;

        [[nodiscard]] u16 TSV(void) const override { return 0; }

        [[nodiscard]] u16 PSV(void) const override { return 0xFFFF; }

        [[nodiscard]] u8 level(void) const override;
        void level(u8 v) override;
        [[nodiscard]] bool shiny(void) const override;
        void shiny(bool v) override;
        [[nodiscard]] u16 formSpecies(void) const override;

        [[nodiscard]] int partyCurrHP(void) const override;
        void partyCurrHP(u16 v) override;
        [[nodiscard]] int partyStat(Stat stat) const override;
        void partyStat(Stat stat, u16 v) override;
        [[nodiscard]] int partyLevel() const override;
        void partyLevel(u8 v) override;
        void updatePartyData(void) override;

        u16 secondaryStatCalc(Stat stat) const override { return ev(stat); }

        void secondaryStatCalc(Stat stat, u16 v) override { ev(stat, v); }

        u16 maxSecondaryStatCalc(void) const override { return maxEV(); }

        u32 maxSecondaryStatCalcTotal(void) const override { return maxEVTotal(); }

        u8 maxIV(void) const override { return 0xF; }

        u16 maxEV(void) const override { return 0xFFFF; }

        u32 maxEVTotal(void) const override { return 0xFFFFFFFF; }

        [[nodiscard]] inline u8 baseHP(void) const override
        {
            return PersonalGSC::baseHP(u8(species()));
        }

        [[nodiscard]] inline u8 baseAtk(void) const override
        {
            return PersonalGSC::baseAtk(u8(species()));
        }

        [[nodiscard]] inline u8 baseDef(void) const override
        {
            return PersonalGSC::baseDef(u8(species()));
        }

        [[nodiscard]] inline u8 baseSpe(void) const override
        {
            return PersonalGSC::baseSpe(u8(species()));
        }

        [[nodiscard]] inline u8 baseSpa(void) const override
        {
            return PersonalGSC::baseSpa(u8(species()));
        }

        [[nodiscard]] inline u8 baseSpd(void) const override
        {
            return PersonalGSC::baseSpd(u8(species()));
        }

        [[nodiscard]] inline Type type1(void) const override
        {
            return PersonalGSC::type1(u8(species()));
        }

        [[nodiscard]] inline Type type2(void) const override
        {
            return PersonalGSC::type2(u8(species()));
        }

        [[nodiscard]] inline u8 genderType(void) const override
        {
            return PersonalGSC::gender(u8(species()));
        }

        [[nodiscard]] inline u8 baseFriendship(void) const override { return 70; }

        [[nodiscard]] inline u8 expType(void) const override
        {
            return PersonalGSC::expType(u8(species()));
        }

        [[nodiscard]] inline Ability abilities(u8) const override { return Ability::None; }

        [[nodiscard]] inline u16 formStatIndex(void) const override { return 0; }

    private:
        [[nodiscard]] u16 statImpl(Stat stat) const override;
    };
}

#endif
