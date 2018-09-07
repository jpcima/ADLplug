//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "parameter_block.h"
#include "adl/chip_settings.h"
#include "adl/instrument.h"
#include "adl/player.h"
#include "utility/pak.h"
#include "BinaryData.h"
#include <fmt/format.h>
#include <cassert>

static WOPNFile_Ptr default_wopn()
{
    Pak_File_Reader pak;
    if (!pak.init_with_data((const uint8_t *)BinaryData::banks_pak, BinaryData::banks_pakSize))
        assert(false);
    std::string default_wopl = pak.extract(0);
    assert(default_wopl.size() != 0);

    WOPNFile_Ptr file(WOPN_LoadBankFromMem((void *)default_wopl.data(), default_wopl.size(), nullptr));
    if (!file)
        throw std::bad_alloc();

    return file;
}

static Instrument default_instrument(const WOPNFile &file)
{
    WOPNBank *bank = nullptr;
    for (unsigned i = 0, n = file.banks_count_melodic; i < n && !bank; ++i) {
        WOPNBank *cur = &file.banks_melodic[i];
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
    last_chip_setting = p.getParameters().size() - 1;

    WOPNFile_Ptr wopn = default_wopn();
    Instrument ins = default_instrument(*wopn);

    first_instrument_parameter = p.getParameters().size();
    for (unsigned pn = 0; pn < 16; ++pn) {
        Part &part = this->part[pn];

        {
            String idprefix = fmt::format("P{:d}", pn + 1);
            String nameprefix = fmt::format("[Part {:d}] ", pn + 1);

            auto id = [idprefix](const char *x) -> String { return idprefix + x; };
            auto name = [nameprefix](const char *x) -> String { return nameprefix + x; };

            // p.addParameter((part.p_ps8op = new AudioParameterBool(id("ps8op"), name("Pseudo 8op"), ins.pseudo_eight_op(), String())));
            p.addParameter((part.p_blank = new AudioParameterBool(id("blank"), name("Blank"), ins.blank(), String())));
            p.addParameter((part.p_tune = new AudioParameterInt(id("tune"), name("Note offset"), -127, +127, ins.note_offset, String())));
            // p.addParameter((part.p_tune34 = new AudioParameterInt(id("tune34"), name("Note offset 3-4"), -127, +127, ins.note_offset2, String())));
            p.addParameter((part.p_feedback = new AudioParameterInt(id("feedback"), name("Feedback"), 0, 7, ins.feedback(), String())));
            p.addParameter((part.p_algorithm = new AudioParameterInt(id("algorithm"), name("Algorithm"), 0, 7, ins.algorithm(), String())));
            p.addParameter((part.p_ams = new AudioParameterInt(id("ams"), name("AM sensitivity"), 0, 3, ins.ams(), String())));
            p.addParameter((part.p_fms = new AudioParameterInt(id("fms"), name("FM sensitivity"), 0, 7, ins.fms(), String())));
            p.addParameter((part.p_veloffset = new AudioParameterInt(id("veloffset"), name("Velocity offset"), -127, +127, ins.midi_velocity_offset, String())));
            // p.addParameter((part.p_voice2ft = new AudioParameterInt(id("voice2ft"), name("Voice 2 fine tune"), -127, +127, ins.second_voice_detune, String())));
            p.addParameter((part.p_drumnote = new AudioParameterInt(id("drumnote"), name("Percussion note"), 0, 127, ins.percussion_key_number, String())));
        }

        for (unsigned opnum = 0; opnum < 4; ++opnum) {
            String idprefix = fmt::format(
                "P{:d}{:s}", pn + 1,
                ((const char *[]){ "op1", "op3", "op2", "op4" })[opnum]);
            String nameprefix = fmt::format(
                "[Part {:d}] {:s} ", pn + 1,
                ((const char *[]){ "Operator 1", "Operator 3", "Operator 2", "Operator 4" })[opnum]);

            auto id = [idprefix](const char *x) -> String { return idprefix + String(x); };
            auto name = [nameprefix](const char *x) -> String { return nameprefix + String(x); };

            Operator &op = part.nth_operator(opnum);
            p.addParameter((op.p_detune = new AudioParameterInt(id("detune"), name("Detune"), 0, 7, ins.detune(opnum), String())));
            p.addParameter((op.p_fmul = new AudioParameterInt(id("fmul"), name("Frequency multiplier"), 0, 15, ins.fmul(opnum), String())));
            p.addParameter((op.p_level = new AudioParameterInt(id("level"), name("Level"), 0, 127, ins.level(opnum), String())));
            p.addParameter((op.p_ratescale = new AudioParameterInt(id("ratescale"), name("Rate scaling"), 0, 3, ins.ratescale(opnum), String())));
            p.addParameter((op.p_attack = new AudioParameterInt(id("attack"), name("Attack"), 0, 31, ins.attack(opnum), String())));
            p.addParameter((op.p_am = new AudioParameterBool(id("am"), name("Amplitude modulation"), ins.am(opnum), String())));
            p.addParameter((op.p_decay1 = new AudioParameterInt(id("decay1"), name("Decay 1"), 0, 31, ins.decay1(opnum), String())));
            p.addParameter((op.p_decay2 = new AudioParameterInt(id("decay2"), name("Decay 2"), 0, 31, ins.decay2(opnum), String())));
            p.addParameter((op.p_sustain = new AudioParameterInt(id("sustain"), name("Sustain"), 0, 15, ins.sustain(opnum), String())));
            p.addParameter((op.p_release = new AudioParameterInt(id("release"), name("Release"), 0, 15, ins.release(opnum), String())));
            p.addParameter((op.p_ssgenable = new AudioParameterBool(id("ssgenable"), name("SSG-EG enable"), ins.ssgenable(opnum), String())));
            p.addParameter((op.p_ssgwave = new AudioParameterInt(id("ssgwave"), name("SSG-EG wave"), 0, 7, ins.ssgwave(opnum), String())));
        }
    }
    last_instrument_parameter = p.getParameters().size() - 1;

    first_global_parameter = p.getParameters().size();
    p.addParameter((p_volmodel = new AudioParameterChoice("volmodel", "Volume model", {"Generic"}, wopn->volume_model, String())));
    p.addParameter((p_lfoenable = new AudioParameterBool("lfoenable", "LFO enable", (wopn->lfo_freq & 8) != 0, String())));
    p.addParameter((p_lfofreq = new AudioParameterChoice("lfofreq", "LFO frequency", {"3.98 Hz", "5.56 Hz", "6.02 Hz", "6.37 Hz", "6.88 Hz", "9.63 Hz", "48.1 Hz", "72.2 Hz"}, wopn->lfo_freq & 7, String())));
    last_global_parameter = p.getParameters().size() - 1;
}
