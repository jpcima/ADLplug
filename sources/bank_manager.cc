//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bank_manager.h"
#include "plugin_processor.h"
#include "worker.h"
#include "messages.h"
#include "adl/player.h"
#include <cassert>

#if 1
#   define trace(fmt, ...)
#else
#   pragma message("enabled debug messages which compromise hard realtime")
#   define trace(fmt, ...) fprintf(stderr, "[Bank Manager] " fmt "\n", ##__VA_ARGS__)
#endif

Bank_Manager::Bank_Manager(AdlplugAudioProcessor &proc, Generic_Player &pl)
    : proc_(proc), pl_(pl)
{
    initialize_all_banks();
}

void Bank_Manager::clear_banks(bool notify)
{
    Generic_Player &pl = pl_;

    trace("Clear banks");

    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info)
            continue;
        pl.ensure_remove_bank(info.bank);
        info.id = Bank_Id();
    }

    if (notify)
        slots_notify_flag_ = true;
}

void Bank_Manager::mark_slots_for_notification()
{
    trace("Mark slots for notification");

    slots_notify_flag_ = true;
}

void Bank_Manager::mark_everything_for_notification()
{
    trace("Mark everything for notification");

    slots_notify_flag_ = true;
    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info)
            continue;
        info.to_notify = info.used;
    }
}

void Bank_Manager::send_notifications()
{
    if (slots_notify_flag_) {
        if (!emit_slots())
            return;
        slots_notify_flag_ = false;
    }

    unsigned p_n = 0;
    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info)
            continue;
        counting_bitset<128> &notify_mask = info.to_notify;
        if (notify_mask.count() == 0)
            continue;
        for (unsigned p_i = 0; p_i < 128; ++p_i) {
            if (!notify_mask.test(p_i))
                continue;
            if (!emit_notification(info, p_i))
                return;
            notify_mask.reset(p_i);
            if (++p_n == max_program_notifications)
                return;
        }
    }
}

void Bank_Manager::send_measurement_requests()
{
    unsigned p_n = 0;
    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info)
            continue;
        counting_bitset<128> &measure_mask = info.to_measure;
        if (measure_mask.count() == 0)
            continue;
        const counting_bitset<128> &used_mask = info.used;
        for (unsigned p_i = 0; p_i < 128; ++p_i) {
            if (!measure_mask.test(p_i))
                continue;
            assert(used_mask.test(p_i));
            if (!emit_measurement_request(info, p_i))
                return;
            measure_mask.reset(p_i);
            if (++p_n == max_program_measurements)
                return;
        }
    }
}

bool Bank_Manager::load_program(const Bank_Id &id, unsigned program, const Instrument &ins, bool need_measurement, bool notify)
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
            return false;
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
    unsigned old_count = info.used.count();
    info.used.set(program, !ins.blank());
    if (notify && info.used.count() != old_count)
        slots_notify_flag_ = true;

    // mark as needing measurement if necessary
    info.to_measure.set(program, need_measurement && !ins.blank());

    // mark for notification
    if (notify)
        info.to_notify.set(program);
    return true;
}

bool Bank_Manager::load_measurement(const Bank_Id &id, unsigned program, const Instrument &ins, unsigned kon, unsigned koff, bool notify)
{
    Generic_Player &pl = pl_;

    trace("Loading measurement for program %c%u:%u:%u: %u ms on, %u ms off",
          id.percussive ? 'P' : 'M', id.msb, id.lsb, program,
          kon, koff);

    unsigned index = find_slot(id);
    if (index == (unsigned)-1) {
        trace("The program for received measurement does not exist");
        return false;
    }

    Bank_Info &info = bank_infos_[index];
    Instrument ins2;
    pl.ensure_get_instrument(info.bank, program, ins2);

    if (!ins.equal_instrument_except_delays(ins2)) {
        trace("The program for received measurement does not match");
        return false;
    }

    trace("The program for received measurement matches");

    ins2.delay_on_ms = kon;
    ins2.delay_off_ms = koff;
    pl.ensure_set_instrument(info.bank, program, ins2);

    if (notify)
        info.to_notify.set(program);

    return true;
}

bool Bank_Manager::find_program(const Bank_Id &id, unsigned program, Instrument &ins)
{
    Generic_Player &pl = pl_;

    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return false;

    Bank_Info &info = bank_infos_[index];
    pl.ensure_get_instrument(info.bank, program, ins);
    return true;
}

void Bank_Manager::initialize_all_banks()
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

        Bank_Info &info = bank_infos_[index];
        info.id = id;
        info.bank = bank;

        Instrument ins;
        info.used.reset();
        for (unsigned i = 0; i < 128; ++i) {
            pl.ensure_get_instrument(bank, i, ins);
            info.used.set(i, !ins.blank());
        }

        ++index;
    }

    trace("Clear slots %u-%u", index, bank_reserve_size - 1);
    for (; index < bank_reserve_size; ++index)
        bank_infos_[index].id = Bank_Id();
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
    for (unsigned i = 0; i < bank_reserve_size; ++i) {
        if (!bank_infos_[i].id || bank_infos_[i].used.count() == 0)
            return i;
    }
    return (unsigned)-1;
}

bool Bank_Manager::emit_slots()
{
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_ui();

    Message_Header hdr(Fx_Message::NotifyBankSlots, sizeof(Messages::Fx::NotifyBankSlots));
    Buffered_Message msg = write_message(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::NotifyBankSlots *)msg.data;
    unsigned count = 0;
    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        Bank_Info &info = bank_infos_[b_i];
        if (!info || info.used.count() == 0)
            continue;
        Messages::Fx::NotifyBankSlots::Entry &ent = data.entry[count++];
        ent.bank = info.id;
        ent.used = info.used;
    }
    data.count = count;
    finish_write_message(queue, msg);

    return true;
}

bool Bank_Manager::emit_notification(const Bank_Info &info, unsigned program)
{
    Generic_Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_ui();

    Message_Header hdr(Fx_Message::NotifyInstrument, sizeof(Messages::Fx::NotifyInstrument));
    Buffered_Message msg = write_message(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::NotifyInstrument *)msg.data;
    data.bank = info.id;
    data.program = program;
    pl.ensure_get_instrument(info.bank, program, data.instrument);
    finish_write_message(queue, msg);

    return true;
}

bool Bank_Manager::emit_measurement_request(const Bank_Info &info, unsigned program)
{
    Generic_Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_worker();

    Message_Header hdr(Fx_Message::RequestMeasurement, sizeof(Messages::Fx::RequestMeasurement));
    Buffered_Message msg = write_message(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::RequestMeasurement *)msg.data;
    data.bank = info.id;
    data.program = program;
    pl.ensure_get_instrument(info.bank, program, data.instrument);
    finish_write_message(queue, msg);

    Worker *worker = proc.worker();
    worker->postSemaphore();

    return true;
}
