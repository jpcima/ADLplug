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

//[Headers] You can add your own extra header files here...
#include "ui/wave_label.h"
//[/Headers]

#include "operator_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Operator_Editor::Operator_Editor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (kn_attack = new Styled_Knob_Default());
    kn_attack->setName ("new component");

    kn_attack->setBounds (56, 8, 48, 48);

    addAndMakeVisible (kn_decay = new Styled_Knob_Default());
    kn_decay->setName ("new component");

    kn_decay->setBounds (104, 8, 48, 48);

    addAndMakeVisible (kn_sustain = new Styled_Knob_Default());
    kn_sustain->setName ("new component");

    kn_sustain->setBounds (152, 8, 48, 48);

    addAndMakeVisible (kn_release = new Styled_Knob_Default());
    kn_release->setName ("new component");

    kn_release->setBounds (200, 8, 48, 48);

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("<"));
    textButton->setConnectedEdges (Button::ConnectedOnRight);
    textButton->addListener (this);

    textButton->setBounds (161, 64, 23, 24);

    addAndMakeVisible (textButton2 = new TextButton ("new button"));
    textButton2->setButtonText (TRANS(">"));
    textButton2->setConnectedEdges (Button::ConnectedOnLeft);
    textButton2->addListener (this);

    textButton2->setBounds (184, 64, 23, 24);

    addAndMakeVisible (btn_trem = new TextButton ("new button"));
    btn_trem->setButtonText (TRANS("TREM"));
    btn_trem->setConnectedEdges (Button::ConnectedOnRight);
    btn_trem->addListener (this);
    btn_trem->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_trem->setBounds (32, 96, 48, 24);

    addAndMakeVisible (btn_vib = new TextButton ("new button"));
    btn_vib->setButtonText (TRANS("VIB"));
    btn_vib->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_vib->addListener (this);
    btn_vib->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_vib->setBounds (80, 96, 48, 24);

    addAndMakeVisible (btn_sus = new TextButton ("new button"));
    btn_sus->setButtonText (TRANS("SUS"));
    btn_sus->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_sus->addListener (this);
    btn_sus->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_sus->setBounds (128, 96, 48, 24);

    addAndMakeVisible (btn_env = new TextButton ("new button"));
    btn_env->setButtonText (TRANS("ENV"));
    btn_env->setConnectedEdges (Button::ConnectedOnLeft);
    btn_env->addListener (this);
    btn_env->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_env->setBounds (176, 96, 48, 24);

    addAndMakeVisible (sl_level = new Slider ("new slider"));
    sl_level->setRange (0, 10, 0);
    sl_level->setSliderStyle (Slider::LinearVertical);
    sl_level->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_level->addListener (this);

    sl_level->setBounds (264, 32, 24, 70);

    addAndMakeVisible (sl_fmul = new Slider ("new slider"));
    sl_fmul->setRange (0, 10, 0);
    sl_fmul->setSliderStyle (Slider::LinearVertical);
    sl_fmul->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_fmul->addListener (this);

    sl_fmul->setBounds (288, 32, 24, 70);

    addAndMakeVisible (sl_ksl = new Slider ("new slider"));
    sl_ksl->setRange (0, 10, 0);
    sl_ksl->setSliderStyle (Slider::LinearVertical);
    sl_ksl->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_ksl->addListener (this);

    sl_ksl->setBounds (312, 32, 24, 70);

    addAndMakeVisible (lb_optype = new Label ("new label",
                                              TRANS("Modulator")));
    lb_optype->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Bold"));
    lb_optype->setJustificationType (Justification::centred);
    lb_optype->setEditable (false, false, false);
    lb_optype->setColour (Label::textColourId, Colours::aliceblue);
    lb_optype->setColour (TextEditor::textColourId, Colours::black);
    lb_optype->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lb_optype->setBounds (264, 8, 78, 24);

    addAndMakeVisible (lbl_level = new Label ("new label",
                                              TRANS("Lv")));
    lbl_level->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_level->setJustificationType (Justification::centred);
    lbl_level->setEditable (false, false, false);
    lbl_level->setColour (TextEditor::textColourId, Colours::black);
    lbl_level->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_level->setBounds (260, 100, 28, 18);

    addAndMakeVisible (lbl_fmul = new Label ("new label",
                                             TRANS("F*")));
    lbl_fmul->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_fmul->setJustificationType (Justification::centred);
    lbl_fmul->setEditable (false, false, false);
    lbl_fmul->setColour (TextEditor::textColourId, Colours::black);
    lbl_fmul->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_fmul->setBounds (284, 100, 28, 18);

    addAndMakeVisible (lbl_ksl = new Label ("new label",
                                            TRANS("Ksl")));
    lbl_ksl->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_ksl->setJustificationType (Justification::centred);
    lbl_ksl->setEditable (false, false, false);
    lbl_ksl->setColour (TextEditor::textColourId, Colours::black);
    lbl_ksl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_ksl->setBounds (308, 100, 28, 18);

    addAndMakeVisible (lbl_wave = new Wave_Label());
    lbl_wave->setName ("new component");

    lbl_wave->setBounds (52, 64, 100, 24);


    //[UserPreSize]
    btn_trem->setClickingTogglesState(true);
    btn_vib->setClickingTogglesState(true);
    btn_sus->setClickingTogglesState(true);
    btn_env->setClickingTogglesState(true);

    kn_attack->setTooltip("Attack");
    kn_decay->setTooltip("Decay");
    kn_sustain->setTooltip("Sustain");
    kn_release->setTooltip("Release");

    btn_trem->setTooltip("Tremolo");
    btn_vib->setTooltip("Vibrato");
    btn_sus->setTooltip("Sustaining voice");
    btn_env->setTooltip("Envelope scale");

    sl_level->setTooltip("Level");
    lbl_level->setTooltip("Level");
    sl_fmul->setTooltip("Frequency multiplication");
    lbl_fmul->setTooltip("Frequency multiplication");
    sl_ksl->setTooltip("Key scale level");
    lbl_ksl->setTooltip("Key scale level");
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Operator_Editor::~Operator_Editor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    kn_attack = nullptr;
    kn_decay = nullptr;
    kn_sustain = nullptr;
    kn_release = nullptr;
    textButton = nullptr;
    textButton2 = nullptr;
    btn_trem = nullptr;
    btn_vib = nullptr;
    btn_sus = nullptr;
    btn_env = nullptr;
    sl_level = nullptr;
    sl_fmul = nullptr;
    sl_ksl = nullptr;
    lb_optype = nullptr;
    lbl_level = nullptr;
    lbl_fmul = nullptr;
    lbl_ksl = nullptr;
    lbl_wave = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Operator_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        int x = 0, y = 0, width = 352, height = 128;
        Colour fillColour = Colour (0xff2e4c4d);
        Colour strokeColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 8, y = 4, width = 36, height = 30;
        String text (TRANS("ADSR"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 8, y = 60, width = 36, height = 30;
        String text (TRANS("Wave"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 52, y = 64, width = 100, height = 24;
        Colour fillColour = Colour (0xff373737);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 264, y = 8, width = 78, height = 24;
        Colour fillColour = Colour (0xd3a52a2a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Operator_Editor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Operator_Editor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        //[/UserButtonCode_textButton2]
    }
    else if (buttonThatWasClicked == btn_trem)
    {
        //[UserButtonCode_btn_trem] -- add your button handler code here..
        //[/UserButtonCode_btn_trem]
    }
    else if (buttonThatWasClicked == btn_vib)
    {
        //[UserButtonCode_btn_vib] -- add your button handler code here..
        //[/UserButtonCode_btn_vib]
    }
    else if (buttonThatWasClicked == btn_sus)
    {
        //[UserButtonCode_btn_sus] -- add your button handler code here..
        //[/UserButtonCode_btn_sus]
    }
    else if (buttonThatWasClicked == btn_env)
    {
        //[UserButtonCode_btn_env] -- add your button handler code here..
        //[/UserButtonCode_btn_env]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Operator_Editor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_level)
    {
        //[UserSliderCode_sl_level] -- add your slider handling code here..
        //[/UserSliderCode_sl_level]
    }
    else if (sliderThatWasMoved == sl_fmul)
    {
        //[UserSliderCode_sl_fmul] -- add your slider handling code here..
        //[/UserSliderCode_sl_fmul]
    }
    else if (sliderThatWasMoved == sl_ksl)
    {
        //[UserSliderCode_sl_ksl] -- add your slider handling code here..
        //[/UserSliderCode_sl_ksl]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Operator_Editor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 352 128" fill="solid: ff2e4c4d" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: fff0f8ff"/>
    <TEXT pos="8 4 36 30" fill="solid: fff0f8ff" hasStroke="0" text="ADSR"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="8 60 36 30" fill="solid: fff0f8ff" hasStroke="0" text="Wave"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="33"/>
    <RECT pos="52 64 100 24" fill="solid: ff373737" hasStroke="0"/>
    <RECT pos="264 8 78 24" fill="solid: d3a52a2a" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="7c54ff103d9f5d" memberName="kn_attack"
                    virtualName="" explicitFocusOrder="0" pos="56 8 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="be39ad00dcf6efe1" memberName="kn_decay"
                    virtualName="" explicitFocusOrder="0" pos="104 8 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="8d88729c124c7b16" memberName="kn_sustain"
                    virtualName="" explicitFocusOrder="0" pos="152 8 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="7d576b68e9b588f" memberName="kn_release"
                    virtualName="" explicitFocusOrder="0" pos="200 8 48 48" class="Styled_Knob_Default"
                    params=""/>
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="161 64 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="184 64 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f60e70ed4f10ef32" memberName="btn_trem"
              virtualName="" explicitFocusOrder="0" pos="32 96 48 24" bgColOn="ff42a2c8"
              buttonText="TREM" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="501ccf7ad0bc53a7" memberName="btn_vib"
              virtualName="" explicitFocusOrder="0" pos="80 96 48 24" bgColOn="ff42a2c8"
              buttonText="VIB" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3e46dd6b966c40b2" memberName="btn_sus"
              virtualName="" explicitFocusOrder="0" pos="128 96 48 24" bgColOn="ff42a2c8"
              buttonText="SUS" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="eb8e9dfd42dd8f57" memberName="btn_env"
              virtualName="" explicitFocusOrder="0" pos="176 96 48 24" bgColOn="ff42a2c8"
              buttonText="ENV" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="b7065e7cd8f3e951" memberName="sl_level"
          virtualName="" explicitFocusOrder="0" pos="264 32 24 70" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="new slider" id="47c9497e72aa0068" memberName="sl_fmul"
          virtualName="" explicitFocusOrder="0" pos="288 32 24 70" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="new slider" id="7da3d626504f1592" memberName="sl_ksl" virtualName=""
          explicitFocusOrder="0" pos="312 32 24 70" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <LABEL name="new label" id="d5cf6971a21036bf" memberName="lb_optype"
         virtualName="" explicitFocusOrder="0" pos="264 8 78 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Modulator" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
         bold="1" italic="0" justification="36" typefaceStyle="Bold"/>
  <LABEL name="new label" id="ce54b68fc1a1f1e1" memberName="lbl_level"
         virtualName="" explicitFocusOrder="0" pos="260 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="Lv" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="1c179b730ba90c01" memberName="lbl_fmul"
         virtualName="" explicitFocusOrder="0" pos="284 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="F*" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="654b3ac6531d1941" memberName="lbl_ksl" virtualName=""
         explicitFocusOrder="0" pos="308 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="Ksl" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="dd16fb8d4c488877" memberName="lbl_wave"
                    virtualName="" explicitFocusOrder="0" pos="52 64 100 24" class="Wave_Label"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
