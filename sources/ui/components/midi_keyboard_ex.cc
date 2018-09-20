//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "midi_keyboard_ex.h"

Midi_Keyboard_Ex::Midi_Keyboard_Ex(MidiKeyboardState &state, Orientation orientation)
    : MidiKeyboardComponent(state, orientation),
      highlight_value_{}
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

static constexpr int gray_min = 0xa0;
static constexpr int gray_max = 0xa0;

void Midi_Keyboard_Ex::drawWhiteNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour line_colour, Colour text_colour)
{
    jassert(midiNoteNumber >= 0 && midiNoteNumber < 128);

    uint8_t hl = highlight_value_[note];

    Colour orig_colour;
    if (hl > 0) {
        orig_colour = findColour(keyDownOverlayColourId);
        float velocity = (hl - 1) * (1 / 126.0f);
        int cc = (int)(gray_min + velocity * (gray_max - gray_min));
        setColour(keyDownOverlayColourId, Colour(cc, cc, cc));
    }

    MidiKeyboardComponent::drawWhiteNote(note, g, area, is_down || hl > 0, is_over, line_colour, text_colour);

    if (hl > 0)
        setColour(keyDownOverlayColourId, orig_colour);
}

void Midi_Keyboard_Ex::drawBlackNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour note_fill_colour)
{
    jassert(midiNoteNumber >= 0 && midiNoteNumber < 128);

    uint8_t hl = highlight_value_[note];

    Colour orig_colour;
    if (hl > 0) {
        orig_colour = findColour(keyDownOverlayColourId);
        float velocity = (hl - 1) * (1 / 126.0f);
        int cc = (int)(gray_min + velocity * (gray_max - gray_min));
        setColour(keyDownOverlayColourId, Colour(cc, cc, cc));
    }

    MidiKeyboardComponent::drawBlackNote(note, g, area, is_down || hl > 0, is_over, note_fill_colour);

    if (hl > 0)
        setColour(keyDownOverlayColourId, orig_colour);
}
