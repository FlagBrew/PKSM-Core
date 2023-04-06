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

#ifndef PKX_HPP
#define PKX_HPP

#include "enums/Ability.hpp"
#include "enums/Ball.hpp"
#include "enums/GameVersion.hpp"
#include "enums/Gender.hpp"
#include "enums/Generation.hpp"
#include "enums/Language.hpp"
#include "enums/Move.hpp"
#include "enums/Nature.hpp"
#include "enums/PKXHandler.hpp"
#include "enums/Ribbon.hpp"
#include "enums/Species.hpp"
#include "enums/Stat.hpp"
#include "enums/Type.hpp"
#include "pkx/IPKFilterable.hpp"
#include "pkx/PKFilter.hpp"
#include "sav/Item.hpp"
#include "utils/coretypes.h"
#include "utils/DateTime.hpp"
#include "utils/genToPkx.hpp"
#include <concepts>
#include <memory>
#include <string>

namespace pksm
{
    class Sav;
    class PK1;
    class PK2;
    class PK3;
    class PK4;
    class PK5;
    class PK6;
    class PK7;
    class PK8;
    class PB7;

    class PKX : public IPKFilterable
    {
    private:
        bool directAccess;
        [[nodiscard]] virtual int eggYear(void) const  = 0;
        virtual void eggYear(int v)                    = 0;
        [[nodiscard]] virtual int eggMonth(void) const = 0;
        virtual void eggMonth(int v)                   = 0;
        [[nodiscard]] virtual int eggDay(void) const   = 0;
        virtual void eggDay(int v)                     = 0;
        [[nodiscard]] virtual int metYear(void) const  = 0;
        virtual void metYear(int v)                    = 0;
        [[nodiscard]] virtual int metMonth(void) const = 0;
        virtual void metMonth(int v)                   = 0;
        [[nodiscard]] virtual int metDay(void) const   = 0;
        virtual void metDay(int v)                     = 0;

    protected:
        struct PrivateConstructor
        {
        };

        PKX(u8* data, size_t length, bool directAccess = false);

        [[nodiscard]] u32 expTable(u8 row, u8 col) const;
        virtual void reorderMoves(void);

        u32 length = 0;
        u8* data;

    public:
        static constexpr Species PKSM_MAX_SPECIES = Species::Calyrex;

        // Returns null if length is not valid for that generation, and a party Pokemon depending on
        // length, or in Gen I and II a Japanese Pokemon depending on length
        [[nodiscard]] static std::unique_ptr<PKX> getPKM(
            Generation gen, u8* data, size_t length, bool directAccess = false);

        template <Generation::EnumType g>
        [[nodiscard]] static std::unique_ptr<typename GenToPkx<g>::PKX> getPKM(
            u8* data, size_t length, bool directAccess = false)
        {
            return getPKM<typename GenToPkx<g>::PKX>(data, length, directAccess);
        }

        template <std::derived_from<::pksm::PKX> Pkm>
        [[nodiscard]] static std::unique_ptr<Pkm> getPKM(
            u8* data, size_t length, bool directAccess = false)
        {
            if constexpr (std::is_same_v<typename GenToPkx<Generation::ONE>::PKX,
                              std::remove_cvref_t<Pkm>> ||
                          std::is_same_v<typename GenToPkx<Generation::TWO>::PKX,
                              std::remove_cvref_t<Pkm>>)
            {
                if (Pkm::JP_LENGTH_WITH_NAMES == length || Pkm::INT_LENGTH_WITH_NAMES == length)
                {
                    return std::make_unique<Pkm>(PrivateConstructor{}, data,
                        length == Pkm::JP_LENGTH_WITH_NAMES, directAccess);
                }
                else if (data == nullptr)
                {
                    return std::make_unique<Pkm>(PrivateConstructor{}, data, false, directAccess);
                }
            }
            else
            {
                if (Pkm::PARTY_LENGTH == length || Pkm::BOX_LENGTH == length)
                {
                    return std::make_unique<Pkm>(
                        PrivateConstructor{}, data, length == Pkm::PARTY_LENGTH, directAccess);
                }
                else if (data == nullptr)
                {
                    return std::make_unique<Pkm>(PrivateConstructor{}, data, false, directAccess);
                }
            }
            return nullptr;
        }

