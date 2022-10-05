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
#include "utils/BidirectionalMap.hpp"
#include "utils/VersionTables.hpp"
#include <ranges>
#include <span>

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

    constexpr std::array Venusaur{Default, Mega};
    constexpr std::array Charizard{Default, MegaX, MegaY};
    constexpr std::array Blastoise{Default, Mega};
    constexpr std::array Beedrill{Default, Mega};
    constexpr std::array Pidgeot{Default, Mega};
    constexpr std::array Rattata{Default, Alolan};
    constexpr std::array Raticate{Default, Alolan, Totem};
    constexpr std::array PikachuSix{Default, RockStar, Belle, PopStar, PhD, Libre, Cosplay};
    constexpr std::array PikachuSeven{
        Default, Original, Hoenn, Sinnoh, Unova, Kalos, Alola, Partner, Default, World};
    constexpr std::array Raichu{Default, Alolan};
    constexpr std::array Sandslash{Default, Alolan};
    constexpr std::array Ninetales{Default, Alolan};
    constexpr std::array Dugtrio{Default, Alolan};
    constexpr std::array Meowth{Default, Alolan, Galarian};
    constexpr std::array Persian{Default, Alolan};
    constexpr std::array Alakazam{Default, Mega};
    constexpr std::array Golem{Default, Alolan};
    constexpr std::array Rapidash{Default, Galarian};
    constexpr std::array Slowpoke{Default, Galarian};
    constexpr std::array Slowbro{Default, Mega, Galarian};
    constexpr std::array Slowking{Default, Galarian};
    constexpr std::array Farfetchd{Default, Galarian};
    constexpr std::array Muk{Default, Alolan};
    constexpr std::array Gengar{Default, Mega};
    constexpr std::array Exeggutor{Default, Alolan};
    constexpr std::array Marowak{Default, Alolan, Totem};
    constexpr std::array Weezing{Default, Galarian};
    constexpr std::array MrMime{Default, Galarian};
    constexpr std::array Kangaskhan{Default, Mega};
    constexpr std::array Pinsir{Default, Mega};
    constexpr std::array Gyarados{Default, Mega};
    constexpr std::array Aerodactyl{Default, Mega};
    constexpr std::array Articuno{Default, Galarian};
    constexpr std::array Zapdos{Default, Galarian};
    constexpr std::array Moltres{Default, Galarian};
    constexpr std::array Mewtwo{Default, MegaX, MegaY};
    constexpr std::array Pichu{Default, SpikyEared};
    constexpr std::array Ampharos{Default, Mega};
    constexpr std::array Unown{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W,
        X, Y, Z, ExclamationPoint, QuestionMark};
    constexpr std::array Steelix{Default, Mega};
    constexpr std::array Scizor{Default, Mega};
    constexpr std::array Heracross{Default, Mega};
    constexpr std::array Corsola{Default, Galarian};
    constexpr std::array Houndoom{Default, Mega};
    constexpr std::array Tyranitar{Default, Mega};
    constexpr std::array Sceptile{Default, Mega};
    constexpr std::array Blaziken{Default, Mega};
    constexpr std::array Swampert{Default, Mega};
    constexpr std::array Linoone{Default, Galarian};
    constexpr std::array Gardevoir{Default, Mega};
    constexpr std::array Sableye{Default, Mega};
    constexpr std::array Mawile{Default, Mega};
    constexpr std::array Aggron{Default, Mega};
    constexpr std::array Medicham{Default, Mega};
    constexpr std::array Manectric{Default, Mega};
    constexpr std::array Sharpedo{Default, Mega};
    constexpr std::array Camerupt{Default, Mega};
    constexpr std::array Altaria{Default, Mega};
    constexpr std::array Castform{Default, Sunny, Rainy, Snowy};
    constexpr std::array Banette{Default, Mega};
    constexpr std::array Absol{Default, Mega};
    constexpr std::array Glalie{Default, Mega};
    constexpr std::array Salamence{Default, Mega};
    constexpr std::array Metagross{Default, Mega};
    constexpr std::array Latias{Default, Mega};
    constexpr std::array Latios{Default, Mega};
    constexpr std::array Kyogre{Default, Primal};
    constexpr std::array Groudon{Default, Primal};
    constexpr std::array Rayquaza{Default, Mega};
    constexpr std::array Deoxys{Normal, Attack, Defense, Speed};
    constexpr std::array Burmy{Plant, Sandy, Trash};
    constexpr std::array Wormadam{Plant, Sandy, Trash};
    constexpr std::array Cherrim{Overcast, Sunshine};
    constexpr std::array Shellos{WestSea, EastSea};
    constexpr std::array Gastrodon{WestSea, EastSea};
    constexpr std::array Lopunny{Default, Mega};
    constexpr std::array Garchomp{Default, Mega};
    constexpr std::array Lucario{Default, Mega};
    constexpr std::array Abomasnow{Default, Mega};
    constexpr std::array Gallade{Default, Mega};
    constexpr std::array Rotom{Default, Heat, Wash, Fridge, Fan, Mow};
    constexpr std::array Giratina{Altered, Origin};
    constexpr std::array Shaymin{Land, Sky};
    constexpr std::array Arceus{Default, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel,
        Fire, Water, Grass, Electric, Psychic, Ice, Dragon, Dark, Fairy};
    constexpr std::array Audino{Default, Mega};
    constexpr std::array Basculin{RedStriped, BlueStriped};
    constexpr std::array Darumaka{Default, Galarian};
    constexpr std::array Darmanitan{Default, Zen, Galarian, Zen};
    constexpr std::array Yamask{Default, Galarian};
    constexpr std::array Sawsbuck{Spring, Summer, Autumn, Winter};
    constexpr std::array Stunfisk{Default, Galarian};
    constexpr std::array Tornadus{Incarnate, Therian};
    constexpr std::array Thundurus{Incarnate, Therian};
    constexpr std::array Landorus{Incarnate, Therian};
    constexpr std::array Kyurem{Default, WhiteKyurem, Black};
    constexpr std::array Keldeo{Ordinary, Resolute};
    constexpr std::array Meloetta{Aria, Pirouette};
    constexpr std::array Genesect{Default, Water, Electric, Fire, Ice};
    constexpr std::array Greninja{Default, BattleBond, Ash};
    constexpr std::array Vivillon{IcySnow, Polar, Tundra, Continental, Garden, Elegant, Meadow,
        Modern, Marine, Archipelago, HighPlains, Sandstorm, River, Monsoon, Savanna, Sun, Ocean,
        Jungle, Fancy, PokeBall};
    constexpr std::array Flabebe{RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower};
    constexpr std::array Floette{
        RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower, EternalFlower};
    constexpr std::array Florges{RedFlower, YellowFlower, OrangeFlower, BlueFlower, WhiteFlower};
    constexpr std::array Furfrou{
        Natural, Heart, Star, Diamond, Debutante, Matron, Dandy, LaReine, Kabuki, Pharaoh};
    constexpr std::array Meowstic{Default, Female};
    constexpr std::array Aegislash{Shield, Blade};
    constexpr std::array Pumpkaboo{Average, Small, Large, Super};
    constexpr std::array Gourgeist{Average, Small, Large, Super};
    constexpr std::array Zygarde{_50Percent, _10Percent, _10Percent_PC, _50Percent_PC, _100Percent};
    constexpr std::array Diancie{Default, Mega};
    constexpr std::array Hoopa{Confined, Unbound};
    constexpr std::array Gumshoos{Default, Totem};
    constexpr std::array Vikavolt{Default, Totem};
    constexpr std::array Oricorio{Baile, PomPom, Pau, Sensu};
    constexpr std::array Ribombee{Default, Totem};
    constexpr std::array Rockruff{Default, Dusk};
    constexpr std::array Lycanroc{Midday, Midnight, Dusk};
    constexpr std::array Wishiwashi{Solo, School};
    constexpr std::array Araquanid{Default, Totem};
    constexpr std::array Lurantis{Default, Totem};
    constexpr std::array Salazzle{Default, Totem};
    constexpr std::array Silvally{Default, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost,
        Steel, Fire, Water, Grass, Electric, Psychic, Ice, Dragon, Dark, Fairy};
    constexpr std::array Minior{CoveredRed, CoveredOrange, CoveredYellow, CoveredGreen, CoveredBlue,
        CoveredIndigo, CoveredViolet, Red, Orange, Yellow, Green, Blue, Indigo, Violet};
    constexpr std::array Togedemaru{Default, Totem};
    constexpr std::array Mimikyu{Default, Default, Totem, Totem};
    constexpr std::array Kommoo{Default, Totem};
    constexpr std::array Necrozma{Default, DawnWings, DuskMane, Ultra};
    constexpr std::array Magearna{Default, OriginalColor};
    constexpr std::array Cramorant{Default, Gulping, Gorging};
    constexpr std::array Toxtricity{AmpedForm, LowKey};
    constexpr std::array Indeedee{Default, Female};
    constexpr std::array Polteageist{Phony, Antique};
    constexpr std::array Alcremie{VanillaCream, RubyCream, MatchaCream, MintCream, LemonCream,
        SaltedCream, RubySwirl, CaramelSwirl, RainbowSwirl};
    constexpr std::array Morpeko{Default, HangryMode};
    constexpr std::array Eiscue{Default, NoiceFace};
    constexpr std::array Zacian{Default, Crowned};
    constexpr std::array Zamazenta{Default, Crowned};
    constexpr std::array Eternatus{Default, Eternamax};
    constexpr std::array Urshifu{SingleStrike, RapidStrike};
    constexpr std::array Zarude{Default, Dada};
    constexpr std::array Calyrex{Default, IceRider, ShadowRider};
}

