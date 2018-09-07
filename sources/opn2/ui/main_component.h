/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "ui/components/styled_knobs.h"
#include "adl/instrument.h"
#include "adl/chip_settings.h"
#include "utility/simple_fifo.h"
#include "messages.h"
#include <map>
#include <vector>
#include <array>
class Operator_Editor;
class Vu_Meter;
class Indicator_NxM;
class AdlplugAudioProcessor;
struct Parameter_Block;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Main_Component  : public Component,
                        FocusChangeListener,
                        public MidiKeyboardStateListener,
                        public Knob::Listener,
                        public Slider::Listener,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    //==============================================================================
    Main_Component (AdlplugAudioProcessor &proc, Parameter_Block &pb);
    ~Main_Component();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void on_ready_processor();

    void handleNoteOn(MidiKeyboardState *, int channel, int note, float velocity) override;
    void handleNoteOff(MidiKeyboardState *, int channel, int note, float velocity) override;

    void knob_value_changed(Knob *k) override;
    void knob_drag_started(Knob *k) override;
    void knob_drag_ended(Knob *k) override;

    void send_controller(unsigned channel, unsigned ctl, unsigned value);
    void send_program_change(unsigned channel, unsigned value);

    bool is_percussion_channel(unsigned channel) const;
    Instrument *find_instrument(uint32_t program, Instrument *if_not_found);

    void reload_selected_instrument(NotificationType ntf);
    void send_selection_update();
    void set_global_parameters(NotificationType ntf);
    void set_instrument_parameters(const Instrument &ins, NotificationType ntf);
    void set_chip_settings(NotificationType ntf);

    void receive_bank_slots(const Messages::Fx::NotifyBankSlots &msg);
    void receive_global_parameters(const Instrument_Global_Parameters &gp);
    void receive_instrument(Bank_Id bank, unsigned pgm, const Instrument &ins);
    void receive_chip_settings(const Chip_Settings &cs);
    void update_instrument_choices();
    void set_program_selection(int selection, NotificationType ntf);
    static String program_selection_to_string(int selection);

    void load_bank(const File &file);
    void load_bank_mem(const uint8_t *mem, size_t length, const String &bank_name);
    void save_bank(const File &file);

    void set_int_parameter_with_delay(unsigned delay, AudioParameterInt &p, int v);

    void on_change_midi_channel(unsigned channel);

    void vu_update();
    void cpu_load_update();
    void midi_activity_update();
    void popup_about_dialog();
    void update_emulator_icon();
    void build_emulator_menu(PopupMenu &menu);
    void create_image_overlay(Component &component, Image image, float ratio);

    void focusGained(FocusChangeType cause) override;
    void globalFocusChanged(Component *component) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

    // Binary resources:
    static const char* opnmidi_png;
    static const int opnmidi_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AdlplugAudioProcessor *proc_ = nullptr;
    Parameter_Block *parameter_block_ = nullptr;

    bool write_to_processor(
        User_Message tag, const void *msgbody, unsigned msgsize);

    struct Editor_Bank {
        char name[32] = {};
        PopupMenu ins_menu;
        std::array<Instrument, 256> ins;
    };
    std::map<uint32_t, Editor_Bank> instrument_map_;
    Instrument_Global_Parameters instrument_gparam_;
    Chip_Settings chip_settings_;

    unsigned midichannel_ = 0;
    uint32_t midiprogram_[16] = {};

    File bank_directory_;
    MidiKeyboardState midi_kb_state_;

    std::vector<std::unique_ptr<ImageComponent>> image_overlays_;

    PopupMenu emulator_menu_;

    std::unique_ptr<Timer> vu_timer_;
    std::unique_ptr<Timer> cpu_load_timer_;
    std::unique_ptr<Timer> midi_activity_timer_;

    std::map<String, std::unique_ptr<Timer>> parameters_delayed_;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Operator_Editor> ed_op2;
    std::unique_ptr<Operator_Editor> ed_op1;
    std::unique_ptr<Operator_Editor> ed_op4;
    std::unique_ptr<Operator_Editor> ed_op3;
    std::unique_ptr<Slider> sl_tune;
    std::unique_ptr<MidiKeyboardComponent> midi_kb;
    std::unique_ptr<ImageButton> btn_about;
    std::unique_ptr<Label> label2;
    std::unique_ptr<Vu_Meter> vu_left;
    std::unique_ptr<Vu_Meter> vu_right;
    std::unique_ptr<Label> label3;
    std::unique_ptr<Label> lbl_cpu;
    std::unique_ptr<Indicator_NxM> ind_midi_activity;
    std::unique_ptr<TextButton> btn_panic;
    std::unique_ptr<TextEditor> edt_bank_name;
    std::unique_ptr<ComboBox> cb_program;
    std::unique_ptr<Label> label4;
    std::unique_ptr<TextButton> btn_bank_load;
    std::unique_ptr<TextButton> btn_bank_save;
    std::unique_ptr<Label> label11;
    std::unique_ptr<ImageButton> btn_emulator;
    std::unique_ptr<Label> label14;
    std::unique_ptr<Slider> sl_num_chips;
    std::unique_ptr<Label> label15;
    std::unique_ptr<Label> label5;
    std::unique_ptr<ComboBox> cb_percussion_key;
    std::unique_ptr<Label> label17;
    std::unique_ptr<Slider> sl_veloffset;
    std::unique_ptr<Label> label19;
    std::unique_ptr<Slider> sl_midi_channel;
    std::unique_ptr<Label> label22;
    std::unique_ptr<ComboBox> cb_volmodel;
    std::unique_ptr<TextButton> btn_algo_help;
    std::unique_ptr<TextButton> btn_lfo_enable;
    std::unique_ptr<Label> label20;
    std::unique_ptr<ComboBox> cb_lfofreq;
    std::unique_ptr<ComboBox> cb_algorithm;
    std::unique_ptr<Styled_Knob_DefaultSmall> kn_feedback;
    std::unique_ptr<Label> label10;
    std::unique_ptr<Styled_Knob_DefaultSmall> kn_ams;
    std::unique_ptr<Label> label6;
    std::unique_ptr<Styled_Knob_DefaultSmall> kn_fms;
    std::unique_ptr<Label> label7;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Main_Component)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