        [[nodiscard]] virtual ~PKX();
        PKX(const PKX& pk);
        PKX(PKX&& pk);
        PKX& operator=(const PKX& pk);
        PKX& operator=(PKX&& pk);
        [[nodiscard]] bool operator==(const PKFilter& filter) const;

        [[nodiscard]] virtual std::string_view extension(void) const = 0;

        [[nodiscard]] std::span<u8> rawData(void) { return {data, length}; }

        [[nodiscard]] std::span<const u8> rawData(void) const { return {data, length}; }

        [[nodiscard]] u32 getLength(void) const { return length; }

        [[nodiscard]] virtual bool isParty(void) const = 0;

        virtual void decrypt(void)                     = 0;
        virtual void encrypt(void)                     = 0;
        [[nodiscard]] virtual bool isEncrypted() const = 0;

        [[nodiscard]] virtual std::unique_ptr<PK1> convertToG1(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK2> convertToG2(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK3> convertToG3(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK4> convertToG4(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK5> convertToG5(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK6> convertToG6(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK7> convertToG7(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PB7> convertToLGPE(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PK8> convertToG8(Sav& save) const;
        [[nodiscard]] virtual std::unique_ptr<PKX> clone(void) const = 0;
        [[nodiscard]] std::unique_ptr<PKX> partyClone(void) const;

        [[nodiscard]] Generation generation(void) const override = 0;
        [[nodiscard]] bool originGen7(void) const;
        [[nodiscard]] bool originGen6(void) const;
        [[nodiscard]] bool originGen5(void) const;
        [[nodiscard]] bool originGen4(void) const;
        [[nodiscard]] bool originGen3(void) const;
        [[nodiscard]] bool originGen2(void) const;
        [[nodiscard]] bool originGen1(void) const;
        [[nodiscard]] Generation originGen(void) const;
        void fixMoves(void);

        [[nodiscard]] static u32 getRandomPID(Species species, Gender gender,
            GameVersion originGame, Nature nature, u8 form, u8 abilityNum, bool shiny, u16 tsv,
            u32 oldPid, Generation gen);
        [[nodiscard]] static Gender genderFromRatio(u32 pid, u8 gt);

        // BLOCK A
        [[nodiscard]] virtual u32 encryptionConstant(void) const = 0;
        virtual void encryptionConstant(u32 v)                   = 0;
        [[nodiscard]] virtual u16 sanity(void) const             = 0;
        virtual void sanity(u16 v)                               = 0;
        [[nodiscard]] virtual u16 checksum(void) const           = 0;
        virtual void checksum(u16 v)                             = 0;
        [[nodiscard]] Species species(void) const override       = 0;
        void species(Species v) override                         = 0;
        [[nodiscard]] u16 heldItem(void) const override          = 0;
        void heldItem(u16 v) override                            = 0;

        virtual void heldItem(const Item& item) { heldItem(item.id()); }

        [[nodiscard]] virtual u16 TID(void) const               = 0;
        virtual void TID(u16 v)                                 = 0;
        [[nodiscard]] virtual u16 SID(void) const               = 0;
        virtual void SID(u16 v)                                 = 0;
        [[nodiscard]] virtual u32 experience(void) const        = 0;
        virtual void experience(u32 v)                          = 0;
        [[nodiscard]] Ability ability(void) const override      = 0;
        void ability(Ability v) override                        = 0;
        [[nodiscard]] virtual u8 abilityNumber(void) const      = 0;
        virtual void abilityNumber(u8 v)                        = 0;
        virtual void setAbility(u8 abilityNumber)               = 0;
        [[nodiscard]] virtual u16 markValue(void) const         = 0;
        virtual void markValue(u16 v)                           = 0;
        [[nodiscard]] virtual u32 PID(void) const               = 0;
        virtual void PID(u32 v)                                 = 0;
        [[nodiscard]] Nature nature(void) const override        = 0;
        void nature(Nature v) override                          = 0;
        [[nodiscard]] virtual bool fatefulEncounter(void) const = 0;
        virtual void fatefulEncounter(bool v)                   = 0;
        [[nodiscard]] Gender gender(void) const override        = 0;
        void gender(Gender g) override                          = 0;
        [[nodiscard]] u16 alternativeForm(void) const override  = 0;
        void alternativeForm(u16 v) override                    = 0;
        [[nodiscard]] virtual u16 ev(Stat ev) const             = 0;
        virtual void ev(Stat ev, u16 v)                         = 0;
        [[nodiscard]] virtual u8 contest(u8 contest) const      = 0;
        virtual void contest(u8 contest, u8 v)                  = 0;
        [[nodiscard]] virtual u8 pkrs(void) const               = 0;
        virtual void pkrs(u8 v)                                 = 0;
        [[nodiscard]] virtual u8 pkrsDays(void) const           = 0;
        virtual void pkrsDays(u8 v)                             = 0;
        [[nodiscard]] virtual u8 pkrsStrain(void) const         = 0;
        virtual void pkrsStrain(u8 v)                           = 0;
        [[nodiscard]] virtual bool hasRibbon(Ribbon rib) const  = 0;
        [[nodiscard]] virtual bool ribbon(Ribbon rib) const     = 0;
        virtual void ribbon(Ribbon rib, bool v)                 = 0;

        // BLOCK B
        [[nodiscard]] virtual std::string nickname(void) const = 0;
        virtual void nickname(const std::string_view& v)       = 0;
        [[nodiscard]] Move move(u8 move) const override        = 0;
        void move(u8 move, Move v) override                    = 0;
        [[nodiscard]] Move relearnMove(u8 move) const override = 0;
        void relearnMove(u8 move, Move v) override             = 0;
        [[nodiscard]] virtual u8 PP(u8 move) const             = 0;
        virtual void PP(u8 move, u8 v)                         = 0;
        [[nodiscard]] virtual u8 PPUp(u8 move) const           = 0;
        virtual void PPUp(u8 move, u8 v)                       = 0;
        [[nodiscard]] u8 iv(Stat iv) const override            = 0;
        void iv(Stat iv, u8 v) override                        = 0;
        [[nodiscard]] bool egg(void) const override            = 0;
        void egg(bool v) override                              = 0;
        [[nodiscard]] virtual bool nicknamed(void) const       = 0;
        virtual void nicknamed(bool v)                         = 0;

        [[nodiscard]] virtual bool hyperTrain(Stat stat) const = 0;
        virtual void hyperTrain(Stat stat, bool v)             = 0;

        [[nodiscard]] u8 maxPP(u8 move) const;
        void healPP();

        void healPP(u8 move) { PP(move, maxPP(move)); }

        // BLOCK C
        [[nodiscard]] virtual PKXHandler currentHandler(void) const = 0;
        virtual void currentHandler(PKXHandler v)                   = 0;

        // BLOCK D
        [[nodiscard]] virtual std::string otName(void) const = 0;
        virtual void otName(const std::string_view& v)       = 0;
        [[nodiscard]] virtual u8 otFriendship(void) const    = 0;
        virtual void otFriendship(u8 v)                      = 0;
        [[nodiscard]] virtual u8 htFriendship(void) const    = 0;
        virtual void htFriendship(u8 v)                      = 0;

        // Raw information handled in private functions
        [[nodiscard]] virtual Date eggDate(void) const
        {
            return Date{(u8)eggDay(), (u8)eggMonth(), (u32)eggYear()};
        }

        virtual void eggDate(const Date& v)
        {
            eggDay(v.day());
            eggMonth(v.month());
            eggYear(v.year());
        }

        [[nodiscard]] virtual Date metDate(void) const
        {
            return Date{(u8)metDay(), (u8)metMonth(), (u32)metYear()};
        }

        virtual void metDate(const Date& v)
        {
            metDay(v.day());
            metMonth(v.month());
            metYear(v.year());
        }

        [[nodiscard]] virtual u16 eggLocation(void) const     = 0;
        virtual void eggLocation(u16 v)                       = 0;
        [[nodiscard]] virtual u16 metLocation(void) const     = 0;
        virtual void metLocation(u16 v)                       = 0;
        [[nodiscard]] Ball ball(void) const override          = 0;
        void ball(Ball v) override                            = 0;
        [[nodiscard]] virtual u8 metLevel(void) const         = 0;
        virtual void metLevel(u8 v)                           = 0;
        [[nodiscard]] virtual Gender otGender(void) const     = 0;
        virtual void otGender(Gender v)                       = 0;
        [[nodiscard]] virtual GameVersion version(void) const = 0;
        virtual void version(GameVersion v)                   = 0;
        [[nodiscard]] Language language(void) const override  = 0;
        void language(Language v) override                    = 0;

        [[nodiscard]] u8 currentFriendship(void) const
        {
            return currentHandler() == PKXHandler::OT ? otFriendship() : htFriendship();
        }

        void currentFriendship(u8 v)
        {
            if (currentHandler() == PKXHandler::OT)
            {
                otFriendship(v);
            }
            else
            {
                htFriendship(v);
            }
        }

        [[nodiscard]] u8 oppositeFriendship(void) const
        {
            return currentHandler() == PKXHandler::NonOT ? otFriendship() : htFriendship();
        }

        void oppositeFriendship(u8 v)
        {
            if (currentHandler() == PKXHandler::NonOT)
            {
                otFriendship(v);
            }
            else
            {
                htFriendship(v);
            }
        }

        virtual void refreshChecksum(void)            = 0;
        [[nodiscard]] virtual Type hpType(void) const = 0;
        virtual void hpType(Type v)                   = 0;
        [[nodiscard]] u16 TSV(void) const override    = 0;
        [[nodiscard]] virtual u16 PSV(void) const     = 0;
        [[nodiscard]] u32 versionTID(void) const;
        [[nodiscard]] u32 versionSID(void) const;
        [[nodiscard]] u32 formatTID(void) const;
        [[nodiscard]] u32 formatSID(void) const;
        [[nodiscard]] u8 level(void) const override       = 0;
        void level(u8 v) override                         = 0;
        [[nodiscard]] bool shiny(void) const override     = 0;
        void shiny(bool v) override                       = 0;
        [[nodiscard]] virtual u16 formSpecies(void) const = 0;

        [[nodiscard]] u16 stat(Stat stat) const
        {
            if (species() == Species::Shedinja && stat == Stat::HP)
            {
                return 1;
            }
            return statImpl(stat);
        }

        // Hehehehe... to be done
        // virtual u8 sleepTurns(void) const = 0;
        // virtual void sleepTurns(u8 v) = 0;
        // virtual bool poison(void) const = 0;
        // virtual void poison(bool v) = 0;
        // virtual bool burn(void) const = 0;
        // virtual void burn(bool v) = 0;
        // virtual bool frozen(void) const = 0;
        // virtual void frozen(bool v) = 0;
        // virtual bool paralyzed(void) const = 0;
        // virtual void paralyzed(bool v) = 0;
        // virtual bool toxic(void) const = 0;
        // virtual void toxic(bool v) = 0;
        // -1 means not a party pokemon
        [[nodiscard]] virtual int partyCurrHP(void) const    = 0;
        virtual void partyCurrHP(u16 v)                      = 0;
        [[nodiscard]] virtual int partyStat(Stat stat) const = 0;
        virtual void partyStat(Stat stat, u16 v)             = 0;
        [[nodiscard]] virtual int partyLevel(void) const     = 0;
        virtual void partyLevel(u8 v)                        = 0;
        // Takes any calculated stats and writes them into the party offsets, provided they exist
        virtual void updatePartyData(void) = 0;

        virtual u8 maxIV(void) const       = 0;
        virtual u16 maxEV(void) const      = 0;
        virtual u32 maxEVTotal(void) const = 0;

        virtual u16 secondaryStatCalc(Stat stat) const    = 0;
        virtual void secondaryStatCalc(Stat stat, u16 v)  = 0;
        virtual u16 maxSecondaryStatCalc(void) const      = 0;
        virtual u32 maxSecondaryStatCalcTotal(void) const = 0;

        // Personal interface
        [[nodiscard]] virtual u8 baseHP(void) const         = 0;
        [[nodiscard]] virtual u8 baseAtk(void) const        = 0;
        [[nodiscard]] virtual u8 baseDef(void) const        = 0;
        [[nodiscard]] virtual u8 baseSpe(void) const        = 0;
        [[nodiscard]] virtual u8 baseSpa(void) const        = 0;
        [[nodiscard]] virtual u8 baseSpd(void) const        = 0;
        [[nodiscard]] virtual Type type1(void) const        = 0;
        [[nodiscard]] virtual Type type2(void) const        = 0;
        [[nodiscard]] virtual u8 genderType(void) const     = 0;
        [[nodiscard]] virtual u8 baseFriendship(void) const = 0;
        [[nodiscard]] virtual u8 expType(void) const        = 0;
        [[nodiscard]] virtual Ability abilities(u8 n) const = 0;
        [[nodiscard]] virtual u16 formStatIndex(void) const = 0;

        [[nodiscard]] bool isFilter() const final { return false; }

    private:
        [[nodiscard]] virtual u16 statImpl(Stat stat) const = 0;
    };
}

#endif
