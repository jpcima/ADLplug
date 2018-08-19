//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/player.h"
#include "midi/insnames.h"
#include "utility/midi.h"
#include "utility/simple_fifo.h"
#include "utility/rt_checker.h"
#include "bank_manager.h"
#include "parameter_block.h"
#include "messages.h"
#include "definitions.h"
#include "plugin_processor.h"
#include "plugin_editor.h"
#include "worker.h"
#include <wopl/wopl_file.h>
#include <cassert>

//==============================================================================
AdlplugAudioProcessor::AdlplugAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", AudioChannelSet::stereo(), true))
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

    Generic_Player *pl = instantiate_player(Player_Type::OPL3);
    player_.reset(pl);
    pl->init(sample_rate);
    pl->reserve_banks(bank_reserve_size);
    pl->load_bank_data(BinaryData::default_wopl, BinaryData::default_woplSize);
    pl->set_volume_model(1);
    pl->set_deep_tremolo(false);
    pl->set_deep_vibrato(false);
    pl->set_soft_pan_enabled(true);

    Chip_Settings cs;
    cs.emulator = get_emulator_defaults().default_index;
    pl->set_num_chips(cs.chip_count);
    pl->set_num_4ops(cs.fourop_count);
    pl->set_emulator(cs.emulator);
    chip_settings_need_notification_ = true;

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

    Bank_Manager *bm = new Bank_Manager(*this, *pl);
    bank_manager_.reset(bm);

    selection_id_ = Bank_Id(0, 0, 0);
    selection_pgm_ = 0;

    ready_ = true;
    set_instrument_parameters_notifying_host();

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

    ready_ = false;

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
    Generic_Player *pl = player_.get();
    return pl->num_chips();
}

void AdlplugAudioProcessor::set_num_chips_nonrt(unsigned chips)
{
    Generic_Player *pl = player_.get();
    pl->set_num_chips(chips);
    reconfigure_chip_nonrt();
}

unsigned AdlplugAudioProcessor::chip_emulator_nonrt() const
{
    Generic_Player *pl = player_.get();
    return pl->emulator();
}

void AdlplugAudioProcessor::set_chip_emulator_nonrt(unsigned emu)
{
    Generic_Player *pl = player_.get();
    pl->set_emulator(emu);
    reconfigure_chip_nonrt();
}

unsigned AdlplugAudioProcessor::num_4ops_nonrt() const
{
    Generic_Player *pl = player_.get();
    return pl->num_4ops();
}

void AdlplugAudioProcessor::set_num_4ops_nonrt(unsigned count)
{
    Generic_Player *pl = player_.get();
    pl->set_num_4ops(count);
}

void AdlplugAudioProcessor::panic_nonrt()
{
    Generic_Player *pl = player_.get();
    pl->panic();
}

void AdlplugAudioProcessor::reconfigure_chip_nonrt()
{
    // Generic_Player *pl = player_.get();
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

    Generic_Player *pl = player_.get();
    float *left = outputs[0];
    float *right = outputs[1];

    std::unique_lock<std::mutex> lock(player_lock_, std::try_to_lock);
    process_messages(midi, lock.owns_lock());

    if (!lock.owns_lock()) {
        // can't use the player while non-rt modifies it
        std::fill_n(left, nframes, 0);
        std::fill_n(right, nframes, 0);
        return;
    }

    if (chip_settings_changed_.compareAndSetBool(false, true)) {
        Simple_Fifo &queue = *mq_to_worker_;
        Message_Header hdr(Fx_Message::RequestChipSettings, sizeof(Messages::Fx::RequestChipSettings));
        Buffered_Message msg = Messages::write(queue, hdr);
        if (!msg)
            chip_settings_changed_ = true;  // do later
        else {
            auto &body = *(Messages::Fx::RequestChipSettings *)msg.data;
            parameters_to_chip_settings(body.cs);
            Messages::finish_write(queue, msg);
            Worker &worker = *worker_;
            worker.postSemaphore();
        }
    }

    if (instrument_parameters_changed_.compareAndSetBool(false, true)) {
        Bank_Manager &bm = *bank_manager_;
        Instrument ins;
        parameters_to_instrument(ins);
        bm.load_program(
            selection_id_, selection_pgm_, ins,
            Bank_Manager::LP_Notify|Bank_Manager::LP_NeedMeasurement|Bank_Manager::LP_KeepName);
    }

    if (global_parameters_changed_.compareAndSetBool(false, true)) {
        Bank_Manager &bm = *bank_manager_;
        Instrument_Global_Parameters gp;
        parameters_to_global(gp);
        bm.load_global_parameters(gp, true);
    }

    if (chip_settings_need_notification_.compareAndSetBool(false, true)) {
        Simple_Fifo &queue = *mq_to_ui_;
        Message_Header hdr(Fx_Message::NotifyChipSettings, sizeof(Messages::Fx::NotifyChipSettings));
        Buffered_Message msg = Messages::write(queue, hdr);
        if (!msg)
            chip_settings_need_notification_ = true;  // do later
        else {
            auto &body = *(Messages::Fx::NotifyChipSettings *)msg.data;
            body.cs.emulator = pl->emulator();
            body.cs.chip_count = pl->num_chips();
            body.cs.fourop_count = pl->num_4ops();
            Messages::finish_write(queue, msg);
        }
    }

    ScopedNoDenormals no_denormals;
    double sample_period = 1.0 / getSampleRate();

    int64_t time_before_generate = Time::getHighResolutionTicks();
    pl->generate(left, right, nframes, 1);
    int64_t time_after_generate = Time::getHighResolutionTicks();
    lock.unlock();

    Dc_Filter &dclf = dc_filter_[0];
    Dc_Filter &dcrf = dc_filter_[1];
    Vu_Monitor &lvu = vu_monitor_[0];
    Vu_Monitor &rvu = vu_monitor_[1];
    double lv_current[2];

    // filter out the DC component
    for (unsigned i = 0; i < nframes; ++i) {
        double left_sample = dclf.process(left[i]);
        double right_sample = dcrf.process(right[i]);
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

void AdlplugAudioProcessor::process_messages(Midi_Input_Source &midi, bool under_lock)
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

    // handle events from MIDI
    while (Midi_Input_Message msg = midi.get_next_event())
        handle_midi(msg.data, msg.size, ctx);

    finish_handling_messages(ctx);
}

bool AdlplugAudioProcessor::handle_midi(const uint8_t *data, unsigned len, Message_Handler_Context &ctx)
{
    if (!ctx.under_lock)
        return false;  // TODO save messages for later processing?

    Generic_Player *pl = player_.get();
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
        if (data[1] == 120 || data[1] == 123) {
            midi_channel_note_count_[channel] = 0;
            midi_channel_note_active_[channel].reset();
        }
        break;
    }

    return true;
}

