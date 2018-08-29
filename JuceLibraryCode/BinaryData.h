/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   knobskin_png;
    const int            knobskin_pngSize = 21416;

    extern const char*   DOSBox_png;
    const int            DOSBox_pngSize = 43300;

    extern const char*   Nuked_png;
    const int            Nuked_pngSize = 20492;

    extern const char*   Nuked2_png;
    const int            Nuked2_pngSize = 18518;

    extern const char*   MonoBold_ttf_gz;
    const int            MonoBold_ttf_gzSize = 20006;

    extern const char*   MonoBoldItalic_ttf_gz;
    const int            MonoBoldItalic_ttf_gzSize = 18727;

    extern const char*   MonoItalic_ttf_gz;
    const int            MonoItalic_ttf_gzSize = 18408;

    extern const char*   MonoRegular_ttf_gz;
    const int            MonoRegular_ttf_gzSize = 19970;

    extern const char*   SansBold_ttf_gz;
    const int            SansBold_ttf_gzSize = 23070;

    extern const char*   SansBoldItalic_ttf_gz;
    const int            SansBoldItalic_ttf_gzSize = 23095;

    extern const char*   SansItalic_ttf_gz;
    const int            SansItalic_ttf_gzSize = 23233;

    extern const char*   SansRegular_ttf_gz;
    const int            SansRegular_ttf_gzSize = 23807;

    extern const char*   SerifBold_ttf_gz;
    const int            SerifBold_ttf_gzSize = 24164;

    extern const char*   SerifBoldItalic_ttf_gz;
    const int            SerifBoldItalic_ttf_gzSize = 24863;

    extern const char*   SerifItalic_ttf_gz;
    const int            SerifItalic_ttf_gzSize = 25879;

    extern const char*   SerifRegular_ttf_gz;
    const int            SerifRegular_ttf_gzSize = 27031;

    extern const char*   emoji_u1f4be_png;
    const int            emoji_u1f4be_pngSize = 1505;

    extern const char*   emoji_u1f4c2_png;
    const int            emoji_u1f4c2_pngSize = 2181;

    extern const char*   banks_pak;
    const int            banks_pakSize = 149159;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 19;

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
