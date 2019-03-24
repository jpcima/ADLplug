//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/player.h"
#include "midi/insnames.h"
#include "utility/midi.h"
#include "utility/simple_fifo.h"
#include "utility/pak.h"
#include "utility/rt_checker.h"
#include "bank_manager.h"
#include "parameter_block.h"
#include "messages.h"
#include "definitions.h"
#include "plugin_processor.h"
#include "plugin_editor.h"
#include "worker.h"
#include "resources.h"
#include <cassert>

#if defined(ADLPLUG_OPL3)
RESOURCE(Res, opl3_banks_pak);
static const Res::Data &banks_pak = Res::opl3_banks_pak;
#elif defined(ADLPLUG_OPN2)
RESOURCE(Res, opn2_banks_pak);
static const Res::Data &banks_pak = Res::opn2_banks_pak;
#endif

//==============================================================================
AdlplugAudioProcessor::AdlplugAudioProcessor()
    : AudioProcessorEx(BusesProperties().withOutput("Output", AudioChannelSet::stereo(), true))
{
    Parameter_Block *pb = new Parameter_Block;
    parameter_block_.reset(pb);
    pb->setup_parameters(*this);

    for (AudioProcessorParameter *p : getParameters())
        p->addListener(this);
}

AdlplugAudioProcessor::~AdlplugAudioProcessor()
{
    if (Worker *worker = worker_.get())
        worker->stop_worker();
}

//==============================================================================
const String AdlplugAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AdlplugAudioProcessor::acceptsMidi() const
{
    return true;
}

bool AdlplugAudioProcessor::producesMidi() const
{
    return false;
}

bool AdlplugAudioProcessor::isMidiEffect() const
{
    return false;
}

double AdlplugAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AdlplugAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0
    // programs, so this should be at least 1, even if you're not
    // really implementing programs.
}

int AdlplugAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AdlplugAudioProcessor::setCurrentProgram(int index)
{
}

const String AdlplugAudioProcessor::getProgramName(int index)
{
    return {};
}

void AdlplugAudioProcessor::changeProgramName(int index, const String &new_name)
{
}

//==============================================================================
void AdlplugAudioProcessor::prepareToPlay(double sample_rate, int block_size)
{
    Simple_Fifo *mq_to_ui = new Simple_Fifo(32 * 1024);
    mq_to_ui_.reset(mq_to_ui);
    mq_from_ui_.reset(new Simple_Fifo(32 * 1024));
    mq_to_worker_.reset(new Simple_Fifo(32 * 1024));
    mq_from_worker_.reset(new Simple_Fifo(32 * 1024));

    Worker *worker = worker_.get();
    if (worker) {
        worker->stop_worker();
        worker_.reset();
    }
    worker = new Worker(*this);
    worker_.reset(worker);
    worker->start_worker();

    Pak_File_Reader pak;
    if (!pak.init_with_data((const uint8_t *)banks_pak.data, (unsigned)banks_pak.size))
        assert(false);
    std::string default_wopl = pak.extract(0);
    assert(default_wopl.size() != 0);

    Player *pl = new Player;
    player_.reset(pl);
    pl->init(sample_rate);
    pl->reserve_banks(bank_reserve_size);
    pl->set_soft_pan_enabled(true);
    pl->set_num_chips(Chip_Settings{}.chip_count);
    pl->set_emulator(get_emulator_defaults().default_index);
    mark_for_notification(Cb_ChipSettings);

    for (unsigned i = 0; i < 2; ++i) {
        Dc_Filter &dcf = dc_filter_[i];
        dcf.cutoff(5.0 / sample_rate);
        Vu_Monitor &vu = vu_monitor_[i];
        vu.release(0.5 * sample_rate);
    }

    midi_channel_mask_.set();

    for (unsigned i = 0; i < 16; ++i) {
        midi_channel_note_count_[i] = 0;
        midi_channel_note_active_[i].reset();
    }

    Bank_Manager *bm = new Bank_Manager(
        *this, *pl, default_wopl.data(), default_wopl.size());
    bank_manager_.reset(bm);
    bm->mark_everything_for_notification();

    for (unsigned p = 0; p < 16; ++p) {
        Selection &sel = selection_[p];
        bool percussive = p == 9;
        sel.bank = Bank_Id(0, 0, percussive);
        sel.program = percussive ? 35 : 0;
        mark_for_notification(Cb_Selection1 + p);
    }

    active_part_ = 0;
    mark_for_notification(Cb_ActivePart);

    std::strncpy(bank_title_, pak.name(0).c_str(), bank_title_size_max);
    mark_for_notification(Cb_BankTitle);

    ready_.store(1);

    setStateInformation(
        last_state_information_.getData(), (unsigned)last_state_information_.getSize());

    set_chip_settings_notifying_host();
    set_global_parameters_notifying_host();
    for (unsigned p = 0; p < 16; ++p)
        set_instrument_parameters_notifying_host(p);

    Message_Header hdr{Fx_Message::NotifyReady, sizeof(Messages::Fx::NotifyReady)};
    Buffered_Message msg = Messages::write(*mq_to_ui, hdr);
    assert(msg);
    Messages::finish_write(*mq_to_ui, msg);
}

void AdlplugAudioProcessor::releaseResources()
{
    if (Worker *worker = worker_.get()) {
        worker->stop_worker();
        worker_.reset();
    }

    getStateInformation(last_state_information_);

    ready_.store(0);

    // avoid destroying the player while the UI is working on it
    std::unique_lock<std::mutex> lock(player_lock_);

    bank_manager_.reset();
    player_.reset();
    mq_from_ui_.reset();
    mq_to_ui_.reset();
    mq_from_worker_.reset();
    mq_to_worker_.reset();
}

