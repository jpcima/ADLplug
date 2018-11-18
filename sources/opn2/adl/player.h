//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "instrument.h"
#include <opnmidi.h>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>

class Player {
public:
    enum {
        Bank_Create = OPNMIDI_Bank_Create,
        Bank_CreateRt = OPNMIDI_Bank_CreateRt,
    };

    void init(unsigned sample_rate);
    void close()
        { player_.reset(); }

    static const char *name()
        { return "OPNMIDI"; }
    static double output_gain()
        { return /*1.0*/std::pow(10.0, 3.0 / 20.0); }
    static std::vector<std::string> enumerate_emulators();

    void reset()
        { opn2_reset(player_.get()); }
    void panic()
        { opn2_panic(player_.get()); }
    unsigned reserve_banks(unsigned banks)
        { return opn2_reserveBanks(player_.get(), banks); }
    bool load_bank_data(const void *mem, size_t size)
        { return opn2_openBankData(player_.get(), mem, size) >= 0; }
    bool get_bank(const Bank_Id &id, int flags, Bank_Ref &bank)
        { return opn2_getBank(player_.get(), &id, flags, &bank) >= 0; }
    bool get_first_bank(Bank_Ref &bank)
        { return opn2_getFirstBank(player_.get(), &bank) >= 0; }
    bool get_next_bank(Bank_Ref &bank)
        { return opn2_getNextBank(player_.get(), &bank) >= 0; }
    bool get_bank_id(const Bank_Ref &bank, Bank_Id &id)
        { return opn2_getBankId(player_.get(), &bank, &id) >= 0; }
    bool remove_bank(Bank_Ref &bank)
        { return opn2_removeBank(player_.get(), &bank) >= 0; }
    bool get_instrument(const Bank_Ref &bank, unsigned index, Instrument &ins)
        { return opn2_getInstrument(player_.get(), &bank, index, &ins) >= 0; }
    bool set_instrument(Bank_Ref &bank, unsigned index, const Instrument &ins)
        { return opn2_setInstrument(player_.get(), &bank, index, &ins) >= 0; }
    const char *emulator_name() const
        { return opn2_chipEmulatorName(player_.get()); }
    unsigned emulator() const
        { return emu_; }
    void set_emulator(unsigned emu)
        { if (opn2_switchEmulator(player_.get(), emu) >= 0) emu_ = emu; }
    unsigned num_chips() const
        { return opn2_getNumChipsObtained(player_.get()); }
    bool set_num_chips(unsigned chips)
        { return opn2_setNumChips(player_.get(), chips) == 0; }
    unsigned chip_type() const
        { return opn2_getChipType(player_.get()); }
    void set_chip_type(unsigned type)
        { return opn2_setChipType(player_.get(), type); }
    unsigned volume_model() const
        { return opn2_getVolumeRangeModel(player_.get()); }
    void set_volume_model(unsigned model)
        { opn2_setVolumeRangeModel(player_.get(), model); }
    bool lfo_enabled() const
        { return opn2_getLfoEnabled(player_.get()); }
    void set_lfo_enabled(int enable)
        { opn2_setLfoEnabled(player_.get(), enable); }
    unsigned lfo_frequency() const
        { return opn2_getLfoFrequency(player_.get()); }
    void set_lfo_frequency(int frequency)
        { opn2_setLfoFrequency(player_.get(), frequency); }
    void set_soft_pan_enabled(bool sp)
        { opn2_setSoftPanEnabled(player_.get(), sp); }
    void play_midi(const uint8_t *msg, unsigned len);
    void generate(float *left, float *right, unsigned nframes, unsigned stride);

    void ensure_get_bank_id(const Bank_Ref &bank, Bank_Id &id)
        { bool success = get_bank_id(bank, id); assert(success); (void)success; }
    void ensure_get_bank(const Bank_Id &id, int flags, Bank_Ref &bank)
        { bool success = get_bank(id, flags, bank); assert(success); (void)success; }
    void ensure_remove_bank(Bank_Ref &bank)
        { bool success = remove_bank(bank); assert(success); (void)success; }
    void ensure_get_instrument(const Bank_Ref &bank, unsigned index, Instrument &ins)
        { bool success = get_instrument(bank, index, ins); assert(success); (void)success; }
    void ensure_set_instrument(Bank_Ref &bank, unsigned index, const Instrument &ins)
        { bool success = set_instrument(bank, index, ins); assert(success); (void)success; }

private:
    unsigned emu_ = 0;
    struct Player_Deleter {
        void operator()(OPN2_MIDIPlayer *p) const { opn2_close(p); }
    };
    std::unique_ptr<OPN2_MIDIPlayer, Player_Deleter> player_;
};
