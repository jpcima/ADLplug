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
class Wave_Label;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Operator_Editor  : public Component,
                         public Button::Listener,
                         public Slider::Listener
{
public:
    //==============================================================================
    Operator_Editor ();
    ~Operator_Editor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void set_op_label(const String &type)
        { lb_optype->setText(type, juce::dontSendNotification); }
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Styled_Knob_Default> kn_attack;
    std::unique_ptr<Styled_Knob_Default> kn_decay;
    std::unique_ptr<Styled_Knob_Default> kn_sustain;
    std::unique_ptr<Styled_Knob_Default> kn_release;
    std::unique_ptr<TextButton> textButton;
    std::unique_ptr<TextButton> textButton2;
    std::unique_ptr<TextButton> btn_trem;
    std::unique_ptr<TextButton> btn_vib;
    std::unique_ptr<TextButton> btn_sus;
    std::unique_ptr<TextButton> btn_env;
    std::unique_ptr<Slider> sl_level;
    std::unique_ptr<Slider> sl_fmul;
    std::unique_ptr<Slider> sl_ksl;
    std::unique_ptr<Label> lb_optype;
    std::unique_ptr<Label> lbl_level;
    std::unique_ptr<Label> lbl_fmul;
    std::unique_ptr<Label> lbl_ksl;
    std::unique_ptr<Wave_Label> lbl_wave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Operator_Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
