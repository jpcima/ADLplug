//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "adl/player.h"
#include "utility/simple_fifo.h"
#include "utility/rt_checker.h"
#include "plugin_processor.h"
#include "plugin_editor.h"

//==============================================================================
AdlplugAudioProcessor::AdlplugAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", AudioChannelSet::stereo(), true))
{
    ui_midi_queue_.reset(new Simple_Fifo(1024));
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    Generic_Player *pl = instantiate_player(Player_Type::OPL3);
    player_.reset(pl);
    pl->init(sample_rate);
    pl->set_num_chips(4);

    for (unsigned i = 0; i < 2; ++i) {
        Dc_Filter &dcf = dc_filter_[i];
        dcf.cutoff(5.0 / sample_rate);
        Vu_Monitor &vu = vu_monitor_[i];
        vu.release(0.5 * sample_rate);
    }
}

void AdlplugAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    player_.reset();
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
    pl->reset();
    // TODO crashes libADLMIDI, must investigate
    pl->set_num_chips(chips);
    reconfigure_chip_nonrt();
}

void AdlplugAudioProcessor::set_chip_emulator_nonrt(unsigned emu)
{
    Generic_Player *pl = player_.get();
    pl->reset();
    pl->set_emulator(emu);
    reconfigure_chip_nonrt();
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

void AdlplugAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                         MidiBuffer &midi_messages)
{
#ifdef ADLplug_RT_CHECKER
    rt_checker_init();
#endif

    ScopedNoDenormals no_denormals;
    Generic_Player *pl = player_.get();
    unsigned nframes = buffer.getNumSamples();
    float *left = buffer.getWritePointer(0);
    float *right = buffer.getWritePointer(1);
    Simple_Fifo &midi_q = *ui_midi_queue_;

    std::unique_lock<std::mutex> lock(player_lock_, std::try_to_lock);
    if (!lock.owns_lock()) {
        // can't use the player while non-rt modifies it
        processBlockBypassed(buffer, midi_messages);
        return;
    }

    // handle MIDI events from GUI
    for (uint8_t len; midi_q.read(&len, 1, false) && midi_q.get_num_ready() >= len + 1;) {
        midi_q.discard(1);
        uint8_t data[3];
        if (len > sizeof(data))
            midi_q.discard(len);
        else
        {
            midi_q.read(data, len);
            pl->play_midi(data, len);
        }
    }

    const uint8_t *midi_data;
    int midi_size;
    int sample_position;
    MidiBuffer::Iterator it(midi_messages);
    while (it.getNextEvent(midi_data, midi_size, sample_position))
        pl->play_midi(midi_data, midi_size);

    pl->generate(left, right, nframes, 1);
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
}

void AdlplugAudioProcessor::processBlockBypassed(AudioBuffer<float> &buffer, MidiBuffer &midi_messages)
{
    // flush MIDI events from GUI
    Simple_Fifo &midi_q = *ui_midi_queue_;
    for (uint8_t len; midi_q.read(&len, 1, false) && midi_q.get_num_ready() >= len + 1;)
        midi_q.discard(len + 1);

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
