//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"

class Midi_Keyboard_Ex : public juce::MidiKeyboardComponent {
public:
    Midi_Keyboard_Ex(MidiKeyboardState &state, Orientation orientation);
    void highlight_note(unsigned note, unsigned velocity);
    void designate_note(int note);

protected:
    void colourChanged() override;
    void drawWhiteNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour line_colour, Colour text_colour) override;
    void drawBlackNote(int note, Graphics &g, Rectangle<float> area, bool is_down, bool is_over, Colour note_fill_colour) override;

private:
    uint8_t highlight_value_[128] = {};
    int designated_note_ = -1;
    Colour designated_note_color_;
    bool block_colour_changed_callback_ = false; // stops excessive repaints

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Midi_Keyboard_Ex);
};