std::unique_lock<std::mutex> AdlplugAudioProcessor::acquire_player_nonrt()
{
    return std::unique_lock<std::mutex>(player_lock_);
}

unsigned AdlplugAudioProcessor::num_chips_nonrt() const
{
    Player *pl = player_.get();
    return pl->num_chips();
}

void AdlplugAudioProcessor::set_num_chips_nonrt(unsigned chips)
{
    Player *pl = player_.get();
    pl->set_num_chips(chips);
    reconfigure_chip_nonrt();
}

unsigned AdlplugAudioProcessor::chip_emulator_nonrt() const
{
    Player *pl = player_.get();
    return pl->emulator();
}

void AdlplugAudioProcessor::set_chip_emulator_nonrt(unsigned emu)
{
    Player *pl = player_.get();
    pl->set_emulator(emu);
    reconfigure_chip_nonrt();
}

#if defined(ADLPLUG_OPL3)
unsigned AdlplugAudioProcessor::num_4ops_nonrt() const
{
    Player *pl = player_.get();
    return pl->num_4ops();
}

void AdlplugAudioProcessor::set_num_4ops_nonrt(unsigned count)
{
    Player *pl = player_.get();
    pl->set_num_4ops(count);
}
#endif  // defined(ADLPLUG_OPL3)

#if defined(ADLPLUG_OPN2)
unsigned AdlplugAudioProcessor::chip_type_nonrt() const
{
    Player *pl = player_.get();
    return pl->chip_type();
}

void AdlplugAudioProcessor::set_chip_type_nonrt(unsigned type)
{
    Player *pl = player_.get();
    pl->set_chip_type(type);
}
#endif  // defined(ADLPLUG_OPN2)

void AdlplugAudioProcessor::panic_nonrt()
{
    Player *pl = player_.get();
    pl->panic();
}

void AdlplugAudioProcessor::reconfigure_chip_nonrt()
{
    // Player *pl = player_.get();
    // TODO any necessary reconfiguration after reset
}

bool AdlplugAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    return layouts.getMainOutputChannelSet() == AudioChannelSet::stereo();
}

struct AdlplugAudioProcessor::Message_Handler_Context
{
    bool under_lock = false;
};

void AdlplugAudioProcessor::process(float *outputs[], unsigned nframes, Midi_Input_Source &midi)
{
#ifdef ADLplug_RT_CHECKER
    rt_checker_init();
#endif

    Player *pl = player_.get();
    float *left = outputs[0];
    float *right = outputs[1];

    std::unique_lock<std::mutex> lock(player_lock_, std::try_to_lock);
    process_messages(lock.owns_lock());

    if (!lock.owns_lock()) {
        // can't use the player while non-rt modifies it
        std::fill_n(left, nframes, 0);
        std::fill_n(right, nframes, 0);
        return;
    }

    process_parameter_changes();
    process_notifications();

    const Parameter_Block &pb = *parameter_block_;
    ScopedNoDenormals no_denormals;
    double sample_period = 1.0 / getSampleRate();

    int64_t time_before_generate = Time::getHighResolutionTicks();
    for (unsigned iframe = 0; iframe != nframes;) {
        unsigned segment_nframes = std::min(nframes - iframe, midi_interval_max);
        bool final_segment = iframe + segment_nframes == nframes;

        // handle events from MIDI
        Midi_Input_Message msg;
        while ((msg = midi.peek_next_event()) &&
               (final_segment || msg.time < (int)iframe ||
                (msg.time - (int)iframe) < (int)(midi_interval_max / 2))) {
            handle_midi(msg.data, msg.size);
            midi.get_next_event();
        }

        pl->generate(&left[iframe], &right[iframe], segment_nframes, 1);
        iframe += segment_nframes;
    }
    int64_t time_after_generate = Time::getHighResolutionTicks();
    lock.unlock();

    Dc_Filter &dclf = dc_filter_[0];
    Dc_Filter &dcrf = dc_filter_[1];
    Vu_Monitor &lvu = vu_monitor_[0];
    Vu_Monitor &rvu = vu_monitor_[1];
    double lv_current[2];
    double master_volume = *pb.p_mastervol;
    double output_gain = pl->output_gain() * master_volume;

    for (unsigned i = 0; i < nframes; ++i) {
        double left_sample = left[i] * output_gain;
        double right_sample = right[i] * output_gain;
        // filter out the DC component
        left_sample = dclf.process(left_sample);
        right_sample = dcrf.process(right_sample);
        left[i] = left_sample;
        right[i] = right_sample;
        lv_current[0] = lvu.process(left_sample);
        lv_current[1] = rvu.process(right_sample);
    }

    lv_current_[0] = lv_current[0];
    lv_current_[1] = lv_current[1];

    double generate_duration = Time::highResolutionTicksToSeconds(time_after_generate - time_before_generate);
    double buffer_duration = nframes * sample_period;
    cpu_load_ = generate_duration / buffer_duration;
}

