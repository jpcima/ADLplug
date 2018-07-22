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
#include "ui/wave_label.h"
#include "adl/instrument.h"
#include "parameter_block.h"
#include <cmath>
//[/Headers]

#include "operator_editor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Operator_Editor::Operator_Editor (unsigned op_id, Parameter_Block &pb)
{
    //[Constructor_pre] You can add your own custom stuff here..
    operator_id_ = op_id;
    parameter_block_ = &pb;
    //[/Constructor_pre]

    kn_attack.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_attack.get());
    kn_attack->setName ("new component");

    kn_attack->setBounds (56, 8, 48, 48);

    kn_decay.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_decay.get());
    kn_decay->setName ("new component");

    kn_decay->setBounds (104, 8, 48, 48);

    kn_sustain.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_sustain.get());
    kn_sustain->setName ("new component");

    kn_sustain->setBounds (152, 8, 48, 48);

    kn_release.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_release.get());
    kn_release->setName ("new component");

    kn_release->setBounds (200, 8, 48, 48);

    btn_prev_wave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_prev_wave.get());
    btn_prev_wave->setButtonText (TRANS("<"));
    btn_prev_wave->setConnectedEdges (Button::ConnectedOnRight);
    btn_prev_wave->addListener (this);

    btn_prev_wave->setBounds (161, 64, 23, 24);

    btn_next_wave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_next_wave.get());
    btn_next_wave->setButtonText (TRANS(">"));
    btn_next_wave->setConnectedEdges (Button::ConnectedOnLeft);
    btn_next_wave->addListener (this);

    btn_next_wave->setBounds (183, 64, 23, 24);

    btn_trem.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_trem.get());
    btn_trem->setButtonText (TRANS("TREM"));
    btn_trem->setConnectedEdges (Button::ConnectedOnRight);
    btn_trem->addListener (this);
    btn_trem->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_trem->setBounds (32, 96, 48, 24);

    btn_vib.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_vib.get());
    btn_vib->setButtonText (TRANS("VIB"));
    btn_vib->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_vib->addListener (this);
    btn_vib->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_vib->setBounds (79, 96, 48, 24);

    btn_sus.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_sus.get());
    btn_sus->setButtonText (TRANS("SUS"));
    btn_sus->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_sus->addListener (this);
    btn_sus->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_sus->setBounds (126, 96, 48, 24);

    btn_env.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_env.get());
    btn_env->setButtonText (TRANS("ENV"));
    btn_env->setConnectedEdges (Button::ConnectedOnLeft);
    btn_env->addListener (this);
    btn_env->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_env->setBounds (173, 96, 48, 24);

    sl_level.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_level.get());
    sl_level->setRange (0, 63, 0);
    sl_level->setSliderStyle (Slider::LinearVertical);
    sl_level->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_level->addListener (this);

    sl_level->setBounds (264, 32, 24, 70);

    sl_fmul.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_fmul.get());
    sl_fmul->setRange (0, 15, 0);
    sl_fmul->setSliderStyle (Slider::LinearVertical);
    sl_fmul->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_fmul->addListener (this);

    sl_fmul->setBounds (288, 32, 24, 70);

    sl_ksl.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_ksl.get());
    sl_ksl->setRange (0, 3, 0);
    sl_ksl->setSliderStyle (Slider::LinearVertical);
    sl_ksl->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sl_ksl->addListener (this);

    sl_ksl->setBounds (312, 32, 24, 70);

    lb_optype.reset (new Label ("new label",
                                TRANS("Modulator")));
    addAndMakeVisible (lb_optype.get());
    lb_optype->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Bold"));
    lb_optype->setJustificationType (Justification::centred);
    lb_optype->setEditable (false, false, false);
    lb_optype->setColour (Label::textColourId, Colours::aliceblue);
    lb_optype->setColour (TextEditor::textColourId, Colours::black);
    lb_optype->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lb_optype->setBounds (264, 8, 78, 24);

    lbl_level.reset (new Label ("new label",
                                TRANS("Lv")));
    addAndMakeVisible (lbl_level.get());
    lbl_level->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_level->setJustificationType (Justification::centred);
    lbl_level->setEditable (false, false, false);
    lbl_level->setColour (TextEditor::textColourId, Colours::black);
    lbl_level->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_level->setBounds (260, 100, 28, 18);

    lbl_fmul.reset (new Label ("new label",
                               TRANS("F*")));
    addAndMakeVisible (lbl_fmul.get());
    lbl_fmul->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_fmul->setJustificationType (Justification::centred);
    lbl_fmul->setEditable (false, false, false);
    lbl_fmul->setColour (TextEditor::textColourId, Colours::black);
    lbl_fmul->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_fmul->setBounds (284, 100, 28, 18);

    lbl_ksl.reset (new Label ("new label",
                              TRANS("Ksl")));
    addAndMakeVisible (lbl_ksl.get());
    lbl_ksl->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_ksl->setJustificationType (Justification::centred);
    lbl_ksl->setEditable (false, false, false);
    lbl_ksl->setColour (TextEditor::textColourId, Colours::black);
    lbl_ksl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_ksl->setBounds (308, 100, 28, 18);

    lbl_wave.reset (new Wave_Label());
    addAndMakeVisible (lbl_wave.get());
    lbl_wave->setName ("new component");

    lbl_wave->setBounds (52, 64, 100, 24);


    //[UserPreSize]
    kn_attack->add_listener(this);
    kn_decay->add_listener(this);
    kn_sustain->add_listener(this);
    kn_release->add_listener(this);

    btn_trem->setClickingTogglesState(true);
    btn_vib->setClickingTogglesState(true);
    btn_sus->setClickingTogglesState(true);
    btn_env->setClickingTogglesState(true);

    kn_attack->setTooltip(TRANS("Attack"));
    kn_decay->setTooltip(TRANS("Decay"));
    kn_sustain->setTooltip(TRANS("Sustain"));
    kn_release->setTooltip(TRANS("Release"));

    btn_trem->setTooltip(TRANS("Tremolo"));
    btn_vib->setTooltip(TRANS("Vibrato"));
    btn_sus->setTooltip(TRANS("Sustaining voice"));
    btn_env->setTooltip(TRANS("Envelope scale"));

    sl_level->setTooltip(TRANS("Level"));
    lbl_level->setTooltip(TRANS("Level"));
    sl_fmul->setTooltip(TRANS("Frequency multiplication"));
    lbl_fmul->setTooltip(TRANS("Frequency multiplication"));
    sl_ksl->setTooltip(TRANS("Key scale level"));
    lbl_ksl->setTooltip(TRANS("Key scale level"));
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    kn_attack->set_range(0, 15);
    kn_decay->set_range(0, 15);
    kn_sustain->set_range(0, 15);
    kn_release->set_range(0, 15);
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
    btn_prev_wave = nullptr;
    btn_next_wave = nullptr;
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
        int x = 8, y = 4, width = 40, height = 30;
        String text (TRANS("ADSR"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 8, y = 60, width = 40, height = 30;
        String text (TRANS("Wave"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
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
        float x = 264.0f, y = 8.0f, width = 78.0f, height = 24.0f;
        Colour fillColour = Colour (0xd3a52a2a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 3.0f);
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
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Operator &op = pb.nth_operator(operator_id_);
    Button *btn = buttonThatWasClicked;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_prev_wave.get())
    {
        //[UserButtonCode_btn_prev_wave] -- add your button handler code here..
        AudioParameterInt &p = *op.p_wave;
        p.beginChangeGesture();
        int wave = p.getRange().clipValue(p.get() - 1);
        p = wave;
        p.endChangeGesture();
        lbl_wave->set_wave(wave, dontSendNotification);
        //[/UserButtonCode_btn_prev_wave]
    }
    else if (buttonThatWasClicked == btn_next_wave.get())
    {
        //[UserButtonCode_btn_next_wave] -- add your button handler code here..
        AudioParameterInt &p = *op.p_wave;
        p.beginChangeGesture();
        int wave = p.getRange().clipValue(p.get() + 1);
        p = wave;
        p.endChangeGesture();
        lbl_wave->set_wave(wave, dontSendNotification);
        //[/UserButtonCode_btn_next_wave]
    }
    else if (buttonThatWasClicked == btn_trem.get())
    {
        //[UserButtonCode_btn_trem] -- add your button handler code here..
        AudioParameterBool &p = *op.p_trem;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_trem]
    }
    else if (buttonThatWasClicked == btn_vib.get())
    {
        //[UserButtonCode_btn_vib] -- add your button handler code here..
        AudioParameterBool &p = *op.p_vib;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_vib]
    }
    else if (buttonThatWasClicked == btn_sus.get())
    {
        //[UserButtonCode_btn_sus] -- add your button handler code here..
        AudioParameterBool &p = *op.p_sus;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_sus]
    }
    else if (buttonThatWasClicked == btn_env.get())
    {
        //[UserButtonCode_btn_env] -- add your button handler code here..
        AudioParameterBool &p = *op.p_env;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_env]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Operator_Editor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Operator &op = pb.nth_operator(operator_id_);
    Slider *sl = sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_level.get())
    {
        //[UserSliderCode_sl_level] -- add your slider handling code here..
        AudioParameterInt &p = *op.p_level;
        p.beginChangeGesture();
        p = std::lround(sl->getValue());
        p.endChangeGesture();
        //[/UserSliderCode_sl_level]
    }
    else if (sliderThatWasMoved == sl_fmul.get())
    {
        //[UserSliderCode_sl_fmul] -- add your slider handling code here..
        AudioParameterInt &p = *op.p_fmul;
        p.beginChangeGesture();
        p = std::lround(sl->getValue());
        p.endChangeGesture();
        //[/UserSliderCode_sl_fmul]
    }
    else if (sliderThatWasMoved == sl_ksl.get())
    {
        //[UserSliderCode_sl_ksl] -- add your slider handling code here..
        AudioParameterInt &p = *op.p_ksl;
        p.beginChangeGesture();
        p = std::lround(sl->getValue());
        p.endChangeGesture();
        //[/UserSliderCode_sl_ksl]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Operator_Editor::set_operator_parameters(const Instrument &ins, unsigned op, NotificationType ntf)
{
    kn_attack->set_value(ins.attack(op), ntf);
    kn_decay->set_value(ins.decay(op), ntf);
    kn_sustain->set_value(ins.sustain(op), ntf);
    kn_release->set_value(ins.release(op), ntf);

    sl_level->setValue(ins.level(op), ntf);
    sl_fmul->setValue(ins.fmul(op), ntf);
    sl_ksl->setValue(ins.ksl(op), ntf);

    btn_trem->setToggleState(ins.trem(op), ntf);
    btn_vib->setToggleState(ins.vib(op), ntf);
    btn_sus->setToggleState(ins.sus(op), ntf);
    btn_env->setToggleState(ins.env(op), ntf);

    lbl_wave->set_wave(ins.wave(op), ntf);
}

void Operator_Editor::set_operator_enabled(bool b)
{
    if (b == operator_enabled_)
        return;

    operator_enabled_ = b;
    repaint();
}

void Operator_Editor::knob_value_changed(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Operator &op = pb.nth_operator(operator_id_);

    if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p.beginChangeGesture();
        p = k->value();
        p.endChangeGesture();
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay;
        p.beginChangeGesture();
        p = std::lround(k->value());
        p.endChangeGesture();
    }
    else if (k == kn_sustain.get()) {
        AudioParameterInt &p = *op.p_sustain;
        p.beginChangeGesture();
        p = std::lround(k->value());
        p.endChangeGesture();
    }
    else if (k == kn_release.get()) {
        AudioParameterInt &p = *op.p_release;
        p.beginChangeGesture();
        p = std::lround(k->value());
        p.endChangeGesture();
    }
}

