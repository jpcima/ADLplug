//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bank_manager.h"
#include "plugin_processor.h"
#include "messages.h"
#include "adl/player.h"
#include <cassert>

#if 1
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[Bank Manager] " fmt "\n", ##__VA_ARGS__)
#endif

Bank_Manager::Bank_Manager(AdlplugAudioProcessor &proc, Generic_Player &pl)
    : proc_(proc), pl_(pl)
{
}

void Bank_Manager::update_all_banks()
{
    Generic_Player &pl = pl_;

    trace("Update all banks");

    Bank_Ref bank;
    unsigned index = 0;

    for (bool have = pl.get_first_bank(bank); have; have = pl.get_next_bank(bank)) {
        Bank_Id id;
        pl.ensure_get_bank_id(bank, id);

        trace("Update bank %c%u:%u at slot %u",
              id.percussive ? 'P' : 'M', id.msb, id.lsb, index);

        Instrument ins;
        unsigned num_programs = 0;
        for (unsigned i = 0; i < 128; ++i) {
            pl.ensure_get_instrument(bank, i, ins);
            if (!ins.blank()) {
                ++num_programs;
                program_notify_mask_[index].set(i);
            }
        }

        if (num_programs > 0)
            bank_notify_mask_.set(index);

        Bank_Info &info = bank_infos_[index];
        info.id = id;
        info.bank = bank;
        info.num_programs = num_programs;

        ++index;
    }

    trace("Clear slots %u-%u", index, bank_reserve_size - 1);
    for (; index < bank_reserve_size; ++index)
        bank_infos_[index].id = Bank_Id();
}

void Bank_Manager::send_notifications()
{
    Generic_Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_ui();

    unsigned p_n = 0;
    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info)
            continue;

        for (unsigned p_i = 0; p_i < 128; ++p_i) {
            if (!program_notify_mask_[b_i].test(p_i))
                continue;

            Message_Header hdr(Fx_Message::NotifyInstrument, sizeof(Messages::Fx::NotifyInstrument));
            Buffered_Message msg = write_message(queue, hdr);
            if (!msg)
                return;

            auto &data = *(Messages::Fx::NotifyInstrument *)msg.data;
            data.bank = info.id;
            data.program = p_i;
            pl.ensure_get_instrument(info.bank, p_i, data.instrument);
            finish_write_message(queue, msg);

            program_notify_mask_[b_i].reset(p_i);
            ++p_n;

            if (p_n == max_program_notifications)
                return;
        }
    }
}

void Bank_Manager::load_program(const Bank_Id &id, unsigned program, const Instrument &ins)
{
    Generic_Player &pl = pl_;

    unsigned index = find_slot(id);
    if (index != (unsigned)-1) {
        trace("Loading program %c%u:%u:%u into existing bank at slot %u",
              id.percussive ? 'P' : 'M', id.msb, id.lsb, program, index);
    }
    else {
        // no slots, try to find empty
        index = find_empty_slot();

        if (index == (unsigned)-1) {
            trace("No empty slot to load program %c%u:%u:%u",
                  id.percussive ? 'P' : 'M', id.msb, id.lsb, program);
            return;
        }

        Bank_Info &info = bank_infos_[index];
        if (!info.id) {
            trace("Loading program %c%u:%u:%u at empty slot %u",
                  id.percussive ? 'P' : 'M', id.msb, id.lsb, program, index);
        }
        else {
            // remove the old bank if one was there
            trace("Loading program %c%u:%u:%u over existing blank bank %c%u:%u at slot %u",
                  id.percussive ? 'P' : 'M', id.msb, id.lsb, program,
                  info.id.percussive ? 'P' : 'M', info.id.lsb, info.id.msb, index);
            pl.ensure_remove_bank(info.bank);
        }

        info.id = id;
        pl.ensure_get_bank(id, ADLMIDI_Bank_CreateRt, info.bank);
    }

    Bank_Info &info = bank_infos_[index];

    Instrument old_ins;
    pl.ensure_get_instrument(info.bank, program, old_ins);
    pl.ensure_set_instrument(info.bank, program, ins);

    // update program counts
    if (old_ins.blank() != ins.blank()) {
        unsigned num_programs = info.num_programs;
        num_programs = old_ins.blank() ? (num_programs + 1) : (num_programs - 1);
        info.num_programs = num_programs;
    }
    // mark for notification
    bank_notify_mask_.set(index);
    program_notify_mask_[index].set(program);
}

unsigned Bank_Manager::find_slot(const Bank_Id &id)
{
    for (unsigned i = 0; i < bank_reserve_size; ++i)
        if (bank_infos_[i].id == id)
            return i;
    return (unsigned)-1;
}

inline unsigned Bank_Manager::ensure_find_slot(const Bank_Id &id)
{
    unsigned index = find_slot(id);
    assert(index != (unsigned)-1);
    return index;
}

unsigned Bank_Manager::find_empty_slot()
{
    for (unsigned i = 0; i < bank_reserve_size; ++i)
        if (!bank_infos_[i].id || bank_infos_[i].num_programs == 0)
            return i;
    return (unsigned)-1;
}
