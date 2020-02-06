# PKSM-Core
The core save editing code for PKSM (https://github.com/FlagBrew/PKSM)

Usage in another project:

Special info:
- Your include path must have a file named `PKSMCORE_CONFIG.h` that includes some defines, listed below
- All \_PKSMCORE_DISABLE\_\*\_STRINGS defines remove the dependency on the file(s) listed in parentheses after the removed method names being in _PKSMCORE_LANG_FOLDER

Defines:
- _PKSMCORE_LANG_FOLDER: the folder from which languages are loaded
- _PKSMCORE_GETLINE_FUNC: the name of your stdio.h getline function
- _PKSMCORE_DISABLE_THREAD_SAFE_RANDOM: if defined, thread_local is not used for the random number generator
- _PKSMCORE_DISABLE_ABILITY_STRINGS: removes `i18n::ability` and `i18n::rawAbilities` (`abilities.txt`)
- _PKSMCORE_DISABLE_BALL_STRINGS: removes `i18n::ball` (`balls.txt`)
- _PKSMCORE_DISABLE_FORM_STRINGS: removes `i18n::form` (`forms.txt`)
- _PKSMCORE_DISABLE_HIDDEN_POWER_STRINGS: removes `i18n::hp` (`hp.txt`)
- _PKSMCORE_DISABLE_ITEM_STRINGS: removes `i18n::item` and `i18n::rawItems` (`items.txt`)
- _PKSMCORE_DISABLE_MOVE_STRINGS: removes `i18n::move` and `i18n::rawMoves` (`moves.txt`)
- _PKSMCORE_DISABLE_NATURE_STRINGS: removes `i18n::nature` (`natures.txt`)
- _PKSMCORE_DISABLE_SPECIES_STRINGS: removes `i18n::species` and `i18n::rawSpecies` (`species.txt`)
- _PKSMCORE_DISABLE_GAME_STRINGS: removes `i18n::game` and `i18n::rawGames` (`games.txt`)
- _PKSMCORE_DISABLE_LOCATION_STRINGS: removes `i18n::location` and `i18n::locations` (`locations3.txt`, `locations4.txt`, `locations5.txt`, `locations6.txt`, `locations7.txt`, `locationsLGPE.txt`, `locations8.txt`)
- _PKSMCORE_DISABLE_GEO_STRINGS: removes `i18n::subregion`, `i18n::country`, `i18n::rawCountries`, and `i18n::rawSubregions` (`countries.txt`, `subregions/*`)
- _PKSMCORE_DISABLE_GUI_STRINGS: removes `i18n::localize` (`gui.json`)
- _PKSMCORE_EXTRA_LANGUAGES: If defined, this must be a comma-separated list of language names. They will be added to the Language enum in order of appearance in the list with values starting at `u8(Language::CHT) + 1`, and will work properly with all i18n functions. If a given file for a language does not exist, it will use the English version. If there's no English version, it will likely fail gracefully, but I'd suggest staying away from that situation.

Necessary include directories:
- memecrypto
- swshcrypto
- include

Necessary source directories:
- memecrypto
- swshcrypto
- source and all its subdirectories