void AdlplugAudioProcessor::process_messages(bool under_lock)
{
    Message_Handler_Context ctx;
    ctx.under_lock = under_lock;

    begin_handling_messages(ctx);

    // handle events from GUI
    Simple_Fifo &mq_from_ui = *mq_from_ui_;
    while (Buffered_Message msg = Messages::read(mq_from_ui)) {
        if (!handle_message(msg, ctx))
            break;
        Messages::finish_read(mq_from_ui, msg);
    }

    // handle events from worker
    Simple_Fifo &mq_from_worker = *mq_from_worker_;
    while (Buffered_Message msg = Messages::read(mq_from_worker)) {
        if (!handle_message(msg, ctx))
            break;
        Messages::finish_read(mq_from_worker, msg);
    }

    finish_handling_messages(ctx);
}

void AdlplugAudioProcessor::process_parameter_changes()
{
    if (unmark_parameter_as_changed(Cb_ChipSettings)) {
        Chip_Settings cs, cs_current;
        parameters_to_chip_settings(cs);
        chip_settings_from_emulator(cs_current);
        if (cs != cs_current) {
            Simple_Fifo &queue = *mq_to_worker_;
            Message_Header hdr(Fx_Message::RequestChipSettings, sizeof(Messages::Fx::RequestChipSettings));
            Buffered_Message msg = Messages::write(queue, hdr);
            if (!msg)
                mark_parameter_as_changed(Cb_ChipSettings);  // do later
            else {
                auto &body = *(Messages::Fx::RequestChipSettings *)msg.data;
                parameters_to_chip_settings(body.cs);
                Messages::finish_write(queue, msg);
                Worker &worker = *worker_;
                worker.postSemaphore();
            }
        }
    }

    for (unsigned p = 0; p < 16; ++p) {
        if (unmark_parameter_as_changed(Cb_Instrument1 + p)) {
            Bank_Manager &bm = *bank_manager_;
            Instrument ins;
            parameters_to_instrument(p, ins);
            const Selection &sel = selection_[p];
            bm.load_program(
                sel.bank, sel.program, ins,
                Bank_Manager::LP_Notify|Bank_Manager::LP_NeedMeasurement|Bank_Manager::LP_KeepName);
        }
    }

    if (unmark_parameter_as_changed(Cb_GlobalParameters)) {
        Bank_Manager &bm = *bank_manager_;
        Instrument_Global_Parameters gp;
        parameters_to_global(gp);
        bm.load_global_parameters(gp, true);
    }
}

void AdlplugAudioProcessor::process_notifications()
{
    Player *pl = player_.get();
    Simple_Fifo &queue = *mq_to_ui_;

    if (unmark_for_notification(Cb_ChipSettings)) {
        Message_Header hdr(Fx_Message::NotifyChipSettings, sizeof(Messages::Fx::NotifyChipSettings));
        Buffered_Message msg = Messages::write(queue, hdr);
        if (!msg)
            mark_for_notification(Cb_ChipSettings);  // do later
        else {
            auto &body = *(Messages::Fx::NotifyChipSettings *)msg.data;
            body.cs.emulator = pl->emulator();
            body.cs.chip_count = pl->num_chips();
#if defined(ADLPLUG_OPL3)
            body.cs.fourop_count = pl->num_4ops();
#elif defined(ADLPLUG_OPN2)
            body.cs.chip_type = pl->chip_type();
#endif
            Messages::finish_write(queue, msg);
        }
    }

    if (unmark_for_notification(Cb_ActivePart)) {
        Message_Header hdr(Fx_Message::NotifyActivePart, sizeof(Messages::Fx::NotifyActivePart));
        Buffered_Message msg = Messages::write(queue, hdr);
        if (!msg)
            mark_for_notification(Cb_ActivePart);  // do later
        else {
            auto &body = *(Messages::Fx::NotifyActivePart *)msg.data;
            body.part = active_part_;
            Messages::finish_write(queue, msg);
        }
    }

    if (unmark_for_notification(Cb_BankTitle)) {
        Message_Header hdr(Fx_Message::NotifyBankTitle, sizeof(Messages::Fx::NotifyBankTitle));
        Buffered_Message msg = Messages::write(queue, hdr);
        if (!msg)
            mark_for_notification(Cb_BankTitle);  // do later
        else {
            auto &body = *(Messages::Fx::NotifyBankTitle *)msg.data;
            std::memcpy(body.title, bank_title_, bank_title_size_max);
            Messages::finish_write(queue, msg);
        }
    }

    for (unsigned part = 0; part < 16; ++part) {
        if (unmark_for_notification(Cb_Selection1 + part)) {
            Message_Header hdr(Fx_Message::NotifySelection, sizeof(Messages::Fx::NotifySelection));
            Buffered_Message msg = Messages::write(queue, hdr);
            if (!msg)
                mark_for_notification(Cb_Selection1 + part);  // do later
            else {
                auto &body = *(Messages::Fx::NotifySelection *)msg.data;
                body.part = part;
                body.bank = selection_[part].bank;
                body.program = selection_[part].program;
                Messages::finish_write(queue, msg);
            }
        }
    }
}

