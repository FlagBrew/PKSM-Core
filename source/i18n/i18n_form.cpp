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

#include "enums/GameVersion.hpp"
#include "enums/Species.hpp"
#include "i18n_internal.hpp"
#include "personal/personal.hpp"
#include "utils/VersionTables.hpp"

namespace
{
    constexpr size_t Default          = 0;
    constexpr size_t Alolan           = 1;
    constexpr size_t RockStar         = 2;
    constexpr size_t Belle            = 3;
    constexpr size_t PopStar          = 4;
    constexpr size_t PhD              = 5;
    constexpr size_t Libre            = 6;
    constexpr size_t Cosplay          = 7;
    constexpr size_t Original         = 8;
    constexpr size_t Hoenn            = 9;
    constexpr size_t Sinnoh           = 10;
    constexpr size_t Unova            = 11;
    constexpr size_t Kalos            = 12;
    constexpr size_t Alola            = 13;
    constexpr size_t A                = 14;
    constexpr size_t B                = 15;
    constexpr size_t C                = 16;
    constexpr size_t D                = 17;
    constexpr size_t E                = 18;
    constexpr size_t F                = 19;
    constexpr size_t G                = 20;
    constexpr size_t H                = 21;
    constexpr size_t I                = 22;
    constexpr size_t J                = 23;
    constexpr size_t K                = 24;
    constexpr size_t L                = 25;
    constexpr size_t M                = 26;
    constexpr size_t N                = 27;
    constexpr size_t O                = 28;
    constexpr size_t P                = 29;
    constexpr size_t Q                = 30;
    constexpr size_t R                = 31;
    constexpr size_t S                = 32;
    constexpr size_t T                = 33;
    constexpr size_t U                = 34;
    constexpr size_t V                = 35;
    constexpr size_t W                = 36;
    constexpr size_t X                = 37;
    constexpr size_t Y                = 38;
    constexpr size_t Z                = 39;
    constexpr size_t ExclamationPoint = 40;
    constexpr size_t QuestionMark     = 41;
    constexpr size_t Normal           = 42;
    constexpr size_t Attack           = 43;
    constexpr size_t Defense          = 44;
    constexpr size_t Speed            = 45;
    constexpr size_t Plant            = 46;
    constexpr size_t Sandy            = 47;
    constexpr size_t Trash            = 48;
    constexpr size_t WestSea          = 49;
    constexpr size_t EastSea          = 50;
    constexpr size_t Heat             = 51;
    constexpr size_t Wash             = 52;
    constexpr size_t Fridge           = 53;
    constexpr size_t Fan              = 54;
    constexpr size_t Mow              = 55;
    constexpr size_t Altered          = 56;
    constexpr size_t Origin           = 57;
    constexpr size_t Land             = 58;
    constexpr size_t Sky              = 59;
    constexpr size_t RedStriped       = 60;
    constexpr size_t BlueStriped      = 61;
    constexpr size_t Spring           = 62;
    constexpr size_t Summer           = 63;
    constexpr size_t Autumn           = 64;
    constexpr size_t Winter           = 65;
    constexpr size_t Incarnate        = 66;
    constexpr size_t Therian          = 67;
    constexpr size_t WhiteKyurem      = 68;
    constexpr size_t Black            = 69;
    constexpr size_t Ordinary         = 70;
    constexpr size_t Resolute         = 71;
    constexpr size_t Aria             = 72;
    constexpr size_t Pirouette        = 73;
    constexpr size_t BattleBond       = 74;
    constexpr size_t IcySnow          = 75;
    constexpr size_t Polar            = 76;
    constexpr size_t Tundra           = 77;
    constexpr size_t Continental      = 78;
    constexpr size_t Garden           = 79;
    constexpr size_t Elegant          = 80;
    constexpr size_t Meadow           = 81;
    constexpr size_t Modern           = 82;
    constexpr size_t Marine           = 83;
    constexpr size_t Archipelago      = 84;
    constexpr size_t HighPlains       = 85;
    constexpr size_t Sandstorm        = 86;
    constexpr size_t River            = 87;
    constexpr size_t Monsoon          = 88;
    constexpr size_t Savanna          = 89;
    constexpr size_t Sun              = 90;
    constexpr size_t Ocean            = 91;
    constexpr size_t Jungle           = 92;
    constexpr size_t Fancy            = 93;
    constexpr size_t PokeBall         = 94;
    constexpr size_t RedFlower        = 95;
    constexpr size_t YellowFlower     = 96;
    constexpr size_t OrangeFlower     = 97;
    constexpr size_t BlueFlower       = 98;
    constexpr size_t WhiteFlower      = 99;
    constexpr size_t EternalFlower    = 100;
    constexpr size_t Natural          = 101;
    constexpr size_t Heart            = 102;
    constexpr size_t Star             = 103;
    constexpr size_t Diamond          = 104;
    constexpr size_t Debutante        = 105;
    constexpr size_t Matron           = 106;
    constexpr size_t Dandy            = 107;
    constexpr size_t LaReine          = 108;
    constexpr size_t Kabuki           = 109;
    constexpr size_t Pharaoh          = 110;
    constexpr size_t Average          = 111;
    constexpr size_t Small            = 112;
    constexpr size_t Large            = 113;
    constexpr size_t Super            = 114;
    constexpr size_t _50Percent       = 115;
    constexpr size_t _10Percent       = 116;
    constexpr size_t _10Percent_PC    = 117;
    constexpr size_t _50Percent_PC    = 118;
    constexpr size_t Confined         = 119;
    constexpr size_t Unbound          = 120;
    constexpr size_t Baile            = 121;
    constexpr size_t PomPom           = 122;
    constexpr size_t Pau              = 123;
    constexpr size_t Sensu            = 124;
    constexpr size_t Midday           = 125;
    constexpr size_t Midnight         = 126;
    constexpr size_t Dusk             = 127;
    constexpr size_t CoveredRed       = 128;
    constexpr size_t CoveredOrange    = 129;
    constexpr size_t CoveredYellow    = 130;
    constexpr size_t CoveredGreen     = 131;
    constexpr size_t CoveredBlue      = 132;
    constexpr size_t CoveredIndigo    = 133;
    constexpr size_t CoveredViolet    = 134;
    constexpr size_t Red              = 135;
    constexpr size_t Orange           = 136;
    constexpr size_t Yellow           = 137;
    constexpr size_t Green            = 138;
    constexpr size_t Blue             = 139;
    constexpr size_t Indigo           = 140;
    constexpr size_t Violet           = 141;
    constexpr size_t OriginalColor    = 142;
    constexpr size_t SpikyEared       = 143;
    constexpr size_t Female           = 144;
    constexpr size_t Totem            = 145;
    constexpr size_t Mega             = 146;
    constexpr size_t DawnWings        = 147;
    constexpr size_t DuskMane         = 148;
    constexpr size_t Ultra            = 149;
    constexpr size_t Primal           = 150;
    constexpr size_t MegaX            = 151;
    constexpr size_t MegaY            = 152;
    constexpr size_t Partner          = 153;
    constexpr size_t _100Percent      = 154;
    constexpr size_t Fighting         = 155;
    constexpr size_t Flying           = 156;
    constexpr size_t Poison           = 157;
    constexpr size_t Ground           = 158;
    constexpr size_t Rock             = 159;
    constexpr size_t Bug              = 160;
    constexpr size_t Ghost            = 161;
    constexpr size_t Steel            = 162;
    constexpr size_t Fire             = 163;
    constexpr size_t Water            = 164;
    constexpr size_t Grass            = 165;
    constexpr size_t Electric         = 166;
    constexpr size_t Psychic          = 167;
    constexpr size_t Ice              = 168;
    constexpr size_t Dragon           = 169;
    constexpr size_t Dark             = 170;
    constexpr size_t Fairy            = 171;
    constexpr size_t Overcast         = 172;
    constexpr size_t Sunshine         = 173;
    constexpr size_t Zen              = 174;
    constexpr size_t Ash              = 175;
    constexpr size_t Shield           = 176;
    constexpr size_t Blade            = 177;
    constexpr size_t Solo             = 178;
    constexpr size_t School           = 179;
    constexpr size_t Sunny            = 180;
    constexpr size_t Rainy            = 181;
    constexpr size_t Snowy            = 182;
    constexpr size_t Galarian         = 183;
    constexpr size_t Gigantamax       = 184;
    constexpr size_t Gulping          = 185;
    constexpr size_t Gorging          = 186;
    constexpr size_t LowKey           = 187;
    constexpr size_t RubyCream        = 188;
    constexpr size_t MatchaCream      = 189;
    constexpr size_t MintCream        = 190;
    constexpr size_t LemonCream       = 191;
    constexpr size_t SaltedCream      = 192;
    constexpr size_t RubySwirl        = 193;
    constexpr size_t CaramelSwirl     = 194;
    constexpr size_t RainbowSwirl     = 195;
    constexpr size_t NoiceFace        = 196;
    constexpr size_t HangryMode       = 197;
    constexpr size_t Crowned          = 198;
    constexpr size_t Eternamax        = 199;
    constexpr size_t AmpedForm        = 200;
    constexpr size_t VanillaCream     = 201;
    constexpr size_t Phony            = 202;
    constexpr size_t Antique          = 203;
    constexpr size_t World            = 204;
    constexpr size_t SingleStrike     = 205;
    constexpr size_t RapidStrike      = 206;
    constexpr size_t Dada             = 207;
    constexpr size_t IceRider         = 208;
    constexpr size_t ShadowRider      = 209;
}

