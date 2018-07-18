//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parameter_block.h"
#include "adl/instrument.h"

void Parameter_Block::setup_parameters(AudioProcessor &p)
{
    Instrument ins;

    p.addParameter((p_4op = new AudioParameterBool("4op", "4op", ins.four_op(), String())));
    p.addParameter((p_ps4op = new AudioParameterBool("ps4op", "Pseudo 4op", ins.pseudo_four_op(), String())));
    p.addParameter((p_blank = new AudioParameterBool("blank", "Blank", ins.blank(), String())));
    p.addParameter((p_con12 = new AudioParameterChoice("con12", "Mode 1-2", {"FM", "AM"}, ins.con12(), String())));
    p.addParameter((p_con34 = new AudioParameterChoice("con34", "Mode 3-4", {"FM", "AM"}, ins.con34(), String())));
    p.addParameter((p_tune12 = new AudioParameterInt("tune12", "Note offset 1-2", -127, +127, ins.note_offset1, String())));
    p.addParameter((p_tune34 = new AudioParameterInt("tune34", "Note offset 3-4", -127, +127, ins.note_offset2, String())));
    p.addParameter((p_fb12 = new AudioParameterInt("fb12", "Feedback 1-2", 0, 7, ins.fb12(), String())));
    p.addParameter((p_fb34 = new AudioParameterInt("fb34", "Feedback 3-4", 0, 7, ins.fb34(), String())));
    p.addParameter((p_veloffset = new AudioParameterInt("veloffset", "Velocity offset", -127, +127, ins.midi_velocity_offset, String())));
    p.addParameter((p_voice2ft = new AudioParameterInt("voice2ft", "Voice 2 fine tune", -127, +127, ins.second_voice_detune, String())));
    p.addParameter((p_drumnote = new AudioParameterInt("drumnote", "Percussion note", 0, 127, ins.percussion_key_number, String())));

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const char *idprefix =
            ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum];
        const char *nameprefix =
            ((const char *[]){ "Carrier 1 ", "Modulator 1 ", "Carrier 2 ", "Modulator 2 " })[opnum];

        auto id = [idprefix](const char *x) -> String { return idprefix + String(x); };
        auto name = [nameprefix](const char *x) -> String { return nameprefix + String(x); };

        Operator &op = nth_operator(opnum);
        p.addParameter((op.p_attack = new AudioParameterInt(id("attack"), name("Attack"), 0, 15, ins.attack(opnum), String())));
        p.addParameter((op.p_decay = new AudioParameterInt(id("decay"), name("Decay"), 0, 15, ins.decay(opnum), String())));
        p.addParameter((op.p_sustain = new AudioParameterInt(id("sustain"), name("Sustain"), 0, 15, ins.sustain(opnum), String())));
        p.addParameter((op.p_release = new AudioParameterInt(id("release"), name("Release"), 0, 15, ins.release(opnum), String())));
        p.addParameter((op.p_level = new AudioParameterInt(id("level"), name("Level"), 0, 63, ins.level(opnum), String())));
        p.addParameter((op.p_ksl = new AudioParameterInt(id("ksl"), name("Key scale level"), 0, 3, ins.ksl(opnum), String())));
        p.addParameter((op.p_fmul = new AudioParameterInt(id("fmul"), name("Frequency multiplier"), 0, 15, ins.ksl(opnum), String())));
        p.addParameter((op.p_trem = new AudioParameterBool(id("trem"), name("Tremolo"), ins.trem(opnum), String())));
        p.addParameter((op.p_vib = new AudioParameterBool(id("vib"), name("Vibrato"), ins.vib(opnum), String())));
        p.addParameter((op.p_sus = new AudioParameterBool(id("sus"), name("Sustaining"), ins.sus(opnum), String())));
        p.addParameter((op.p_env = new AudioParameterBool(id("env"), name("Key scaling"), ins.env(opnum), String())));
        p.addParameter((op.p_wave = new AudioParameterInt(id("wave"), name("Waveform"), 0, 7, ins.wave(opnum), String())));
    }
}