bool AdlplugAudioProcessor::handle_midi(const uint8_t *data, unsigned len)
{
    Player *pl = player_.get();
    pl->play_midi(data, len);

    unsigned status = (len > 0) ? data[0] : 0;
    unsigned channel = status & 0x0f;

    if ((status & 0xf0) != 0xf0 && !midi_channel_mask_[channel])
        return true;

    switch (status & 0xf0) {
    case 0x90:
        if (len < 3) break;
        if (data[2] > 0) {
            if (!midi_channel_note_active_[channel][data[1]]) {
                ++midi_channel_note_count_[channel];
                midi_channel_note_active_[channel][data[1]] = true;
            }
            break;
        }
    case 0x80:
        if (len < 3) break;
        if (midi_channel_note_active_[channel][data[1]]) {
            --midi_channel_note_count_[channel];
            midi_channel_note_active_[channel][data[1]] = false;
        }
        break;
    case 0xb0:
        if (len < 3) break;
        switch (data[1]) {
        case 0:
            midi_bank_msb_[channel] = data[2];
            break;
        case 32:
            midi_bank_lsb_[channel] = data[2];
            break;
        case 120: case 123:
            midi_channel_note_count_[channel] = 0;
            midi_channel_note_active_[channel].reset();
            break;
        }
        break;
    case 0xc0: {
        if (len < 2) break;
        bool is_drum = channel == 9;
        if (!is_drum) {
            selection_[channel].program = data[1];
            selection_[channel].bank.percussive = false;
            selection_[channel].bank.msb = midi_bank_msb_[channel];
            selection_[channel].bank.lsb = midi_bank_lsb_[channel];
        }
        else {
            //--- TODO percussion banks/XG banks?
            // selection_[channel].bank.percussive = true;
            // selection_[channel].bank.msb = 0;
            // selection_[channel].bank.lsb = data[1];
        }
        mark_for_notification(Cb_Selection1 + channel);
        set_instrument_parameters_notifying_host(channel);
        break;
    }
    }

    return true;
}

bool AdlplugAudioProcessor::handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx)
{
    const uint8_t *data = msg.data;
    unsigned tag = msg.header->tag;
    unsigned size = msg.header->size;

    if (tag == (unsigned)User_Message::Midi)
        return handle_midi(data, size);

    if (!ctx.under_lock)
        return false;

    Bank_Manager &bm = *bank_manager_;

    switch (tag) {
    case (unsigned)User_Message::RequestBankSlots:
        bm.mark_slots_for_notification();
        break;
    case (unsigned)User_Message::RequestFullBankState:
        bm.mark_everything_for_notification();
        break;
    case (unsigned)User_Message::RequestChipSettings:
        mark_for_notification(Cb_ChipSettings);
        break;
    case (unsigned)User_Message::RequestSelections: {
        auto &body = *(const Messages::User::RequestSelections *)data;
        for (unsigned p = 0; p < 16; ++p) {
            if (body.channel_mask.test(p))
                mark_for_notification(Cb_Selection1 + p);
        }
        break;
    }
    case (unsigned)User_Message::RequestActivePart:
        mark_for_notification(Cb_ActivePart);
        break;
    case (unsigned)User_Message::RequestBankTitle:
        mark_for_notification(Cb_BankTitle);
        break;
    case (unsigned)User_Message::ClearBanks: {
        auto &body = *(const Messages::User::ClearBanks *)data;
        bm.clear_banks(body.notify_back);
        break;
    }
    case (unsigned)User_Message::LoadGlobalParameters: {
        auto &body = *(const Messages::User::LoadGlobalParameters *)data;
        if (bm.load_global_parameters(body.param, body.notify_back))
            set_global_parameters_notifying_host();
        break;
    }
    case (unsigned)User_Message::LoadInstrument: {
        auto &body = *(const Messages::User::LoadInstrument *)data;
        unsigned flags =
            (body.need_measurement ? Bank_Manager::LP_NeedMeasurement : 0) |
            (body.notify_back ? Bank_Manager::LP_Notify : 0);
        if (bm.load_program(body.bank, body.program, body.instrument, flags)) {
            const Selection &sel = selection_[body.part];
            if (body.bank == sel.bank && body.program == sel.program)
                set_instrument_parameters_notifying_host(body.part);
        }
        break;
    }
    case (unsigned)User_Message::CreateInstrument: {
        auto &body = *(const Messages::User::CreateInstrument *)data;
        unsigned flags = Bank_Manager::LP_NoReplaceExisting |
            (body.notify_back ? Bank_Manager::LP_Notify : 0);
        Instrument ins;
        ins.blank(false);
        bm.load_program(body.bank, body.program, ins, flags);
        break;
    }
    case (unsigned)User_Message::DeleteInstrument: {
        auto &body = *(const Messages::User::DeleteInstrument *)data;
        unsigned flags =
            (body.notify_back ? Bank_Manager::LP_Notify : 0);
        bm.delete_program(body.bank, body.program, flags);
        break;
    }
    case (unsigned)User_Message::DeleteBank: {
        auto &body = *(const Messages::User::DeleteBank *)data;
        unsigned flags =
            (body.notify_back ? Bank_Manager::LP_Notify : 0);
        bm.delete_bank(body.bank, flags);
        break;
    }
    case (unsigned)User_Message::RenameBank: {
        auto &body = *(const Messages::User::RenameBank *)data;
        bm.rename_bank(body.bank, body.name, body.notify_back);
        break;
    }
    case (unsigned)User_Message::RenameProgram: {
        auto &body = *(const Messages::User::RenameProgram *)data;
        bm.rename_program(body.bank, body.program, body.name, body.notify_back);
        break;
    }
    case (unsigned)User_Message::SelectProgram: {
        auto &body = *(const Messages::User::SelectProgram *)data;
        Selection &sel = selection_[body.part];
        if (sel.bank != body.bank || sel.program != body.program) {
            sel.bank = body.bank;
            sel.program = body.program;
            send_program_change_from_selection(body.part);
            set_instrument_parameters_notifying_host(body.part);
        }
        break;
    }
    case (unsigned)User_Message::SetActivePart: {
        auto &body = *(const Messages::User::SetActivePart *)data;
        if (active_part_ == body.part)
            break;
        active_part_ = body.part;
        mark_for_notification(Cb_ActivePart);
        break;
    }
    case (unsigned)User_Message::SetBankTitle: {
        auto &body = *(const Messages::User::SetBankTitle *)data;
        memcpy(bank_title_, body.title, bank_title_size_max);
        break;
    }
#if defined(ADLPLUG_OPL3)
    case (unsigned)User_Message::SelectOptimal4Ops: {
        Player &pl = *player_;
        Parameter_Block &pb = *parameter_block_;
        pl.panic();
        pl.set_num_4ops(~0u);
        *pb.p_n4op = pl.num_4ops();
        mark_for_notification(Cb_ChipSettings);
        break;
    }
#endif
    case (unsigned)Worker_Message::MeasurementResult: {
        auto &body = *(const Messages::Worker::MeasurementResult *)data;
        bm.load_measurement(body.bank, body.program, body.instrument, body.ms_sound_kon, body.ms_sound_koff, true);
        break;
    }
    default:
        assert(false);
    }

    return true;
}

