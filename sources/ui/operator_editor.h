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
    ScopedPointer<Styled_Knob_Default> kn_attack;
    ScopedPointer<Styled_Knob_Default> kn_decay;
    ScopedPointer<Styled_Knob_Default> kn_sustain;
    ScopedPointer<Styled_Knob_Default> kn_release;
    ScopedPointer<TextButton> textButton;
    ScopedPointer<TextButton> textButton2;
    ScopedPointer<TextButton> btn_trem;
    ScopedPointer<TextButton> btn_vib;
    ScopedPointer<TextButton> btn_sus;
    ScopedPointer<TextButton> btn_env;
    ScopedPointer<Slider> sl_level;
    ScopedPointer<Slider> sl_fmul;
    ScopedPointer<Slider> sl_ksl;
    ScopedPointer<Label> lb_optype;
    ScopedPointer<Label> lbl_level;
    ScopedPointer<Label> lbl_fmul;
    ScopedPointer<Label> lbl_ksl;
    ScopedPointer<Wave_Label> lbl_wave;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Operator_Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
