//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "dsp/dc_filter.h"
#include "dsp/vu_monitor.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include <mutex>
class Generic_Player;
class Simple_Fifo;

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
    void reconfigure_chip_nonrt();
    std::vector<std::string> enumerate_emulators();

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    void processBlockBypassed(AudioBuffer<float> &, MidiBuffer &) override;

    //==========================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    Simple_Fifo &midi_queue_for_ui() const { return *ui_midi_queue_; }

    double vu_level(unsigned channel) const
        { return (channel < 2) ? lv_current_[channel] : 0; }

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
    std::mutex player_lock_;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessor)
};
