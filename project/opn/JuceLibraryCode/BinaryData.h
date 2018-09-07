/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   banks_pak;
    const int            banks_pakSize = 30251;

    extern const char*   knobskin_png;
    const int            knobskin_pngSize = 21416;

    extern const char*   Gens_png;
    const int            Gens_pngSize = 11926;

    extern const char*   MAME_png;
    const int            MAME_pngSize = 11057;

    extern const char*   Nuked_png;
    const int            Nuked_pngSize = 20492;

    extern const char*   MonoBold_ttf_gz;
    const int            MonoBold_ttf_gzSize = 30243;

    extern const char*   MonoBoldItalic_ttf_gz;
    const int            MonoBoldItalic_ttf_gzSize = 28621;

    extern const char*   MonoItalic_ttf_gz;
    const int            MonoItalic_ttf_gzSize = 28112;

    extern const char*   MonoRegular_ttf_gz;
    const int            MonoRegular_ttf_gzSize = 30321;

    extern const char*   SansBold_ttf_gz;
    const int            SansBold_ttf_gzSize = 36017;

    extern const char*   SansBoldItalic_ttf_gz;
    const int            SansBoldItalic_ttf_gzSize = 34736;

    extern const char*   SansItalic_ttf_gz;
    const int            SansItalic_ttf_gzSize = 34623;

    extern const char*   SansRegular_ttf_gz;
    const int            SansRegular_ttf_gzSize = 35921;

    extern const char*   SerifBold_ttf_gz;
    const int            SerifBold_ttf_gzSize = 36623;

    extern const char*   SerifBoldItalic_ttf_gz;
    const int            SerifBoldItalic_ttf_gzSize = 37567;

    extern const char*   SerifItalic_ttf_gz;
    const int            SerifItalic_ttf_gzSize = 39082;

    extern const char*   SerifRegular_ttf_gz;
    const int            SerifRegular_ttf_gzSize = 39502;

    extern const char*   emoji_u1f4a1_png;
    const int            emoji_u1f4a1_pngSize = 6808;

    extern const char*   emoji_u1f4be_png;
    const int            emoji_u1f4be_pngSize = 1505;

    extern const char*   emoji_u1f4c2_png;
    const int            emoji_u1f4c2_pngSize = 2181;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 20;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
