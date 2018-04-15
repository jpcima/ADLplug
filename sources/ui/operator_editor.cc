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
//[/Headers]

#include "operator_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Operator_Editor::Operator_Editor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (component = new Styled_Knob_Default());
    component->setName ("new component");

    component->setBounds (8, 32, 48, 48);

    addAndMakeVisible (component2 = new Styled_Knob_Default());
    component2->setName ("new component");

    component2->setBounds (56, 32, 48, 48);

    addAndMakeVisible (component3 = new Styled_Knob_Default());
    component3->setName ("new component");

    component3->setBounds (104, 32, 48, 48);

    addAndMakeVisible (component4 = new Styled_Knob_Default());
    component4->setName ("new component");

    component4->setBounds (152, 32, 48, 48);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("label text")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (56, 88, 88, 24);

    addAndMakeVisible (textButton = new TextButton ("new button"));
    textButton->setButtonText (TRANS("<"));
    textButton->setConnectedEdges (Button::ConnectedOnRight);
    textButton->addListener (this);

    textButton->setBounds (161, 90, 23, 24);

    addAndMakeVisible (textButton2 = new TextButton ("new button"));
    textButton2->setButtonText (TRANS(">"));
    textButton2->setConnectedEdges (Button::ConnectedOnLeft);
    textButton2->addListener (this);

    textButton2->setBounds (184, 90, 23, 24);

    addAndMakeVisible (textButton3 = new TextButton ("new button"));
    textButton3->setButtonText (TRANS("TREM"));
    textButton3->setConnectedEdges (Button::ConnectedOnRight);
    textButton3->addListener (this);

    textButton3->setBounds (8, 136, 48, 24);

    addAndMakeVisible (textButton4 = new TextButton ("new button"));
    textButton4->setButtonText (TRANS("VIB"));
    textButton4->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    textButton4->addListener (this);

    textButton4->setBounds (56, 136, 48, 24);

    addAndMakeVisible (textButton5 = new TextButton ("new button"));
    textButton5->setButtonText (TRANS("SUS"));
    textButton5->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    textButton5->addListener (this);

    textButton5->setBounds (104, 136, 48, 24);

    addAndMakeVisible (textButton6 = new TextButton ("new button"));
    textButton6->setButtonText (TRANS("ENV"));
    textButton6->setConnectedEdges (Button::ConnectedOnLeft);
    textButton6->addListener (this);

    textButton6->setBounds (152, 136, 48, 24);

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 10, 0);
    slider->setSliderStyle (Slider::LinearVertical);
    slider->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider->addListener (this);

    slider->setBounds (224, 32, 24, 96);

    addAndMakeVisible (slider2 = new Slider ("new slider"));
    slider2->setRange (0, 10, 0);
    slider2->setSliderStyle (Slider::LinearVertical);
    slider2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider2->addListener (this);

    slider2->setBounds (256, 32, 24, 96);

    addAndMakeVisible (slider3 = new Slider ("new slider"));
    slider3->setRange (0, 10, 0);
    slider3->setSliderStyle (Slider::LinearVertical);
    slider3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    slider3->addListener (this);

    slider3->setBounds (288, 32, 24, 96);

    addAndMakeVisible (lb_optype = new Label ("new label",
                                              TRANS("Modulator")));
    lb_optype->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    lb_optype->setJustificationType (Justification::centred);
    lb_optype->setEditable (false, false, false);
    lb_optype->setColour (TextEditor::textColourId, Colours::black);
    lb_optype->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lb_optype->setBounds (240, 8, 78, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Operator_Editor::~Operator_Editor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    component = nullptr;
    component2 = nullptr;
    component3 = nullptr;
    component4 = nullptr;
    label = nullptr;
    textButton = nullptr;
    textButton2 = nullptr;
    textButton3 = nullptr;
    textButton4 = nullptr;
    textButton5 = nullptr;
    textButton6 = nullptr;
    slider = nullptr;
    slider2 = nullptr;
    slider3 = nullptr;
    lb_optype = nullptr;


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
        int x = 0, y = 0, width = 324, height = 172;
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
        int x = 4, y = 4, width = 36, height = 30;
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
        int x = 4, y = 84, width = 36, height = 30;
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
        int x = 52, y = 92, width = 100, height = 20;
        Colour fillColour = Colour (0xff373737);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 220, y = 124, width = 28, height = 18;
        String text (TRANS("Lv"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 252, y = 124, width = 28, height = 18;
        String text (TRANS("F*"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 284, y = 124, width = 28, height = 18;
        String text (TRANS("Eg"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 240, y = 8, width = 78, height = 24;
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
    else if (buttonThatWasClicked == textButton3)
    {
        //[UserButtonCode_textButton3] -- add your button handler code here..
        //[/UserButtonCode_textButton3]
    }
    else if (buttonThatWasClicked == textButton4)
    {
        //[UserButtonCode_textButton4] -- add your button handler code here..
        //[/UserButtonCode_textButton4]
    }
    else if (buttonThatWasClicked == textButton5)
    {
        //[UserButtonCode_textButton5] -- add your button handler code here..
        //[/UserButtonCode_textButton5]
    }
    else if (buttonThatWasClicked == textButton6)
    {
        //[UserButtonCode_textButton6] -- add your button handler code here..
        //[/UserButtonCode_textButton6]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Operator_Editor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        //[/UserSliderCode_slider]
    }
    else if (sliderThatWasMoved == slider2)
    {
        //[UserSliderCode_slider2] -- add your slider handling code here..
        //[/UserSliderCode_slider2]
    }
    else if (sliderThatWasMoved == slider3)
    {
        //[UserSliderCode_slider3] -- add your slider handling code here..
        //[/UserSliderCode_slider3]
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
    <RECT pos="0 0 324 172" fill="solid: ff2e4c4d" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: fff0f8ff"/>
    <TEXT pos="4 4 36 30" fill="solid: fff0f8ff" hasStroke="0" text="ADSR"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="33"/>
    <TEXT pos="4 84 36 30" fill="solid: fff0f8ff" hasStroke="0" text="Wave"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="33"/>
    <RECT pos="52 92 100 20" fill="solid: ff373737" hasStroke="0"/>
    <TEXT pos="220 124 28 18" fill="solid: fff0f8ff" hasStroke="0" text="Lv"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="252 124 28 18" fill="solid: fff0f8ff" hasStroke="0" text="F*"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="36"/>
    <TEXT pos="284 124 28 18" fill="solid: fff0f8ff" hasStroke="0" text="Eg"
          fontname="Default font" fontsize="15.00000000000000000000" kerning="0.00000000000000000000"
          bold="0" italic="0" justification="36"/>
    <RECT pos="240 8 78 24" fill="solid: d3a52a2a" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="7c54ff103d9f5d" memberName="component"
                    virtualName="" explicitFocusOrder="0" pos="8 32 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="be39ad00dcf6efe1" memberName="component2"
                    virtualName="" explicitFocusOrder="0" pos="56 32 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="8d88729c124c7b16" memberName="component3"
                    virtualName="" explicitFocusOrder="0" pos="104 32 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="7d576b68e9b588f" memberName="component4"
                    virtualName="" explicitFocusOrder="0" pos="152 32 48 48" class="Styled_Knob_Default"
                    params=""/>
  <LABEL name="new label" id="78c50cdc26d2ebfe" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="56 88 88 24" edTextCol="ff000000"
         edBkgCol="0" labelText="label text" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="161 90 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="184 90 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f60e70ed4f10ef32" memberName="textButton3"
              virtualName="" explicitFocusOrder="0" pos="8 136 48 24" buttonText="TREM"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="501ccf7ad0bc53a7" memberName="textButton4"
              virtualName="" explicitFocusOrder="0" pos="56 136 48 24" buttonText="VIB"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3e46dd6b966c40b2" memberName="textButton5"
              virtualName="" explicitFocusOrder="0" pos="104 136 48 24" buttonText="SUS"
              connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="eb8e9dfd42dd8f57" memberName="textButton6"
              virtualName="" explicitFocusOrder="0" pos="152 136 48 24" buttonText="ENV"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="b7065e7cd8f3e951" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="224 32 24 96" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="new slider" id="47c9497e72aa0068" memberName="slider2"
          virtualName="" explicitFocusOrder="0" pos="256 32 24 96" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <SLIDER name="new slider" id="7da3d626504f1592" memberName="slider3"
          virtualName="" explicitFocusOrder="0" pos="288 32 24 96" min="0.00000000000000000000"
          max="10.00000000000000000000" int="0.00000000000000000000" style="LinearVertical"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.00000000000000000000" needsCallback="1"/>
  <LABEL name="new label" id="d5cf6971a21036bf" memberName="lb_optype"
         virtualName="" explicitFocusOrder="0" pos="240 8 78 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Modulator" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.00000000000000000000"
         kerning="0.00000000000000000000" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
