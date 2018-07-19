//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "dsp/dc_filter.h"
#include "dsp/vu_monitor.h"
#include "adl/instrument.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <bitset>
#include <memory>
#include <mutex>
class Generic_Player;
class Bank_Manager;
class Simple_Fifo;
class Midi_Input_Source;
struct Parameter_Block;
struct Buffered_Message;
struct Instrument;

//==============================================================================
/**
 */
class AdlplugAudioProcessor : public AudioProcessor,
                              public AudioProcessorParameter::Listener {
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

    static std::vector<std::string> enumerate_emulators();
    static unsigned identify_default_emulator();

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    void processBlockBypassed(AudioBuffer<float> &, MidiBuffer &) override;

    void process(float *outputs[], unsigned nframes, Midi_Input_Source &midi);
    void process_messages(Midi_Input_Source &midi, bool under_lock);

    struct Message_Handler_Context;
    bool handle_midi(const uint8_t *data, unsigned len, Message_Handler_Context &ctx);
    bool handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx);
    void begin_handling_messages(Message_Handler_Context &ctx) {}
    void finish_handling_messages(Message_Handler_Context &ctx);

    void parameters_to_instrument(Instrument &ins) const;
    void set_instrument_parameters_notifying_host();

    //==========================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    unsigned default_emulator() const { return default_emulator_; }

    Simple_Fifo &message_queue_for_ui() const { return *mq_from_ui_; }
    Simple_Fifo &message_queue_to_ui() const { return *mq_to_ui_; }

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

protected:
    //==========================================================================
    void parameterValueChanged(int index, float value) override;
    void parameterGestureChanged(int index, bool is_starting) override {}

private:
    std::unique_ptr<Generic_Player> player_;
    unsigned default_emulator_ = 0;

    std::unique_ptr<Bank_Manager> bank_manager_;

    std::unique_ptr<Simple_Fifo> mq_from_ui_;
    std::unique_ptr<Simple_Fifo> mq_to_ui_;

    Dc_Filter dc_filter_[2];
    Vu_Monitor vu_monitor_[2];
    double lv_current_[2] {};
    double cpu_load_ = 0;

    Atomic<bool> parameters_changed_;
    std::unique_ptr<Parameter_Block> parameter_block_;
    Bank_Id selection_id_ {0, 0, false};
    uint8_t selection_pgm_ = 0;

    std::bitset<16> midi_channel_mask_;
    unsigned midi_channel_note_count_[16] = {};
    std::bitset<128> midi_channel_note_active_[16];

    std::mutex player_lock_;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessor)
};
