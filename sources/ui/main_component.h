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
class Operator_Editor;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Main_Component  : public Component,
                        public Button::Listener,
                        public Slider::Listener
{
public:
    //==============================================================================
    Main_Component ();
    ~Main_Component();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;

    // Binary resources:
    static const char* logo_png;
    static const int logo_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    MidiKeyboardState midi_kb_state_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Operator_Editor> ed_op2;
    ScopedPointer<TextButton> textButton;
    ScopedPointer<TextButton> textButton2;
    ScopedPointer<TextButton> textButton3;
    ScopedPointer<TextButton> textButton4;
    ScopedPointer<TextButton> textButton5;
    ScopedPointer<Operator_Editor> ed_op1;
    ScopedPointer<Operator_Editor> ed_op4;
    ScopedPointer<TextButton> textButton6;
    ScopedPointer<TextButton> textButton7;
    ScopedPointer<Operator_Editor> ed_op3;
    ScopedPointer<Slider> sl_tune12;
    ScopedPointer<Slider> sl_tune34;
    ScopedPointer<Styled_Knob_DefaultSmall> component5;
    ScopedPointer<Styled_Knob_DefaultSmall> component6;
    ScopedPointer<MidiKeyboardComponent> component;
    Image cachedImage_logo_png_1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Main_Component)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