void Operator_Editor::paintOverChildren(Graphics &g)
{
    if (!operator_enabled_)
        g.fillAll(Colour(0x66aaaaaa));
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Operator_Editor" componentName=""
                 parentClasses="public Component, public Knob::Listener" constructorParams="unsigned op_id, Parameter_Block &amp;pb"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.33" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 352 128" fill="solid: ff2e4c4d" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: fff0f8ff"/>
    <TEXT pos="8 4 40 30" fill="solid: fff0f8ff" hasStroke="0" text="ADSR"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="8 60 40 30" fill="solid: fff0f8ff" hasStroke="0" text="Wave"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <RECT pos="52 64 100 24" fill="solid: ff373737" hasStroke="0"/>
    <ROUNDRECT pos="264 8 78 24" cornerSize="3.0" fill="solid: d3a52a2a" hasStroke="0"/>
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
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="btn_prev_wave"
              virtualName="" explicitFocusOrder="0" pos="161 64 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="btn_next_wave"
              virtualName="" explicitFocusOrder="0" pos="183 64 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f60e70ed4f10ef32" memberName="btn_trem"
              virtualName="" explicitFocusOrder="0" pos="32 96 48 24" bgColOn="ff42a2c8"
              buttonText="TREM" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="501ccf7ad0bc53a7" memberName="btn_vib"
              virtualName="" explicitFocusOrder="0" pos="79 96 48 24" bgColOn="ff42a2c8"
              buttonText="VIB" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3e46dd6b966c40b2" memberName="btn_sus"
              virtualName="" explicitFocusOrder="0" pos="126 96 48 24" bgColOn="ff42a2c8"
              buttonText="SUS" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="eb8e9dfd42dd8f57" memberName="btn_env"
              virtualName="" explicitFocusOrder="0" pos="173 96 48 24" bgColOn="ff42a2c8"
              buttonText="ENV" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="b7065e7cd8f3e951" memberName="sl_level"
          virtualName="" explicitFocusOrder="0" pos="264 32 24 70" min="0.0"
          max="63.0" int="0.0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="47c9497e72aa0068" memberName="sl_fmul"
          virtualName="" explicitFocusOrder="0" pos="288 32 24 70" min="0.0"
          max="15.0" int="0.0" style="LinearVertical" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="7da3d626504f1592" memberName="sl_ksl" virtualName=""
          explicitFocusOrder="0" pos="312 32 24 70" min="0.0" max="3.0"
          int="0.0" style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="d5cf6971a21036bf" memberName="lb_optype"
         virtualName="" explicitFocusOrder="0" pos="264 8 78 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Modulator" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="1" italic="0" justification="36"
         typefaceStyle="Bold"/>
  <LABEL name="new label" id="ce54b68fc1a1f1e1" memberName="lbl_level"
         virtualName="" explicitFocusOrder="0" pos="260 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="Lv" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="1c179b730ba90c01" memberName="lbl_fmul"
         virtualName="" explicitFocusOrder="0" pos="284 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="F*" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="654b3ac6531d1941" memberName="lbl_ksl" virtualName=""
         explicitFocusOrder="0" pos="308 100 28 18" edTextCol="ff000000"
         edBkgCol="0" labelText="Ksl" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="dd16fb8d4c488877" memberName="lbl_wave"
                    virtualName="" explicitFocusOrder="0" pos="52 64 100 24" class="Wave_Label"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