namespace i18n
{
    std::unordered_map<pksm::Language, std::vector<std::string>> formss = std::invoke([] {
        std::unordered_map<pksm::Language, std::vector<std::string>> ret;
        MAP(MAKE_GENERIC_LANGMAP, LANGUAGES_TO_USE)
        return ret;
    });

    void initForm(pksm::Language lang)
    {
        std::vector<std::string> vec;
        load(lang, "/forms.txt", vec);
        formss.insert_or_assign(lang, std::move(vec));
    }

    void exitForm(pksm::Language lang) { formss[lang].clear(); }

    const std::span<const size_t> formIndices(pksm::GameVersion version, pksm::Species species)
    {
        // TODO: Gigantamax and Isle of Armor Galarian additions, including Slowbro
        u8 forms = pksm::VersionTables::formCount(version, species);

        constexpr std::array DefaultFormArray{Default};
        std::span<const size_t> ret = DefaultFormArray;
        switch (species)
        {
            case pksm::Species::Venusaur:
                ret = Venusaur;
                break;
            case pksm::Species::Charizard:
                ret = Charizard;
                break;
            case pksm::Species::Blastoise:
                ret = Blastoise;
                break;
            case pksm::Species::Beedrill:
                ret = Beedrill;
                break;
            case pksm::Species::Pidgeot:
                ret = Pidgeot;
                break;
            case pksm::Species::Rattata:
                ret = Rattata;
                break;
            case pksm::Species::Raticate:
                ret = Raticate;
                break;
            case pksm::Species::Pikachu:
                switch ((pksm::Generation)version)
                {
                    case pksm::Generation::SIX:
                        ret = PikachuSix;
                        break;
                    case pksm::Generation::SEVEN:
                    case pksm::Generation::LGPE:
                        ret = PikachuSeven;
                        break;
                    default:
                        break;
                }
                break;
            case pksm::Species::Raichu:
                ret = Raichu;
                break;
            case pksm::Species::Sandshrew:
            case pksm::Species::Sandslash:
                ret = Sandslash;
                break;
            case pksm::Species::Vulpix:
            case pksm::Species::Ninetales:
                ret = Ninetales;
                break;
            case pksm::Species::Diglett:
            case pksm::Species::Dugtrio:
                ret = Dugtrio;
                break;
            case pksm::Species::Meowth:
                ret = Meowth;
                break;
            case pksm::Species::Persian:
                ret = Persian;
                break;
            case pksm::Species::Alakazam:
                ret = Alakazam;
                break;
            case pksm::Species::Geodude:
            case pksm::Species::Graveler:
            case pksm::Species::Golem:
                ret = Golem;
                break;
            case pksm::Species::Ponyta:
            case pksm::Species::Rapidash:
                ret = Rapidash;
                break;
            case pksm::Species::Slowpoke:
                ret = Slowpoke;
                break;
            case pksm::Species::Slowbro:
                ret = Slowbro;
                break;
            case pksm::Species::Slowking:
                ret = Slowking;
                break;
            case pksm::Species::Farfetchd:
                ret = Farfetchd;
                break;
            case pksm::Species::Grimer:
            case pksm::Species::Muk:
                ret = Muk;
                break;
            case pksm::Species::Gengar:
                ret = Gengar;
                break;
            case pksm::Species::Exeggutor:
                ret = Exeggutor;
                break;
            case pksm::Species::Marowak:
                ret = Marowak;
                break;
            case pksm::Species::Weezing:
                ret = Weezing;
                break;
            case pksm::Species::MrMime:
                ret = MrMime;
                break;
            case pksm::Species::Kangaskhan:
                ret = Kangaskhan;
                break;
            case pksm::Species::Pinsir:
                ret = Pinsir;
                break;
            case pksm::Species::Gyarados:
                ret = Gyarados;
                break;
            case pksm::Species::Aerodactyl:
                ret = Aerodactyl;
                break;
            case pksm::Species::Articuno:
                ret = Articuno;
                break;
            case pksm::Species::Zapdos:
                ret = Zapdos;
                break;
            case pksm::Species::Moltres:
                ret = Moltres;
                break;
            case pksm::Species::Mewtwo:
                ret = Mewtwo;
                break;
            case pksm::Species::Pichu:
                ret = Pichu;
                break;
            case pksm::Species::Ampharos:
                ret = Ampharos;
                break;
            case pksm::Species::Unown:
                ret = Unown;
                break;
            case pksm::Species::Steelix:
                ret = Steelix;
                break;
            case pksm::Species::Scizor:
                ret = Scizor;
                break;
            case pksm::Species::Heracross:
                ret = Heracross;
                break;
            case pksm::Species::Corsola:
                ret = Corsola;
                break;
            case pksm::Species::Houndoom:
                ret = Houndoom;
                break;
            case pksm::Species::Tyranitar:
                ret = Tyranitar;
                break;
            case pksm::Species::Sceptile:
                ret = Sceptile;
                break;
            case pksm::Species::Blaziken:
                ret = Blaziken;
                break;
            case pksm::Species::Swampert:
                ret = Swampert;
                break;
            case pksm::Species::Zigzagoon:
            case pksm::Species::Linoone:
                ret = Linoone;
                break;
            case pksm::Species::Gardevoir:
                ret = Gardevoir;
                break;
            case pksm::Species::Sableye:
                ret = Sableye;
                break;
            case pksm::Species::Mawile:
                ret = Mawile;
                break;
            case pksm::Species::Aggron:
                ret = Aggron;
                break;
            case pksm::Species::Medicham:
                ret = Medicham;
                break;
            case pksm::Species::Manectric:
                ret = Manectric;
                break;
            case pksm::Species::Sharpedo:
                ret = Sharpedo;
                break;
            case pksm::Species::Camerupt:
                ret = Camerupt;
                break;
            case pksm::Species::Altaria:
                ret = Altaria;
                break;
            case pksm::Species::Castform:
                ret = Castform;
                break;
            case pksm::Species::Banette:
                ret = Banette;
                break;
            case pksm::Species::Absol:
                ret = Absol;
                break;
            case pksm::Species::Glalie:
                ret = Glalie;
                break;
            case pksm::Species::Salamence:
                ret = Salamence;
                break;
            case pksm::Species::Metagross:
                ret = Metagross;
                break;
            case pksm::Species::Latias:
                ret = Latias;
                break;
            case pksm::Species::Latios:
                ret = Latios;
                break;
            case pksm::Species::Kyogre:
                ret = Kyogre;
                break;
            case pksm::Species::Groudon:
                ret = Groudon;
                break;
            case pksm::Species::Rayquaza:
                ret = Rayquaza;
                break;
            case pksm::Species::Deoxys:
                ret = Deoxys;
                break;
            case pksm::Species::Burmy:
                ret = Burmy;
                break;
            case pksm::Species::Wormadam:
                ret = Wormadam;
                break;
            case pksm::Species::Cherrim:
                ret = Cherrim;
                break;
            case pksm::Species::Shellos:
                ret = Shellos;
                break;
            case pksm::Species::Gastrodon:
                ret = Gastrodon;
                break;
            case pksm::Species::Lopunny:
                ret = Lopunny;
                break;
            case pksm::Species::Garchomp:
                ret = Garchomp;
                break;
            case pksm::Species::Lucario:
                ret = Lucario;
                break;
            case pksm::Species::Abomasnow:
                ret = Abomasnow;
                break;
            case pksm::Species::Gallade:
                ret = Gallade;
                break;
            case pksm::Species::Rotom:
                ret = Rotom;
                break;
            case pksm::Species::Giratina:
                ret = Giratina;
                break;
            case pksm::Species::Shaymin:
                ret = Shaymin;
                break;
            case pksm::Species::Arceus:
                ret = Arceus;
                break;
            case pksm::Species::Audino:
                ret = Audino;
                break;
            case pksm::Species::Basculin:
                ret = Basculin;
                break;
            case pksm::Species::Darumaka:
                ret = Darumaka;
                break;
            case pksm::Species::Darmanitan:
                ret = Darmanitan;
                break;
            case pksm::Species::Yamask:
                ret = Yamask;
                break;
            case pksm::Species::Deerling:
            case pksm::Species::Sawsbuck:
                ret = Sawsbuck;
                break;
            case pksm::Species::Stunfisk:
                ret = Stunfisk;
                break;
            case pksm::Species::Tornadus:
                ret = Tornadus;
                break;
            case pksm::Species::Thundurus:
                ret = Thundurus;
                break;
            case pksm::Species::Landorus:
                ret = Landorus;
                break;
            case pksm::Species::Kyurem:
                ret = Kyurem;
                break;
            case pksm::Species::Keldeo:
                ret = Keldeo;
                break;
            case pksm::Species::Meloetta:
                ret = Meloetta;
                break;
            case pksm::Species::Genesect:
                ret = Genesect;
                break;
            case pksm::Species::Greninja:
                ret = Greninja;
                break;
            case pksm::Species::Scatterbug:
            case pksm::Species::Spewpa:
            case pksm::Species::Vivillon:
                ret = Vivillon;
                break;
            case pksm::Species::Flabebe:
                ret = Flabebe;
                break;
            case pksm::Species::Floette:
                ret = Floette;
                break;
            case pksm::Species::Florges:
                ret = Florges;
                break;
            case pksm::Species::Furfrou:
                ret = Furfrou;
                break;
            case pksm::Species::Meowstic:
                ret = Meowstic;
                break;
            case pksm::Species::Aegislash:
                ret = Aegislash;
                break;
            case pksm::Species::Pumpkaboo:
                ret = Pumpkaboo;
                break;
            case pksm::Species::Gourgeist:
                ret = Gourgeist;
                break;
            case pksm::Species::Zygarde:
                ret = Zygarde;
                break;
            case pksm::Species::Diancie:
                ret = Diancie;
                break;
            case pksm::Species::Hoopa:
                ret = Hoopa;
                break;
            case pksm::Species::Gumshoos:
                ret = Gumshoos;
                break;
            case pksm::Species::Vikavolt:
                ret = Vikavolt;
                break;
            case pksm::Species::Oricorio:
                ret = Oricorio;
                break;
            case pksm::Species::Ribombee:
                ret = Ribombee;
                break;
            case pksm::Species::Rockruff:
                ret = Rockruff;
                break;
            case pksm::Species::Lycanroc:
                ret = Lycanroc;
                break;
            case pksm::Species::Wishiwashi:
                ret = Wishiwashi;
                break;
            case pksm::Species::Araquanid:
                ret = Araquanid;
                break;
            case pksm::Species::Lurantis:
                ret = Lurantis;
                break;
            case pksm::Species::Salazzle:
                ret = Salazzle;
                break;
            case pksm::Species::Silvally:
                ret = Silvally;
                break;
            case pksm::Species::Minior:
                ret = Minior;
                break;
            case pksm::Species::Togedemaru:
                ret = Togedemaru;
                break;
            case pksm::Species::Mimikyu:
                ret = Mimikyu;
                break;
            case pksm::Species::Kommoo:
                ret = Kommoo;
                break;
            case pksm::Species::Necrozma:
                ret = Necrozma;
                break;
            case pksm::Species::Magearna:
                ret = Magearna;
                break;
            case pksm::Species::Cramorant:
                ret = Cramorant;
                break;
            case pksm::Species::Toxtricity:
                ret = Toxtricity;
                break;
            case pksm::Species::Indeedee:
                ret = Indeedee;
                break;
            case pksm::Species::Sinistea:
            case pksm::Species::Polteageist:
                ret = Polteageist;
                break;
            case pksm::Species::Alcremie:
                ret = Alcremie;
                break;
            case pksm::Species::Morpeko:
                ret = Morpeko;
                break;
            case pksm::Species::Eiscue:
                ret = Eiscue;
                break;
            case pksm::Species::Zacian:
                ret = Zacian;
                break;
            case pksm::Species::Zamazenta:
                ret = Zamazenta;
                break;
            case pksm::Species::Eternatus:
                ret = Eternatus;
                break;
            case pksm::Species::Urshifu:
                ret = Urshifu;
                break;
            case pksm::Species::Zarude:
                ret = Zarude;
                break;
            case pksm::Species::Calyrex:
                ret = Calyrex;
                break;
            default:
                break;
        }

        return ret.subspan(0, forms);
    }

    const std::string& form(
        pksm::Language lang, pksm::GameVersion version, pksm::Species species, u8 form)
    {
        checkInitialized(lang);
        auto indices = formIndices(version, species);
        if (form < indices.size())
        {
            size_t index = indices[form];
            if (index < formss[lang].size())
            {
                return formss[lang][index];
            }
        }
        return emptyString;
    }

    std::vector<std::string> forms(
        pksm::Language lang, pksm::GameVersion version, pksm::Species species)
    {
        checkInitialized(lang);
        std::vector<std::string> ret;
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
        return ret;
    }
}
