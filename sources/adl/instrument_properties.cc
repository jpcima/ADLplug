//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "instrument.h"
#include "JuceHeader.h"

void Instrument::to_properties(PropertySet &set, const char *key_prefix) const
{
    String pfx = key_prefix;

    set.setValue(pfx + "four_op", four_op());
    set.setValue(pfx + "pseudo_four_op", pseudo_four_op());
    set.setValue(pfx + "blank", blank());
    set.setValue(pfx + "con12", con12());
    set.setValue(pfx + "con34", con34());
    set.setValue(pfx + "note_offset1", (int)note_offset1);
    set.setValue(pfx + "note_offset2", (int)note_offset2);
    set.setValue(pfx + "fb12", (int)fb12());
    set.setValue(pfx + "fb34", (int)fb34());
    set.setValue(pfx + "midi_velocity_offset", (int)midi_velocity_offset);
    set.setValue(pfx + "second_voice_detune", (int)second_voice_detune);
    set.setValue(pfx + "percussion_key_number", (int)percussion_key_number);

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const String opfx = pfx +
            ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum];
        set.setValue(opfx + "attack", (int)attack(opnum));
        set.setValue(opfx + "decay", (int)decay(opnum));
        set.setValue(opfx + "sustain", (int)sustain(opnum));
        set.setValue(opfx + "release", (int)release(opnum));
        set.setValue(opfx + "level", (int)level(opnum));
        set.setValue(opfx + "ksl", (int)ksl(opnum));
        set.setValue(opfx + "fmul", (int)fmul(opnum));
        set.setValue(opfx + "trem", trem(opnum));
        set.setValue(opfx + "vib", vib(opnum));
        set.setValue(opfx + "sus", sus(opnum));
        set.setValue(opfx + "env", env(opnum));
        set.setValue(opfx + "wave", (int)wave(opnum));
    }
}

Instrument Instrument::from_properties(const juce::PropertySet &set, const char *key_prefix)
{
    Instrument ins;
    String pfx = key_prefix;

    ins.four_op(set.getBoolValue(pfx + "four_op"));
    ins.pseudo_four_op(set.getBoolValue(pfx + "pseudo_four_op"));
    ins.blank(set.getBoolValue(pfx + "blank"));
    ins.con12(set.getBoolValue(pfx + "con12"));
    ins.con34(set.getBoolValue(pfx + "con34"));
    ins.note_offset1 = set.getIntValue("note_offset1");
    ins.note_offset2 = set.getIntValue("note_offset2");
    ins.fb12(set.getIntValue(pfx + "fb12"));
    ins.fb34(set.getIntValue(pfx + "fb34"));
    ins.midi_velocity_offset = set.getIntValue(pfx + "midi_velocity_offset");
    ins.second_voice_detune = set.getIntValue(pfx + "second_voice_detune");
    ins.percussion_key_number = set.getIntValue(pfx + "percussion_key_number");

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const String opfx = pfx +
            ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum];
        ins.attack(opnum, set.getIntValue(opfx + "attack"));
        ins.decay(opnum, set.getIntValue(opfx + "decay"));
        ins.sustain(opnum, set.getIntValue(opfx + "sustain"));
        ins.release(opnum, set.getIntValue(opfx + "release"));
        ins.level(opnum, set.getIntValue(opfx + "level"));
        ins.ksl(opnum, set.getIntValue(opfx + "ksl"));
        ins.fmul(opnum, set.getIntValue(opfx + "fmul"));
        ins.trem(opnum, set.getBoolValue(opfx + "trem"));
        ins.vib(opnum, set.getBoolValue(opfx + "vib"));
        ins.sus(opnum, set.getBoolValue(opfx + "sus"));
        ins.env(opnum, set.getBoolValue(opfx + "env"));
        ins.wave(opnum, set.getIntValue(opfx + "wave"));
    }

    return ins;
}
