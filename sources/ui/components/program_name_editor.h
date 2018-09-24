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
#include "adl/instrument.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Program_Name_Editor  : public Component,
                             public Button::Listener
{
public:
    //==============================================================================
    Program_Name_Editor ();
    ~Program_Name_Editor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void set_program(
        Bank_Id bank, unsigned pgm, const String &bank_name, const String &pgm_name);

    struct Result {
        Bank_Id bank;
        unsigned pgm = 0;
        String bank_name;
        String pgm_name;
    };

    std::function<void(const Result &result)> on_ok;
    std::function<void()> on_cancel;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Bank_Id bank_;
    unsigned pgm_ = 0;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TextEditor> edt_pgm_name;
    std::unique_ptr<TextEditor> edt_bank_name;
    std::unique_ptr<Label> label;
    std::unique_ptr<Label> label2;
    std::unique_ptr<TextEditor> edt_pgm_id;
    std::unique_ptr<TextEditor> edt_bank_id;
    std::unique_ptr<TextButton> btn_ok;
    std::unique_ptr<TextButton> btn_cancel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Program_Name_Editor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