namespace i18n
{
    std::unordered_map<pksm::Language, std::vector<std::string>> formss;

    void initForm(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/forms.txt", vec);
        formss.insert_or_assign(lang, std::move(vec));
    }

    void exitForm(pksm::Language lang) { formss.erase(lang); }

    const std::vector<size_t> formIndices(pksm::GameVersion version, pksm::Species species)
    {
        // TODO: Gigantamax and Isle of Armor Galarian additions, including Slowbro
        u8 forms = pksm::VersionTables::formCount(version, species);
        if (forms == 1)
        {
            return {Default};
        }
        std::vector<size_t> ret;
        switch (species)
        {
            case pksm::Species::Venusaur:
                ret = {Default, Mega};
                break;
            case pksm::Species::Charizard:
                ret = {Default, MegaX, MegaY};
                break;
            case pksm::Species::Blastoise:
                ret = {Default, Mega};
                break;
            case pksm::Species::Beedrill:
                ret = {Default, Mega};
                break;
            case pksm::Species::Pidgeot:
                ret = {Default, Mega};
                break;
            case pksm::Species::Rattata:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Raticate:
                ret = {Default, Alolan, Totem};
                break;
            case pksm::Species::Pikachu:
                switch ((pksm::Generation)version)
                {
                    case pksm::Generation::SIX:
                        ret = {Default, RockStar, Belle, PopStar, PhD, Libre, Cosplay};
                        break;
                    case pksm::Generation::SEVEN:
                    case pksm::Generation::LGPE:
                        ret = {Default, Original, Hoenn, Sinnoh, Unova, Kalos, Alola, Partner,
                            Default, World};
                        break;
                    default:
                        break;
                }
                break;
            case pksm::Species::Raichu:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Sandshrew:
            case pksm::Species::Sandslash:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Vulpix:
            case pksm::Species::Ninetales:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Diglett:
            case pksm::Species::Dugtrio:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Meowth:
                ret = {Default, Alolan, Galarian};
                break;
            case pksm::Species::Persian:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Alakazam:
                ret = {Default, Mega};
                break;
            case pksm::Species::Geodude:
            case pksm::Species::Graveler:
            case pksm::Species::Golem:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Ponyta:
            case pksm::Species::Rapidash:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Slowpoke:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Slowbro:
                ret = {Default, Mega, Galarian};
                break;
            case pksm::Species::Slowking:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Farfetchd:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Grimer:
            case pksm::Species::Muk:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Gengar:
                ret = {Default, Mega};
                break;
            case pksm::Species::Exeggutor:
                ret = {Default, Alolan};
                break;
            case pksm::Species::Marowak:
                ret = {Default, Alolan, Totem};
                break;
            case pksm::Species::Weezing:
                ret = {Default, Galarian};
                break;
            case pksm::Species::MrMime:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Kangaskhan:
                ret = {Default, Mega};
                break;
            case pksm::Species::Pinsir:
                ret = {Default, Mega};
                break;
            case pksm::Species::Gyarados:
                ret = {Default, Mega};
                break;
            case pksm::Species::Aerodactyl:
                ret = {Default, Mega};
                break;
            case pksm::Species::Articuno:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Zapdos:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Moltres:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Mewtwo:
                ret = {Default, MegaX, MegaY};
                break;
            case pksm::Species::Pichu:
                ret = {Default, SpikyEared};
                break;
            case pksm::Species::Ampharos:
                ret = {Default, Mega};
                break;
            case pksm::Species::Unown:
                ret = {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
                    ExclamationPoint, QuestionMark};
                break;
            case pksm::Species::Steelix:
                ret = {Default, Mega};
                break;
            case pksm::Species::Scizor:
                ret = {Default, Mega};
                break;
            case pksm::Species::Heracross:
                ret = {Default, Mega};
                break;
            case pksm::Species::Corsola:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Houndoom:
                ret = {Default, Mega};
                break;
            case pksm::Species::Tyranitar:
                ret = {Default, Mega};
                break;
            case pksm::Species::Sceptile:
                ret = {Default, Mega};
                break;
            case pksm::Species::Blaziken:
                ret = {Default, Mega};
                break;
            case pksm::Species::Swampert:
                ret = {Default, Mega};
                break;
            case pksm::Species::Zigzagoon:
            case pksm::Species::Linoone:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Gardevoir:
                ret = {Default, Mega};
                break;
            case pksm::Species::Sableye:
                ret = {Default, Mega};
                break;
            case pksm::Species::Mawile:
                ret = {Default, Mega};
                break;
            case pksm::Species::Aggron:
                ret = {Default, Mega};
                break;
            case pksm::Species::Medicham:
                ret = {Default, Mega};
                break;
            case pksm::Species::Manectric:
                ret = {Default, Mega};
                break;
            case pksm::Species::Sharpedo:
                ret = {Default, Mega};
                break;
            case pksm::Species::Camerupt:
                ret = {Default, Mega};
                break;
            case pksm::Species::Altaria:
                ret = {Default, Mega};
                break;
            case pksm::Species::Castform:
                ret = {Default, Sunny, Rainy, Snowy};
                break;
            case pksm::Species::Banette:
                ret = {Default, Mega};
                break;
            case pksm::Species::Absol:
                ret = {Default, Mega};
                break;
            case pksm::Species::Glalie:
                ret = {Default, Mega};
                break;
            case pksm::Species::Salamence:
                ret = {Default, Mega};
                break;
            case pksm::Species::Metagross:
                ret = {Default, Mega};
                break;
            case pksm::Species::Latias:
                ret = {Default, Mega};
                break;
            case pksm::Species::Latios:
                ret = {Default, Mega};
                break;
            case pksm::Species::Kyogre:
                ret = {Default, Primal};
                break;
            case pksm::Species::Groudon:
                ret = {Default, Primal};
                break;
            case pksm::Species::Rayquaza:
                ret = {Default, Mega};
                break;
            case pksm::Species::Deoxys:
                ret = {Normal, Attack, Defense, Speed};
                break;
            case pksm::Species::Burmy:
                ret = {Plant, Sandy, Trash};
                break;
            case pksm::Species::Wormadam:
                ret = {Plant, Sandy, Trash};
                break;
            case pksm::Species::Cherrim:
                ret = {Overcast, Sunshine};
                break;
            case pksm::Species::Shellos:
                ret = {WestSea, EastSea};
                break;
            case pksm::Species::Gastrodon:
                ret = {WestSea, EastSea};
                break;
            case pksm::Species::Lopunny:
                ret = {Default, Mega};
                break;
            case pksm::Species::Garchomp:
                ret = {Default, Mega};
                break;
            case pksm::Species::Lucario:
                ret = {Default, Mega};
                break;
            case pksm::Species::Abomasnow:
                ret = {Default, Mega};
                break;
            case pksm::Species::Gallade:
                ret = {Default, Mega};
                break;
            case pksm::Species::Rotom:
                ret = {Default, Heat, Wash, Fridge, Fan, Mow};
                break;
            case pksm::Species::Giratina:
                ret = {Altered, Origin};
                break;
            case pksm::Species::Shaymin:
                ret = {Land, Sky};
                break;
            case pksm::Species::Arceus:
                ret = {Default, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel, Fire,
                    Water, Grass, Electric, Psychic, Ice, Dragon, Dark, Fairy};
                break;
            case pksm::Species::Audino:
                ret = {Default, Mega};
                break;
            case pksm::Species::Basculin:
                ret = {RedStriped, BlueStriped};
                break;
            case pksm::Species::Darumaka:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Darmanitan:
                ret = {Default, Zen, Galarian, Zen};
                break;
            case pksm::Species::Yamask:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Deerling:
            case pksm::Species::Sawsbuck:
                ret = {Spring, Summer, Autumn, Winter};
                break;
            case pksm::Species::Stunfisk:
                ret = {Default, Galarian};
                break;
            case pksm::Species::Tornadus:
                ret = {Incarnate, Therian};
                break;
            case pksm::Species::Thundurus:
                ret = {Incarnate, Therian};
                break;
            case pksm::Species::Landorus:
                ret = {Incarnate, Therian};
                break;
            case pksm::Species::Kyurem:
                ret = {Default, WhiteKyurem, Black};
                break;
            case pksm::Species::Keldeo:
                ret = {Ordinary, Resolute};
                break;
            case pksm::Species::Meloetta:
                ret = {Aria, Pirouette};
                break;
            case pksm::Species::Genesect:
                ret = {Default, Water, Electric, Fire, Ice};
                break;
            case pksm::Species::Greninja:
                ret = {Default, BattleBond, Ash};
                break;
            case pksm::Species::Scatterbug:
            case pksm::Species::Spewpa:
            case pksm::Species::Vivillon:
                ret = {IcySnow, Polar, Tundra, Continental, Garden, Elegant, Meadow, Modern, Marine,
                    Archipelago, HighPlains, Sandstorm, River, Monsoon, Savanna, Sun, Ocean, Jungle,
                    Fancy, PokeBall};
                break;
            case pksm::Species::Flabebe:
                ret = {RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower};
                break;
            case pksm::Species::Floette:
                ret = {
                    RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower, EternalFlower};
                break;
            case pksm::Species::Florges:
                ret = {RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower};
                break;
            case pksm::Species::Furfrou:
                ret = {Natural, Heart, Star, Diamond, Debutante, Matron, Dandy, LaReine, Kabuki,
                    Pharaoh};
                break;
            case pksm::Species::Meowstic:
                ret = {Default, Female};
                break;
            case pksm::Species::Aegislash:
                ret = {Shield, Blade};
                break;
            case pksm::Species::Pumpkaboo:
                ret = {Average, Small, Large, Super};
                break;
            case pksm::Species::Gourgeist:
                ret = {Average, Small, Large, Super};
                break;
            case pksm::Species::Zygarde:
                ret = {_50Percent, _10Percent, _10Percent_PC, _50Percent_PC, _100Percent};
                break;
            case pksm::Species::Diancie:
                ret = {Default, Mega};
                break;
            case pksm::Species::Hoopa:
                ret = {Confined, Unbound};
                break;
            case pksm::Species::Gumshoos:
                ret = {Default, Totem};
                break;
            case pksm::Species::Vikavolt:
                ret = {Default, Totem};
                break;
            case pksm::Species::Oricorio:
                ret = {Baile, PomPom, Pau, Sensu};
                break;
            case pksm::Species::Ribombee:
                ret = {Default, Totem};
                break;
            case pksm::Species::Rockruff:
                ret = {Default, Dusk};
                break;
            case pksm::Species::Lycanroc:
                ret = {Midday, Midnight, Dusk};
                break;
            case pksm::Species::Wishiwashi:
                ret = {Solo, School};
                break;
            case pksm::Species::Araquanid:
                ret = {Default, Totem};
                break;
            case pksm::Species::Lurantis:
                ret = {Default, Totem};
                break;
            case pksm::Species::Salazzle:
                ret = {Default, Totem};
                break;
            case pksm::Species::Silvally:
                ret = {Default, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel, Fire,
                    Water, Grass, Electric, Psychic, Ice, Dragon, Dark, Fairy};
                break;
            case pksm::Species::Minior:
                ret = {CoveredRed, CoveredOrange, CoveredYellow, CoveredGreen, CoveredBlue,
                    CoveredIndigo, CoveredViolet, Red, Orange, Yellow, Green, Blue, Indigo, Violet};
                break;
            case pksm::Species::Togedemaru:
                ret = {Default, Totem};
                break;
            case pksm::Species::Mimikyu:
                ret = {Default, Default, Totem, Totem};
                break;
            case pksm::Species::Kommoo:
                ret = {Default, Totem};
                break;
            case pksm::Species::Necrozma:
                ret = {Default, DawnWings, DuskMane, Ultra};
                break;
            case pksm::Species::Magearna:
                ret = {Default, OriginalColor};
                break;
            case pksm::Species::Cramorant:
                ret = {Default, Gulping, Gorging};
                break;
            case pksm::Species::Toxtricity:
                ret = {AmpedForm, LowKey};
                break;
            case pksm::Species::Indeedee:
                ret = {Default, Female};
                break;
            case pksm::Species::Sinistea:
            case pksm::Species::Polteageist:
                ret = {Phony, Antique};
                break;
            case pksm::Species::Alcremie:
                ret = {VanillaCream, RubyCream, MatchaCream, MintCream, LemonCream, SaltedCream,
                    RubySwirl, CaramelSwirl, RainbowSwirl};
                break;
            case pksm::Species::Morpeko:
                ret = {Default, HangryMode};
                break;
            case pksm::Species::Eiscue:
                ret = {Default, NoiceFace};
                break;
            case pksm::Species::Zacian:
                ret = {Default, Crowned};
                break;
            case pksm::Species::Zamazenta:
                ret = {Default, Crowned};
                break;
            case pksm::Species::Eternatus:
                ret = {Default, Eternamax};
                break;
            case pksm::Species::Urshifu:
                ret = {SingleStrike, RapidStrike};
                break;
            case pksm::Species::Zarude:
                ret = {Default, Dada};
                break;
            case pksm::Species::Calyrex:
                ret = {Default, IceRider, ShadowRider};
                break;
            default:
                ret = {Default};
                break;
        }

        while (ret.size() > forms)
        {
            ret.pop_back();
        }

        return ret;
    }

    const std::string& form(
        pksm::Language lang, pksm::GameVersion version, pksm::Species species, u8 form)
    {
        checkInitialized(lang);
        if (formss.count(lang) > 0)
        {
            auto indices = formIndices(version, species);
            if (form < indices.size())
            {
                size_t index = indices[form];
                if (index < formss[lang].size())
                {
                    return formss[lang][index];
                }
            }
        }
        return emptyString;
    }

    std::vector<std::string> forms(
        pksm::Language lang, pksm::GameVersion version, pksm::Species species)
    {
        checkInitialized(lang);
        std::vector<std::string> ret;
        if (formss.count(lang) > 0)
        {
            auto indices = formIndices(version, species);
            for (const auto& index : indices)
            {
                if (index < formss[lang].size())
                {
                    ret.emplace_back(formss[lang][index]);
                }
                else
                {
                    ret.emplace_back("");
                }
            }
        }
        return ret;
    }
}
