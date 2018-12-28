/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "ui/generic_main_component.h"
#include "ui/components/styled_knobs.h"
#include "messages.h"
#include <map>
#include <vector>
#include <array>
class Operator_Editor;
class Vu_Meter;
class Indicator_NxM;
class Midi_Keyboard_Ex;
class AdlplugAudioProcessor;
struct Parameter_Block;
class Configuration;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Main_Component  : public Generic_Main_Component<Main_Component>,
                        public Knob::Listener,
                        public Slider::Listener,
                        public Button::Listener,
                        public ComboBox::Listener
{
public:
    //==============================================================================
    Main_Component (AdlplugAudioProcessor &proc, Parameter_Block &pb, Configuration &conf);
    ~Main_Component();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void sliderDragStarted(Slider *slider) override;
    void sliderDragEnded(Slider *slider) override;

    void knob_value_changed(Knob *k) override;
    void knob_drag_started(Knob *k) override;
    void knob_drag_ended(Knob *k) override;

    void set_global_parameters(NotificationType ntf);
    void set_instrument_parameters(const Instrument &ins, NotificationType ntf);
    void set_chip_settings(NotificationType ntf);

    void on_change_midi_channel(unsigned channel);

    void popup_about_dialog();

    bool display_info_for_component(Component *c);
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
    friend class Generic_Main_Component<Main_Component>;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Operator_Editor> ed_op2;
    std::unique_ptr<Operator_Editor> ed_op1;
    std::unique_ptr<Operator_Editor> ed_op4;
    std::unique_ptr<Operator_Editor> ed_op3;
    std::unique_ptr<Slider> sl_tune;
    std::unique_ptr<Midi_Keyboard_Ex> midi_kb;
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
    std::unique_ptr<TextButton> btn_keymap;
    std::unique_ptr<TextButton> btn_octave_up;
    std::unique_ptr<TextButton> btn_octave_down;
    std::unique_ptr<TextButton> btn_pgm_edit;
    std::unique_ptr<TextButton> btn_pgm_add;
    std::unique_ptr<Label> label23;
    std::unique_ptr<Styled_Knob_Default> kn_mastervol;
    std::unique_ptr<Label> lbl_mastervol;
    std::unique_ptr<Label> label16;
    std::unique_ptr<ComboBox> cb_chip_type;
    std::unique_ptr<Label> lbl_info;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Main_Component)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
