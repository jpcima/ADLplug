//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "definitions.h"
#include "adl/instrument.h"
#include <array>
#include <bitset>
class AdlplugAudioProcessor;

class Bank_Manager
{
public:
    explicit Bank_Manager(AdlplugAudioProcessor &proc, Generic_Player &pl);
    void update_all_banks(bool notify);
    void clear_banks(bool notify);

    void mark_everything_for_notification();
    void mark_slots_for_notification();
    void send_notifications();

    bool load_program(const Bank_Id &id, unsigned program, const Instrument &ins, bool notify);
    bool find_program(const Bank_Id &id, unsigned program, Instrument &ins);

private:
    unsigned find_slot(const Bank_Id &id);
    unsigned ensure_find_slot(const Bank_Id &id);
    unsigned find_empty_slot();

    struct Bank_Info;

    bool emit_slots();
    bool emit_notification(const Bank_Info &info, unsigned program);

    AdlplugAudioProcessor &proc_;
    Generic_Player &pl_;

    struct Bank_Info {
        Bank_Id id;
        Bank_Ref bank;
        unsigned num_programs = 0;
        explicit operator bool() const
            { return bool(id); }
    };
    std::array<Bank_Info, bank_reserve_size> bank_infos_;

    bool slots_notify_flag_ = false;
    std::bitset<128> program_notify_mask_[bank_reserve_size];
};
