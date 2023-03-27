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

#include "personal/personal.hpp"
#include "personal1.h"
#include "personal2.h"
#include "personal3.h"
#include "personal4.h"
#include "personal5.h"
#include "personal6.h"
#include "personal7.h"
#include "personal7b.h"
#include "personal8.h"
#include "utils/endian.hpp"

#include <memory>

#define READ_PERSONAL(name, size) readPersonal(_PKSMCORE_PERSONAL_FOLDER name, size)

namespace
{
    std::unique_ptr<u8[]> readPersonal(std::string_view path, size_t nominalSize)
    {
        FILE* personal = fopen(path.data(), "rb");
        if (personal)
        {
            fseek(personal, 0, SEEK_END);
            size_t size = ftell(personal);
            rewind(personal);

            if (nominalSize != size)
            {
                throw std::runtime_error("Personal size does not match");
            }

            std::unique_ptr<u8[]> ret = std::unique_ptr<u8[]>{new u8[size]};
            if (fread(ret.get(), 1, size, personal) != size)
            {
                throw std::runtime_error("Could not read all of personal file");
            }
            fclose(personal);

            return ret;
        }
        else
        {
            throw std::runtime_error("Could not open personal file `" + std::string(path) + "`");
        }
    }

    const u8* personal7b()
    {
        static auto personal = READ_PERSONAL("personal7b", personal7b_size);
        return personal.get();
    }

    const u8* personal7()
    {
        static auto personal = READ_PERSONAL("personal7", personal7_size);
        return personal.get();
    }

    const u8* personal6()
    {
        static auto personal = READ_PERSONAL("personal6", personal6_size);
        return personal.get();
    }

    const u8* personal5()
    {
        static auto personal = READ_PERSONAL("personal5", personal5_size);
        return personal.get();
    }

    const u8* personal4()
    {
        static auto personal = READ_PERSONAL("personal4", personal4_size);
        return personal.get();
    }

    const u8* personal8()
    {
        static auto personal = READ_PERSONAL("personal8", personal8_size);
        return personal.get();
    }

    const u8* personal3()
    {
        static auto personal = READ_PERSONAL("personal3", personal3_size);
        return personal.get();
    }

    const u8* personal2()
    {
        static auto personal = READ_PERSONAL("personal2", personal2_size);
        return personal.get();
    }

    const u8* personal1()
    {
        static auto personal = READ_PERSONAL("personal1", personal1_size);
        return personal.get();
    }
}

namespace pksm
{
    namespace PersonalLGPE
    {
        u8 baseHP(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            return Type{personal7b()[species * personal7b_entrysize + 0x6]};
        }

        Type type2(u16 species)
        {
            return Type{personal7b()[species * personal7b_entrysize + 0x7]};
        }

        u8 gender(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal7b()[species * personal7b_entrysize + 0xB + n]};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(
                personal7b() + species * personal7b_entrysize + 0xE);
        }

