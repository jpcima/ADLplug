//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/player.h"
#include "utility/midi.h"
#include "utility/simple_fifo.h"
#include "utility/rt_checker.h"
#include "bank_manager.h"
#include "messages.h"
#include "definitions.h"
#include "plugin_processor.h"
#include "plugin_editor.h"
#include <cassert>

//==============================================================================
AdlplugAudioProcessor::AdlplugAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", AudioChannelSet::stereo(), true))
{
}

AdlplugAudioProcessor::~AdlplugAudioProcessor()
{
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
    mq_from_ui_.reset(new Simple_Fifo(1024));
    mq_to_ui_.reset(new Simple_Fifo(1024));

    Generic_Player *pl = instantiate_player(Player_Type::OPL3);
    player_.reset(pl);
    pl->init(sample_rate);
    pl->reserve_banks(bank_reserve_size);
    pl->set_num_chips(2);

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
    bm->update_all_banks();
}

void AdlplugAudioProcessor::releaseResources()
{
    bank_manager_.reset();
    player_.reset();
    mq_from_ui_.reset();
}

unsigned AdlplugAudioProcessor::get_num_chips() const
{
    Generic_Player *pl = player_.get();
    return pl->num_chips();
}

std::unique_lock<std::mutex> AdlplugAudioProcessor::acquire_player_nonrt()
{
    return std::unique_lock<std::mutex>(player_lock_);
}

void AdlplugAudioProcessor::set_num_chips_nonrt(unsigned chips)
{
    Generic_Player *pl = player_.get();
    panic_nonrt();
    pl->set_num_chips(chips);
    reconfigure_chip_nonrt();
}

void AdlplugAudioProcessor::set_chip_emulator_nonrt(unsigned emu)
{
    Generic_Player *pl = player_.get();
    panic_nonrt();
    pl->set_emulator(emu);
    reconfigure_chip_nonrt();
}

void AdlplugAudioProcessor::panic_nonrt()
{
    Generic_Player *pl = player_.get();
    pl->panic();
}

void AdlplugAudioProcessor::reconfigure_chip_nonrt()
{
    Generic_Player *pl = player_.get();
    // TODO any necessary reconfiguration after reset
}

std::vector<std::string> AdlplugAudioProcessor::enumerate_emulators()
{
    Generic_Player *pl = player_.get();
    return ::enumerate_emulators(pl->type());
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
        double right_sample = dcrf.process(left[i]);
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
    while (Buffered_Message msg = read_message(mq_from_ui)) {
        handle_message(msg, ctx);
        finish_read_message(mq_from_ui, msg);
    }

    // handle events from MIDI
    while (Midi_Input_Message msg = midi.get_next_event())
        handle_midi(msg.data, msg.size, ctx);

    finish_handling_messages(ctx);
}

void AdlplugAudioProcessor::handle_midi(const uint8_t *data, unsigned len, Message_Handler_Context &ctx)
{
    if (!ctx.under_lock)
        return;  // TODO save messages for later processing?

    Generic_Player *pl = player_.get();
    pl->play_midi(data, len);

    unsigned status = (len > 0) ? data[0] : 0;
    unsigned channel = status & 0x0f;

    if ((status & 0xf0) != 0xf0 && !midi_channel_mask_[channel])
        return;

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
}

void AdlplugAudioProcessor::handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx)
{
    const uint8_t *data = msg.data;
    User_Message tag = (User_Message)msg.header->tag;
    unsigned size = msg.header->size;

    if (tag == User_Message::Midi) {
        handle_midi(data, size, ctx);
        return;
    }

    if (!ctx.under_lock)
        return;

    Generic_Player *pl = player_.get();

    switch (tag) {
    case User_Message::LoadInstrument: {
        auto &body = *(const Messages::User::LoadInstrument *)data;
        bank_manager_->load_program(body.bank, body.program, body.instrument);
        break;
    }
    default:
        assert(false);
    }
}

void AdlplugAudioProcessor::begin_handling_messages(Message_Handler_Context &ctx)
{
}

void AdlplugAudioProcessor::finish_handling_messages(Message_Handler_Context &ctx)
{
    bank_manager_->send_notifications();
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
    return new AdlplugAudioProcessorEditor(*this);
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
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new AdlplugAudioProcessor();
}
