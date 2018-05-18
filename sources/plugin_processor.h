//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "dsp/dc_filter.h"
#include "dsp/vu_monitor.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <bitset>
#include <memory>
#include <mutex>
class Generic_Player;
class Simple_Fifo;
class Midi_Input_Source;
struct Buffered_Message;

//==============================================================================
/**
 */
class AdlplugAudioProcessor : public AudioProcessor {
public:
    //==========================================================================
    AdlplugAudioProcessor();
    ~AdlplugAudioProcessor();

    //==========================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    unsigned get_num_chips() const;
    std::unique_lock<std::mutex> acquire_player_nonrt();
    void set_num_chips_nonrt(unsigned chips);
    void set_chip_emulator_nonrt(unsigned emu);
    void panic_nonrt();
    void reconfigure_chip_nonrt();
    std::vector<std::string> enumerate_emulators();

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    void processBlockBypassed(AudioBuffer<float> &, MidiBuffer &) override;

    void process(float *outputs[], unsigned nframes, Midi_Input_Source &midi);
    void process_messages(Midi_Input_Source &midi, bool under_lock);

    struct Message_Handler_Context;
    void handle_midi(const uint8_t *data, unsigned len, Message_Handler_Context &ctx);
    void handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx);
    void begin_handling_messages(Message_Handler_Context &ctx);
    void finish_handling_messages(Message_Handler_Context &ctx);

    //==========================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    Simple_Fifo &midi_queue_for_ui() const { return *ui_midi_queue_; }

    double vu_level(unsigned channel) const
        { return (channel < 2) ? lv_current_[channel] : 0; }

    double cpu_load() const
        { return cpu_load_; }

    unsigned midi_channel_note_count(unsigned channel) const
        { return (channel < 16) ? midi_channel_note_count_[channel] : 0; }

    //==========================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==========================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String &newName) override;

    //==========================================================================
    void getStateInformation(MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    std::unique_ptr<Generic_Player> player_;
    std::unique_ptr<Simple_Fifo> ui_midi_queue_;
    Dc_Filter dc_filter_[2];
    Vu_Monitor vu_monitor_[2];
    double lv_current_[2] {};
    double cpu_load_ = 0;
    std::bitset<16> midi_channel_mask_;
    unsigned midi_channel_note_count_[16] = {};
    std::bitset<128> midi_channel_note_active_[16];
    std::mutex player_lock_;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessor)
};
