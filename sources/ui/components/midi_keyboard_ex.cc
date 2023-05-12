//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "midi_keyboard_ex.h"

Midi_Keyboard_Ex::Midi_Keyboard_Ex(MidiKeyboardState &state, Orientation orientation)
    : MidiKeyboardComponent(state, orientation),
      designated_note_color_((uint8_t)245, 0, 41, 0.5f)
{
}

void Midi_Keyboard_Ex::highlight_note(unsigned note, unsigned velocity)
{
    if (note >= 128)
        return;

    if (highlight_value_[note] == velocity)
        return;

    highlight_value_[note] = velocity;
    repaint(getRectangleForKey(note).toNearestInt());
}

void Midi_Keyboard_Ex::designate_note(int note)
{
    if (note == designated_note_)
        return;

    designated_note_ = note;
    repaint();
}

static constexpr int gray_min = 0xa0;
static constexpr int gray_max = 0xa0;

void Midi_Keyboard_Ex::colourChanged()
{
    if (!block_colour_changed_callback_)
        MidiKeyboardComponent::colourChanged();
}

void Midi_Keyboard_Ex::drawWhiteNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour line_colour, Colour text_colour)
{
    jassert(note >= 0 && note < 128);

    uint8_t hl = highlight_value_[note];

    Colour orig_colour;
    if (hl > 0) {
        orig_colour = findColour(keyDownOverlayColourId);
        float velocity = (hl - 1) * (1 / 126.0f);
        int cc = (int)(gray_min + velocity * (gray_max - gray_min));
        block_colour_changed_callback_ = true;
        setColour(keyDownOverlayColourId, Colour(cc, cc, cc));
        block_colour_changed_callback_ = false;
    }

    MidiKeyboardComponent::drawWhiteNote(note, g, area, is_down || hl > 0, is_over, line_colour, text_colour);

    if (hl > 0) {
        block_colour_changed_callback_ = true;
        setColour(keyDownOverlayColourId, orig_colour);
        block_colour_changed_callback_ = false;
    }

    if (note == designated_note_) {
        float w = area.getWidth();
        float r = w * 0.7f * getBlackNoteWidthProportion();
        g.setColour(designated_note_color_);
        g.fillEllipse(area.getX() + 0.5f * (w - r), area.getBottom() - 1.5f * r, r, r);
    }
}

void Midi_Keyboard_Ex::drawBlackNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour note_fill_colour)
{
    jassert(note >= 0 && note < 128);

    uint8_t hl = highlight_value_[note];

    Colour orig_colour;
    if (hl > 0) {
        orig_colour = findColour(keyDownOverlayColourId);
        float velocity = (hl - 1) * (1 / 126.0f);
        int cc = (int)(gray_min + velocity * (gray_max - gray_min));
        block_colour_changed_callback_ = true;
        setColour(keyDownOverlayColourId, Colour(cc, cc, cc));
        block_colour_changed_callback_ = false;
    }

    MidiKeyboardComponent::drawBlackNote(note, g, area, is_down || hl > 0, is_over, note_fill_colour);

    if (hl > 0) {
        block_colour_changed_callback_ = true;
        setColour(keyDownOverlayColourId, orig_colour);
        block_colour_changed_callback_ = false;
    }

    if (note == designated_note_) {
        float w = area.getWidth();
        float r = w * 0.7f;
        g.setColour(designated_note_color_);
        g.fillEllipse(area.getX() + 0.5f * (w - r), area.getBottom() - 1.5f * r, r, r);
    }
}