void AdlplugAudioProcessor::finish_handling_messages(Message_Handler_Context &ctx)
{
    bank_manager_->send_notifications();
    bank_manager_->send_measurement_requests();
}

void AdlplugAudioProcessor::parameters_to_chip_settings(Chip_Settings &cs) const
{
    const Parameter_Block &pb = *parameter_block_;

    cs.emulator = pb.p_emulator->getIndex();
    cs.chip_count = pb.p_nchip->get();
#if defined(ADLPLUG_OPL3)
    cs.fourop_count = pb.p_n4op->get();
#elif defined(ADLPLUG_OPN2)
    cs.chip_type = pb.p_chiptype->getIndex();
#endif
}

void AdlplugAudioProcessor::parameters_to_global(Instrument_Global_Parameters &gp) const
{
    const Parameter_Block &pb = *parameter_block_;

    gp.volume_model = pb.p_volmodel->getIndex();
#if defined(ADLPLUG_OPL3)
    gp.deep_tremolo = pb.p_deeptrem->get();
    gp.deep_vibrato = pb.p_deepvib->get();
#elif defined(ADLPLUG_OPN2)
    gp.lfo_enable = pb.p_lfoenable->get();
    gp.lfo_frequency = pb.p_lfofreq->getIndex();
#endif
}

void AdlplugAudioProcessor::parameters_to_instrument(unsigned part_number, Instrument &ins) const
{
    const Parameter_Block &pb = *parameter_block_;
    const Parameter_Block::Part &part = pb.part[part_number];

    ins.version = Instrument::latest_version;
    ins.inst_flags = 0;

#if defined(ADLPLUG_OPL3)
    ins.four_op(part.p_is4op->get());
    ins.pseudo_four_op(part.p_ps4op->get());
    ins.blank(part.p_blank->get());
    ins.con12(part.p_con12->getIndex());
    ins.con34(part.p_con34->getIndex());
    ins.note_offset1 = part.p_tune12->get();
    ins.note_offset2 = part.p_tune34->get();
    ins.fb12(part.p_fb12->get());
    ins.fb34(part.p_fb34->get());
    ins.midi_velocity_offset = part.p_veloffset->get();
    ins.second_voice_detune = part.p_voice2ft->get();
    ins.percussion_key_number = part.p_drumnote->get();
#elif defined(ADLPLUG_OPN2)
    // ins.pseudo_eight_op(part.p_ps8op->get());
    ins.blank(part.p_blank->get());
    ins.note_offset = part.p_tune->get();
    // ins.note_offset2 = part.p_tune34->get();
    ins.feedback(part.p_feedback->get());
    ins.algorithm(part.p_algorithm->get());
    ins.ams(part.p_ams->get());
    ins.fms(part.p_fms->get());
    ins.midi_velocity_offset = part.p_veloffset->get();
    // ins.second_voice_detune = part.p_voice2ft->get();
    ins.percussion_key_number = part.p_drumnote->get();
#endif

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const Parameter_Block::Operator &op = part.nth_operator(opnum);
#if defined(ADLPLUG_OPL3)
        ins.attack(opnum, op.p_attack->get());
        ins.decay(opnum, op.p_decay->get());
        ins.sustain(opnum, op.p_sustain->get());
        ins.release(opnum, op.p_release->get());
        ins.level(opnum, op.p_level->get());
        ins.ksl(opnum, op.p_ksl->get());
        ins.fmul(opnum, op.p_fmul->get());
        ins.trem(opnum, op.p_trem->get());
        ins.vib(opnum, op.p_vib->get());
        ins.sus(opnum, op.p_sus->get());
        ins.env(opnum, op.p_env->get());
        ins.wave(opnum, op.p_wave->getIndex());
#elif defined(ADLPLUG_OPN2)
        ins.detune(opnum, op.p_detune->get());
        ins.fmul(opnum, op.p_fmul->get());
        ins.level(opnum, op.p_level->get());
        ins.ratescale(opnum, op.p_ratescale->get());
        ins.attack(opnum, op.p_attack->get());
        ins.am(opnum, op.p_am->get());
        ins.decay1(opnum, op.p_decay1->get());
        ins.decay2(opnum, op.p_decay2->get());
        ins.sustain(opnum, op.p_sustain->get());
        ins.release(opnum, op.p_release->get());
        ins.ssgenable(opnum, op.p_ssgenable->get());
        ins.ssgwave(opnum, op.p_ssgwave->getIndex());
#endif
    }
}

