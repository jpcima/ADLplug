//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parameter_block.h"
#include "adl/chip_settings.h"
#include "adl/instrument.h"
#include "adl/player.h"
#include "utility/pak.h"
#include <wopl/wopl_file.h>
#include <fmt/format.h>
#include <cassert>

static Instrument default_instrument()
{
    Pak_File_Reader pak;
    if (!pak.init_with_data((const uint8_t *)BinaryData::banks_pak, BinaryData::banks_pakSize))
        assert(false);
    std::string default_wopl = pak.extract(0);
    assert(default_wopl.size() != 0);

    WOPLFile_Ptr file(WOPL_LoadBankFromMem((void *)default_wopl.data(), default_wopl.size(), nullptr));
    if (!file)
        throw std::bad_alloc();

    WOPLBank *bank = nullptr;
    for (unsigned i = 0, n = file->banks_count_melodic; i < n && !bank; ++i) {
        WOPLBank *cur = &file->banks_melodic[i];
        if (cur->bank_midi_lsb == 0 && cur->bank_midi_msb == 0)
            bank = cur;
    }

    assert(bank);
    return Instrument::from_wopl(bank->ins[0]);
}

static Chip_Settings default_chip_settings()
{
    Chip_Settings cs;
    cs.emulator = ::get_emulator_defaults().default_index;
    return cs;
}

void Parameter_Block::setup_parameters(AudioProcessor &p)
{
    Chip_Settings cs = default_chip_settings();

    first_chip_setting = p.getParameters().size();
    p.addParameter((p_emulator = new AudioParameterChoice("emulator", "Emulator", get_emulator_defaults().choices, cs.emulator, String())));
    p.addParameter((p_nchip = new AudioParameterInt("nchip", "Chip count", 1, 100, cs.chip_count, String())));
    p.addParameter((p_n4op = new AudioParameterInt("n4op", "4op channel count", 0, 600, cs.fourop_count, String())));
    last_chip_setting = p.getParameters().size() - 1;

    Instrument ins = default_instrument();

    first_instrument_parameter = p.getParameters().size();
    for (unsigned pn = 0; pn < 16; ++pn) {
        Part &part = this->part[pn];

        {
            String idprefix = fmt::format("P{:d}", pn + 1);
            String nameprefix = fmt::format("[Part {:d}] ", pn + 1);

            auto id = [idprefix](const char *x) -> String { return idprefix + x; };
            auto name = [nameprefix](const char *x) -> String { return nameprefix + x; };

            p.addParameter((part.p_is4op = new AudioParameterBool(id("is4op"), name("4op"), ins.four_op(), String())));
            p.addParameter((part.p_ps4op = new AudioParameterBool(id("ps4op"), name("Pseudo 4op"), ins.pseudo_four_op(), String())));
            p.addParameter((part.p_blank = new AudioParameterBool(id("blank"), name("Blank"), ins.blank(), String())));
            p.addParameter((part.p_con12 = new AudioParameterChoice(id("con12"), name("Mode 1-2"), {"FM", "AM"}, ins.con12(), String())));
            p.addParameter((part.p_con34 = new AudioParameterChoice(id("con34"), name("Mode 3-4"), {"FM", "AM"}, ins.con34(), String())));
            p.addParameter((part.p_tune12 = new AudioParameterInt(id("tune12"), name("Note offset 1-2"), -127, +127, ins.note_offset1, String())));
            p.addParameter((part.p_tune34 = new AudioParameterInt(id("tune34"), name("Note offset 3-4"), -127, +127, ins.note_offset2, String())));
            p.addParameter((part.p_fb12 = new AudioParameterInt(id("fb12"), name("Feedback 1-2"), 0, 7, ins.fb12(), String())));
            p.addParameter((part.p_fb34 = new AudioParameterInt(id("fb34"), name("Feedback 3-4"), 0, 7, ins.fb34(), String())));
            p.addParameter((part.p_veloffset = new AudioParameterInt(id("veloffset"), name("Velocity offset"), -127, +127, ins.midi_velocity_offset, String())));
            p.addParameter((part.p_voice2ft = new AudioParameterInt(id("voice2ft"), name("Voice 2 fine tune"), -127, +127, ins.second_voice_detune, String())));
            p.addParameter((part.p_drumnote = new AudioParameterInt(id("drumnote"), name("Percussion note"), 0, 127, ins.percussion_key_number, String())));
        }

        for (unsigned opnum = 0; opnum < 4; ++opnum) {
            String idprefix = fmt::format(
                "P{:d}{:s}", pn + 1,
                ((const char *[]){ "c1", "m1", "c2", "m2" })[opnum]);
            String nameprefix = fmt::format(
                "[Part {:d}] {:s} ", pn + 1,
                ((const char *[]){ "Carrier 1", "Modulator 1", "Carrier 2", "Modulator 2" })[opnum]);

            auto id = [idprefix](const char *x) -> String { return idprefix + String(x); };
            auto name = [nameprefix](const char *x) -> String { return nameprefix + String(x); };

            Operator &op = part.nth_operator(opnum);
            p.addParameter((op.p_attack = new AudioParameterInt(id("attack"), name("Attack"), 0, 15, ins.attack(opnum), String())));
            p.addParameter((op.p_decay = new AudioParameterInt(id("decay"), name("Decay"), 0, 15, ins.decay(opnum), String())));
            p.addParameter((op.p_sustain = new AudioParameterInt(id("sustain"), name("Sustain"), 0, 15, ins.sustain(opnum), String())));
            p.addParameter((op.p_release = new AudioParameterInt(id("release"), name("Release"), 0, 15, ins.release(opnum), String())));
            p.addParameter((op.p_level = new AudioParameterInt(id("level"), name("Level"), 0, 63, ins.level(opnum), String())));
            p.addParameter((op.p_ksl = new AudioParameterInt(id("ksl"), name("Key scale level"), 0, 3, ins.ksl(opnum), String())));
            p.addParameter((op.p_fmul = new AudioParameterInt(id("fmul"), name("Frequency multiplier"), 0, 15, ins.fmul(opnum), String())));
            p.addParameter((op.p_trem = new AudioParameterBool(id("trem"), name("Tremolo"), ins.trem(opnum), String())));
            p.addParameter((op.p_vib = new AudioParameterBool(id("vib"), name("Vibrato"), ins.vib(opnum), String())));
            p.addParameter((op.p_sus = new AudioParameterBool(id("sus"), name("Sustaining"), ins.sus(opnum), String())));
            p.addParameter((op.p_env = new AudioParameterBool(id("env"), name("Key scaling"), ins.env(opnum), String())));
            p.addParameter((op.p_wave = new AudioParameterInt(id("wave"), name("Waveform"), 0, 7, ins.wave(opnum), String())));
        }
    }
    last_instrument_parameter = p.getParameters().size() - 1;

    first_global_parameter = p.getParameters().size();
    p.addParameter((p_volmodel = new AudioParameterChoice("volmodel", "Volume model", {"Generic", "Native", "DMX", "Apogee", "Win9x"}, 0, String())));
    p.addParameter((p_deeptrem = new AudioParameterBool("deeptrem", "Deep tremolo", false, String())));
    p.addParameter((p_deepvib = new AudioParameterBool("deepvib", "Deep vibrato", false, String())));
    last_global_parameter = p.getParameters().size() - 1;
}