        u8 formCount(u16 species)
        {
            return personal7b()[species * personal7b_entrysize + 0x10];
        }
    }

    namespace PersonalSMUSUM
    {
        u8 baseHP(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            return Type{personal7()[species * personal7_entrysize + 0x6]};
        }

        Type type2(u16 species)
        {
            return Type{personal7()[species * personal7_entrysize + 0x7]};
        }

        u8 gender(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal7()[species * personal7_entrysize + 0xB + n]};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal7() + species * personal7_entrysize + 0xE);
        }

        u8 formCount(u16 species)
        {
            return personal7()[species * personal7_entrysize + 0x10];
        }
    }

    namespace PersonalXYORAS
    {
        u8 baseHP(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            return Type{personal6()[species * personal6_entrysize + 0x6]};
        }

        Type type2(u16 species)
        {
            return Type{personal6()[species * personal6_entrysize + 0x7]};
        }

        u8 gender(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal6()[species * personal6_entrysize + 0xB + n]};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal6() + species * personal6_entrysize + 0xE);
        }

        u8 formCount(u16 species)
        {
            return personal6()[species * personal6_entrysize + 0x10];
        }
    }

    namespace PersonalBWB2W2
    {
        u8 baseHP(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            return Type{personal5()[species * personal5_entrysize + 0x6]};
        }

        Type type2(u16 species)
        {
            return Type{personal5()[species * personal5_entrysize + 0x7]};
        }

        u8 gender(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal5()[species * personal5_entrysize + 0xB + n]};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal5() + species * personal5_entrysize + 0xE);
        }

        u8 formCount(u16 species)
        {
            return personal5()[species * personal5_entrysize + 0x10];
        }
    }

    namespace PersonalDPPtHGSS
    {
        u8 baseHP(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            u8 typeVal = personal4()[species * personal4_entrysize + 0x6];
            if (typeVal > 8)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        Type type2(u16 species)
        {
            u8 typeVal = personal4()[species * personal4_entrysize + 0x7];
            if (typeVal > 8)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        u8 gender(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal4()[species * personal4_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal4()[species * personal4_entrysize + 0xB + n]};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal4() + species * personal4_entrysize + 0xD);
        }

        // Normalized to fit with other formCounts' return values
        u8 formCount(u16 species)
        {
            if (species == 201)
            {
                return 28;
            }
            else
            {
                u8 count = personal4()[species * personal4_entrysize + 0xF];
                if (count == 0)
                {
                    return 1;
                }
                return count;
            }
        }
    }

    namespace PersonalSWSH
    {
        u8 baseHP(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            return Type{personal8()[species * personal8_entrysize + 0x6]};
        }

        Type type2(u16 species)
        {
            return Type{personal8()[species * personal8_entrysize + 0x7]};
        }

        u8 gender(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0xA];
        }

        u8 formCount(u16 species)
        {
            return personal8()[species * personal8_entrysize + 0xB];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{LittleEndian::convertTo<u16>(
                personal8() + species * personal8_entrysize + 0xC + 2 * n)};
        }

        u16 formStatIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal8() + species * personal8_entrysize + 0x12);
        }

        u16 pokedexIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal8() + species * personal8_entrysize + 0x14);
        }

        u16 armordexIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal8() + species * personal8_entrysize + 0x16);
        }

        u16 crowndexIndex(u16 species)
        {
            return LittleEndian::convertTo<u16>(personal8() + species + personal8_entrysize + 0x18);
        }

        bool canLearnTR(u16 species, u8 trID)
        {
            return (personal8()[species * personal8_entrysize + 0x1A + (trID >> 3)] &
                       (1 << (trID & 7))) != 0
                     ? true
                     : false;
        }
    }

    namespace PersonalRSFRLGE
    {
        u8 baseHP(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x0];
        }

        u8 baseAtk(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x1];
        }

        u8 baseDef(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x2];
        }

        u8 baseSpe(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x3];
        }

        u8 baseSpa(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x4];
        }

        u8 baseSpd(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x5];
        }

        Type type1(u16 species)
        {
            u8 typeVal = personal3()[species * personal3_entrysize + 0x6];
            if (typeVal > 8)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        Type type2(u16 species)
        {
            u8 typeVal = personal3()[species * personal3_entrysize + 0x7];
            if (typeVal > 8)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        u8 gender(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x8];
        }

        u8 baseFriendship(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0x9];
        }

        u8 expType(u16 species)
        {
            return personal3()[species * personal3_entrysize + 0xA];
        }

        Ability ability(u16 species, u8 n)
        {
            return Ability{personal3()[species * personal3_entrysize + 0xB + n]};
        }

        u8 formCount(u16 species)
        {
            switch (species)
            {
                default:
                    return 1;
                case 201: // Unown
                    return 28;
                case 386: // Deoxys
                case 351: // Castform
                    return 4;
            }
        }
    }

    namespace PersonalGSC
    {
        u8 baseHP(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x0];
        }

        u8 baseAtk(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x1];
        }

        u8 baseDef(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x2];
        }

        u8 baseSpe(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x3];
        }

        u8 baseSpa(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x4];
        }

        u8 baseSpd(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x5];
        }

        Type type1(u8 species)
        {
            u8 typeVal = personal2()[species * personal2_entrysize + 0x6];
            if (typeVal >= 20)
            {
                return Type{u8(typeVal - 11)};
            }
            else if (typeVal >= 7) // compensating for bird type
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        Type type2(u8 species)
        {
            u8 typeVal = personal2()[species * personal2_entrysize + 0x7];
            if (typeVal >= 20)
            {
                return Type{u8(typeVal - 11)};
            }
            else if (typeVal >= 7)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        u8 gender(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x8];
        }

        u8 expType(u8 species)
        {
            return personal2()[species * personal2_entrysize + 0x9];
        }

        u8 formCount(u16 species)
        {
            switch (species)
            {
                default:
                    return 1;
                case 201: // Unown
                    return 26;
            }
        }
    }

    namespace PersonalRGBY
    {
        u8 baseHP(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x0];
        }

        u8 baseAtk(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x1];
        }

        u8 baseDef(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x2];
        }

        u8 baseSpe(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x3];
        }

        u8 baseSpad(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x4];
        }

        Type type1(u8 species)
        {
            u8 typeVal = personal1()[species * personal1_entrysize + 0x5];
            if (typeVal >= 20)
            {
                return Type{u8(typeVal - 11)};
            }
            else if (typeVal >= 7)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        Type type2(u8 species)
        {
            u8 typeVal = personal1()[species * personal1_entrysize + 0x6];
            if (typeVal >= 20)
            {
                return Type{u8(typeVal - 11)};
            }
            else if (typeVal >= 7)
            {
                return Type{u8(typeVal - 1)};
            }
            else
            {
                return Type{typeVal};
            }
        }

        u8 catchRate(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x7];
        }

        u8 expType(u8 species)
        {
            return personal1()[species * personal1_entrysize + 0x8];
        }
    }

}
