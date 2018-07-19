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
#include "../../JuceLibraryCode/JuceHeader.h"
#include "ui/styled_knobs.h"
#include "adl/instrument.h"
#include "utility/simple_fifo.h"
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
                        public MidiKeyboardStateListener,
                        public Knob::Listener,
                        public Button::Listener,
                        public Slider::Listener,
                        public ComboBox::Listener
{
public:
    //==============================================================================
    Main_Component (AdlplugAudioProcessor &proc, Parameter_Block &pb);
    ~Main_Component();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void handleNoteOn(MidiKeyboardState *, int channel, int note, float velocity) override;
    void handleNoteOff(MidiKeyboardState *, int channel, int note, float velocity) override;

    void knob_value_changed(Knob *k) override;

    void send_controller(unsigned channel, unsigned ctl, unsigned value);
    void send_program_change(unsigned channel, unsigned value);

    bool is_percussion_channel(unsigned channel) const;
    Instrument *find_instrument(uint32_t program, Instrument *if_not_found);

    void reload_selected_instrument(NotificationType ntf);
    void send_selection_update();
    void set_instrument_parameters(const Instrument &ins, NotificationType ntf);

    void receive_instrument(Bank_Id bank, unsigned pgm, const Instrument &ins);
    void update_instrument_choices();
    void set_program_selection(int selection, NotificationType ntf);
    static String program_selection_to_string(int selection);

    void on_change_midi_channel(unsigned channel);

    void vu_update();
    void cpu_load_update();
    void midi_activity_update();
    void popup_about_dialog();
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

    // Binary resources:
    static const char* logo_png;
    static const int logo_pngSize;
    static const char* emoji_u1f4be_png;
    static const int emoji_u1f4be_pngSize;
    static const char* emoji_u1f4c2_png;
    static const int emoji_u1f4c2_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AdlplugAudioProcessor *proc_ = nullptr;
    Parameter_Block *parameter_block_ = nullptr;

    struct Editor_Bank {
        PopupMenu ins_menu;
        std::array<Instrument, 256> ins;
    };
    std::map<uint32_t, Editor_Bank> instrument_map_;

    unsigned midichannel_ = 0;
    uint32_t midiprogram_[16] = {};

    File bank_directory_;
    MidiKeyboardState midi_kb_state_;

    class Vu_Timer : public Timer {
    public:
        explicit Vu_Timer(Main_Component *mc) : mc_(mc) {}
        void timerCallback() override { mc_->vu_update(); }
    private:
        Main_Component *mc_ = nullptr;
    };
    std::unique_ptr<Vu_Timer> vu_timer_;

    class Cpu_Load_Timer : public Timer {
    public:
        explicit Cpu_Load_Timer(Main_Component *mc) : mc_(mc) {}
        void timerCallback() override { mc_->cpu_load_update(); }
    private:
        Main_Component *mc_ = nullptr;
    };
    std::unique_ptr<Cpu_Load_Timer> cpu_load_timer_;

    class Midi_Activity_Timer : public Timer {
    public:
        explicit Midi_Activity_Timer(Main_Component *mc) : mc_(mc) {}
        void timerCallback() override { mc_->midi_activity_update(); }
    private:
        Main_Component *mc_ = nullptr;
    };
    std::unique_ptr<Midi_Activity_Timer> midi_activity_timer_;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Operator_Editor> ed_op2;
    std::unique_ptr<TextButton> btn_4op;
    std::unique_ptr<TextButton> btn_pseudo4op;
    std::unique_ptr<TextButton> btn_2op;
    std::unique_ptr<TextButton> btn_fm12;
    std::unique_ptr<TextButton> btn_am12;
    std::unique_ptr<Operator_Editor> ed_op1;
    std::unique_ptr<Operator_Editor> ed_op4;
    std::unique_ptr<TextButton> btn_fm34;
    std::unique_ptr<TextButton> btn_am34;
    std::unique_ptr<Operator_Editor> ed_op3;
    std::unique_ptr<Slider> sl_tune12;
    std::unique_ptr<Slider> sl_tune34;
    std::unique_ptr<Styled_Knob_DefaultSmall> kn_fb12;
    std::unique_ptr<Styled_Knob_DefaultSmall> kn_fb34;
    std::unique_ptr<MidiKeyboardComponent> midi_kb;
    std::unique_ptr<ImageButton> btn_about;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> lbl_num_chips;
    std::unique_ptr<TextButton> btn_less_chips;
    std::unique_ptr<TextButton> btn_more_chips;
    std::unique_ptr<ComboBox> cb_emulator;
    std::unique_ptr<Label> label2;
    std::unique_ptr<Vu_Meter> vu_left;
    std::unique_ptr<Vu_Meter> vu_right;
    std::unique_ptr<Label> label3;
    std::unique_ptr<Label> lbl_cpu;
    std::unique_ptr<Indicator_NxM> ind_midi_activity;
    std::unique_ptr<TextButton> btn_panic;
    std::unique_ptr<ImageButton> btn_bank_save;
    std::unique_ptr<ImageButton> btn_bank_load;
    std::unique_ptr<TextEditor> edt_bank_name;
    std::unique_ptr<ComboBox> cb_program;
    std::unique_ptr<Label> label4;
    std::unique_ptr<Label> lbl_channel;
    std::unique_ptr<TextButton> btn_prev_channel;
    std::unique_ptr<TextButton> btn_next_channel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Main_Component)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
