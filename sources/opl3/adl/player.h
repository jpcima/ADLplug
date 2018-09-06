//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "instrument.h"
#include <adlmidi.h>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>

class Player {
public:
    enum {
        Bank_Create = ADLMIDI_Bank_Create,
        Bank_CreateRt = ADLMIDI_Bank_CreateRt,
    };

    void init(unsigned sample_rate);
    void close()
        { player_.reset(); }

    static const char *name()
        { return "ADLMIDI"; }
    static double output_gain()
        { return std::pow(10.0, 3.0 / 20.0); }
    static std::vector<std::string> enumerate_emulators();

    void reset()
        { adl_reset(player_.get()); }
    void panic()
        { adl_panic(player_.get()); }
    unsigned reserve_banks(unsigned banks)
        { return adl_reserveBanks(player_.get(), banks); }
    bool load_bank_data(const void *mem, size_t size)
        { return adl_openBankData(player_.get(), mem, size) >= 0; }
    bool get_bank(const Bank_Id &id, int flags, Bank_Ref &bank)
        { return adl_getBank(player_.get(), &id, flags, &bank) >= 0; }
    bool get_first_bank(Bank_Ref &bank)
        { return adl_getFirstBank(player_.get(), &bank) >= 0; }
    bool get_next_bank(Bank_Ref &bank)
        { return adl_getNextBank(player_.get(), &bank) >= 0; }
    bool get_bank_id(const Bank_Ref &bank, Bank_Id &id)
        { return adl_getBankId(player_.get(), &bank, &id) >= 0; }
    bool remove_bank(Bank_Ref &bank)
        { return adl_removeBank(player_.get(), &bank) >= 0; }
    bool get_instrument(const Bank_Ref &bank, unsigned index, Instrument &ins)
        { return adl_getInstrument(player_.get(), &bank, index, &ins) >= 0; }
    bool set_instrument(Bank_Ref &bank, unsigned index, const Instrument &ins)
        { return adl_setInstrument(player_.get(), &bank, index, &ins) >= 0; }
    const char *emulator_name() const
        { return adl_chipEmulatorName(player_.get()); }
    unsigned emulator() const
        { return emu_; }
    void set_emulator(unsigned emu)
        { if (adl_switchEmulator(player_.get(), emu) >= 0) emu_ = emu; }
    unsigned num_chips() const
        { return adl_getNumChips(player_.get()); }
    bool set_num_chips(unsigned chips)
        { return adl_setNumChips(player_.get(), chips) == 0; }
    unsigned num_4ops() const
        { return adl_getNumFourOpsChn(player_.get()); }
    bool set_num_4ops(unsigned count)
        { return adl_setNumFourOpsChn(player_.get(), count) >= 0; }
    unsigned volume_model() const
        { return adl_getVolumeRangeModel(player_.get()); }
    void set_volume_model(int model)
        { adl_setVolumeRangeModel(player_.get(), model); }
    bool deep_tremolo() const
        { return adl_getHTremolo(player_.get()); }
    void set_deep_tremolo(int trem)
        { adl_setHTremolo(player_.get(), trem); }
    bool deep_vibrato() const
        { return adl_getHVibrato(player_.get()); }
    void set_deep_vibrato(int vib)
        { adl_setHVibrato(player_.get(), vib); }
    void set_soft_pan_enabled(bool sp)
        { adl_setSoftPanEnabled(player_.get(), sp); }
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
        void operator()(ADL_MIDIPlayer *p) const { adl_close(p); }
    };
    std::unique_ptr<ADL_MIDIPlayer, Player_Deleter> player_;
};
