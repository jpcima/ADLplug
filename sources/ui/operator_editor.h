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
    ScopedPointer<Styled_Knob_Default> component;
    ScopedPointer<Styled_Knob_Default> component2;
    ScopedPointer<Styled_Knob_Default> component3;
    ScopedPointer<Styled_Knob_Default> component4;
    ScopedPointer<Label> label;
    ScopedPointer<TextButton> textButton;
    ScopedPointer<TextButton> textButton2;
    ScopedPointer<TextButton> textButton3;
    ScopedPointer<TextButton> textButton4;
    ScopedPointer<TextButton> textButton5;
    ScopedPointer<TextButton> textButton6;
    ScopedPointer<Slider> slider;
    ScopedPointer<Slider> slider2;
    ScopedPointer<Slider> slider3;
    ScopedPointer<Label> lb_optype;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Operator_Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