void AdlplugAudioProcessor::set_chip_settings_notifying_host()
{
    Player *pl = player_.get();
    Parameter_Block &pb = *parameter_block_;

    *pb.p_emulator = pl->emulator();
    *pb.p_nchip = pl->num_chips();
#if defined(ADLPLUG_OPL3)
    *pb.p_n4op = pl->num_4ops();
#elif defined(ADLPLUG_OPN2)
    *pb.p_chiptype = pl->chip_type();
#endif
}

void AdlplugAudioProcessor::set_global_parameters_notifying_host()
{
    Player *pl = player_.get();
    Parameter_Block &pb = *parameter_block_;

    *pb.p_volmodel = pl->volume_model() - 1;
#if defined(ADLPLUG_OPL3)
    *pb.p_deeptrem = pl->deep_tremolo();
    *pb.p_deepvib = pl->deep_vibrato();
#elif defined(ADLPLUG_OPN2)
    *pb.p_lfoenable = pl->lfo_enabled();
    *pb.p_lfofreq = pl->lfo_frequency();
#endif
}

void AdlplugAudioProcessor::set_instrument_parameters_notifying_host(unsigned part_number)
{
    Instrument ins;
    Bank_Manager &bm = *bank_manager_;
    const Selection &sel = selection_[part_number];

    if (!bm.find_program(sel.bank, sel.program, ins))
        return;

    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[part_number];

#if defined(ADLPLUG_OPL3)
    *part.p_is4op = ins.four_op();
    *part.p_ps4op = ins.pseudo_four_op();
    *part.p_blank = ins.blank();
    *part.p_con12 = ins.con12();
    *part.p_con34 = ins.con34();
    *part.p_tune12 = ins.note_offset1;
    *part.p_tune34 = ins.note_offset2;
    *part.p_fb12 = ins.fb12();
    *part.p_fb34 = ins.fb34();
    *part.p_veloffset = ins.midi_velocity_offset;
    *part.p_voice2ft = ins.second_voice_detune;
    *part.p_drumnote = ins.percussion_key_number;
#elif defined(ADLPLUG_OPN2)
    // *part.p_ps8op = ins.pseudo_eight_op();
    *part.p_blank = ins.blank();
    *part.p_tune = ins.note_offset;
    // *part.p_tune34 = ins.note_offset2;
    *part.p_feedback = ins.feedback();
    *part.p_algorithm = ins.algorithm();
    *part.p_ams = ins.ams();
    *part.p_fms = ins.fms();
    *part.p_veloffset = ins.midi_velocity_offset;
    // *part.p_voice2ft = ins.second_voice_detune;
    *part.p_drumnote = ins.percussion_key_number;
#endif

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        Parameter_Block::Operator &op = part.nth_operator(opnum);
#if defined(ADLPLUG_OPL3)
        *op.p_attack = ins.attack(opnum);
        *op.p_decay = ins.decay(opnum);
        *op.p_sustain = ins.sustain(opnum);
        *op.p_release = ins.release(opnum);
        *op.p_level = ins.level(opnum);
        *op.p_ksl = ins.ksl(opnum);
        *op.p_fmul = ins.fmul(opnum);
        *op.p_trem = ins.trem(opnum);
        *op.p_vib = ins.vib(opnum);
        *op.p_sus = ins.sus(opnum);
        *op.p_env = ins.env(opnum);
        *op.p_wave = ins.wave(opnum);
#elif defined(ADLPLUG_OPN2)
        *op.p_detune = ins.detune(opnum);
        *op.p_fmul = ins.fmul(opnum);
        *op.p_level = ins.level(opnum);
        *op.p_ratescale = ins.ratescale(opnum);
        *op.p_attack = ins.attack(opnum);
        *op.p_am = ins.am(opnum);
        *op.p_decay1 = ins.decay1(opnum);
        *op.p_decay2 = ins.decay2(opnum);
        *op.p_sustain = ins.sustain(opnum);
        *op.p_release = ins.release(opnum);
        *op.p_ssgenable = ins.ssgenable(opnum);
        *op.p_ssgwave = ins.ssgwave(opnum);
#endif
    }
}

void AdlplugAudioProcessor::chip_settings_from_emulator(Chip_Settings &cs) const
{
    const Player &pl = *player_;

    cs.emulator = pl.emulator();
    cs.chip_count = pl.num_chips();
#if defined(ADLPLUG_OPL3)
    cs.fourop_count = pl.num_4ops();
#elif defined(ADLPLUG_OPN2)
    cs.chip_type = pl.chip_type();
#endif
}

void AdlplugAudioProcessor::send_program_change_from_selection(unsigned part)
{
    bool is_drum = part == 9;
    Selection sel = selection_[part];

    if (is_drum != sel.bank.percussive)
        return;

    Player &pl = *player_;
    uint8_t msg[3];
    if (!is_drum) {
        // melodic bank change
        msg[0] = 0xb0 | part; msg[1] = 0; msg[2] = sel.bank.msb;
        pl.play_midi(msg, 3);
        msg[0] = 0xb0 | part; msg[1] = 32; msg[2] = sel.bank.lsb;
        pl.play_midi(msg, 3);
        // melodic program change
        msg[0] = 0xc0 | part; msg[1] = sel.program;
        pl.play_midi(msg, 2);
    }
    else {
        // percussion bank change LSB only
        msg[0] = 0xc0 | part; msg[1] = sel.bank.lsb;
        pl.play_midi(msg, 2);
    }
}

void AdlplugAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                         MidiBuffer &midi_messages)
{
    unsigned nframes = buffer.getNumSamples();
    float *outputs[2] = {buffer.getWritePointer(0), buffer.getWritePointer(1)};

    MidiBuffer::Iterator midi_iterator(midi_messages);
    Midi_Input_Source midi_source(midi_iterator);

    process(outputs, nframes, midi_source);
}

void AdlplugAudioProcessor::processBlockBypassed(AudioBuffer<float> &buffer, MidiBuffer &midi_messages)
{
    MidiBuffer::Iterator midi_iterator(midi_messages);
    Midi_Input_Source midi_source(midi_iterator);

    std::unique_lock<std::mutex> lock(player_lock_, std::try_to_lock);
    process_messages(lock.owns_lock());
    lock.unlock();

    cpu_load_ = 0;
    AudioProcessor::processBlockBypassed(buffer, midi_messages);
}

//==============================================================================
bool AdlplugAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor *AdlplugAudioProcessor::createEditor()
{
    return new AdlplugAudioProcessorEditor(*this, *parameter_block_);
}

//==============================================================================
void AdlplugAudioProcessor::getStateInformation(MemoryBlock &data)
{
    std::lock_guard<std::mutex> lock(player_lock_);

    Player *pl = player_.get();
    if (!pl)
        return;

    const Parameter_Block &pb = *parameter_block_;
    const Bank_Manager &bm = *bank_manager_;
    const Bank_Manager::Bank_Info *infos = bm.bank_infos();

    XmlElement root("ADLMIDI-state");

    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        const Bank_Manager::Bank_Info &info = infos[b_i];
        if (!info)
            continue;
        PropertySet bank_set;
        char name[33];
        name[32] = '\0';
        memcpy(name, info.bank_name, 32);
        bank_set.setValue("bank", (int)info.id.to_integer());
        bank_set.setValue("name", name);
        std::unique_ptr<XmlElement> elt(bank_set.createXml("bank"));
        root.addChildElement(elt.get());
        elt.release();
    }

    for (unsigned b_i = 0; b_i < bank_reserve_size; ++b_i) {
        const Bank_Manager::Bank_Info &info = infos[b_i];
        if (!info)
            continue;
        Instrument ins;
        for (unsigned p_i = 0; p_i < 128; ++p_i) {
            if (!info.used.test(p_i))
                continue;
            pl->ensure_get_instrument(info.bank, p_i, ins);
            PropertySet ins_set;
            ins_set.setValue("bank", (int)info.id.to_integer());
            ins_set.setValue("program", (int)p_i);
            char name[33];
            name[32] = '\0';
            memcpy(name, info.ins_names + 32 * p_i, 32);
            ins_set.setValue("name", name);
            ins.to_properties(ins_set, "");
            std::unique_ptr<XmlElement> elt(ins_set.createXml("instrument"));
            root.addChildElement(elt.get());
            elt.release();
        }
    }

    for (unsigned p = 0; p < 16; ++p) {
        const Selection &sel = selection_[p];
        PropertySet sel_set;
        sel_set.setValue("part", (int)p);
        sel_set.setValue("bank", (int)sel.bank.to_integer());
        sel_set.setValue("program", (int)sel.program);
        std::unique_ptr<XmlElement> elt(sel_set.createXml("selection"));
        root.addChildElement(elt.get());
        elt.release();
    }

    // chip settings
    {
        PropertySet chip_set;
        chip_set.setValue("emulator", (int)pl->emulator());
        chip_set.setValue("chip_count", (int)pl->num_chips());
#if defined(ADLPLUG_OPL3)
        chip_set.setValue("4op_count", (int)pl->num_4ops());
#elif defined(ADLPLUG_OPN2)
        chip_set.setValue("chip_type", (int)pl->chip_type());
#endif
        std::unique_ptr<XmlElement> elt(chip_set.createXml("chip"));
        root.addChildElement(elt.get());
        elt.release();
    }

    // global parameters
    {
        PropertySet global_set;
        global_set.setValue("volume_model", (int)pl->volume_model() - 1);
#if defined(ADLPLUG_OPL3)
        global_set.setValue("deep_tremolo", pl->deep_tremolo());
        global_set.setValue("deep_vibrato", pl->deep_vibrato());
#elif defined(ADLPLUG_OPN2)
        global_set.setValue("lfo_enable", pl->lfo_enabled());
        global_set.setValue("lfo_frequency", (int)pl->lfo_frequency());
#endif
        std::unique_ptr<XmlElement> elt(global_set.createXml("global"));
        root.addChildElement(elt.get());
        elt.release();
    }

    // common parameters
    {
        PropertySet common_set;
        common_set.setValue("bank_title", String(CharPointer_UTF8(bank_title_)));
        common_set.setValue("part", (int)active_part_);
        common_set.setValue("master_volume", (double)*pb.p_mastervol);
        std::unique_ptr<XmlElement> elt(common_set.createXml("common"));
        root.addChildElement(elt.get());
        elt.release();
    }

    copyXmlToBinary(root, data);
}

