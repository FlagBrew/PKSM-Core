/*
 *   This file is part of PKSM-Core
 *   Copyright (C) 2016-2020 Bernardo Giordano, Admiral Fish, piepie62, Pk11
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

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include "enums/Generation.hpp"
#include "enums/Language.hpp"
#include <limits>
#include <type_traits>

class Species_impl
{
    friend class Species;

private:
    enum class SpeciesEnum : u16
    {
        None,
        Bulbasaur,
        Ivysaur,
        Venusaur,
        Charmander,
        Charmeleon,
        Charizard,
        Squirtle,
        Wartortle,
        Blastoise,
        Caterpie,
        Metapod,
        Butterfree,
        Weedle,
        Kakuna,
        Beedrill,
        Pidgey,
        Pidgeotto,
        Pidgeot,
        Rattata,
        Raticate,
        Spearow,
        Fearow,
        Ekans,
        Arbok,
        Pikachu,
        Raichu,
        Sandshrew,
        Sandslash,
        NidoranF,
        Nidorina,
        Nidoqueen,
        NidoranM,
        Nidorino,
        Nidoking,
        Clefairy,
        Clefable,
        Vulpix,
        Ninetales,
        Jigglypuff,
        Wigglytuff,
        Zubat,
        Golbat,
        Oddish,
        Gloom,
        Vileplume,
        Paras,
        Parasect,
        Venonat,
        Venomoth,
        Diglett,
        Dugtrio,
        Meowth,
        Persian,
        Psyduck,
        Golduck,
        Mankey,
        Primeape,
        Growlithe,
        Arcanine,
        Poliwag,
        Poliwhirl,
        Poliwrath,
        Abra,
        Kadabra,
        Alakazam,
        Machop,
        Machoke,
        Machamp,
        Bellsprout,
        Weepinbell,
        Victreebel,
        Tentacool,
        Tentacruel,
        Geodude,
        Graveler,
        Golem,
        Ponyta,
        Rapidash,
        Slowpoke,
        Slowbro,
        Magnemite,
        Magneton,
        Farfetchd,
        Doduo,
        Dodrio,
        Seel,
        Dewgong,
        Grimer,
        Muk,
        Shellder,
        Cloyster,
        Gastly,
        Haunter,
        Gengar,
        Onix,
        Drowzee,
        Hypno,
        Krabby,
        Kingler,
        Voltorb,
        Electrode,
        Exeggcute,
        Exeggutor,
        Cubone,
        Marowak,
        Hitmonlee,
        Hitmonchan,
        Lickitung,
        Koffing,
        Weezing,
        Rhyhorn,
        Rhydon,
        Chansey,
        Tangela,
        Kangaskhan,
        Horsea,
        Seadra,
        Goldeen,
        Seaking,
        Staryu,
        Starmie,
        MrMime,
        Scyther,
        Jynx,
        Electabuzz,
        Magmar,
        Pinsir,
        Tauros,
        Magikarp,
        Gyarados,
        Lapras,
        Ditto,
        Eevee,
        Vaporeon,
        Jolteon,
        Flareon,
        Porygon,
        Omanyte,
        Omastar,
        Kabuto,
        Kabutops,
        Aerodactyl,
        Snorlax,
        Articuno,
        Zapdos,
        Moltres,
        Dratini,
        Dragonair,
        Dragonite,
        Mewtwo,
        Mew,
        Chikorita,
        Bayleef,
        Meganium,
        Cyndaquil,
        Quilava,
        Typhlosion,
        Totodile,
        Croconaw,
        Feraligatr,
        Sentret,
        Furret,
        Hoothoot,
        Noctowl,
        Ledyba,
        Ledian,
        Spinarak,
        Ariados,
        Crobat,
        Chinchou,
        Lanturn,
        Pichu,
        Cleffa,
        Igglybuff,
        Togepi,
        Togetic,
        Natu,
        Xatu,
        Mareep,
        Flaaffy,
        Ampharos,
        Bellossom,
        Marill,
        Azumarill,
        Sudowoodo,
        Politoed,
        Hoppip,
        Skiploom,
        Jumpluff,
        Aipom,
        Sunkern,
        Sunflora,
        Yanma,
        Wooper,
        Quagsire,
        Espeon,
        Umbreon,
        Murkrow,
        Slowking,
        Misdreavus,
        Unown,
        Wobbuffet,
        Girafarig,
        Pineco,
        Forretress,
        Dunsparce,
        Gligar,
        Steelix,
        Snubbull,
        Granbull,
        Qwilfish,
        Scizor,
        Shuckle,
        Heracross,
        Sneasel,
        Teddiursa,
        Ursaring,
        Slugma,
        Magcargo,
        Swinub,
        Piloswine,
        Corsola,
        Remoraid,
        Octillery,
        Delibird,
        Mantine,
        Skarmory,
        Houndour,
        Houndoom,
        Kingdra,
        Phanpy,
        Donphan,
        Porygon2,
        Stantler,
        Smeargle,
        Tyrogue,
        Hitmontop,
        Smoochum,
        Elekid,
        Magby,
        Miltank,
        Blissey,
        Raikou,
        Entei,
        Suicune,
        Larvitar,
        Pupitar,
        Tyranitar,
        Lugia,
        HoOh,
        Celebi,
        Treecko,
        Grovyle,
        Sceptile,
        Torchic,
        Combusken,
        Blaziken,
        Mudkip,
        Marshtomp,
        Swampert,
        Poochyena,
        Mightyena,
        Zigzagoon,
        Linoone,
        Wurmple,
        Silcoon,
        Beautifly,
        Cascoon,
        Dustox,
        Lotad,
        Lombre,
        Ludicolo,
        Seedot,
        Nuzleaf,
        Shiftry,
        Taillow,
        Swellow,
        Wingull,
        Pelipper,
        Ralts,
        Kirlia,
        Gardevoir,
        Surskit,
        Masquerain,
        Shroomish,
        Breloom,
        Slakoth,
        Vigoroth,
        Slaking,
        Nincada,
        Ninjask,
        Shedinja,
        Whismur,
        Loudred,
        Exploud,
        Makuhita,
        Hariyama,
        Azurill,
        Nosepass,
        Skitty,
        Delcatty,
        Sableye,
        Mawile,
        Aron,
        Lairon,
        Aggron,
        Meditite,
        Medicham,
        Electrike,
        Manectric,
        Plusle,
        Minun,
        Volbeat,
        Illumise,
        Roselia,
        Gulpin,
        Swalot,
        Carvanha,
        Sharpedo,
        Wailmer,
        Wailord,
        Numel,
        Camerupt,
        Torkoal,
        Spoink,
        Grumpig,
        Spinda,
        Trapinch,
        Vibrava,
        Flygon,
        Cacnea,
        Cacturne,
        Swablu,
        Altaria,
        Zangoose,
        Seviper,
        Lunatone,
        Solrock,
        Barboach,
        Whiscash,
        Corphish,
        Crawdaunt,
        Baltoy,
        Claydol,
        Lileep,
        Cradily,
        Anorith,
        Armaldo,
        Feebas,
        Milotic,
        Castform,
        Kecleon,
        Shuppet,
        Banette,
        Duskull,
        Dusclops,
        Tropius,
        Chimecho,
        Absol,
        Wynaut,
        Snorunt,
        Glalie,
        Spheal,
        Sealeo,
        Walrein,
        Clamperl,
        Huntail,
        Gorebyss,
        Relicanth,
        Luvdisc,
        Bagon,
        Shelgon,
        Salamence,
        Beldum,
        Metang,
        Metagross,
        Regirock,
        Regice,
        Registeel,
        Latias,
        Latios,
        Kyogre,
        Groudon,
        Rayquaza,
        Jirachi,
        Deoxys,
        Turtwig,
        Grotle,
        Torterra,
        Chimchar,
        Monferno,
        Infernape,
        Piplup,
        Prinplup,
        Empoleon,
        Starly,
        Staravia,
        Staraptor,
        Bidoof,
        Bibarel,
        Kricketot,
        Kricketune,
        Shinx,
        Luxio,
        Luxray,
        Budew,
        Roserade,
        Cranidos,
        Rampardos,
        Shieldon,
        Bastiodon,
        Burmy,
        Wormadam,
        Mothim,
        Combee,
        Vespiquen,
        Pachirisu,
        Buizel,
        Floatzel,
        Cherubi,
        Cherrim,
        Shellos,
        Gastrodon,
        Ambipom,
        Drifloon,
        Drifblim,
        Buneary,
        Lopunny,
        Mismagius,
        Honchkrow,
        Glameow,
        Purugly,
        Chingling,
        Stunky,
        Skuntank,
        Bronzor,
        Bronzong,
        Bonsly,
        MimeJr,
        Happiny,
        Chatot,
        Spiritomb,
        Gible,
        Gabite,
        Garchomp,
        Munchlax,
        Riolu,
        Lucario,
        Hippopotas,
        Hippowdon,
        Skorupi,
        Drapion,
        Croagunk,
        Toxicroak,
        Carnivine,
        Finneon,
        Lumineon,
        Mantyke,
        Snover,
        Abomasnow,
        Weavile,
        Magnezone,
        Lickilicky,
        Rhyperior,
        Tangrowth,
        Electivire,
        Magmortar,
        Togekiss,
        Yanmega,
        Leafeon,
        Glaceon,
        Gliscor,
        Mamoswine,
        PorygonZ,
        Gallade,
        Probopass,
        Dusknoir,
        Froslass,
        Rotom,
        Uxie,
        Mesprit,
        Azelf,
        Dialga,
        Palkia,
        Heatran,
        Regigigas,
        Giratina,
        Cresselia,
        Phione,
        Manaphy,
        Darkrai,
        Shaymin,
        Arceus,
        Victini,
        Snivy,
        Servine,
        Serperior,
        Tepig,
        Pignite,
        Emboar,
        Oshawott,
        Dewott,
        Samurott,
        Patrat,
        Watchog,
        Lillipup,
        Herdier,
        Stoutland,
        Purrloin,
        Liepard,
        Pansage,
        Simisage,
        Pansear,
        Simisear,
        Panpour,
        Simipour,
        Munna,
        Musharna,
        Pidove,
        Tranquill,
        Unfezant,
        Blitzle,
        Zebstrika,
        Roggenrola,
        Boldore,
        Gigalith,
        Woobat,
        Swoobat,
        Drilbur,
        Excadrill,
        Audino,
        Timburr,
        Gurdurr,
        Conkeldurr,
        Tympole,
        Palpitoad,
        Seismitoad,
        Throh,
        Sawk,
        Sewaddle,
        Swadloon,
        Leavanny,
        Venipede,
        Whirlipede,
        Scolipede,
        Cottonee,
        Whimsicott,
        Petilil,
        Lilligant,
        Basculin,
        Sandile,
        Krokorok,
        Krookodile,
        Darumaka,
        Darmanitan,
        Maractus,
        Dwebble,
        Crustle,
        Scraggy,
        Scrafty,
        Sigilyph,
        Yamask,
        Cofagrigus,
        Tirtouga,
        Carracosta,
        Archen,
        Archeops,
        Trubbish,
        Garbodor,
        Zorua,
        Zoroark,
        Minccino,
        Cinccino,
        Gothita,
        Gothorita,
        Gothitelle,
        Solosis,
        Duosion,
        Reuniclus,
        Ducklett,
        Swanna,
        Vanillite,
        Vanillish,
        Vanilluxe,
        Deerling,
        Sawsbuck,
        Emolga,
        Karrablast,
        Escavalier,
        Foongus,
        Amoonguss,
        Frillish,
        Jellicent,
        Alomomola,
        Joltik,
        Galvantula,
        Ferroseed,
        Ferrothorn,
        Klink,
        Klang,
        Klinklang,
        Tynamo,
        Eelektrik,
        Eelektross,
        Elgyem,
        Beheeyem,
        Litwick,
        Lampent,
        Chandelure,
        Axew,
        Fraxure,
        Haxorus,
        Cubchoo,
        Beartic,
        Cryogonal,
        Shelmet,
        Accelgor,
        Stunfisk,
        Mienfoo,
        Mienshao,
        Druddigon,
        Golett,
        Golurk,
        Pawniard,
        Bisharp,
        Bouffalant,
        Rufflet,
        Braviary,
        Vullaby,
        Mandibuzz,
        Heatmor,
        Durant,
        Deino,
        Zweilous,
        Hydreigon,
        Larvesta,
        Volcarona,
        Cobalion,
        Terrakion,
        Virizion,
        Tornadus,
        Thundurus,
        Reshiram,
        Zekrom,
        Landorus,
        Kyurem,
        Keldeo,
        Meloetta,
        Genesect,
        Chespin,
        Quilladin,
        Chesnaught,
        Fennekin,
        Braixen,
        Delphox,
        Froakie,
        Frogadier,
        Greninja,
        Bunnelby,
        Diggersby,
        Fletchling,
        Fletchinder,
        Talonflame,
        Scatterbug,
        Spewpa,
        Vivillon,
        Litleo,
        Pyroar,
        Flabebe,
        Floette,
        Florges,
        Skiddo,
        Gogoat,
        Pancham,
        Pangoro,
        Furfrou,
        Espurr,
        Meowstic,
        Honedge,
        Doublade,
        Aegislash,
        Spritzee,
        Aromatisse,
        Swirlix,
        Slurpuff,
        Inkay,
        Malamar,
        Binacle,
        Barbaracle,
        Skrelp,
        Dragalge,
        Clauncher,
        Clawitzer,
        Helioptile,
        Heliolisk,
        Tyrunt,
        Tyrantrum,
        Amaura,
        Aurorus,
        Sylveon,
        Hawlucha,
        Dedenne,
        Carbink,
        Goomy,
        Sliggoo,
        Goodra,
        Klefki,
        Phantump,
        Trevenant,
        Pumpkaboo,
        Gourgeist,
        Bergmite,
        Avalugg,
        Noibat,
        Noivern,
        Xerneas,
        Yveltal,
        Zygarde,
        Diancie,
        Hoopa,
        Volcanion,
        Rowlet,
        Dartrix,
        Decidueye,
        Litten,
        Torracat,
        Incineroar,
        Popplio,
        Brionne,
        Primarina,
        Pikipek,
        Trumbeak,
        Toucannon,
        Yungoos,
        Gumshoos,
        Grubbin,
        Charjabug,
        Vikavolt,
        Crabrawler,
        Crabominable,
        Oricorio,
        Cutiefly,
        Ribombee,
        Rockruff,
        Lycanroc,
        Wishiwashi,
        Mareanie,
        Toxapex,
        Mudbray,
        Mudsdale,
        Dewpider,
        Araquanid,
        Fomantis,
        Lurantis,
        Morelull,
        Shiinotic,
        Salandit,
        Salazzle,
        Stufful,
        Bewear,
        Bounsweet,
        Steenee,
        Tsareena,
        Comfey,
        Oranguru,
        Passimian,
        Wimpod,
        Golisopod,
        Sandygast,
        Palossand,
        Pyukumuku,
        TypeNull,
        Silvally,
        Minior,
        Komala,
        Turtonator,
        Togedemaru,
        Mimikyu,
        Bruxish,
        Drampa,
        Dhelmise,
        Jangmoo,
        Hakamoo,
        Kommoo,
        TapuKoko,
        TapuLele,
        TapuBulu,
        TapuFini,
        Cosmog,
        Cosmoem,
        Solgaleo,
        Lunala,
        Nihilego,
        Buzzwole,
        Pheromosa,
        Xurkitree,
        Celesteela,
        Kartana,
        Guzzlord,
        Necrozma,
        Magearna,
        Marshadow,
        Poipole,
        Naganadel,
        Stakataka,
        Blacephalon,
        Zeraora,
        Meltan,
        Melmetal,
        Grookey,
        Thwackey,
        Rillaboom,
        Scorbunny,
        Raboot,
        Cinderace,
        Sobble,
        Drizzile,
        Inteleon,
        Skwovet,
        Greedent,
        Rookidee,
        Corvisquire,
        Corviknight,
        Blipbug,
        Dottler,
        Orbeetle,
        Nickit,
        Thievul,
        Gossifleur,
        Eldegoss,
        Wooloo,
        Dubwool,
        Chewtle,
        Drednaw,
        Yamper,
        Boltund,
        Rolycoly,
        Carkol,
        Coalossal,
        Applin,
        Flapple,
        Appletun,
        Silicobra,
        Sandaconda,
        Cramorant,
        Arrokuda,
        Barraskewda,
        Toxel,
        Toxtricity,
        Sizzlipede,
        Centiskorch,
        Clobbopus,
        Grapploct,
        Sinistea,
        Polteageist,
        Hatenna,
        Hattrem,
        Hatterene,
        Impidimp,
        Morgrem,
        Grimmsnarl,
        Obstagoon,
        Perrserker,
        Cursola,
        Sirfetchd,
        MrRime,
        Runerigus,
        Milcery,
        Alcremie,
        Falinks,
        Pincurchin,
        Snom,
        Frosmoth,
        Stonjourner,
        Eiscue,
        Indeedee,
        Morpeko,
        Cufant,
        Copperajah,
        Dracozolt,
        Arctozolt,
        Dracovish,
        Arctovish,
        Duraludon,
        Dreepy,
        Drakloak,
        Dragapult,
        Zacian,
        Zamazenta,
        Eternatus,

        INVALID = std::numeric_limits<std::underlying_type_t<SpeciesEnum>>::max()
    } v;
    constexpr explicit Species_impl(SpeciesEnum v) : v(v) {}
    constexpr Species_impl(const Species_impl&) = default;
    constexpr Species_impl(Species_impl&&)      = default;
    constexpr Species_impl& operator=(const Species_impl&) = default;
    constexpr Species_impl& operator=(Species_impl&&) = default;

public:
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        static_assert(std::is_integral_v<T>);
        return T(v);
    }
    constexpr operator SpeciesEnum() const noexcept { return v; }

    constexpr bool operator<(const Species_impl& other) const noexcept { return v < other.v; }
    constexpr bool operator<=(const Species_impl& other) const noexcept { return v <= other.v; }

    constexpr bool operator>(const Species_impl& other) const noexcept { return v > other.v; }
    constexpr bool operator>=(const Species_impl& other) const noexcept { return v >= other.v; }

    constexpr bool operator==(const Species_impl& other) const noexcept { return v == other.v; }
    constexpr bool operator!=(const Species_impl& other) const noexcept { return v != other.v; }

    const std::string& localize(Language lang) const;
};

class Species
{
private:
    Species_impl impl;

public:
    using EnumType = Species_impl::SpeciesEnum;
    constexpr Species() noexcept : impl(EnumType{0}) {}
    constexpr Species(const Species_impl& impl) noexcept : impl(impl) {}
    constexpr explicit Species(std::underlying_type_t<EnumType> v) noexcept : impl(EnumType{v}) {}
    template <typename T>
    constexpr explicit operator T() const noexcept
    {
        static_assert(std::is_integral_v<T>);
        return T(impl);
    }
    constexpr operator EnumType() const noexcept { return (EnumType)impl; }

    constexpr bool operator<(const Species& other) const noexcept { return impl < other.impl; }
    constexpr bool operator<=(const Species& other) const noexcept { return impl <= other.impl; }

    constexpr bool operator>(const Species& other) const noexcept { return impl > other.impl; }
    constexpr bool operator>=(const Species& other) const noexcept { return impl >= other.impl; }

    constexpr bool operator==(const Species& other) const noexcept { return impl == other.impl; }
    constexpr bool operator!=(const Species& other) const noexcept { return impl != other.impl; }

    constexpr bool operator<(const Species_impl& other) const noexcept { return impl < other; }
    constexpr bool operator<=(const Species_impl& other) const noexcept { return impl <= other; }

    constexpr bool operator>(const Species_impl& other) const noexcept { return impl > other; }
    constexpr bool operator>=(const Species_impl& other) const noexcept { return impl >= other; }

    constexpr bool operator==(const Species_impl& other) const noexcept { return impl == other; }
    constexpr bool operator!=(const Species_impl& other) const noexcept { return impl != other; }

    const std::string& localize(Language lang) const { return impl.localize(lang); }

    static constexpr Species_impl None{EnumType::None};
    static constexpr Species_impl Bulbasaur{EnumType::Bulbasaur};
    static constexpr Species_impl Ivysaur{EnumType::Ivysaur};
    static constexpr Species_impl Venusaur{EnumType::Venusaur};
    static constexpr Species_impl Charmander{EnumType::Charmander};
    static constexpr Species_impl Charmeleon{EnumType::Charmeleon};
    static constexpr Species_impl Charizard{EnumType::Charizard};
    static constexpr Species_impl Squirtle{EnumType::Squirtle};
    static constexpr Species_impl Wartortle{EnumType::Wartortle};
    static constexpr Species_impl Blastoise{EnumType::Blastoise};
    static constexpr Species_impl Caterpie{EnumType::Caterpie};
    static constexpr Species_impl Metapod{EnumType::Metapod};
    static constexpr Species_impl Butterfree{EnumType::Butterfree};
    static constexpr Species_impl Weedle{EnumType::Weedle};
    static constexpr Species_impl Kakuna{EnumType::Kakuna};
    static constexpr Species_impl Beedrill{EnumType::Beedrill};
    static constexpr Species_impl Pidgey{EnumType::Pidgey};
    static constexpr Species_impl Pidgeotto{EnumType::Pidgeotto};
    static constexpr Species_impl Pidgeot{EnumType::Pidgeot};
    static constexpr Species_impl Rattata{EnumType::Rattata};
    static constexpr Species_impl Raticate{EnumType::Raticate};
    static constexpr Species_impl Spearow{EnumType::Spearow};
    static constexpr Species_impl Fearow{EnumType::Fearow};
    static constexpr Species_impl Ekans{EnumType::Ekans};
    static constexpr Species_impl Arbok{EnumType::Arbok};
    static constexpr Species_impl Pikachu{EnumType::Pikachu};
    static constexpr Species_impl Raichu{EnumType::Raichu};
    static constexpr Species_impl Sandshrew{EnumType::Sandshrew};
    static constexpr Species_impl Sandslash{EnumType::Sandslash};
    static constexpr Species_impl NidoranF{EnumType::NidoranF};
    static constexpr Species_impl Nidorina{EnumType::Nidorina};
    static constexpr Species_impl Nidoqueen{EnumType::Nidoqueen};
    static constexpr Species_impl NidoranM{EnumType::NidoranM};
    static constexpr Species_impl Nidorino{EnumType::Nidorino};
    static constexpr Species_impl Nidoking{EnumType::Nidoking};
    static constexpr Species_impl Clefairy{EnumType::Clefairy};
    static constexpr Species_impl Clefable{EnumType::Clefable};
    static constexpr Species_impl Vulpix{EnumType::Vulpix};
    static constexpr Species_impl Ninetales{EnumType::Ninetales};
    static constexpr Species_impl Jigglypuff{EnumType::Jigglypuff};
    static constexpr Species_impl Wigglytuff{EnumType::Wigglytuff};
    static constexpr Species_impl Zubat{EnumType::Zubat};
    static constexpr Species_impl Golbat{EnumType::Golbat};
    static constexpr Species_impl Oddish{EnumType::Oddish};
    static constexpr Species_impl Gloom{EnumType::Gloom};
    static constexpr Species_impl Vileplume{EnumType::Vileplume};
    static constexpr Species_impl Paras{EnumType::Paras};
    static constexpr Species_impl Parasect{EnumType::Parasect};
    static constexpr Species_impl Venonat{EnumType::Venonat};
    static constexpr Species_impl Venomoth{EnumType::Venomoth};
    static constexpr Species_impl Diglett{EnumType::Diglett};
    static constexpr Species_impl Dugtrio{EnumType::Dugtrio};
    static constexpr Species_impl Meowth{EnumType::Meowth};
    static constexpr Species_impl Persian{EnumType::Persian};
    static constexpr Species_impl Psyduck{EnumType::Psyduck};
    static constexpr Species_impl Golduck{EnumType::Golduck};
    static constexpr Species_impl Mankey{EnumType::Mankey};
    static constexpr Species_impl Primeape{EnumType::Primeape};
    static constexpr Species_impl Growlithe{EnumType::Growlithe};
    static constexpr Species_impl Arcanine{EnumType::Arcanine};
    static constexpr Species_impl Poliwag{EnumType::Poliwag};
    static constexpr Species_impl Poliwhirl{EnumType::Poliwhirl};
    static constexpr Species_impl Poliwrath{EnumType::Poliwrath};
    static constexpr Species_impl Abra{EnumType::Abra};
    static constexpr Species_impl Kadabra{EnumType::Kadabra};
    static constexpr Species_impl Alakazam{EnumType::Alakazam};
    static constexpr Species_impl Machop{EnumType::Machop};
    static constexpr Species_impl Machoke{EnumType::Machoke};
    static constexpr Species_impl Machamp{EnumType::Machamp};
    static constexpr Species_impl Bellsprout{EnumType::Bellsprout};
    static constexpr Species_impl Weepinbell{EnumType::Weepinbell};
    static constexpr Species_impl Victreebel{EnumType::Victreebel};
    static constexpr Species_impl Tentacool{EnumType::Tentacool};
    static constexpr Species_impl Tentacruel{EnumType::Tentacruel};
    static constexpr Species_impl Geodude{EnumType::Geodude};
    static constexpr Species_impl Graveler{EnumType::Graveler};
    static constexpr Species_impl Golem{EnumType::Golem};
    static constexpr Species_impl Ponyta{EnumType::Ponyta};
    static constexpr Species_impl Rapidash{EnumType::Rapidash};
    static constexpr Species_impl Slowpoke{EnumType::Slowpoke};
    static constexpr Species_impl Slowbro{EnumType::Slowbro};
    static constexpr Species_impl Magnemite{EnumType::Magnemite};
    static constexpr Species_impl Magneton{EnumType::Magneton};
    static constexpr Species_impl Farfetchd{EnumType::Farfetchd};
    static constexpr Species_impl Doduo{EnumType::Doduo};
    static constexpr Species_impl Dodrio{EnumType::Dodrio};
    static constexpr Species_impl Seel{EnumType::Seel};
    static constexpr Species_impl Dewgong{EnumType::Dewgong};
    static constexpr Species_impl Grimer{EnumType::Grimer};
    static constexpr Species_impl Muk{EnumType::Muk};
    static constexpr Species_impl Shellder{EnumType::Shellder};
    static constexpr Species_impl Cloyster{EnumType::Cloyster};
    static constexpr Species_impl Gastly{EnumType::Gastly};
    static constexpr Species_impl Haunter{EnumType::Haunter};
    static constexpr Species_impl Gengar{EnumType::Gengar};
    static constexpr Species_impl Onix{EnumType::Onix};
    static constexpr Species_impl Drowzee{EnumType::Drowzee};
    static constexpr Species_impl Hypno{EnumType::Hypno};
    static constexpr Species_impl Krabby{EnumType::Krabby};
    static constexpr Species_impl Kingler{EnumType::Kingler};
    static constexpr Species_impl Voltorb{EnumType::Voltorb};
    static constexpr Species_impl Electrode{EnumType::Electrode};
    static constexpr Species_impl Exeggcute{EnumType::Exeggcute};
    static constexpr Species_impl Exeggutor{EnumType::Exeggutor};
    static constexpr Species_impl Cubone{EnumType::Cubone};
    static constexpr Species_impl Marowak{EnumType::Marowak};
    static constexpr Species_impl Hitmonlee{EnumType::Hitmonlee};
    static constexpr Species_impl Hitmonchan{EnumType::Hitmonchan};
    static constexpr Species_impl Lickitung{EnumType::Lickitung};
    static constexpr Species_impl Koffing{EnumType::Koffing};
    static constexpr Species_impl Weezing{EnumType::Weezing};
    static constexpr Species_impl Rhyhorn{EnumType::Rhyhorn};
    static constexpr Species_impl Rhydon{EnumType::Rhydon};
    static constexpr Species_impl Chansey{EnumType::Chansey};
    static constexpr Species_impl Tangela{EnumType::Tangela};
    static constexpr Species_impl Kangaskhan{EnumType::Kangaskhan};
    static constexpr Species_impl Horsea{EnumType::Horsea};
    static constexpr Species_impl Seadra{EnumType::Seadra};
    static constexpr Species_impl Goldeen{EnumType::Goldeen};
    static constexpr Species_impl Seaking{EnumType::Seaking};
    static constexpr Species_impl Staryu{EnumType::Staryu};
    static constexpr Species_impl Starmie{EnumType::Starmie};
    static constexpr Species_impl MrMime{EnumType::MrMime};
    static constexpr Species_impl Scyther{EnumType::Scyther};
    static constexpr Species_impl Jynx{EnumType::Jynx};
    static constexpr Species_impl Electabuzz{EnumType::Electabuzz};
    static constexpr Species_impl Magmar{EnumType::Magmar};
    static constexpr Species_impl Pinsir{EnumType::Pinsir};
    static constexpr Species_impl Tauros{EnumType::Tauros};
    static constexpr Species_impl Magikarp{EnumType::Magikarp};
    static constexpr Species_impl Gyarados{EnumType::Gyarados};
    static constexpr Species_impl Lapras{EnumType::Lapras};
    static constexpr Species_impl Ditto{EnumType::Ditto};
    static constexpr Species_impl Eevee{EnumType::Eevee};
    static constexpr Species_impl Vaporeon{EnumType::Vaporeon};
    static constexpr Species_impl Jolteon{EnumType::Jolteon};
    static constexpr Species_impl Flareon{EnumType::Flareon};
    static constexpr Species_impl Porygon{EnumType::Porygon};
    static constexpr Species_impl Omanyte{EnumType::Omanyte};
    static constexpr Species_impl Omastar{EnumType::Omastar};
    static constexpr Species_impl Kabuto{EnumType::Kabuto};
    static constexpr Species_impl Kabutops{EnumType::Kabutops};
    static constexpr Species_impl Aerodactyl{EnumType::Aerodactyl};
    static constexpr Species_impl Snorlax{EnumType::Snorlax};
    static constexpr Species_impl Articuno{EnumType::Articuno};
    static constexpr Species_impl Zapdos{EnumType::Zapdos};
    static constexpr Species_impl Moltres{EnumType::Moltres};
    static constexpr Species_impl Dratini{EnumType::Dratini};
    static constexpr Species_impl Dragonair{EnumType::Dragonair};
    static constexpr Species_impl Dragonite{EnumType::Dragonite};
    static constexpr Species_impl Mewtwo{EnumType::Mewtwo};
    static constexpr Species_impl Mew{EnumType::Mew};
    static constexpr Species_impl Chikorita{EnumType::Chikorita};
    static constexpr Species_impl Bayleef{EnumType::Bayleef};
    static constexpr Species_impl Meganium{EnumType::Meganium};
    static constexpr Species_impl Cyndaquil{EnumType::Cyndaquil};
    static constexpr Species_impl Quilava{EnumType::Quilava};
    static constexpr Species_impl Typhlosion{EnumType::Typhlosion};
    static constexpr Species_impl Totodile{EnumType::Totodile};
    static constexpr Species_impl Croconaw{EnumType::Croconaw};
    static constexpr Species_impl Feraligatr{EnumType::Feraligatr};
    static constexpr Species_impl Sentret{EnumType::Sentret};
    static constexpr Species_impl Furret{EnumType::Furret};
    static constexpr Species_impl Hoothoot{EnumType::Hoothoot};
    static constexpr Species_impl Noctowl{EnumType::Noctowl};
    static constexpr Species_impl Ledyba{EnumType::Ledyba};
    static constexpr Species_impl Ledian{EnumType::Ledian};
    static constexpr Species_impl Spinarak{EnumType::Spinarak};
    static constexpr Species_impl Ariados{EnumType::Ariados};
    static constexpr Species_impl Crobat{EnumType::Crobat};
    static constexpr Species_impl Chinchou{EnumType::Chinchou};
    static constexpr Species_impl Lanturn{EnumType::Lanturn};
    static constexpr Species_impl Pichu{EnumType::Pichu};
    static constexpr Species_impl Cleffa{EnumType::Cleffa};
    static constexpr Species_impl Igglybuff{EnumType::Igglybuff};
    static constexpr Species_impl Togepi{EnumType::Togepi};
    static constexpr Species_impl Togetic{EnumType::Togetic};
    static constexpr Species_impl Natu{EnumType::Natu};
    static constexpr Species_impl Xatu{EnumType::Xatu};
    static constexpr Species_impl Mareep{EnumType::Mareep};
    static constexpr Species_impl Flaaffy{EnumType::Flaaffy};
    static constexpr Species_impl Ampharos{EnumType::Ampharos};
    static constexpr Species_impl Bellossom{EnumType::Bellossom};
    static constexpr Species_impl Marill{EnumType::Marill};
    static constexpr Species_impl Azumarill{EnumType::Azumarill};
    static constexpr Species_impl Sudowoodo{EnumType::Sudowoodo};
    static constexpr Species_impl Politoed{EnumType::Politoed};
    static constexpr Species_impl Hoppip{EnumType::Hoppip};
    static constexpr Species_impl Skiploom{EnumType::Skiploom};
    static constexpr Species_impl Jumpluff{EnumType::Jumpluff};
    static constexpr Species_impl Aipom{EnumType::Aipom};
    static constexpr Species_impl Sunkern{EnumType::Sunkern};
    static constexpr Species_impl Sunflora{EnumType::Sunflora};
    static constexpr Species_impl Yanma{EnumType::Yanma};
    static constexpr Species_impl Wooper{EnumType::Wooper};
    static constexpr Species_impl Quagsire{EnumType::Quagsire};
    static constexpr Species_impl Espeon{EnumType::Espeon};
    static constexpr Species_impl Umbreon{EnumType::Umbreon};
    static constexpr Species_impl Murkrow{EnumType::Murkrow};
    static constexpr Species_impl Slowking{EnumType::Slowking};
    static constexpr Species_impl Misdreavus{EnumType::Misdreavus};
    static constexpr Species_impl Unown{EnumType::Unown};
    static constexpr Species_impl Wobbuffet{EnumType::Wobbuffet};
    static constexpr Species_impl Girafarig{EnumType::Girafarig};
    static constexpr Species_impl Pineco{EnumType::Pineco};
    static constexpr Species_impl Forretress{EnumType::Forretress};
    static constexpr Species_impl Dunsparce{EnumType::Dunsparce};
    static constexpr Species_impl Gligar{EnumType::Gligar};
    static constexpr Species_impl Steelix{EnumType::Steelix};
    static constexpr Species_impl Snubbull{EnumType::Snubbull};
    static constexpr Species_impl Granbull{EnumType::Granbull};
    static constexpr Species_impl Qwilfish{EnumType::Qwilfish};
    static constexpr Species_impl Scizor{EnumType::Scizor};
    static constexpr Species_impl Shuckle{EnumType::Shuckle};
    static constexpr Species_impl Heracross{EnumType::Heracross};
    static constexpr Species_impl Sneasel{EnumType::Sneasel};
    static constexpr Species_impl Teddiursa{EnumType::Teddiursa};
    static constexpr Species_impl Ursaring{EnumType::Ursaring};
    static constexpr Species_impl Slugma{EnumType::Slugma};
    static constexpr Species_impl Magcargo{EnumType::Magcargo};
    static constexpr Species_impl Swinub{EnumType::Swinub};
    static constexpr Species_impl Piloswine{EnumType::Piloswine};
    static constexpr Species_impl Corsola{EnumType::Corsola};
    static constexpr Species_impl Remoraid{EnumType::Remoraid};
    static constexpr Species_impl Octillery{EnumType::Octillery};
    static constexpr Species_impl Delibird{EnumType::Delibird};
    static constexpr Species_impl Mantine{EnumType::Mantine};
    static constexpr Species_impl Skarmory{EnumType::Skarmory};
    static constexpr Species_impl Houndour{EnumType::Houndour};
    static constexpr Species_impl Houndoom{EnumType::Houndoom};
    static constexpr Species_impl Kingdra{EnumType::Kingdra};
    static constexpr Species_impl Phanpy{EnumType::Phanpy};
    static constexpr Species_impl Donphan{EnumType::Donphan};
    static constexpr Species_impl Porygon2{EnumType::Porygon2};
    static constexpr Species_impl Stantler{EnumType::Stantler};
    static constexpr Species_impl Smeargle{EnumType::Smeargle};
    static constexpr Species_impl Tyrogue{EnumType::Tyrogue};
    static constexpr Species_impl Hitmontop{EnumType::Hitmontop};
    static constexpr Species_impl Smoochum{EnumType::Smoochum};
    static constexpr Species_impl Elekid{EnumType::Elekid};
    static constexpr Species_impl Magby{EnumType::Magby};
    static constexpr Species_impl Miltank{EnumType::Miltank};
    static constexpr Species_impl Blissey{EnumType::Blissey};
    static constexpr Species_impl Raikou{EnumType::Raikou};
    static constexpr Species_impl Entei{EnumType::Entei};
    static constexpr Species_impl Suicune{EnumType::Suicune};
    static constexpr Species_impl Larvitar{EnumType::Larvitar};
    static constexpr Species_impl Pupitar{EnumType::Pupitar};
    static constexpr Species_impl Tyranitar{EnumType::Tyranitar};
    static constexpr Species_impl Lugia{EnumType::Lugia};
    static constexpr Species_impl HoOh{EnumType::HoOh};
    static constexpr Species_impl Celebi{EnumType::Celebi};
    static constexpr Species_impl Treecko{EnumType::Treecko};
    static constexpr Species_impl Grovyle{EnumType::Grovyle};
    static constexpr Species_impl Sceptile{EnumType::Sceptile};
    static constexpr Species_impl Torchic{EnumType::Torchic};
    static constexpr Species_impl Combusken{EnumType::Combusken};
    static constexpr Species_impl Blaziken{EnumType::Blaziken};
    static constexpr Species_impl Mudkip{EnumType::Mudkip};
    static constexpr Species_impl Marshtomp{EnumType::Marshtomp};
    static constexpr Species_impl Swampert{EnumType::Swampert};
    static constexpr Species_impl Poochyena{EnumType::Poochyena};
    static constexpr Species_impl Mightyena{EnumType::Mightyena};
    static constexpr Species_impl Zigzagoon{EnumType::Zigzagoon};
    static constexpr Species_impl Linoone{EnumType::Linoone};
    static constexpr Species_impl Wurmple{EnumType::Wurmple};
    static constexpr Species_impl Silcoon{EnumType::Silcoon};
    static constexpr Species_impl Beautifly{EnumType::Beautifly};
    static constexpr Species_impl Cascoon{EnumType::Cascoon};
    static constexpr Species_impl Dustox{EnumType::Dustox};
    static constexpr Species_impl Lotad{EnumType::Lotad};
    static constexpr Species_impl Lombre{EnumType::Lombre};
    static constexpr Species_impl Ludicolo{EnumType::Ludicolo};
    static constexpr Species_impl Seedot{EnumType::Seedot};
    static constexpr Species_impl Nuzleaf{EnumType::Nuzleaf};
    static constexpr Species_impl Shiftry{EnumType::Shiftry};
    static constexpr Species_impl Taillow{EnumType::Taillow};
    static constexpr Species_impl Swellow{EnumType::Swellow};
    static constexpr Species_impl Wingull{EnumType::Wingull};
    static constexpr Species_impl Pelipper{EnumType::Pelipper};
    static constexpr Species_impl Ralts{EnumType::Ralts};
    static constexpr Species_impl Kirlia{EnumType::Kirlia};
    static constexpr Species_impl Gardevoir{EnumType::Gardevoir};
    static constexpr Species_impl Surskit{EnumType::Surskit};
    static constexpr Species_impl Masquerain{EnumType::Masquerain};
    static constexpr Species_impl Shroomish{EnumType::Shroomish};
    static constexpr Species_impl Breloom{EnumType::Breloom};
    static constexpr Species_impl Slakoth{EnumType::Slakoth};
    static constexpr Species_impl Vigoroth{EnumType::Vigoroth};
    static constexpr Species_impl Slaking{EnumType::Slaking};
    static constexpr Species_impl Nincada{EnumType::Nincada};
    static constexpr Species_impl Ninjask{EnumType::Ninjask};
    static constexpr Species_impl Shedinja{EnumType::Shedinja};
    static constexpr Species_impl Whismur{EnumType::Whismur};
    static constexpr Species_impl Loudred{EnumType::Loudred};
    static constexpr Species_impl Exploud{EnumType::Exploud};
    static constexpr Species_impl Makuhita{EnumType::Makuhita};
    static constexpr Species_impl Hariyama{EnumType::Hariyama};
    static constexpr Species_impl Azurill{EnumType::Azurill};
    static constexpr Species_impl Nosepass{EnumType::Nosepass};
    static constexpr Species_impl Skitty{EnumType::Skitty};
    static constexpr Species_impl Delcatty{EnumType::Delcatty};
    static constexpr Species_impl Sableye{EnumType::Sableye};
    static constexpr Species_impl Mawile{EnumType::Mawile};
    static constexpr Species_impl Aron{EnumType::Aron};
    static constexpr Species_impl Lairon{EnumType::Lairon};
    static constexpr Species_impl Aggron{EnumType::Aggron};
    static constexpr Species_impl Meditite{EnumType::Meditite};
    static constexpr Species_impl Medicham{EnumType::Medicham};
    static constexpr Species_impl Electrike{EnumType::Electrike};
    static constexpr Species_impl Manectric{EnumType::Manectric};
    static constexpr Species_impl Plusle{EnumType::Plusle};
    static constexpr Species_impl Minun{EnumType::Minun};
    static constexpr Species_impl Volbeat{EnumType::Volbeat};
    static constexpr Species_impl Illumise{EnumType::Illumise};
    static constexpr Species_impl Roselia{EnumType::Roselia};
    static constexpr Species_impl Gulpin{EnumType::Gulpin};
    static constexpr Species_impl Swalot{EnumType::Swalot};
    static constexpr Species_impl Carvanha{EnumType::Carvanha};
    static constexpr Species_impl Sharpedo{EnumType::Sharpedo};
    static constexpr Species_impl Wailmer{EnumType::Wailmer};
    static constexpr Species_impl Wailord{EnumType::Wailord};
    static constexpr Species_impl Numel{EnumType::Numel};
    static constexpr Species_impl Camerupt{EnumType::Camerupt};
    static constexpr Species_impl Torkoal{EnumType::Torkoal};
    static constexpr Species_impl Spoink{EnumType::Spoink};
    static constexpr Species_impl Grumpig{EnumType::Grumpig};
    static constexpr Species_impl Spinda{EnumType::Spinda};
    static constexpr Species_impl Trapinch{EnumType::Trapinch};
    static constexpr Species_impl Vibrava{EnumType::Vibrava};
    static constexpr Species_impl Flygon{EnumType::Flygon};
    static constexpr Species_impl Cacnea{EnumType::Cacnea};
    static constexpr Species_impl Cacturne{EnumType::Cacturne};
    static constexpr Species_impl Swablu{EnumType::Swablu};
    static constexpr Species_impl Altaria{EnumType::Altaria};
    static constexpr Species_impl Zangoose{EnumType::Zangoose};
    static constexpr Species_impl Seviper{EnumType::Seviper};
    static constexpr Species_impl Lunatone{EnumType::Lunatone};
    static constexpr Species_impl Solrock{EnumType::Solrock};
    static constexpr Species_impl Barboach{EnumType::Barboach};
    static constexpr Species_impl Whiscash{EnumType::Whiscash};
    static constexpr Species_impl Corphish{EnumType::Corphish};
    static constexpr Species_impl Crawdaunt{EnumType::Crawdaunt};
    static constexpr Species_impl Baltoy{EnumType::Baltoy};
    static constexpr Species_impl Claydol{EnumType::Claydol};
    static constexpr Species_impl Lileep{EnumType::Lileep};
    static constexpr Species_impl Cradily{EnumType::Cradily};
    static constexpr Species_impl Anorith{EnumType::Anorith};
    static constexpr Species_impl Armaldo{EnumType::Armaldo};
    static constexpr Species_impl Feebas{EnumType::Feebas};
    static constexpr Species_impl Milotic{EnumType::Milotic};
    static constexpr Species_impl Castform{EnumType::Castform};
    static constexpr Species_impl Kecleon{EnumType::Kecleon};
    static constexpr Species_impl Shuppet{EnumType::Shuppet};
    static constexpr Species_impl Banette{EnumType::Banette};
    static constexpr Species_impl Duskull{EnumType::Duskull};
    static constexpr Species_impl Dusclops{EnumType::Dusclops};
    static constexpr Species_impl Tropius{EnumType::Tropius};
    static constexpr Species_impl Chimecho{EnumType::Chimecho};
    static constexpr Species_impl Absol{EnumType::Absol};
    static constexpr Species_impl Wynaut{EnumType::Wynaut};
    static constexpr Species_impl Snorunt{EnumType::Snorunt};
    static constexpr Species_impl Glalie{EnumType::Glalie};
    static constexpr Species_impl Spheal{EnumType::Spheal};
    static constexpr Species_impl Sealeo{EnumType::Sealeo};
    static constexpr Species_impl Walrein{EnumType::Walrein};
    static constexpr Species_impl Clamperl{EnumType::Clamperl};
    static constexpr Species_impl Huntail{EnumType::Huntail};
    static constexpr Species_impl Gorebyss{EnumType::Gorebyss};
    static constexpr Species_impl Relicanth{EnumType::Relicanth};
    static constexpr Species_impl Luvdisc{EnumType::Luvdisc};
    static constexpr Species_impl Bagon{EnumType::Bagon};
    static constexpr Species_impl Shelgon{EnumType::Shelgon};
    static constexpr Species_impl Salamence{EnumType::Salamence};
    static constexpr Species_impl Beldum{EnumType::Beldum};
    static constexpr Species_impl Metang{EnumType::Metang};
    static constexpr Species_impl Metagross{EnumType::Metagross};
    static constexpr Species_impl Regirock{EnumType::Regirock};
    static constexpr Species_impl Regice{EnumType::Regice};
    static constexpr Species_impl Registeel{EnumType::Registeel};
    static constexpr Species_impl Latias{EnumType::Latias};
    static constexpr Species_impl Latios{EnumType::Latios};
    static constexpr Species_impl Kyogre{EnumType::Kyogre};
    static constexpr Species_impl Groudon{EnumType::Groudon};
    static constexpr Species_impl Rayquaza{EnumType::Rayquaza};
    static constexpr Species_impl Jirachi{EnumType::Jirachi};
    static constexpr Species_impl Deoxys{EnumType::Deoxys};
    static constexpr Species_impl Turtwig{EnumType::Turtwig};
    static constexpr Species_impl Grotle{EnumType::Grotle};
    static constexpr Species_impl Torterra{EnumType::Torterra};
    static constexpr Species_impl Chimchar{EnumType::Chimchar};
    static constexpr Species_impl Monferno{EnumType::Monferno};
    static constexpr Species_impl Infernape{EnumType::Infernape};
    static constexpr Species_impl Piplup{EnumType::Piplup};
    static constexpr Species_impl Prinplup{EnumType::Prinplup};
    static constexpr Species_impl Empoleon{EnumType::Empoleon};
    static constexpr Species_impl Starly{EnumType::Starly};
    static constexpr Species_impl Staravia{EnumType::Staravia};
    static constexpr Species_impl Staraptor{EnumType::Staraptor};
    static constexpr Species_impl Bidoof{EnumType::Bidoof};
    static constexpr Species_impl Bibarel{EnumType::Bibarel};
    static constexpr Species_impl Kricketot{EnumType::Kricketot};
    static constexpr Species_impl Kricketune{EnumType::Kricketune};
    static constexpr Species_impl Shinx{EnumType::Shinx};
    static constexpr Species_impl Luxio{EnumType::Luxio};
    static constexpr Species_impl Luxray{EnumType::Luxray};
    static constexpr Species_impl Budew{EnumType::Budew};
    static constexpr Species_impl Roserade{EnumType::Roserade};
    static constexpr Species_impl Cranidos{EnumType::Cranidos};
    static constexpr Species_impl Rampardos{EnumType::Rampardos};
    static constexpr Species_impl Shieldon{EnumType::Shieldon};
    static constexpr Species_impl Bastiodon{EnumType::Bastiodon};
    static constexpr Species_impl Burmy{EnumType::Burmy};
    static constexpr Species_impl Wormadam{EnumType::Wormadam};
    static constexpr Species_impl Mothim{EnumType::Mothim};
    static constexpr Species_impl Combee{EnumType::Combee};
    static constexpr Species_impl Vespiquen{EnumType::Vespiquen};
    static constexpr Species_impl Pachirisu{EnumType::Pachirisu};
    static constexpr Species_impl Buizel{EnumType::Buizel};
    static constexpr Species_impl Floatzel{EnumType::Floatzel};
    static constexpr Species_impl Cherubi{EnumType::Cherubi};
    static constexpr Species_impl Cherrim{EnumType::Cherrim};
    static constexpr Species_impl Shellos{EnumType::Shellos};
    static constexpr Species_impl Gastrodon{EnumType::Gastrodon};
    static constexpr Species_impl Ambipom{EnumType::Ambipom};
    static constexpr Species_impl Drifloon{EnumType::Drifloon};
    static constexpr Species_impl Drifblim{EnumType::Drifblim};
    static constexpr Species_impl Buneary{EnumType::Buneary};
    static constexpr Species_impl Lopunny{EnumType::Lopunny};
    static constexpr Species_impl Mismagius{EnumType::Mismagius};
    static constexpr Species_impl Honchkrow{EnumType::Honchkrow};
    static constexpr Species_impl Glameow{EnumType::Glameow};
    static constexpr Species_impl Purugly{EnumType::Purugly};
    static constexpr Species_impl Chingling{EnumType::Chingling};
    static constexpr Species_impl Stunky{EnumType::Stunky};
    static constexpr Species_impl Skuntank{EnumType::Skuntank};
    static constexpr Species_impl Bronzor{EnumType::Bronzor};
    static constexpr Species_impl Bronzong{EnumType::Bronzong};
    static constexpr Species_impl Bonsly{EnumType::Bonsly};
    static constexpr Species_impl MimeJr{EnumType::MimeJr};
    static constexpr Species_impl Happiny{EnumType::Happiny};
    static constexpr Species_impl Chatot{EnumType::Chatot};
    static constexpr Species_impl Spiritomb{EnumType::Spiritomb};
    static constexpr Species_impl Gible{EnumType::Gible};
    static constexpr Species_impl Gabite{EnumType::Gabite};
    static constexpr Species_impl Garchomp{EnumType::Garchomp};
    static constexpr Species_impl Munchlax{EnumType::Munchlax};
    static constexpr Species_impl Riolu{EnumType::Riolu};
    static constexpr Species_impl Lucario{EnumType::Lucario};
    static constexpr Species_impl Hippopotas{EnumType::Hippopotas};
    static constexpr Species_impl Hippowdon{EnumType::Hippowdon};
    static constexpr Species_impl Skorupi{EnumType::Skorupi};
    static constexpr Species_impl Drapion{EnumType::Drapion};
    static constexpr Species_impl Croagunk{EnumType::Croagunk};
    static constexpr Species_impl Toxicroak{EnumType::Toxicroak};
    static constexpr Species_impl Carnivine{EnumType::Carnivine};
    static constexpr Species_impl Finneon{EnumType::Finneon};
    static constexpr Species_impl Lumineon{EnumType::Lumineon};
    static constexpr Species_impl Mantyke{EnumType::Mantyke};
    static constexpr Species_impl Snover{EnumType::Snover};
    static constexpr Species_impl Abomasnow{EnumType::Abomasnow};
    static constexpr Species_impl Weavile{EnumType::Weavile};
    static constexpr Species_impl Magnezone{EnumType::Magnezone};
    static constexpr Species_impl Lickilicky{EnumType::Lickilicky};
    static constexpr Species_impl Rhyperior{EnumType::Rhyperior};
    static constexpr Species_impl Tangrowth{EnumType::Tangrowth};
    static constexpr Species_impl Electivire{EnumType::Electivire};
    static constexpr Species_impl Magmortar{EnumType::Magmortar};
    static constexpr Species_impl Togekiss{EnumType::Togekiss};
    static constexpr Species_impl Yanmega{EnumType::Yanmega};
    static constexpr Species_impl Leafeon{EnumType::Leafeon};
    static constexpr Species_impl Glaceon{EnumType::Glaceon};
    static constexpr Species_impl Gliscor{EnumType::Gliscor};
    static constexpr Species_impl Mamoswine{EnumType::Mamoswine};
    static constexpr Species_impl PorygonZ{EnumType::PorygonZ};
    static constexpr Species_impl Gallade{EnumType::Gallade};
    static constexpr Species_impl Probopass{EnumType::Probopass};
    static constexpr Species_impl Dusknoir{EnumType::Dusknoir};
    static constexpr Species_impl Froslass{EnumType::Froslass};
    static constexpr Species_impl Rotom{EnumType::Rotom};
    static constexpr Species_impl Uxie{EnumType::Uxie};
    static constexpr Species_impl Mesprit{EnumType::Mesprit};
    static constexpr Species_impl Azelf{EnumType::Azelf};
    static constexpr Species_impl Dialga{EnumType::Dialga};
    static constexpr Species_impl Palkia{EnumType::Palkia};
    static constexpr Species_impl Heatran{EnumType::Heatran};
    static constexpr Species_impl Regigigas{EnumType::Regigigas};
    static constexpr Species_impl Giratina{EnumType::Giratina};
    static constexpr Species_impl Cresselia{EnumType::Cresselia};
    static constexpr Species_impl Phione{EnumType::Phione};
    static constexpr Species_impl Manaphy{EnumType::Manaphy};
    static constexpr Species_impl Darkrai{EnumType::Darkrai};
    static constexpr Species_impl Shaymin{EnumType::Shaymin};
    static constexpr Species_impl Arceus{EnumType::Arceus};
    static constexpr Species_impl Victini{EnumType::Victini};
    static constexpr Species_impl Snivy{EnumType::Snivy};
    static constexpr Species_impl Servine{EnumType::Servine};
    static constexpr Species_impl Serperior{EnumType::Serperior};
    static constexpr Species_impl Tepig{EnumType::Tepig};
    static constexpr Species_impl Pignite{EnumType::Pignite};
    static constexpr Species_impl Emboar{EnumType::Emboar};
    static constexpr Species_impl Oshawott{EnumType::Oshawott};
    static constexpr Species_impl Dewott{EnumType::Dewott};
    static constexpr Species_impl Samurott{EnumType::Samurott};
    static constexpr Species_impl Patrat{EnumType::Patrat};
    static constexpr Species_impl Watchog{EnumType::Watchog};
    static constexpr Species_impl Lillipup{EnumType::Lillipup};
    static constexpr Species_impl Herdier{EnumType::Herdier};
    static constexpr Species_impl Stoutland{EnumType::Stoutland};
    static constexpr Species_impl Purrloin{EnumType::Purrloin};
    static constexpr Species_impl Liepard{EnumType::Liepard};
    static constexpr Species_impl Pansage{EnumType::Pansage};
    static constexpr Species_impl Simisage{EnumType::Simisage};
    static constexpr Species_impl Pansear{EnumType::Pansear};
    static constexpr Species_impl Simisear{EnumType::Simisear};
    static constexpr Species_impl Panpour{EnumType::Panpour};
    static constexpr Species_impl Simipour{EnumType::Simipour};
    static constexpr Species_impl Munna{EnumType::Munna};
    static constexpr Species_impl Musharna{EnumType::Musharna};
    static constexpr Species_impl Pidove{EnumType::Pidove};
    static constexpr Species_impl Tranquill{EnumType::Tranquill};
    static constexpr Species_impl Unfezant{EnumType::Unfezant};
    static constexpr Species_impl Blitzle{EnumType::Blitzle};
    static constexpr Species_impl Zebstrika{EnumType::Zebstrika};
    static constexpr Species_impl Roggenrola{EnumType::Roggenrola};
    static constexpr Species_impl Boldore{EnumType::Boldore};
    static constexpr Species_impl Gigalith{EnumType::Gigalith};
    static constexpr Species_impl Woobat{EnumType::Woobat};
    static constexpr Species_impl Swoobat{EnumType::Swoobat};
    static constexpr Species_impl Drilbur{EnumType::Drilbur};
    static constexpr Species_impl Excadrill{EnumType::Excadrill};
    static constexpr Species_impl Audino{EnumType::Audino};
    static constexpr Species_impl Timburr{EnumType::Timburr};
    static constexpr Species_impl Gurdurr{EnumType::Gurdurr};
    static constexpr Species_impl Conkeldurr{EnumType::Conkeldurr};
    static constexpr Species_impl Tympole{EnumType::Tympole};
    static constexpr Species_impl Palpitoad{EnumType::Palpitoad};
    static constexpr Species_impl Seismitoad{EnumType::Seismitoad};
    static constexpr Species_impl Throh{EnumType::Throh};
    static constexpr Species_impl Sawk{EnumType::Sawk};
    static constexpr Species_impl Sewaddle{EnumType::Sewaddle};
    static constexpr Species_impl Swadloon{EnumType::Swadloon};
    static constexpr Species_impl Leavanny{EnumType::Leavanny};
    static constexpr Species_impl Venipede{EnumType::Venipede};
    static constexpr Species_impl Whirlipede{EnumType::Whirlipede};
    static constexpr Species_impl Scolipede{EnumType::Scolipede};
    static constexpr Species_impl Cottonee{EnumType::Cottonee};
    static constexpr Species_impl Whimsicott{EnumType::Whimsicott};
    static constexpr Species_impl Petilil{EnumType::Petilil};
    static constexpr Species_impl Lilligant{EnumType::Lilligant};
    static constexpr Species_impl Basculin{EnumType::Basculin};
    static constexpr Species_impl Sandile{EnumType::Sandile};
    static constexpr Species_impl Krokorok{EnumType::Krokorok};
    static constexpr Species_impl Krookodile{EnumType::Krookodile};
    static constexpr Species_impl Darumaka{EnumType::Darumaka};
    static constexpr Species_impl Darmanitan{EnumType::Darmanitan};
    static constexpr Species_impl Maractus{EnumType::Maractus};
    static constexpr Species_impl Dwebble{EnumType::Dwebble};
    static constexpr Species_impl Crustle{EnumType::Crustle};
    static constexpr Species_impl Scraggy{EnumType::Scraggy};
    static constexpr Species_impl Scrafty{EnumType::Scrafty};
    static constexpr Species_impl Sigilyph{EnumType::Sigilyph};
    static constexpr Species_impl Yamask{EnumType::Yamask};
    static constexpr Species_impl Cofagrigus{EnumType::Cofagrigus};
    static constexpr Species_impl Tirtouga{EnumType::Tirtouga};
    static constexpr Species_impl Carracosta{EnumType::Carracosta};
    static constexpr Species_impl Archen{EnumType::Archen};
    static constexpr Species_impl Archeops{EnumType::Archeops};
    static constexpr Species_impl Trubbish{EnumType::Trubbish};
    static constexpr Species_impl Garbodor{EnumType::Garbodor};
    static constexpr Species_impl Zorua{EnumType::Zorua};
    static constexpr Species_impl Zoroark{EnumType::Zoroark};
    static constexpr Species_impl Minccino{EnumType::Minccino};
    static constexpr Species_impl Cinccino{EnumType::Cinccino};
    static constexpr Species_impl Gothita{EnumType::Gothita};
    static constexpr Species_impl Gothorita{EnumType::Gothorita};
    static constexpr Species_impl Gothitelle{EnumType::Gothitelle};
    static constexpr Species_impl Solosis{EnumType::Solosis};
    static constexpr Species_impl Duosion{EnumType::Duosion};
    static constexpr Species_impl Reuniclus{EnumType::Reuniclus};
    static constexpr Species_impl Ducklett{EnumType::Ducklett};
    static constexpr Species_impl Swanna{EnumType::Swanna};
    static constexpr Species_impl Vanillite{EnumType::Vanillite};
    static constexpr Species_impl Vanillish{EnumType::Vanillish};
    static constexpr Species_impl Vanilluxe{EnumType::Vanilluxe};
    static constexpr Species_impl Deerling{EnumType::Deerling};
    static constexpr Species_impl Sawsbuck{EnumType::Sawsbuck};
    static constexpr Species_impl Emolga{EnumType::Emolga};
    static constexpr Species_impl Karrablast{EnumType::Karrablast};
    static constexpr Species_impl Escavalier{EnumType::Escavalier};
    static constexpr Species_impl Foongus{EnumType::Foongus};
    static constexpr Species_impl Amoonguss{EnumType::Amoonguss};
    static constexpr Species_impl Frillish{EnumType::Frillish};
    static constexpr Species_impl Jellicent{EnumType::Jellicent};
    static constexpr Species_impl Alomomola{EnumType::Alomomola};
    static constexpr Species_impl Joltik{EnumType::Joltik};
    static constexpr Species_impl Galvantula{EnumType::Galvantula};
    static constexpr Species_impl Ferroseed{EnumType::Ferroseed};
    static constexpr Species_impl Ferrothorn{EnumType::Ferrothorn};
    static constexpr Species_impl Klink{EnumType::Klink};
    static constexpr Species_impl Klang{EnumType::Klang};
    static constexpr Species_impl Klinklang{EnumType::Klinklang};
    static constexpr Species_impl Tynamo{EnumType::Tynamo};
    static constexpr Species_impl Eelektrik{EnumType::Eelektrik};
    static constexpr Species_impl Eelektross{EnumType::Eelektross};
    static constexpr Species_impl Elgyem{EnumType::Elgyem};
    static constexpr Species_impl Beheeyem{EnumType::Beheeyem};
    static constexpr Species_impl Litwick{EnumType::Litwick};
    static constexpr Species_impl Lampent{EnumType::Lampent};
    static constexpr Species_impl Chandelure{EnumType::Chandelure};
    static constexpr Species_impl Axew{EnumType::Axew};
    static constexpr Species_impl Fraxure{EnumType::Fraxure};
    static constexpr Species_impl Haxorus{EnumType::Haxorus};
    static constexpr Species_impl Cubchoo{EnumType::Cubchoo};
    static constexpr Species_impl Beartic{EnumType::Beartic};
    static constexpr Species_impl Cryogonal{EnumType::Cryogonal};
    static constexpr Species_impl Shelmet{EnumType::Shelmet};
    static constexpr Species_impl Accelgor{EnumType::Accelgor};
    static constexpr Species_impl Stunfisk{EnumType::Stunfisk};
    static constexpr Species_impl Mienfoo{EnumType::Mienfoo};
    static constexpr Species_impl Mienshao{EnumType::Mienshao};
    static constexpr Species_impl Druddigon{EnumType::Druddigon};
    static constexpr Species_impl Golett{EnumType::Golett};
    static constexpr Species_impl Golurk{EnumType::Golurk};
    static constexpr Species_impl Pawniard{EnumType::Pawniard};
    static constexpr Species_impl Bisharp{EnumType::Bisharp};
    static constexpr Species_impl Bouffalant{EnumType::Bouffalant};
    static constexpr Species_impl Rufflet{EnumType::Rufflet};
    static constexpr Species_impl Braviary{EnumType::Braviary};
    static constexpr Species_impl Vullaby{EnumType::Vullaby};
    static constexpr Species_impl Mandibuzz{EnumType::Mandibuzz};
    static constexpr Species_impl Heatmor{EnumType::Heatmor};
    static constexpr Species_impl Durant{EnumType::Durant};
    static constexpr Species_impl Deino{EnumType::Deino};
    static constexpr Species_impl Zweilous{EnumType::Zweilous};
    static constexpr Species_impl Hydreigon{EnumType::Hydreigon};
    static constexpr Species_impl Larvesta{EnumType::Larvesta};
    static constexpr Species_impl Volcarona{EnumType::Volcarona};
    static constexpr Species_impl Cobalion{EnumType::Cobalion};
    static constexpr Species_impl Terrakion{EnumType::Terrakion};
    static constexpr Species_impl Virizion{EnumType::Virizion};
    static constexpr Species_impl Tornadus{EnumType::Tornadus};
    static constexpr Species_impl Thundurus{EnumType::Thundurus};
    static constexpr Species_impl Reshiram{EnumType::Reshiram};
    static constexpr Species_impl Zekrom{EnumType::Zekrom};
    static constexpr Species_impl Landorus{EnumType::Landorus};
    static constexpr Species_impl Kyurem{EnumType::Kyurem};
    static constexpr Species_impl Keldeo{EnumType::Keldeo};
    static constexpr Species_impl Meloetta{EnumType::Meloetta};
    static constexpr Species_impl Genesect{EnumType::Genesect};
    static constexpr Species_impl Chespin{EnumType::Chespin};
    static constexpr Species_impl Quilladin{EnumType::Quilladin};
    static constexpr Species_impl Chesnaught{EnumType::Chesnaught};
    static constexpr Species_impl Fennekin{EnumType::Fennekin};
    static constexpr Species_impl Braixen{EnumType::Braixen};
    static constexpr Species_impl Delphox{EnumType::Delphox};
    static constexpr Species_impl Froakie{EnumType::Froakie};
    static constexpr Species_impl Frogadier{EnumType::Frogadier};
    static constexpr Species_impl Greninja{EnumType::Greninja};
    static constexpr Species_impl Bunnelby{EnumType::Bunnelby};
    static constexpr Species_impl Diggersby{EnumType::Diggersby};
    static constexpr Species_impl Fletchling{EnumType::Fletchling};
    static constexpr Species_impl Fletchinder{EnumType::Fletchinder};
    static constexpr Species_impl Talonflame{EnumType::Talonflame};
    static constexpr Species_impl Scatterbug{EnumType::Scatterbug};
    static constexpr Species_impl Spewpa{EnumType::Spewpa};
    static constexpr Species_impl Vivillon{EnumType::Vivillon};
    static constexpr Species_impl Litleo{EnumType::Litleo};
    static constexpr Species_impl Pyroar{EnumType::Pyroar};
    static constexpr Species_impl Flabebe{EnumType::Flabebe};
    static constexpr Species_impl Floette{EnumType::Floette};
    static constexpr Species_impl Florges{EnumType::Florges};
    static constexpr Species_impl Skiddo{EnumType::Skiddo};
    static constexpr Species_impl Gogoat{EnumType::Gogoat};
    static constexpr Species_impl Pancham{EnumType::Pancham};
    static constexpr Species_impl Pangoro{EnumType::Pangoro};
    static constexpr Species_impl Furfrou{EnumType::Furfrou};
    static constexpr Species_impl Espurr{EnumType::Espurr};
    static constexpr Species_impl Meowstic{EnumType::Meowstic};
    static constexpr Species_impl Honedge{EnumType::Honedge};
    static constexpr Species_impl Doublade{EnumType::Doublade};
    static constexpr Species_impl Aegislash{EnumType::Aegislash};
    static constexpr Species_impl Spritzee{EnumType::Spritzee};
    static constexpr Species_impl Aromatisse{EnumType::Aromatisse};
    static constexpr Species_impl Swirlix{EnumType::Swirlix};
    static constexpr Species_impl Slurpuff{EnumType::Slurpuff};
    static constexpr Species_impl Inkay{EnumType::Inkay};
    static constexpr Species_impl Malamar{EnumType::Malamar};
    static constexpr Species_impl Binacle{EnumType::Binacle};
    static constexpr Species_impl Barbaracle{EnumType::Barbaracle};
    static constexpr Species_impl Skrelp{EnumType::Skrelp};
    static constexpr Species_impl Dragalge{EnumType::Dragalge};
    static constexpr Species_impl Clauncher{EnumType::Clauncher};
    static constexpr Species_impl Clawitzer{EnumType::Clawitzer};
    static constexpr Species_impl Helioptile{EnumType::Helioptile};
    static constexpr Species_impl Heliolisk{EnumType::Heliolisk};
    static constexpr Species_impl Tyrunt{EnumType::Tyrunt};
    static constexpr Species_impl Tyrantrum{EnumType::Tyrantrum};
    static constexpr Species_impl Amaura{EnumType::Amaura};
    static constexpr Species_impl Aurorus{EnumType::Aurorus};
    static constexpr Species_impl Sylveon{EnumType::Sylveon};
    static constexpr Species_impl Hawlucha{EnumType::Hawlucha};
    static constexpr Species_impl Dedenne{EnumType::Dedenne};
    static constexpr Species_impl Carbink{EnumType::Carbink};
    static constexpr Species_impl Goomy{EnumType::Goomy};
    static constexpr Species_impl Sliggoo{EnumType::Sliggoo};
    static constexpr Species_impl Goodra{EnumType::Goodra};
    static constexpr Species_impl Klefki{EnumType::Klefki};
    static constexpr Species_impl Phantump{EnumType::Phantump};
    static constexpr Species_impl Trevenant{EnumType::Trevenant};
    static constexpr Species_impl Pumpkaboo{EnumType::Pumpkaboo};
    static constexpr Species_impl Gourgeist{EnumType::Gourgeist};
    static constexpr Species_impl Bergmite{EnumType::Bergmite};
    static constexpr Species_impl Avalugg{EnumType::Avalugg};
    static constexpr Species_impl Noibat{EnumType::Noibat};
    static constexpr Species_impl Noivern{EnumType::Noivern};
    static constexpr Species_impl Xerneas{EnumType::Xerneas};
    static constexpr Species_impl Yveltal{EnumType::Yveltal};
    static constexpr Species_impl Zygarde{EnumType::Zygarde};
    static constexpr Species_impl Diancie{EnumType::Diancie};
    static constexpr Species_impl Hoopa{EnumType::Hoopa};
    static constexpr Species_impl Volcanion{EnumType::Volcanion};
    static constexpr Species_impl Rowlet{EnumType::Rowlet};
    static constexpr Species_impl Dartrix{EnumType::Dartrix};
    static constexpr Species_impl Decidueye{EnumType::Decidueye};
    static constexpr Species_impl Litten{EnumType::Litten};
    static constexpr Species_impl Torracat{EnumType::Torracat};
    static constexpr Species_impl Incineroar{EnumType::Incineroar};
    static constexpr Species_impl Popplio{EnumType::Popplio};
    static constexpr Species_impl Brionne{EnumType::Brionne};
    static constexpr Species_impl Primarina{EnumType::Primarina};
    static constexpr Species_impl Pikipek{EnumType::Pikipek};
    static constexpr Species_impl Trumbeak{EnumType::Trumbeak};
    static constexpr Species_impl Toucannon{EnumType::Toucannon};
    static constexpr Species_impl Yungoos{EnumType::Yungoos};
    static constexpr Species_impl Gumshoos{EnumType::Gumshoos};
    static constexpr Species_impl Grubbin{EnumType::Grubbin};
    static constexpr Species_impl Charjabug{EnumType::Charjabug};
    static constexpr Species_impl Vikavolt{EnumType::Vikavolt};
    static constexpr Species_impl Crabrawler{EnumType::Crabrawler};
    static constexpr Species_impl Crabominable{EnumType::Crabominable};
    static constexpr Species_impl Oricorio{EnumType::Oricorio};
    static constexpr Species_impl Cutiefly{EnumType::Cutiefly};
    static constexpr Species_impl Ribombee{EnumType::Ribombee};
    static constexpr Species_impl Rockruff{EnumType::Rockruff};
    static constexpr Species_impl Lycanroc{EnumType::Lycanroc};
    static constexpr Species_impl Wishiwashi{EnumType::Wishiwashi};
    static constexpr Species_impl Mareanie{EnumType::Mareanie};
    static constexpr Species_impl Toxapex{EnumType::Toxapex};
    static constexpr Species_impl Mudbray{EnumType::Mudbray};
    static constexpr Species_impl Mudsdale{EnumType::Mudsdale};
    static constexpr Species_impl Dewpider{EnumType::Dewpider};
    static constexpr Species_impl Araquanid{EnumType::Araquanid};
    static constexpr Species_impl Fomantis{EnumType::Fomantis};
    static constexpr Species_impl Lurantis{EnumType::Lurantis};
    static constexpr Species_impl Morelull{EnumType::Morelull};
    static constexpr Species_impl Shiinotic{EnumType::Shiinotic};
    static constexpr Species_impl Salandit{EnumType::Salandit};
    static constexpr Species_impl Salazzle{EnumType::Salazzle};
    static constexpr Species_impl Stufful{EnumType::Stufful};
    static constexpr Species_impl Bewear{EnumType::Bewear};
    static constexpr Species_impl Bounsweet{EnumType::Bounsweet};
    static constexpr Species_impl Steenee{EnumType::Steenee};
    static constexpr Species_impl Tsareena{EnumType::Tsareena};
    static constexpr Species_impl Comfey{EnumType::Comfey};
    static constexpr Species_impl Oranguru{EnumType::Oranguru};
    static constexpr Species_impl Passimian{EnumType::Passimian};
    static constexpr Species_impl Wimpod{EnumType::Wimpod};
    static constexpr Species_impl Golisopod{EnumType::Golisopod};
    static constexpr Species_impl Sandygast{EnumType::Sandygast};
    static constexpr Species_impl Palossand{EnumType::Palossand};
    static constexpr Species_impl Pyukumuku{EnumType::Pyukumuku};
    static constexpr Species_impl TypeNull{EnumType::TypeNull};
    static constexpr Species_impl Silvally{EnumType::Silvally};
    static constexpr Species_impl Minior{EnumType::Minior};
    static constexpr Species_impl Komala{EnumType::Komala};
    static constexpr Species_impl Turtonator{EnumType::Turtonator};
    static constexpr Species_impl Togedemaru{EnumType::Togedemaru};
    static constexpr Species_impl Mimikyu{EnumType::Mimikyu};
    static constexpr Species_impl Bruxish{EnumType::Bruxish};
    static constexpr Species_impl Drampa{EnumType::Drampa};
    static constexpr Species_impl Dhelmise{EnumType::Dhelmise};
    static constexpr Species_impl Jangmoo{EnumType::Jangmoo};
    static constexpr Species_impl Hakamoo{EnumType::Hakamoo};
    static constexpr Species_impl Kommoo{EnumType::Kommoo};
    static constexpr Species_impl TapuKoko{EnumType::TapuKoko};
    static constexpr Species_impl TapuLele{EnumType::TapuLele};
    static constexpr Species_impl TapuBulu{EnumType::TapuBulu};
    static constexpr Species_impl TapuFini{EnumType::TapuFini};
    static constexpr Species_impl Cosmog{EnumType::Cosmog};
    static constexpr Species_impl Cosmoem{EnumType::Cosmoem};
    static constexpr Species_impl Solgaleo{EnumType::Solgaleo};
    static constexpr Species_impl Lunala{EnumType::Lunala};
    static constexpr Species_impl Nihilego{EnumType::Nihilego};
    static constexpr Species_impl Buzzwole{EnumType::Buzzwole};
    static constexpr Species_impl Pheromosa{EnumType::Pheromosa};
    static constexpr Species_impl Xurkitree{EnumType::Xurkitree};
    static constexpr Species_impl Celesteela{EnumType::Celesteela};
    static constexpr Species_impl Kartana{EnumType::Kartana};
    static constexpr Species_impl Guzzlord{EnumType::Guzzlord};
    static constexpr Species_impl Necrozma{EnumType::Necrozma};
    static constexpr Species_impl Magearna{EnumType::Magearna};
    static constexpr Species_impl Marshadow{EnumType::Marshadow};
    static constexpr Species_impl Poipole{EnumType::Poipole};
    static constexpr Species_impl Naganadel{EnumType::Naganadel};
    static constexpr Species_impl Stakataka{EnumType::Stakataka};
    static constexpr Species_impl Blacephalon{EnumType::Blacephalon};
    static constexpr Species_impl Zeraora{EnumType::Zeraora};
    static constexpr Species_impl Meltan{EnumType::Meltan};
    static constexpr Species_impl Melmetal{EnumType::Melmetal};
    static constexpr Species_impl Grookey{EnumType::Grookey};
    static constexpr Species_impl Thwackey{EnumType::Thwackey};
    static constexpr Species_impl Rillaboom{EnumType::Rillaboom};
    static constexpr Species_impl Scorbunny{EnumType::Scorbunny};
    static constexpr Species_impl Raboot{EnumType::Raboot};
    static constexpr Species_impl Cinderace{EnumType::Cinderace};
    static constexpr Species_impl Sobble{EnumType::Sobble};
    static constexpr Species_impl Drizzile{EnumType::Drizzile};
    static constexpr Species_impl Inteleon{EnumType::Inteleon};
    static constexpr Species_impl Skwovet{EnumType::Skwovet};
    static constexpr Species_impl Greedent{EnumType::Greedent};
    static constexpr Species_impl Rookidee{EnumType::Rookidee};
    static constexpr Species_impl Corvisquire{EnumType::Corvisquire};
    static constexpr Species_impl Corviknight{EnumType::Corviknight};
    static constexpr Species_impl Blipbug{EnumType::Blipbug};
    static constexpr Species_impl Dottler{EnumType::Dottler};
    static constexpr Species_impl Orbeetle{EnumType::Orbeetle};
    static constexpr Species_impl Nickit{EnumType::Nickit};
    static constexpr Species_impl Thievul{EnumType::Thievul};
    static constexpr Species_impl Gossifleur{EnumType::Gossifleur};
    static constexpr Species_impl Eldegoss{EnumType::Eldegoss};
    static constexpr Species_impl Wooloo{EnumType::Wooloo};
    static constexpr Species_impl Dubwool{EnumType::Dubwool};
    static constexpr Species_impl Chewtle{EnumType::Chewtle};
    static constexpr Species_impl Drednaw{EnumType::Drednaw};
    static constexpr Species_impl Yamper{EnumType::Yamper};
    static constexpr Species_impl Boltund{EnumType::Boltund};
    static constexpr Species_impl Rolycoly{EnumType::Rolycoly};
    static constexpr Species_impl Carkol{EnumType::Carkol};
    static constexpr Species_impl Coalossal{EnumType::Coalossal};
    static constexpr Species_impl Applin{EnumType::Applin};
    static constexpr Species_impl Flapple{EnumType::Flapple};
    static constexpr Species_impl Appletun{EnumType::Appletun};
    static constexpr Species_impl Silicobra{EnumType::Silicobra};
    static constexpr Species_impl Sandaconda{EnumType::Sandaconda};
    static constexpr Species_impl Cramorant{EnumType::Cramorant};
    static constexpr Species_impl Arrokuda{EnumType::Arrokuda};
    static constexpr Species_impl Barraskewda{EnumType::Barraskewda};
    static constexpr Species_impl Toxel{EnumType::Toxel};
    static constexpr Species_impl Toxtricity{EnumType::Toxtricity};
    static constexpr Species_impl Sizzlipede{EnumType::Sizzlipede};
    static constexpr Species_impl Centiskorch{EnumType::Centiskorch};
    static constexpr Species_impl Clobbopus{EnumType::Clobbopus};
    static constexpr Species_impl Grapploct{EnumType::Grapploct};
    static constexpr Species_impl Sinistea{EnumType::Sinistea};
    static constexpr Species_impl Polteageist{EnumType::Polteageist};
    static constexpr Species_impl Hatenna{EnumType::Hatenna};
    static constexpr Species_impl Hattrem{EnumType::Hattrem};
    static constexpr Species_impl Hatterene{EnumType::Hatterene};
    static constexpr Species_impl Impidimp{EnumType::Impidimp};
    static constexpr Species_impl Morgrem{EnumType::Morgrem};
    static constexpr Species_impl Grimmsnarl{EnumType::Grimmsnarl};
    static constexpr Species_impl Obstagoon{EnumType::Obstagoon};
    static constexpr Species_impl Perrserker{EnumType::Perrserker};
    static constexpr Species_impl Cursola{EnumType::Cursola};
    static constexpr Species_impl Sirfetchd{EnumType::Sirfetchd};
    static constexpr Species_impl MrRime{EnumType::MrRime};
    static constexpr Species_impl Runerigus{EnumType::Runerigus};
    static constexpr Species_impl Milcery{EnumType::Milcery};
    static constexpr Species_impl Alcremie{EnumType::Alcremie};
    static constexpr Species_impl Falinks{EnumType::Falinks};
    static constexpr Species_impl Pincurchin{EnumType::Pincurchin};
    static constexpr Species_impl Snom{EnumType::Snom};
    static constexpr Species_impl Frosmoth{EnumType::Frosmoth};
    static constexpr Species_impl Stonjourner{EnumType::Stonjourner};
    static constexpr Species_impl Eiscue{EnumType::Eiscue};
    static constexpr Species_impl Indeedee{EnumType::Indeedee};
    static constexpr Species_impl Morpeko{EnumType::Morpeko};
    static constexpr Species_impl Cufant{EnumType::Cufant};
    static constexpr Species_impl Copperajah{EnumType::Copperajah};
    static constexpr Species_impl Dracozolt{EnumType::Dracozolt};
    static constexpr Species_impl Arctozolt{EnumType::Arctozolt};
    static constexpr Species_impl Dracovish{EnumType::Dracovish};
    static constexpr Species_impl Arctovish{EnumType::Arctovish};
    static constexpr Species_impl Duraludon{EnumType::Duraludon};
    static constexpr Species_impl Dreepy{EnumType::Dreepy};
    static constexpr Species_impl Drakloak{EnumType::Drakloak};
    static constexpr Species_impl Dragapult{EnumType::Dragapult};
    static constexpr Species_impl Zacian{EnumType::Zacian};
    static constexpr Species_impl Zamazenta{EnumType::Zamazenta};
    static constexpr Species_impl Eternatus{EnumType::Eternatus};

    static constexpr Species_impl INVALID{EnumType::INVALID};
};

#endif
