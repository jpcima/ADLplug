//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/look_and_feel.h"
#include "../../JuceLibraryCode/BinaryData.h"

#if 0
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[LF] " fmt "\n", ##__VA_ARGS__)
#endif

Typeface::Ptr Custom_Look_And_Feel::getTypefaceForFont(const Font &font)
{
    const String &name = font.getTypefaceName();
    const String &style = font.getTypefaceStyle();

    trace("Query Typeface name='%s' style='%s'",
          name.toRawUTF8(), style.toRawUTF8());

#define BINARY_FONT(x) BinaryData::x##_ttf_gz, BinaryData::x##_ttf_gzSize

    Typeface::Ptr tf;
    if (name == Font::getDefaultSansSerifFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontSansItalic, BINARY_FONT(SansItalic));
        else if (style == "Bold")
            tf = getOrCreateFont(fontSansBold, BINARY_FONT(SansBold));
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontSansBoldItalic, BINARY_FONT(SansBoldItalic));
        else
            tf = getOrCreateFont(fontSansRegular, BINARY_FONT(SansRegular));
    }
    else if (name == Font::getDefaultSerifFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontSerifItalic, BINARY_FONT(SerifItalic));
        else if (style == "Bold")
            tf = getOrCreateFont(fontSerifBold, BINARY_FONT(SerifBold));
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontSerifBoldItalic, BINARY_FONT(SerifBoldItalic));
        else
            tf = getOrCreateFont(fontSerifRegular, BINARY_FONT(SerifRegular));
    }
    else if (name == Font::getDefaultMonospacedFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontMonoItalic, BINARY_FONT(MonoItalic));
        else if (style == "Bold")
            tf = getOrCreateFont(fontMonoBold, BINARY_FONT(MonoBold));
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontMonoBoldItalic, BINARY_FONT(MonoBoldItalic));
        else
            tf = getOrCreateFont(fontMonoRegular, BINARY_FONT(MonoRegular));
    }

    if (!tf) {
        trace("Typeface not found, fallback");
        tf = LookAndFeel::getTypefaceForFont(font);
    }

    return tf;

#undef BINARY_FONT
}

Typeface::Ptr Custom_Look_And_Feel::getOrCreateFont(
    Typeface::Ptr &font, const char *data, unsigned size)
{
    if (!font) {
        MemoryInputStream memStream(data, size, false);
        GZIPDecompressorInputStream gzStream(&memStream, false, GZIPDecompressorInputStream::gzipFormat);

        MemoryBlock memBlock;
        gzStream.readIntoMemoryBlock(memBlock);

        font = Typeface::createSystemTypefaceFor(
            memBlock.getData(), memBlock.getSize());

        if (!font)
            trace("Could not load font data.");
        else
            trace("Font loaded name='%s' style='%s'",
                  font->getName().toRawUTF8(), font->getStyle().toRawUTF8());
    }
    return font;
}

void Custom_Look_And_Feel::drawButtonBackground(Graphics &g, Button &button, const Colour &background_colour, bool is_mouse_over_button, bool is_button_down)
{
    float corner_size = 6.0f;
    Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

    auto base_colour = background_colour
        .withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);
    if (is_button_down || is_mouse_over_button)
        base_colour = base_colour.contrasting(is_button_down ? 0.2f : 0.05f);

    g.setColour(base_colour);
    if (button.isConnectedOnLeft() || button.isConnectedOnRight() ||
        button.isConnectedOnTop() || button.isConnectedOnBottom()) {
        Path path;
        path.addRoundedRectangle(bounds.getX(), bounds.getY(),
                                 bounds.getWidth(), bounds.getHeight(),
                                 corner_size, corner_size,
                                 !button.isConnectedOnLeft() && !button.isConnectedOnTop(),
                                 !button.isConnectedOnRight() && !button.isConnectedOnTop(),
                                 !button.isConnectedOnLeft() && !button.isConnectedOnBottom(),
                                 !button.isConnectedOnRight() && !button.isConnectedOnBottom());
        g.fillPath(path);
        g.setColour(button.findColour(ComboBox::outlineColourId));
        g.strokePath(path, PathStrokeType(1.0f));
    }
    else {
        g.fillRoundedRectangle(bounds, corner_size);
        g.setColour(button.findColour(ComboBox::outlineColourId));
        g.drawRoundedRectangle(bounds, corner_size, 1.0f);
    }
}
