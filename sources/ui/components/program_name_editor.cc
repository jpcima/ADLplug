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

//[Headers] You can add your own extra header files here...
#include "midi/insnames.h"
#include <fmt/format.h>
//[/Headers]

#include "program_name_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Program_Name_Editor::Program_Name_Editor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    edt_pgm_name.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_pgm_name.get());
    edt_pgm_name->setMultiLine (false);
    edt_pgm_name->setReturnKeyStartsNewLine (false);
    edt_pgm_name->setReadOnly (false);
    edt_pgm_name->setScrollbarsShown (true);
    edt_pgm_name->setCaretVisible (true);
    edt_pgm_name->setPopupMenuEnabled (true);
    edt_pgm_name->setText (String());

    edt_pgm_name->setBounds (88, 106, 200, 24);

    edt_bank_name.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_name.get());
    edt_bank_name->setMultiLine (false);
    edt_bank_name->setReturnKeyStartsNewLine (false);
    edt_bank_name->setReadOnly (false);
    edt_bank_name->setScrollbarsShown (true);
    edt_bank_name->setCaretVisible (true);
    edt_bank_name->setPopupMenuEnabled (true);
    edt_bank_name->setText (String());

    edt_bank_name->setBounds (88, 34, 200, 24);

    label.reset (new Label ("new label",
                            TRANS("Program")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::aliceblue);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (8, 80, 64, 24);

    label2.reset (new Label ("new label",
                             TRANS("Bank")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::aliceblue);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (8, 8, 64, 24);

    edt_pgm_id.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_pgm_id.get());
    edt_pgm_id->setMultiLine (false);
    edt_pgm_id->setReturnKeyStartsNewLine (false);
    edt_pgm_id->setReadOnly (true);
    edt_pgm_id->setScrollbarsShown (true);
    edt_pgm_id->setCaretVisible (false);
    edt_pgm_id->setPopupMenuEnabled (true);
    edt_pgm_id->setText (TRANS("M000"));

    edt_pgm_id->setBounds (88, 80, 60, 24);

    edt_bank_id.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_id.get());
    edt_bank_id->setMultiLine (false);
    edt_bank_id->setReturnKeyStartsNewLine (false);
    edt_bank_id->setReadOnly (true);
    edt_bank_id->setScrollbarsShown (true);
    edt_bank_id->setCaretVisible (false);
    edt_bank_id->setPopupMenuEnabled (true);
    edt_bank_id->setText (TRANS("000:000"));

    edt_bank_id->setBounds (88, 8, 60, 24);

    btn_ok.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_ok.get());
    btn_ok->setButtonText (TRANS("OK"));
    btn_ok->addListener (this);

    btn_ok->setBounds (130, 150, 70, 24);

    btn_cancel.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_cancel.get());
    btn_cancel->setButtonText (TRANS("Cancel"));
    btn_cancel->addListener (this);

    btn_cancel->setBounds (218, 150, 70, 24);


    //[UserPreSize]
#if JUCE_MAC
    {
        Rectangle<int> bounds_ok = btn_ok->getBounds();
        Rectangle<int> bounds_cancel = btn_cancel->getBounds();
        btn_ok->setBounds(bounds_cancel);
        btn_cancel->setBounds(bounds_ok);
    }
#endif

    edt_bank_id->setJustification(Justification::centred);
    edt_pgm_id->setJustification(Justification::centred);

    Colour label_color = findColour(TextEditor::backgroundColourId).contrasting(0.5f);
    edt_bank_name->setTextToShowWhenEmpty("<Untitled bank>", label_color);
    edt_pgm_name->setTextToShowWhenEmpty("<Untitled program>", label_color);
    //[/UserPreSize]

    setSize (300, 182);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Program_Name_Editor::~Program_Name_Editor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    edt_pgm_name = nullptr;
    edt_bank_name = nullptr;
    label = nullptr;
    label2 = nullptr;
    edt_pgm_id = nullptr;
    edt_bank_id = nullptr;
    btn_ok = nullptr;
    btn_cancel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Program_Name_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Program_Name_Editor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Program_Name_Editor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_ok.get())
    {
        //[UserButtonCode_btn_ok] -- add your button handler code here..
        if (on_ok) {
            Result result;
            result.bank = bank_;
            result.pgm = pgm_;
            result.bank_name = edt_bank_name->getText();
            result.pgm_name = edt_pgm_name->getText();
            on_ok(result);
        }
        //[/UserButtonCode_btn_ok]
    }
    else if (buttonThatWasClicked == btn_cancel.get())
    {
        //[UserButtonCode_btn_cancel] -- add your button handler code here..
        if (on_cancel)
            on_cancel();
        //[/UserButtonCode_btn_cancel]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Program_Name_Editor::set_program(
    Bank_Id bank, unsigned pgm, const String &bank_name, const String &pgm_name)
{
    bank_ = bank;
    pgm_ = pgm;

    edt_bank_id->setText(fmt::format("{:03d}:{:03d}", bank.msb, bank.lsb));
    edt_pgm_id->setText(fmt::format("{:c}{:03d}", bank.percussive ? 'P' : 'M', pgm));
    edt_bank_name->setText(bank_name);
    edt_pgm_name->setText(pgm_name);

    const Midi_Program_Ex *ex = midi_db.find_ex(bank.msb, bank.lsb, pgm + (bank.percussive ? 128 : 0));
    const char *name = ex ? ex->name : bank.percussive ?
        midi_db.perc(pgm).name : midi_db.inst(pgm);

    Colour label_color = findColour(TextEditor::backgroundColourId).contrasting(0.5f);
    edt_pgm_name->setTextToShowWhenEmpty(name, label_color);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Program_Name_Editor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="1" initialWidth="300" initialHeight="182">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="79f3e8a0d408b06a" memberName="edt_pgm_name"
              virtualName="" explicitFocusOrder="0" pos="88 106 200 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="f55e80ae90a127d0" memberName="edt_bank_name"
              virtualName="" explicitFocusOrder="0" pos="88 34 200 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="160fefc52427eaeb" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 80 64 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="Program" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3165a7da9bc7b3e" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 8 64 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="Bank" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTEDITOR name="new text editor" id="3d833d75a4895d3e" memberName="edt_pgm_id"
              virtualName="" explicitFocusOrder="0" pos="88 80 60 24" initialText="M000"
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="c4c24f09e14b3d01" memberName="edt_bank_id"
              virtualName="" explicitFocusOrder="0" pos="88 8 60 24" initialText="000:000"
              multiline="0" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <TEXTBUTTON name="new button" id="a00526bae3e43a6a" memberName="btn_ok" virtualName=""
              explicitFocusOrder="0" pos="130 150 70 24" buttonText="OK" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d5f3c6d4a0271367" memberName="btn_cancel"
              virtualName="" explicitFocusOrder="0" pos="218 150 70 24" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