void AdlplugAudioProcessor::setStateInformation(const void *data, int size)
{
    std::lock_guard<std::mutex> lock(player_lock_);
    Player &pl = *player_;
    Parameter_Block &pb = *parameter_block_;
    Bank_Manager &bm = *bank_manager_;

    last_state_information_.replaceWith(data, size);

    if (!is_playback_ready())
        return;  // not ready yet, will load state information later

    std::unique_ptr<XmlElement> root(
        getXmlFromBinary(data, size));
    if (!root)
        return;

    if (root->getTagName() != "ADLMIDI-state")
        return;

    bm.clear_banks(false);

    for (XmlElement *elt = root->getFirstChildElement(); elt; elt = elt->getNextElement()) {
        if (elt->getTagName() != "instrument")
            continue;
        PropertySet ins_set;
        ins_set.restoreFromXml(*elt);
        Bank_Id bank = Bank_Id::from_integer(ins_set.getIntValue("bank"));
        unsigned program = ins_set.getIntValue("program");
        if (bank.lsb > 127 || bank.msb > 127 || program > 127)
            continue;
        Instrument ins = Instrument::from_properties(ins_set, "");
        String name = ins_set.getValue("name");
        const char *name_utf8  = name.toRawUTF8();
        memset(ins.name, 0, 32);
        memcpy(ins.name, name_utf8, strnlen(name_utf8, 32));
        bm.load_program(bank, program, ins, 0);
    }

    for (XmlElement *elt = root->getFirstChildElement(); elt; elt = elt->getNextElement()) {
        if (elt->getTagName() != "bank")
            continue;
        PropertySet bank_set;
        bank_set.restoreFromXml(*elt);
        String name = bank_set.getValue("name");
        Bank_Id bank = Bank_Id::from_integer(bank_set.getIntValue("bank"));
        if (bank.lsb > 127 || bank.msb > 127)
            continue;
        bm.rename_bank(bank, name.toRawUTF8(), false);
    }

    for (XmlElement *elt = root->getFirstChildElement(); elt; elt = elt->getNextElement()) {
        if (elt->getTagName() != "selection")
            continue;
        PropertySet sel_set;
        sel_set.restoreFromXml(*elt);
        unsigned part = sel_set.getIntValue("part");
        Bank_Id bank = Bank_Id::from_integer(sel_set.getIntValue("bank"));
        unsigned program = sel_set.getIntValue("program");
        if (part > 15 || bank.lsb > 127 || bank.msb > 127 || program > 127)
            continue;
        Selection &sel = selection_[part];
        sel.bank = bank;
        sel.program = program;
    }

    // chip settings
    PropertySet chip_set;
    if (XmlElement *elt = root->getChildByName("chip"))
        chip_set.restoreFromXml(*elt);
    pl.set_emulator(chip_set.getIntValue("emulator"));
    pl.set_num_chips(chip_set.getIntValue("chip_count"));
#if defined(ADLPLUG_OPL3)
    pl.set_num_4ops(chip_set.getIntValue("4op_count"));
#elif defined(ADLPLUG_OPN2)
    pl.set_chip_type(chip_set.getIntValue("chip_type"));
#endif

    // global parameters
    PropertySet global_set;
    if (XmlElement *elt = root->getChildByName("global"))
        global_set.restoreFromXml(*elt);
    Instrument_Global_Parameters gp;
    gp.volume_model = global_set.getIntValue("volume_model");
#if defined(ADLPLUG_OPL3)
    gp.deep_tremolo = global_set.getBoolValue("deep_tremolo");
    gp.deep_vibrato = global_set.getBoolValue("deep_vibrato");
#elif defined(ADLPLUG_OPN2)
    gp.lfo_enable = global_set.getBoolValue("lfo_enable");
    gp.lfo_frequency = global_set.getIntValue("lfo_frequency");
#endif
    bm.load_global_parameters(gp, false);

    // common parameters
    PropertySet common_set;
    if (XmlElement *elt = root->getChildByName("common"))
        common_set.restoreFromXml(*elt);
    common_set.getValue("bank_title").copyToUTF8(bank_title_, bank_title_size_max + 1);
    active_part_ = jlimit(0, 15, common_set.getIntValue("part"));

    // notify everything
    mark_for_notification(Cb_ChipSettings);
    bm.mark_everything_for_notification();
    for (unsigned p = 0; p < 16; ++p)
        mark_for_notification(Cb_Selection1 + p);
    mark_for_notification(Cb_ActivePart);
    mark_for_notification(Cb_BankTitle);

    // send program changes
    for (unsigned p = 0; p < 16; ++p)
        send_program_change_from_selection(p);

    // make the host aware of changed parameters
    set_chip_settings_notifying_host();
    set_global_parameters_notifying_host();
    for (unsigned p = 0; p < 16; ++p)
        set_instrument_parameters_notifying_host(p);
    *pb.p_mastervol = common_set.getDoubleValue("master_volume", 1.0f);
}

//==============================================================================
void AdlplugAudioProcessor::parameterValueChangedEx(int tag)
{
    if (tag == 'chip')
        mark_parameter_as_changed(Cb_ChipSettings);
    else if (tag == 'glob')
        mark_parameter_as_changed(Cb_GlobalParameters);
    else {
        const uint32_t part_tag = ((uint8_t)'i' << 24) | ((uint8_t)'n' << 16) | ((uint8_t)'s' << 8);
        if (((uint32_t)tag & 0xffffff00) == part_tag) {
            unsigned part = tag & 15;
            mark_parameter_as_changed(Cb_Instrument1 + part);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    midi_db.init();
    return new AdlplugAudioProcessor();
}
