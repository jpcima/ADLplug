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
#include <fmt/format.h>
//[/Headers]

#include "new_program_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
New_Program_Editor::New_Program_Editor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    label.reset (new Label ("new label",
                            TRANS("Program")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::aliceblue);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (8, 48, 64, 24);

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

    btn_ok.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_ok.get());
    btn_ok->setButtonText (TRANS("OK"));
    btn_ok->addListener (this);

    btn_ok->setBounds (130, 86, 70, 24);

    btn_cancel.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_cancel.get());
    btn_cancel->setButtonText (TRANS("Cancel"));
    btn_cancel->addListener (this);

    btn_cancel->setBounds (218, 86, 70, 24);

    cb_pgm_kind.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_pgm_kind.get());
    cb_pgm_kind->setEditableText (false);
    cb_pgm_kind->setJustificationType (Justification::centredLeft);
    cb_pgm_kind->setTextWhenNothingSelected (String());
    cb_pgm_kind->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_pgm_kind->addListener (this);

    cb_pgm_kind->setBounds (96, 48, 144, 24);

    edt_pgm_num.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_pgm_num.get());
    edt_pgm_num->setMultiLine (false);
    edt_pgm_num->setReturnKeyStartsNewLine (false);
    edt_pgm_num->setReadOnly (false);
    edt_pgm_num->setScrollbarsShown (true);
    edt_pgm_num->setCaretVisible (true);
    edt_pgm_num->setPopupMenuEnabled (true);
    edt_pgm_num->setText (TRANS("000"));

    edt_pgm_num->setBounds (248, 48, 40, 24);

    edt_bank_msb.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_msb.get());
    edt_bank_msb->setMultiLine (false);
    edt_bank_msb->setReturnKeyStartsNewLine (false);
    edt_bank_msb->setReadOnly (false);
    edt_bank_msb->setScrollbarsShown (true);
    edt_bank_msb->setCaretVisible (true);
    edt_bank_msb->setPopupMenuEnabled (true);
    edt_bank_msb->setText (TRANS("000"));

    edt_bank_msb->setBounds (96, 8, 40, 24);

    edt_bank_lsb.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_lsb.get());
    edt_bank_lsb->setMultiLine (false);
    edt_bank_lsb->setReturnKeyStartsNewLine (false);
    edt_bank_lsb->setReadOnly (false);
    edt_bank_lsb->setScrollbarsShown (true);
    edt_bank_lsb->setCaretVisible (true);
    edt_bank_lsb->setPopupMenuEnabled (true);
    edt_bank_lsb->setText (TRANS("000"));

    edt_bank_lsb->setBounds (152, 8, 40, 24);

    label3.reset (new Label ("new label",
                             TRANS(":")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::aliceblue);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (136, 8, 16, 24);


    //[UserPreSize]
#if JUCE_MAC
    {
        Rectangle<int> bounds_ok = btn_ok->getBounds();
        Rectangle<int> bounds_cancel = btn_cancel->getBounds();
        btn_ok->setBounds(bounds_cancel);
        btn_cancel->setBounds(bounds_ok);
    }
#endif

    edt_bank_msb->setJustification(Justification::centred);
    edt_bank_lsb->setJustification(Justification::centred);
    edt_pgm_num->setJustification(Justification::centred);

    cb_pgm_kind->addItem("Melodic", 1);
    cb_pgm_kind->addItem("Percussive", 2);
    cb_pgm_kind->setSelectedId(1);
    //[/UserPreSize]

    setSize (300, 120);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

New_Program_Editor::~New_Program_Editor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    label2 = nullptr;
    btn_ok = nullptr;
    btn_cancel = nullptr;
    cb_pgm_kind = nullptr;
    edt_pgm_num = nullptr;
    edt_bank_msb = nullptr;
    edt_bank_lsb = nullptr;
    label3 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void New_Program_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void New_Program_Editor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void New_Program_Editor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_ok.get())
    {
        //[UserButtonCode_btn_ok] -- add your button handler code here..
        if (on_ok) {
            auto to_uint7 =
                [](const char *str) -> unsigned {
                    unsigned result, count;
                    return (sscanf(str, "%u%n", &result, &count) == 1 &&
                            (result < 128) && count == strlen(str)) ? result : ~0u;
                };

            unsigned msb = to_uint7(edt_bank_msb->getText().toRawUTF8());
            unsigned lsb = to_uint7(edt_bank_lsb->getText().toRawUTF8());
            unsigned pgm = to_uint7(edt_pgm_num->getText().toRawUTF8());

            if (msb == ~0u || lsb == ~0u || pgm == ~0u)
                AlertWindow::showMessageBox(
                    AlertWindow::WarningIcon,
                    "Invalid value", "Identifiers must be integers between 0 and 127.");
            else {
                Result result;
                result.bank = Bank_Id(msb, lsb, cb_pgm_kind->getSelectedId() - 1);
                result.pgm = pgm;
                on_ok(result);
            }
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

void New_Program_Editor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cb_pgm_kind.get())
    {
        //[UserComboBoxCode_cb_pgm_kind] -- add your combo box handling code here..
        //[/UserComboBoxCode_cb_pgm_kind]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void New_Program_Editor::set_current(const Bank_Id &id, unsigned pgm)
{
    cb_pgm_kind->setSelectedId(id.percussive ? 2 : 1);
    edt_pgm_num->setText(fmt::format("{:03d}", pgm));
    edt_bank_msb->setText(fmt::format("{:03d}", id.msb));
    edt_bank_lsb->setText(fmt::format("{:03d}", id.lsb));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="New_Program_Editor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.33"
                 fixedSize="1" initialWidth="300" initialHeight="120">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="160fefc52427eaeb" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 48 64 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="Program" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="3165a7da9bc7b3e" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 8 64 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="Bank" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="a00526bae3e43a6a" memberName="btn_ok" virtualName=""
              explicitFocusOrder="0" pos="130 86 70 24" buttonText="OK" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="d5f3c6d4a0271367" memberName="btn_cancel"
              virtualName="" explicitFocusOrder="0" pos="218 86 70 24" buttonText="Cancel"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="8578b227b06134ff" memberName="cb_pgm_kind"
            virtualName="" explicitFocusOrder="0" pos="96 48 144 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTEDITOR name="new text editor" id="f55e80ae90a127d0" memberName="edt_pgm_num"
              virtualName="" explicitFocusOrder="0" pos="248 48 40 24" initialText="000"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="b0ab631d0df87485" memberName="edt_bank_msb"
              virtualName="" explicitFocusOrder="0" pos="96 8 40 24" initialText="000"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <TEXTEDITOR name="new text editor" id="4d0c7f4509556565" memberName="edt_bank_lsb"
              virtualName="" explicitFocusOrder="0" pos="152 8 40 24" initialText="000"
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
  <LABEL name="new label" id="f12ad2cea8128443" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="136 8 16 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText=":" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
