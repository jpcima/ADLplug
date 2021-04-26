//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/look_and_feel.h"
#include "resources.h"

RESOURCE(Res, Mono_BoldItalic)
RESOURCE(Res, Mono_Bold)
RESOURCE(Res, Mono_Italic)
RESOURCE(Res, Mono_Regular)
RESOURCE(Res, Sans_BoldItalic)
RESOURCE(Res, Sans_Bold)
RESOURCE(Res, Sans_Italic)
RESOURCE(Res, Sans_Regular)
RESOURCE(Res, Serif_BoldItalic)
RESOURCE(Res, Serif_Bold)
RESOURCE(Res, Serif_Italic)
RESOURCE(Res, Serif_Regular)

#if 1
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[LF] " fmt "\n", ##__VA_ARGS__)
#endif

//==============================================================================
void Custom_Look_And_Feel::add_custom_tooltip(const String &key, Component *component, bool owned)
{
    OptionalScopedPointer<Component> ptr(component, owned);
    custom_tooltips_[key].component = std::move(ptr);
}

//==============================================================================
Typeface::Ptr Custom_Look_And_Feel::getTypefaceForFont(const Font &font)
{
    const String &name = font.getTypefaceName();
    const String &style = font.getTypefaceStyle();

    trace("Query Typeface name='%s' style='%s'",
          name.toRawUTF8(), style.toRawUTF8());

    Typeface::Ptr tf;
    if (name == Font::getDefaultSansSerifFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontSansItalic, Res::Sans_Italic);
        else if (style == "Bold")
            tf = getOrCreateFont(fontSansBold, Res::Sans_Bold);
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontSansBoldItalic, Res::Sans_BoldItalic);
        else
            tf = getOrCreateFont(fontSansRegular, Res::Sans_Regular);
    }
    else if (name == Font::getDefaultSerifFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontSerifItalic, Res::Serif_Italic);
        else if (style == "Bold")
            tf = getOrCreateFont(fontSerifBold, Res::Serif_Bold);
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontSerifBoldItalic, Res::Serif_BoldItalic);
        else
            tf = getOrCreateFont(fontSerifRegular, Res::Serif_Regular);
    }
    else if (name == Font::getDefaultMonospacedFontName()) {
        if (style == "Italic")
            tf = getOrCreateFont(fontMonoItalic, Res::Mono_Italic);
        else if (style == "Bold")
            tf = getOrCreateFont(fontMonoBold, Res::Mono_Bold);
        else if (style == "Bold Italic")
            tf = getOrCreateFont(fontMonoBoldItalic, Res::Mono_BoldItalic);
        else
            tf = getOrCreateFont(fontMonoRegular, Res::Mono_Regular);
    }

    if (!tf) {
        trace("Typeface not found, fallback");
        tf = LookAndFeel::getTypefaceForFont(font);
    }

    return tf;

#undef BINARY_FONT
}

Typeface::Ptr Custom_Look_And_Feel::getOrCreateFont(
    Typeface::Ptr &font, const Res::Data &data)
{
    if (!font) {
        MemoryInputStream memStream(data.data, data.size, false);
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

Font Custom_Look_And_Feel::getComboBoxFont(ComboBox &box)
{
    return { jmin (15.0f, box.getHeight() * 0.85f) };
}

Label *Custom_Look_And_Feel::createSliderTextBox(Slider &slider)
{
    std::unique_ptr<Label> label(Base::createSliderTextBox(slider));
    NamedValueSet &props = slider.getProperties();
    props.set("X-Slider-Text-Box", (int64)(intptr_t)label.get());
    return label.release();
}

Rectangle<int> Custom_Look_And_Feel::getTooltipBounds(const String &text, Point<int> pos, Rectangle<int> parent_area)
{
    if (text.startsWith("<<") && text.endsWith(">>")) {
        String key = text.substring(2, text.length() - 2);
        auto it = custom_tooltips_.find(key);
        if (it != custom_tooltips_.end()) {
            Component *comp = it->second.component.get();
            int w = comp->getWidth() + 14;
            int h = comp->getHeight() + 6;
            return Rectangle<int>(pos.x > parent_area.getCentreX() ? pos.x - (w + 12) : pos.x + 24,
                                  pos.y > parent_area.getCentreY() ? pos.y - (h + 6) : pos.y + 6,
                                  w, h).constrainedWithin(parent_area);
        }
    }
    return Base::getTooltipBounds(text, pos, parent_area);
}

void Custom_Look_And_Feel::drawTooltip(Graphics &g, const String &text, int width, int height)
{
    if (text.startsWith("<<") && text.endsWith(">>")) {
        String key = text.substring(2, text.length() - 2);
        auto it = custom_tooltips_.find(key);
        if (it != custom_tooltips_.end()) {
            Component *comp = it->second.component.get();
            Rectangle<int> bounds(width, height);
            float cornerSize = 5.0f;
            g.setColour(findColour(TooltipWindow::backgroundColourId));
            g.fillRoundedRectangle(bounds.toFloat(), cornerSize);
            g.setColour(findColour(TooltipWindow::outlineColourId));
            g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);
            g.setOrigin((width - comp->getWidth()) / 2, (height - comp->getHeight()) / 2);
            comp->paintEntireComponent(g, false);
            return;
        }
    }
    return Base::drawTooltip(g, text, width, height);
}
