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

void Parameter_Block::setup_parameters(AudioProcessorEx &p)
{
    Chip_Settings cs = default_chip_settings();

    typedef AudioParameterType Pt;
    typedef NormalisableRange<float> Rf;

    p_mastervol = add_automatable_parameter<Pt::Float>(p, 0, "mastervol", "Master volume", Rf{0.0f, 10.0f}, 1.0f, String());

    StringArray emu_choices = get_emulator_defaults().choices;
    for (unsigned i = 0, n = emu_choices.size(); i < n; ++i) {
        if (emu_choices[i].isEmpty())
            emu_choices.set(i, "<Reserved " + String(i) + ">");
    }
    p_emulator = add_parameter<Pt::Choice>(p, 'chip', "emulator", "Emulator", emu_choices, cs.emulator, String());
    p_nchip = add_parameter<Pt::Int>(p, 'chip', "nchip", "Chip count", 1, 100, cs.chip_count, String());
    StringArray chiptype_choices = {"OPN2 53 kHz", "OPNA 55 kHz"};
    p_chiptype = add_parameter<Pt::Choice>(p, 'chip', "chiptype", "Chip type", chiptype_choices, cs.chip_type, String());

    WOPNFile_Ptr wopn = default_wopn();
    Instrument ins = default_instrument(*wopn);

    for (unsigned pn = 0; pn < 16; ++pn) {
        Part &part = this->part[pn];
        const uint32_t tag = ((uint8_t)'i' << 24) | ((uint8_t)'n' << 16) | ((uint8_t)'s' << 8) | pn;

        {
            String idprefix = fmt::format("P{:d}", pn + 1);
            String nameprefix = fmt::format("[Part {:d}] ", pn + 1);

            auto id = [idprefix](const char *x) -> String { return idprefix + x; };
            auto name = [nameprefix](const char *x) -> String { return nameprefix + x; };

            // part.p_ps8op = add_internal_parameter<Pt::Bool>(p, tag, id("ps8op"), name("Pseudo 8op"), ins.pseudo_eight_op(), String());
            part.p_blank = add_internal_parameter<Pt::Bool>(p, tag, id("blank"), name("Blank"), ins.blank(), String());
            part.p_tune = add_internal_parameter<Pt::Int>(p, tag, id("tune"), name("Note offset"), -127, +127, ins.note_offset, String());
            // part.p_tune34 = add_internal_parameter<Pt::Int>(p, tag, id("tune34"), name("Note offset 3-4"), -127, +127, ins.note_offset2, String());
            part.p_feedback = add_internal_parameter<Pt::Int>(p, tag, id("feedback"), name("Feedback"), 0, 7, ins.feedback(), String());
            part.p_algorithm = add_internal_parameter<Pt::Int>(p, tag, id("algorithm"), name("Algorithm"), 0, 7, ins.algorithm(), String());
            part.p_ams = add_internal_parameter<Pt::Int>(p, tag, id("ams"), name("AM sensitivity"), 0, 3, ins.ams(), String());
            part.p_fms = add_internal_parameter<Pt::Int>(p, tag, id("fms"), name("FM sensitivity"), 0, 7, ins.fms(), String());
            part.p_veloffset = add_internal_parameter<Pt::Int>(p, tag, id("veloffset"), name("Velocity offset"), -127, +127, ins.midi_velocity_offset, String());
            // part.p_voice2ft = add_internal_parameter<Pt::Int>(p, tag, id("voice2ft"), name("Voice 2 fine tune"), -127, +127, ins.second_voice_detune, String());
            part.p_drumnote = add_internal_parameter<Pt::Int>(p, tag, id("drumnote"), name("Percussion note"), 0, 127, ins.percussion_key_number, String());
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
            op.p_detune = add_internal_parameter<Pt::Int>(p, tag, id("detune"), name("Detune"), 0, 7, ins.detune(opnum), String());
            op.p_fmul = add_internal_parameter<Pt::Int>(p, tag, id("fmul"), name("Frequency multiplier"), 0, 15, ins.fmul(opnum), String());
            op.p_level = add_automatable_parameter<Pt::Int>(p, tag, id("level"), name("Level"), 0, 127, ins.level(opnum), String());
            op.p_ratescale = add_internal_parameter<Pt::Int>(p, tag, id("ratescale"), name("Rate scaling"), 0, 3, ins.ratescale(opnum), String());
            op.p_attack = add_internal_parameter<Pt::Int>(p, tag, id("attack"), name("Attack"), 0, 31, ins.attack(opnum), String());
            op.p_am = add_internal_parameter<Pt::Bool>(p, tag, id("am"), name("Amplitude modulation"), ins.am(opnum), String());
            op.p_decay1 = add_internal_parameter<Pt::Int>(p, tag, id("decay1"), name("Decay 1"), 0, 31, ins.decay1(opnum), String());
            op.p_decay2 = add_internal_parameter<Pt::Int>(p, tag, id("decay2"), name("Decay 2"), 0, 31, ins.decay2(opnum), String());
            op.p_sustain = add_internal_parameter<Pt::Int>(p, tag, id("sustain"), name("Sustain"), 0, 15, ins.sustain(opnum), String());
            op.p_release = add_internal_parameter<Pt::Int>(p, tag, id("release"), name("Release"), 0, 15, ins.release(opnum), String());
            op.p_ssgenable = add_internal_parameter<Pt::Bool>(p, tag, id("ssgenable"), name("SSG-EG enable"), ins.ssgenable(opnum), String());
            StringArray ssgwaves;
            ssgwaves.ensureStorageAllocated(8);
            for (unsigned i = 0; i < 8; ++i) {
                String x = (i & 4) ? "Up" : "Down";
                if (i & 2) x += " - Alternate";
                if (i & 1) x += " - Hold";
                ssgwaves.add(x);
            }
            op.p_ssgwave = add_internal_parameter<Pt::Choice>(p, tag, id("ssgwave"), name("SSG-EG wave"), ssgwaves, ins.ssgwave(opnum), String());
        }
    }

    StringArray volmodel_choices = {"Generic"};
    p_volmodel = add_parameter<Pt::Choice>(p, 'glob', "volmodel", "Volume model", volmodel_choices, wopn->volume_model, String());
    p_lfoenable = add_parameter<Pt::Bool>(p, 'glob', "lfoenable", "LFO enable", (wopn->lfo_freq & 8) != 0, String());
    StringArray lfofreq_choices = {"3.98 Hz", "5.56 Hz", "6.02 Hz", "6.37 Hz", "6.88 Hz", "9.63 Hz", "48.1 Hz", "72.2 Hz"};
    p_lfofreq = add_parameter<Pt::Choice>(p, 'glob', "lfofreq", "LFO frequency", lfofreq_choices, wopn->lfo_freq & 7, String());
}
