//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bank_manager.h"
#include "plugin_processor.h"
#include "worker.h"
#include "messages.h"
#include "adl/player.h"
#include "adl/wopx_file.h"
#include <algorithm>
#include <cstring>
#include <cassert>

#if 1
#   define trace(fmt, ...)
#else
#   pragma message("enabled debug messages which compromise hard realtime")
#   define trace(fmt, ...) fprintf(stderr, "[Bank Manager] " fmt "\n", ##__VA_ARGS__)
#endif

Bank_Manager::Bank_Manager(AdlplugAudioProcessor &proc, Player &pl, const void *wopl_data, size_t wopl_size)
    : proc_(proc), pl_(pl)
{
    WOPx::BankFile_Ptr wopl;
    if (pl.load_bank_data(wopl_data, wopl_size))
        wopl.reset(WOPx::LoadBankFromMem((void *)wopl_data, wopl_size, nullptr));

    initialize_all_banks();

    unsigned nm = wopl ? wopl->banks_count_melodic : 0;
    unsigned np = wopl ? wopl->banks_count_percussion : 0;

    for (unsigned b_i = 0; b_i < nm + np; ++b_i) {
        bool percussive = b_i >= nm;
        const WOPx::Bank &bank = percussive ?
            wopl->banks_percussive[b_i - nm] : wopl->banks_melodic[b_i];
        Bank_Id id(bank.bank_midi_msb, bank.bank_midi_lsb, percussive);
        rename_bank(id, bank.bank_name, false);
        for (unsigned p_i = 0; p_i < 128; ++p_i) {
            const WOPx::Instrument &ins = bank.ins[p_i];
            if ((ins.inst_flags & WOPx::Ins_IsBlank) == 0)
                rename_program(id, p_i, ins.inst_name, false);
        }
    }
}

