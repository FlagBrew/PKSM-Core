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

#ifndef PA9_HPP
#define PA9_HPP

#include "personal/personal.hpp"
#include "pkx/PKX.hpp"

namespace pksm
{
    class PA9 : public PKX
    {
    private:
        static constexpr size_t BLOCK_LENGTH     = 80;
        static constexpr size_t ENCRYPTION_START = 8;

        [[nodiscard]] int eggYear(void) const override;
        void eggYear(int v) override;
        [[nodiscard]] int eggMonth(void) const override;
        void eggMonth(int v) override;
        [[nodiscard]] int eggDay(void) const override;
        void eggDay(int v) override;
        [[nodiscard]] int metYear(void) const override;
        void metYear(int v) override;
        [[nodiscard]] int metMonth(void) const override;
        void metMonth(int v) override;
        [[nodiscard]] int metDay(void) const override;
        void metDay(int v) override;

    public:
        static constexpr size_t BOX_LENGTH            = 0x148;
        static constexpr size_t PARTY_LENGTH          = 0x158;
        static constexpr Species FORMAT_SPECIES_LIMIT = Species::Pecharunt;

        PA9(PrivateConstructor, u8* dt, bool party = false, bool directAccess = false);

        [[nodiscard]] std::string_view extension() const override { return ".pa9"; }

        [[nodiscard]] std::unique_ptr<PKX> clone(void) const override;
        [[nodiscard]] std::unique_ptr<PKX> convertToG9(Sav& save) const override;
        [[nodiscard]] std::unique_ptr<PK8> convertToG8(Sav& save) const override;

        [[nodiscard]] Generation generation(void) const override;
        void decrypt(void) override;
        void encrypt(void) override;
        [[nodiscard]] bool isEncrypted(void) const override;

        [[nodiscard]] bool isParty(void) const override { return getLength() == PARTY_LENGTH; }

        [[nodiscard]] u32 encryptionConstant(void) const override;
        void encryptionConstant(u32 v) override;
        [[nodiscard]] u16 sanity(void) const override;
        void sanity(u16 v) override;
        [[nodiscard]] u16 checksum(void) const override;
        void checksum(u16 v) override;
        [[nodiscard]] Species species(void) const override;
        void species(Species v) override;
        [[nodiscard]] u16 heldItem(void) const override;
        void heldItem(u16 v) override;
        [[nodiscard]] u16 TID(void) const override;
        void TID(u16 v) override;
        [[nodiscard]] u16 SID(void) const override;
        void SID(u16 v) override;
        [[nodiscard]] u32 experience(void) const override;
        void experience(u32 v) override;
        Ability ability(void) const override;
        void ability(Ability v) override;
        u8 abilityNumber(void) const override;
        void abilityNumber(u8 v) override;
        void setAbility(u8 abilityNumber) override;
        u16 markValue(void) const override;
        void markValue(u16 v) override;
        u32 PID(void) const override;
        void PID(u32 v) override;
        Nature nature(void) const override;
        void nature(Nature v) override;
        bool fatefulEncounter(void) const override;
        void fatefulEncounter(bool v) override;
        Gender gender(void) const override;
        void gender(Gender g) override;
        bool isAlpha(void) const;
        void isAlpha(bool v);
        u16 alternativeForm(void) const override;
        void alternativeForm(u16 v) override;
        u16 ev(Stat ev) const override;
        void ev(Stat ev, u16 v) override;
        u8 contest(u8 contest) const override;
        void contest(u8 contest, u8 v) override;

        u8 pkrs(void) const override;
        void pkrs(u8 v) override;
        u8 pkrsDays(void) const override;
        void pkrsDays(u8 v) override;
        u8 pkrsStrain(void) const override;
        void pkrsStrain(u8 v) override;
        bool hasRibbon(Ribbon rib) const override;
        bool ribbon(Ribbon rib) const override;
        void ribbon(Ribbon rib, bool v) override;

