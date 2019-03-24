//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "instrument.h"
#include "JuceHeader.h"

PropertySet Instrument::to_properties() const
{
    PropertySet set;

    set.setValue("four_op", four_op());
    set.setValue("pseudo_four_op", pseudo_four_op());
    set.setValue("blank", blank());
    set.setValue("con12", con12());
    set.setValue("con34", con34());
    set.setValue("note_offset1", (int)note_offset1);
    set.setValue("note_offset2", (int)note_offset2);
    set.setValue("fb12", (int)fb12());
    set.setValue("fb34", (int)fb34());
    set.setValue("midi_velocity_offset", (int)midi_velocity_offset);
    set.setValue("second_voice_detune", (int)second_voice_detune);
    set.setValue("percussion_key_number", (int)percussion_key_number);

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const String opfx = ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum];
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

    set.setValue("delay_off_ms", (int)delay_off_ms);
    set.setValue("delay_on_ms", (int)delay_on_ms);

    return set;
}

Instrument Instrument::from_properties(const juce::PropertySet &set)
{
    Instrument ins;

    ins.four_op(set.getBoolValue("four_op"));
    ins.pseudo_four_op(set.getBoolValue("pseudo_four_op"));
    ins.blank(set.getBoolValue("blank"));
    ins.con12(set.getBoolValue("con12"));
    ins.con34(set.getBoolValue("con34"));
    ins.note_offset1 = set.getIntValue("note_offset1");
    ins.note_offset2 = set.getIntValue("note_offset2");
    ins.fb12(set.getIntValue("fb12"));
    ins.fb34(set.getIntValue("fb34"));
    ins.midi_velocity_offset = set.getIntValue("midi_velocity_offset");
    ins.second_voice_detune = set.getIntValue("second_voice_detune");
    ins.percussion_key_number = set.getIntValue("percussion_key_number");

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const String opfx = ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum];
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

    ins.delay_off_ms = set.getIntValue("delay_off_ms");
    ins.delay_on_ms = set.getIntValue("delay_on_ms");

    return ins;
}

PropertySet Instrument_Global_Parameters::to_properties() const
{
    PropertySet set;
    set.setValue("volume_model", (int)volume_model);
    set.setValue("deep_tremolo", deep_tremolo);
    set.setValue("deep_vibrato", deep_vibrato);
    return set;
}

Instrument_Global_Parameters Instrument_Global_Parameters::from_properties(const PropertySet &set)
{
    Instrument_Global_Parameters gp;
    gp.volume_model = set.getIntValue("volume_model");
    gp.deep_tremolo = set.getBoolValue("deep_tremolo");
    gp.deep_vibrato = set.getBoolValue("deep_vibrato");
    return gp;
}