void Bank_Manager::clear_banks(bool notify)
{
    Player &pl = pl_;

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
    global_parameters_notify_flag_ = true;
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

    if (global_parameters_notify_flag_) {
        if (!emit_global_parameters())
            return;
        global_parameters_notify_flag_ = false;
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

bool Bank_Manager::load_global_parameters(const Instrument_Global_Parameters &gp, bool notify)
{
    Player &pl = pl_;

    trace("Loading global parameters");

    bool update = false;
    if (pl.volume_model() - 1 != gp.volume_model) {
        pl.set_volume_model(gp.volume_model + 1);
        update = true;
    }
#if defined(ADLPLUG_OPL3)
    if (pl.deep_tremolo() != gp.deep_tremolo) {
        pl.set_deep_tremolo(gp.deep_tremolo);
        update = true;
    }
    if (pl.deep_vibrato() != gp.deep_vibrato) {
        pl.set_deep_vibrato(gp.deep_vibrato);
        update = true;
    }
#elif defined(ADLPLUG_OPN2)
    if (pl.lfo_enabled() != gp.lfo_enable) {
        pl.set_lfo_enabled(gp.lfo_enable);
        update = true;
    }
    if (pl.lfo_frequency() != gp.lfo_frequency) {
        pl.set_lfo_frequency(gp.lfo_frequency);
        update = true;
    }
#endif

    if (notify && update)
        global_parameters_notify_flag_ = true;
    return update;
}

bool Bank_Manager::load_program(const Bank_Id &id, unsigned program, const Instrument &ins, unsigned flags)
{
    Player &pl = pl_;

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
        pl.ensure_get_bank(id, Player::Bank_CreateRt, info.bank);
        info.used.reset();
        info.to_notify.reset();
        info.to_measure.reset();
        std::memset(info.bank_name, 0, 32);
        std::memset(info.ins_names, 0, 128 * 32);
    }

    Bank_Info &info = bank_infos_[index];

    Instrument old_ins;
    pl.ensure_get_instrument(info.bank, program, old_ins);

    bool replace = !(flags & LP_NoReplaceExisting) || old_ins.blank();
    if (!replace)
        return false;

    pl.ensure_set_instrument(info.bank, program, ins);

    // copy name
    if (!(flags & LP_KeepName))
        std::memcpy(&info.ins_names[program * 32], ins.name, 32);

    // update program counts
    unsigned old_count = (unsigned)info.used.count();
    info.used.set(program, !ins.blank());
    if ((flags & LP_Notify) && info.used.count() != old_count)
        slots_notify_flag_ = true;

    // mark as needing measurement if necessary
    info.to_measure.set(program, (flags & LP_NeedMeasurement) && !ins.blank());

    // mark for notification
    if (flags & LP_Notify)
        info.to_notify.set(program);
    return true;
}

bool Bank_Manager::delete_program(const Bank_Id &id, unsigned program, unsigned flags)
{
    Player &pl = pl_;

    trace("Deleting program %c%u:%u:%u",
          id.percussive ? 'P' : 'M', id.msb, id.lsb, program);

    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return false;

    Bank_Info &info = bank_infos_[index];
    if (!info.used.test(program))
        return false;

    Instrument ins;
    pl.ensure_get_instrument(info.bank, program, ins);
    ins.blank(true);
    pl.ensure_set_instrument(info.bank, program, ins);
    info.used.reset(program);

    if (flags & LP_Notify)
        slots_notify_flag_ = true;
    return true;
}

bool Bank_Manager::delete_bank(const Bank_Id &id, unsigned flags)
{
    Player &pl = pl_;

    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return false;

    Bank_Info &info = bank_infos_[index];
    pl.ensure_remove_bank(info.bank);
    info.id = Bank_Id();

    if (flags & LP_Notify)
        slots_notify_flag_ = true;

    return true;
}

bool Bank_Manager::load_measurement(const Bank_Id &id, unsigned program, const Instrument &ins, unsigned kon, unsigned koff, bool notify)
{
    Player &pl = pl_;

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

void Bank_Manager::rename_bank(const Bank_Id &id, const char *name, bool notify)
{
    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return;

    Bank_Info &info = bank_infos_[index];
    unsigned length = (unsigned)strnlen(name, 32);
    if (std::memcmp(info.bank_name, name, std::min(length + 1, 32u)) == 0)
        return;

    std::memset(info.bank_name, 0, 32);
    std::memcpy(info.bank_name, name, length);
    if (notify)
        slots_notify_flag_ = true;
}

void Bank_Manager::rename_program(const Bank_Id &id, unsigned program, const char *name, bool notify)
{
    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return;

    Bank_Info &info = bank_infos_[index];
    char *name_dst = &info.ins_names[program * 32];

    unsigned length = (unsigned)strnlen(name, 32);
    if (std::memcmp(name_dst, name, std::min(length + 1, 32u)) == 0)
        return;

    std::memset(name_dst, 0, 32);
    std::memcpy(name_dst, name, length);
    if (notify)
        info.to_notify.set(program);
}

bool Bank_Manager::find_program(const Bank_Id &id, unsigned program, Instrument &ins)
{
    Player &pl = pl_;

    unsigned index = find_slot(id);
    if (index == (unsigned)-1)
        return false;

    Bank_Info &info = bank_infos_[index];
    pl.ensure_get_instrument(info.bank, program, ins);
    return true;
}

void Bank_Manager::initialize_all_banks()
{
    Player &pl = pl_;

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

        std::memset(info.bank_name, 0, 32);
        std::memset(info.ins_names, 0, 128 * 32);

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
    Simple_Fifo &queue = *proc.message_queue_to_ui();

    Message_Header hdr(Fx_Message::NotifyBankSlots, sizeof(Messages::Fx::NotifyBankSlots));
    Buffered_Message msg = Messages::write(queue, hdr);
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
        std::memcpy(ent.name, info.bank_name, 32);
    }
    data.count = count;
    Messages::finish_write(queue, msg);

    return true;
}

bool Bank_Manager::emit_global_parameters()
{
    Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = *proc.message_queue_to_ui();

    Message_Header hdr(Fx_Message::NotifyGlobalParameters, sizeof(Messages::Fx::NotifyGlobalParameters));
    Buffered_Message msg = Messages::write(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::NotifyGlobalParameters *)msg.data;
    data.param.volume_model = pl.volume_model() - 1;
#if defined(ADLPLUG_OPL3)
    data.param.deep_tremolo = pl.deep_tremolo();
    data.param.deep_vibrato = pl.deep_vibrato();
#elif defined(ADLPLUG_OPN2)
    data.param.lfo_enable = pl.lfo_enabled();
    data.param.lfo_frequency = pl.lfo_frequency();
#endif

    Messages::finish_write(queue, msg);
    return true;
}

bool Bank_Manager::emit_notification(const Bank_Info &info, unsigned program)
{
    Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = *proc.message_queue_to_ui();

    Message_Header hdr(Fx_Message::NotifyInstrument, sizeof(Messages::Fx::NotifyInstrument));
    Buffered_Message msg = Messages::write(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::NotifyInstrument *)msg.data;
    data.bank = info.id;
    data.program = program;
    pl.ensure_get_instrument(info.bank, program, data.instrument);
    std::memcpy(data.instrument.name, &info.ins_names[program * 32], 32);
    Messages::finish_write(queue, msg);

    return true;
}

bool Bank_Manager::emit_measurement_request(const Bank_Info &info, unsigned program)
{
    Player &pl = pl_;
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_worker();

    Message_Header hdr(Fx_Message::RequestMeasurement, sizeof(Messages::Fx::RequestMeasurement));
    Buffered_Message msg = Messages::write(queue, hdr);
    if (!msg)
        return false;

    auto &data = *(Messages::Fx::RequestMeasurement *)msg.data;
    data.bank = info.id;
    data.program = program;
    pl.ensure_get_instrument(info.bank, program, data.instrument);
    Messages::finish_write(queue, msg);

    Worker *worker = proc.worker();
    worker->postSemaphore();

    return true;
}