        std::string nickname(void) const override;
        void nickname(const std::string_view& v) override;
        Move move(u8 move) const override;
        void move(u8 move, Move v) override;
        u8 PP(u8 move) const override;
        void PP(u8 move, u8 v) override;
        u8 PPUp(u8 move) const override;
        void PPUp(u8 move, u8 v) override;
        Move relearnMove(u8 move) const override;
        void relearnMove(u8 move, Move v) override;
        int partyCurrHP(void) const override;
        void partyCurrHP(u16 v) override;
        u8 iv(Stat iv) const override;
        void iv(Stat iv, u8 v) override;

        bool egg(void) const override;
        void egg(bool v) override;
        bool nicknamed(void) const override;
        void nicknamed(bool v) override;

        bool hyperTrain(Stat stat) const override;
        void hyperTrain(Stat stat, bool v) override;

        PKXHandler currentHandler(void) const override;
        void currentHandler(PKXHandler v) override;

        std::string otName(void) const override;
        void otName(const std::string_view& v) override;
        u8 otFriendship(void) const override;
        void otFriendship(u8 v) override;
        u8 htFriendship(void) const override;
        void htFriendship(u8 v) override;

        u16 eggLocation(void) const override;
        void eggLocation(u16 v) override;
        u16 metLocation(void) const override;
        void metLocation(u16 v) override;
        Ball ball(void) const override;
        void ball(Ball v) override;
        u8 metLevel(void) const override;
        void metLevel(u8 v) override;
        Gender otGender(void) const override;
        void otGender(Gender v) override;
        GameVersion version(void) const override;
        void version(GameVersion v) override;
        Language language(void) const override;
        void language(Language v) override;

        int partyStat(Stat stat) const override;
        void partyStat(Stat stat, u16 v) override;
        int partyLevel(void) const override;
        void partyLevel(u8 v) override;
        void updatePartyData(void) override;

        void refreshChecksum(void) override;
        Type hpType(void) const override;
        void hpType(Type v) override;
        u16 TSV(void) const override;
        u16 PSV(void) const override;
        u8 level(void) const override;
        void level(u8 v) override;
        bool shiny(void) const override;
        void shiny(bool v) override;
        u16 formSpecies(void) const override;

        u16 secondaryStatCalc(Stat stat) const override { return ev(stat); }

        void secondaryStatCalc(Stat stat, u16 v) override { ev(stat, v); }

        u16 maxSecondaryStatCalc(void) const override { return maxEV(); }

        u32 maxSecondaryStatCalcTotal(void) const override { return maxEVTotal(); }

        u8 maxIV(void) const override { return 31; }

        u16 maxEV(void) const override { return 252; }

        u32 maxEVTotal(void) const override { return 510; }

        inline u8 baseHP(void) const override { return PersonalZA::baseHP(formSpecies()); }

        inline u8 baseAtk(void) const override { return PersonalZA::baseAtk(formSpecies()); }

        inline u8 baseDef(void) const override { return PersonalZA::baseDef(formSpecies()); }

        inline u8 baseSpe(void) const override { return PersonalZA::baseSpe(formSpecies()); }

        inline u8 baseSpa(void) const override { return PersonalZA::baseSpa(formSpecies()); }

        inline u8 baseSpd(void) const override { return PersonalZA::baseSpd(formSpecies()); }

        inline Type type1(void) const override { return PersonalZA::type1(formSpecies()); }

        inline Type type2(void) const override { return PersonalZA::type2(formSpecies()); }

        inline u8 genderType(void) const override { return PersonalZA::gender(formSpecies()); }

        inline u8 baseFriendship(void) const override
        {
            return PersonalZA::baseFriendship(formSpecies());
        }

        inline u8 expType(void) const override { return PersonalZA::expType(formSpecies()); }

        inline Ability abilities(u8 n) const override
        {
            return PersonalZA::ability(formSpecies(), n);
        }

        inline u16 formStatIndex(void) const override
        {
            return PersonalZA::formStatIndex(formSpecies());
        }

    private:
        u16 statImpl(Stat stat) const override;
    };
}

#endif