bool AdlplugAudioProcessor::handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx)
{
    const uint8_t *data = msg.data;
    unsigned tag = msg.header->tag;
    unsigned size = msg.header->size;

    if (tag == (unsigned)User_Message::Midi)
        return handle_midi(data, size, ctx);

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
        chip_settings_need_notification_ = true;
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
            if (body.bank == selection_id_ && body.program == selection_pgm_)
                set_instrument_parameters_notifying_host();
        }
        break;
    }
    case (unsigned)User_Message::RenameBank: {
        auto &body = *(const Messages::User::RenameBank *)data;
        bm.rename_bank(body.bank, body.name, body.notify_back);
        break;
    }
    case (unsigned)User_Message::SelectProgram: {
        auto &body = *(const Messages::User::SelectProgram *)data;
        if (selection_id_ != body.bank || selection_pgm_ != body.program) {
            selection_id_ = body.bank;
            selection_pgm_ = body.program;
            set_instrument_parameters_notifying_host();
        }
        break;
    }
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
    cs.fourop_count = pb.p_n4op->get();
}

void AdlplugAudioProcessor::parameters_to_global(Instrument_Global_Parameters &gp) const
{
    const Parameter_Block &pb = *parameter_block_;

    gp.volume_model = pb.p_volmodel->getIndex();
    gp.deep_tremolo = pb.p_deeptrem->get();
    gp.deep_vibrato = pb.p_deepvib->get();
}

void AdlplugAudioProcessor::parameters_to_instrument(Instrument &ins) const
{
    const Parameter_Block &pb = *parameter_block_;

    ins.version = ADLMIDI_InstrumentVersion;
    ins.inst_flags = 0;

    ins.four_op(pb.p_is4op->get());
    ins.pseudo_four_op(pb.p_ps4op->get());
    ins.blank(pb.p_blank->get());
    ins.con12(pb.p_con12->getIndex());
    ins.con34(pb.p_con34->getIndex());
    ins.note_offset1 = pb.p_tune12->get();
    ins.note_offset2 = pb.p_tune34->get();
    ins.fb12(pb.p_fb12->get());
    ins.fb34(pb.p_fb34->get());
    ins.midi_velocity_offset = pb.p_veloffset->get();
    ins.second_voice_detune = pb.p_voice2ft->get();
    ins.percussion_key_number = pb.p_drumnote->get();

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        const Parameter_Block::Operator &op = pb.nth_operator(opnum);
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
        ins.wave(opnum, op.p_wave->get());
    }
}

void AdlplugAudioProcessor::set_global_parameters_notifying_host()
{
    Generic_Player *pl = player_.get();
    Parameter_Block &pb = *parameter_block_;

    *pb.p_volmodel = pl->volume_model() - 1;
    *pb.p_deeptrem = pl->deep_tremolo();
    *pb.p_deepvib = pl->deep_vibrato();
}

void AdlplugAudioProcessor::set_instrument_parameters_notifying_host()
{
    Instrument ins;
    Bank_Manager &bm = *bank_manager_;

    if (!bm.find_program(selection_id_, selection_pgm_, ins))
        return;

    Parameter_Block &pb = *parameter_block_;

    *pb.p_is4op = ins.four_op();
    *pb.p_ps4op = ins.pseudo_four_op();
    *pb.p_blank = ins.blank();
    *pb.p_con12 = ins.con12();
    *pb.p_con34 = ins.con34();
    *pb.p_tune12 = ins.note_offset1;
    *pb.p_tune34 = ins.note_offset2;
    *pb.p_fb12 = ins.fb12();
    *pb.p_fb34 = ins.fb34();
    *pb.p_veloffset = ins.midi_velocity_offset;
    *pb.p_voice2ft = ins.second_voice_detune;
    *pb.p_drumnote = ins.percussion_key_number;

    for (unsigned opnum = 0; opnum < 4; ++opnum) {
        Parameter_Block::Operator &op = pb.nth_operator(opnum);
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
    process_messages(midi_source, lock.owns_lock());
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
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AdlplugAudioProcessor::setStateInformation(const void *data, int size)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
}

//==============================================================================
void AdlplugAudioProcessor::parameterValueChanged(int index, float value)
{
    const Parameter_Block &pb = *parameter_block_;

    if (index >= pb.first_chip_setting && index <= pb.last_chip_setting)
        chip_settings_changed_ = true;
    else if (index >= pb.first_global_parameter && index <= pb.last_global_parameter)
        global_parameters_changed_ = true;
    else if (index >= pb.first_instrument_parameter && index <= pb.last_instrument_parameter)
        instrument_parameters_changed_ = true;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    midi_db.init();
    return new AdlplugAudioProcessor();
}
