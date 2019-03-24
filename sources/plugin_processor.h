//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "plugin_state.h"
#include "dsp/dc_filter.h"
#include "dsp/vu_monitor.h"
#include "adl/instrument.h"
#include "adl/chip_settings.h"
#include "utility/processor_ex.h"
#include "JuceHeader.h"
#include <folly/AtomicBitSet.h>
#include <bitset>
#include <memory>
#include <mutex>
class Player;
class Bank_Manager;
class Simple_Fifo;
class Midi_Input_Source;
class Worker;
struct Parameter_Block;
struct Buffered_Message;
struct Instrument;

//==============================================================================
/**
 */
class AdlplugAudioProcessor : public AudioProcessorEx,
                              public AudioProcessorParameter::Listener {
public:
    //==========================================================================
    AdlplugAudioProcessor();
    ~AdlplugAudioProcessor();

    //==========================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool is_playback_ready() const
        { return ready_.load(); }

    std::unique_lock<std::mutex> acquire_player_nonrt();
    unsigned num_chips_nonrt() const;
    void set_num_chips_nonrt(unsigned chips);
    unsigned chip_emulator_nonrt() const;
    void set_chip_emulator_nonrt(unsigned emu);
#if defined(ADLPLUG_OPL3)
    unsigned num_4ops_nonrt() const;
    void set_num_4ops_nonrt(unsigned count);
#endif
#if defined(ADLPLUG_OPN2)
    unsigned chip_type_nonrt() const;
    void set_chip_type_nonrt(unsigned type);
#endif
    void panic_nonrt();
    void reconfigure_chip_nonrt();

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
    void processBlockBypassed(AudioBuffer<float> &, MidiBuffer &) override;

    void process(float *outputs[], unsigned nframes, Midi_Input_Source &midi);

private:
    void process_messages(bool under_lock);
    void process_parameter_changes();
    void process_notifications();

public:
    struct Message_Handler_Context;
    bool handle_midi(const uint8_t *data, unsigned len);
    bool handle_message(const Buffered_Message &msg, Message_Handler_Context &ctx);
    void begin_handling_messages(Message_Handler_Context &ctx) {}
    void finish_handling_messages(Message_Handler_Context &ctx);

    void parameters_to_chip_settings(Chip_Settings &cs) const;
    void parameters_to_global(Instrument_Global_Parameters &gp) const;
    void parameters_to_instrument(unsigned part_number, Instrument &ins) const;
    void set_chip_settings_notifying_host();
    void set_global_parameters_notifying_host();
    void set_instrument_parameters_notifying_host(unsigned part_number);

    void chip_settings_from_emulator(Chip_Settings &cs) const;

    void send_program_change_from_selection(unsigned part);

    //==========================================================================
    AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    const std::shared_ptr<Simple_Fifo> &message_queue_for_ui() const
        { return mq_from_ui_; }
    const std::shared_ptr<Simple_Fifo> &message_queue_to_ui() const
        { return mq_to_ui_; }

    Simple_Fifo &message_queue_for_worker() const { return *mq_from_worker_; }
    Simple_Fifo &message_queue_to_worker() const { return *mq_to_worker_; }

    Parameter_Block &parameter_block() const { return *parameter_block_; }

    void mark_for_notification(unsigned changebit)
        { to_notify_.set(changebit); }
    bool unmark_for_notification(unsigned changebit)
        { return to_notify_.reset(changebit); }

private:
    void mark_parameter_as_changed(unsigned changebit)
        { pr_changed_.set(changebit); }
    bool unmark_parameter_as_changed(unsigned changebit)
        { return pr_changed_.reset(changebit); }

public:
    Worker *worker() const
        { return worker_.get(); }

    double vu_level(unsigned channel) const
        { return (channel < 2) ? lv_current_[channel] : 0; }

    double cpu_load() const
        { return cpu_load_; }

    unsigned midi_channel_note_count(unsigned channel) const
        { return (channel < 16) ? midi_channel_note_count_[channel] : 0; }
    bool midi_channel_note_active(unsigned channel, unsigned note) const
        { return (channel < 16) ? midi_channel_note_active_[channel].test(note) : false; }

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
    void parameterValueChanged(int index, float value) override {}
    void parameterGestureChanged(int index, bool is_starting) override {}
    void parameterValueChangedEx(int tag) override;

private:
    std::unique_ptr<Player> player_;

    std::unique_ptr<Bank_Manager> bank_manager_;

    std::atomic<int> ready_ {0};

    std::shared_ptr<Simple_Fifo> mq_from_ui_;
    std::shared_ptr<Simple_Fifo> mq_to_ui_;

    std::unique_ptr<Simple_Fifo> mq_from_worker_;
    std::unique_ptr<Simple_Fifo> mq_to_worker_;

    Dc_Filter dc_filter_[2];
    Vu_Monitor vu_monitor_[2];
    double lv_current_[2] {};
    double cpu_load_ = 0;

    folly::AtomicBitSet<Cb_Count> pr_changed_;
    folly::AtomicBitSet<Cb_Count> to_notify_;

    std::unique_ptr<Parameter_Block> parameter_block_;

    struct Selection {
        Bank_Id bank {0, 0, false};
        uint8_t program = 0;
    };
    Selection selection_[16];

    std::bitset<16> midi_channel_mask_;
    unsigned midi_channel_note_count_[16] = {};
    std::bitset<128> midi_channel_note_active_[16];
    unsigned midi_bank_msb_[16] = {};
    unsigned midi_bank_lsb_[16] = {};

    unsigned active_part_ = 0;

    static constexpr unsigned bank_title_size_max = 64;
    char bank_title_[bank_title_size_max + 1] {};

    std::mutex player_lock_;

    std::unique_ptr<Worker> worker_;

    MemoryBlock last_state_information_;

    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessor)
};
