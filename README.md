# PKSM-Core
The core save editing code for [PKSM](https://github.com/FlagBrew/PKSM). This library is warning-free, with one exception: PKSM-Core will trigger Wpsabi. However, as PKSM-Core does not provide binary releases, this warning is not relevant to it.
In nearly all cases, PKSM-Core is also exception-free. The only time an exception may be thrown is if something interesting happens in Generation 8 decryption. If such an exception is thrown and you are certain that the save file in question is valid, please immediately open an issue.

Usage in another project:

Necessary include directories:
- memecrypto
- swshcrypto
- include

Necessary source directories:
- memecrypto
- swshcrypto
- source and all its subdirectories

Special info:
- A few defines must be provided. This may be done by either having a file in your include path named `PKSMCORE_CONFIG.h`, or with your compiler's command line options. If using the command line options, an additional define, `_PKSMCORE_CONFIGURED`, is required to indicate that you do not wish to read `PKSMCORE_CONFIG.h`.

Required defines:
- _PKSMCORE_LANG_FOLDER: the folder from which languages are loaded at runtime

Optional defines:
- _PKSMCORE_GETLINE_FUNC: the name of your stdio.h getline function (on certain platforms, it may be named something different, such as `__getline`). Defaults to `getline`
- _PKSMCORE_DISABLE_THREAD_SAFE_RANDOM: If defined, `thread_local` is removed from the random number generator
- _PKSMCORE_EXTRA_LANGUAGES: If defined, this must be a comma-separated list of language names. They will be added to the Language enum in order of appearance in the list with values starting at `u8(Language::CHT) + 1`, and will work properly with all i18n functions. If a given file for a language does not exist, it will use the English version. If there is no English version, it will likely fail gracefully, but I'd suggest staying away from that situation.

Credits:
- @BernardoGiordano for the original PKSM editing code
  - @Admiral-Fish and @piepie62 for helping to rewrite that original code into C++
- @kwsch for PKHeX, from which most code is translated into C++
- @SciresM for a huge amount of Pokémon format research and memecrypto
- @Epicpkmn11 for convincing me to make this a separate repository and most generation 3 stuff
