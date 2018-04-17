/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../../JuceLibraryCode/JuceHeader.h"
#include "ui/styled_knobs.h"
#include "utility/simple_fifo.h"
class Operator_Editor;
class Vu_Meter;
class AdlplugAudioProcessor;
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
                        public Button::Listener,
                        public Slider::Listener,
                        public ComboBox::Listener
{
public:
    //==============================================================================
    Main_Component (AdlplugAudioProcessor &proc);
    ~Main_Component();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void handleNoteOn(MidiKeyboardState *, int channel, int note, float velocity) override;
    void handleNoteOff(MidiKeyboardState *, int channel, int note, float velocity) override;
    void vu_update();
    void cpu_load_update();
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


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    AdlplugAudioProcessor *proc_ = nullptr;
    MidiKeyboardState midi_kb_state_;

    class Vu_Timer : public Timer {
    public:
        explicit Vu_Timer(Main_Component *mc) : mc_(mc) {}
        void timerCallback() override { mc_->vu_update(); }
    private:
        Main_Component *mc_ = nullptr;
    };

    ScopedPointer<Vu_Timer> vu_timer_;

    class Cpu_Load_Timer : public Timer {
    public:
        explicit Cpu_Load_Timer(Main_Component *mc) : mc_(mc) {}
        void timerCallback() override { mc_->cpu_load_update(); }
    private:
        Main_Component *mc_ = nullptr;
    };

    ScopedPointer<Cpu_Load_Timer> cpu_load_timer_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Operator_Editor> ed_op2;
    ScopedPointer<TextButton> btn_4op;
    ScopedPointer<TextButton> btn_pseudo4op;
    ScopedPointer<TextButton> btn_2op;
    ScopedPointer<TextButton> btn_fm12;
    ScopedPointer<TextButton> btn_am12;
    ScopedPointer<Operator_Editor> ed_op1;
    ScopedPointer<Operator_Editor> ed_op4;
    ScopedPointer<TextButton> btn_fm34;
    ScopedPointer<TextButton> btn_am34;
    ScopedPointer<Operator_Editor> ed_op3;
    ScopedPointer<Slider> sl_tune12;
    ScopedPointer<Slider> sl_tune34;
    ScopedPointer<Styled_Knob_DefaultSmall> component5;
    ScopedPointer<Styled_Knob_DefaultSmall> component6;
    ScopedPointer<MidiKeyboardComponent> midi_kb;
    ScopedPointer<ImageButton> btn_about;
    ScopedPointer<Label> label;
    ScopedPointer<Label> lbl_num_chips;
    ScopedPointer<TextButton> btn_less_chips;
    ScopedPointer<TextButton> btn_more_chips;
    ScopedPointer<ComboBox> cb_emulator;
    ScopedPointer<Label> label2;
    ScopedPointer<Vu_Meter> vu_left;
    ScopedPointer<Vu_Meter> vu_right;
    ScopedPointer<Label> label3;
    ScopedPointer<Label> lbl_cpu;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Main_Component)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
