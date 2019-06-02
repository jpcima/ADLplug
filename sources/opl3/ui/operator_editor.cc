/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ui/components/wave_label.h"
#include "ui/components/info_display.h"
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

    kn_attack->setBounds (24, 3, 48, 48);

    kn_decay.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_decay.get());
    kn_decay->setName ("new component");

    kn_decay->setBounds (96, 3, 48, 48);

    kn_sustain.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_sustain.get());
    kn_sustain->setName ("new component");

    kn_sustain->setBounds (24, 48, 48, 48);

    kn_release.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_release.get());
    kn_release->setName ("new component");

    kn_release->setBounds (96, 48, 48, 48);

    btn_prev_wave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_prev_wave.get());
    btn_prev_wave->setButtonText (TRANS("<"));
    btn_prev_wave->setConnectedEdges (Button::ConnectedOnRight);
    btn_prev_wave->addListener (this);

    btn_prev_wave->setBounds (3, 96, 23, 24);

    btn_next_wave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_next_wave.get());
    btn_next_wave->setButtonText (TRANS(">"));
    btn_next_wave->setConnectedEdges (Button::ConnectedOnLeft);
    btn_next_wave->addListener (this);

    btn_next_wave->setBounds (132, 96, 23, 24);

    btn_trem.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_trem.get());
    btn_trem->setButtonText (String());
    btn_trem->addListener (this);
    btn_trem->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_trem->setBounds (168, 3, 15, 15);

    btn_vib.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_vib.get());
    btn_vib->setButtonText (String());
    btn_vib->addListener (this);
    btn_vib->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_vib->setBounds (168, 20, 15, 15);

    btn_sus.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_sus.get());
    btn_sus->setButtonText (String());
    btn_sus->addListener (this);
    btn_sus->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_sus->setBounds (168, 37, 15, 15);

    btn_env.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_env.get());
    btn_env->setButtonText (String());
    btn_env->addListener (this);
    btn_env->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_env->setBounds (168, 54, 15, 15);

    lbl_level.reset (new Label ("new label",
                                TRANS("Lv")));
    addAndMakeVisible (lbl_level.get());
    lbl_level->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_level->setJustificationType (Justification::centredLeft);
    lbl_level->setEditable (false, false, false);
    lbl_level->setColour (Label::textColourId, Colours::aliceblue);
    lbl_level->setColour (TextEditor::textColourId, Colours::black);
    lbl_level->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_level->setBounds (163, 72, 28, 16);

    lbl_wave.reset (new Wave_Label (chip_waves_));
    addAndMakeVisible (lbl_wave.get());
    lbl_wave->setName ("new component");

    lbl_wave->setBounds (26, 96, 106, 24);

    label.reset (new Label ("new label",
                            TRANS("A")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredTop);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::aliceblue);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (4, 0, 20, 16);

    label2.reset (new Label ("new label",
                             TRANS("D")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredTop);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::aliceblue);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (76, 0, 20, 16);

    label3.reset (new Label ("new label",
                             TRANS("S")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredTop);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::aliceblue);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (4, 48, 20, 16);

    label4.reset (new Label ("new label",
                             TRANS("R")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredTop);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colours::aliceblue);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (76, 48, 20, 16);

    label5.reset (new Label ("new label",
                             TRANS("Tremolo")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (Label::textColourId, Colours::aliceblue);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (184, 3, 80, 15);

    label6.reset (new Label ("new label",
                             TRANS("Vibrato")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (Label::textColourId, Colours::aliceblue);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (184, 20, 80, 15);

    label7.reset (new Label ("new label",
                             TRANS("Sustain")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (Label::textColourId, Colours::aliceblue);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (184, 37, 80, 15);

    label8.reset (new Label ("new label",
                             TRANS("Key scaling")));
    addAndMakeVisible (label8.get());
    label8->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (Label::textColourId, Colours::aliceblue);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (184, 54, 80, 15);

    lbl_fmul.reset (new Label ("new label",
                               TRANS("F*")));
    addAndMakeVisible (lbl_fmul.get());
    lbl_fmul->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_fmul->setJustificationType (Justification::centredLeft);
    lbl_fmul->setEditable (false, false, false);
    lbl_fmul->setColour (Label::textColourId, Colours::aliceblue);
    lbl_fmul->setColour (TextEditor::textColourId, Colours::black);
    lbl_fmul->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_fmul->setBounds (163, 88, 28, 16);

    lbl_ksl.reset (new Label ("new label",
                              TRANS("Ksl")));
    addAndMakeVisible (lbl_ksl.get());
    lbl_ksl->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_ksl->setJustificationType (Justification::centredLeft);
    lbl_ksl->setEditable (false, false, false);
    lbl_ksl->setColour (Label::textColourId, Colours::aliceblue);
    lbl_ksl->setColour (TextEditor::textColourId, Colours::black);
    lbl_ksl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_ksl->setBounds (163, 104, 28, 16);

    sl_level.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_level.get());
    sl_level->setName ("new component");

    sl_level->setBounds (195, 70, 64, 20);

    sl_fmul.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_fmul.get());
    sl_fmul->setName ("new component");

    sl_fmul->setBounds (195, 86, 64, 20);

    sl_ksl.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_ksl.get());
    sl_ksl->setName ("new component");

    sl_ksl->setBounds (195, 102, 64, 20);


    //[UserPreSize]
    sl_level->add_listener(this);
    sl_level->set_range(0, 63);
    sl_level->set_max_increment(1);
    sl_fmul->add_listener(this);
    sl_fmul->set_range(0, 15);
    sl_fmul->set_max_increment(1);
    sl_ksl->add_listener(this);
    sl_ksl->set_range(0, 3);
    sl_ksl->set_max_increment(1);

    kn_attack->add_listener(this);
    kn_attack->set_max_increment(1);
    kn_decay->add_listener(this);
    kn_decay->set_max_increment(1);
    kn_sustain->add_listener(this);
    kn_sustain->set_max_increment(1);
    kn_release->add_listener(this);
    kn_release->set_max_increment(1);

    btn_trem->setClickingTogglesState(true);
    btn_vib->setClickingTogglesState(true);
    btn_sus->setClickingTogglesState(true);
    btn_env->setClickingTogglesState(true);
    //[/UserPreSize]

    setSize (264, 128);


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
    lbl_level = nullptr;
    lbl_wave = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    label5 = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    label8 = nullptr;
    lbl_fmul = nullptr;
    lbl_ksl = nullptr;
    sl_level = nullptr;
    sl_fmul = nullptr;
    sl_ksl = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Operator_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    {
        int x = 25, y = 96, width = 108, height = 24;
        Colour strokeColour = Colour (0xff8e989b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        float x = 0.0f, y = 0.0f, width = 264.0f, height = 128.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
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
    Parameter_Block::Part &part = pb.part[midichannel_];
    Parameter_Block::Operator &op = part.nth_operator(operator_id_);
    Button *btn = buttonThatWasClicked;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_prev_wave.get())
    {
        //[UserButtonCode_btn_prev_wave] -- add your button handler code here..
        AudioParameterChoice &p = *op.p_wave;
        p.beginChangeGesture();
        int wave = std::max(p.getIndex() - 1, 0);
        p = wave;
        p.endChangeGesture();
        lbl_wave->set_wave(wave, dontSendNotification);
        //[/UserButtonCode_btn_prev_wave]
    }
    else if (buttonThatWasClicked == btn_next_wave.get())
    {
        //[UserButtonCode_btn_next_wave] -- add your button handler code here..
        AudioParameterChoice &p = *op.p_wave;
        p.beginChangeGesture();
        int wave = std::min(p.getIndex() + 1, p.choices.size() - 1);
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
    if (display_info_for_component(btn))
        info_->expire_info_in();
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
static unsigned swap_ksl(unsigned ksl)
{
    // OPL mapping for KSL (dB/oct): 0=>0, 1=>3, 2=>1.5, 3=>6
    // for user-friendly ordering, the UI control will swap 1 and 2
    switch (ksl) {
    case 1: return 2;
    case 2: return 1;
    default: return ksl;
    }
}

void Operator_Editor::set_operator_parameters(const Instrument &ins, unsigned op, NotificationType ntf)
{
    kn_attack->set_value(ins.attack(op), ntf);
    kn_decay->set_value(ins.decay(op), ntf);
    kn_sustain->set_value(ins.sustain(op), ntf);
    kn_release->set_value(ins.release(op), ntf);

    sl_level->set_value(ins.level(op), ntf);
    sl_fmul->set_value(ins.fmul(op), ntf);
    sl_ksl->set_value(swap_ksl(ins.ksl(op)), ntf);

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
    Parameter_Block::Part &part = pb.part[midichannel_];
    Parameter_Block::Operator &op = part.nth_operator(operator_id_);

    if (k == sl_level.get()) {
        AudioParameterInt &p = *op.p_level;
        p = (int)std::lround(k->value());
    }
    else if (k == sl_fmul.get()) {
        AudioParameterInt &p = *op.p_fmul;
        p = (int)std::lround(k->value());
    }
    else if (k == sl_ksl.get()) {
        AudioParameterInt &p = *op.p_ksl;
        p = swap_ksl((int)std::lround(k->value()));
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_sustain.get()) {
        AudioParameterInt &p = *op.p_sustain;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_release.get()) {
        AudioParameterInt &p = *op.p_release;
        p = (int)std::lround(k->value());
    }

    display_info_for_component(k);
}

void Operator_Editor::knob_drag_started(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Parameter_Block::Operator &op = part.nth_operator(operator_id_);

    if (k == sl_level.get()) {
        AudioParameterInt &p = *op.p_level;
        p.beginChangeGesture();
    }
    else if (k == sl_fmul.get()) {
        AudioParameterInt &p = *op.p_fmul;
        p.beginChangeGesture();
    }
    else if (k == sl_ksl.get()) {
        AudioParameterInt &p = *op.p_ksl;
        p.beginChangeGesture();
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p.beginChangeGesture();
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay;
        p.beginChangeGesture();
    }
    else if (k == kn_sustain.get()) {
        AudioParameterInt &p = *op.p_sustain;
        p.beginChangeGesture();
    }
    else if (k == kn_release.get()) {
        AudioParameterInt &p = *op.p_release;
        p.beginChangeGesture();
    }

    display_info_for_component(k);
}

void Operator_Editor::knob_drag_ended(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Parameter_Block::Operator &op = part.nth_operator(operator_id_);

    if (k == sl_level.get()) {
        AudioParameterInt &p = *op.p_level;
        p.endChangeGesture();
    }
    else if (k == sl_fmul.get()) {
        AudioParameterInt &p = *op.p_fmul;
        p.endChangeGesture();
    }
    else if (k == sl_ksl.get()) {
        AudioParameterInt &p = *op.p_ksl;
        p.endChangeGesture();
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p.endChangeGesture();
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay;
        p.endChangeGesture();
    }
    else if (k == kn_sustain.get()) {
        AudioParameterInt &p = *op.p_sustain;
        p.endChangeGesture();
    }
    else if (k == kn_release.get()) {
        AudioParameterInt &p = *op.p_release;
        p.endChangeGesture();
    }

    info_->expire_info_in();
}

void Operator_Editor::paintOverChildren(Graphics &g)
{
    if (!operator_enabled_) {
        Rectangle<int> bounds = getLocalBounds();
        g.setColour(Colour(0x66777777));
        g.fillRoundedRectangle(bounds.toFloat(), 7.0f);
    }
}

bool Operator_Editor::display_info_for_component(Component *c)
{
    String param;
    int val = 0;
    const char *prefixes[4] = {"Op2 ", "Op1 ", "Op4 ", "Op3 "};
    String prefix = prefixes[operator_id_];

    Knob *kn = static_cast<Knob *>(c);

    if (c == sl_level.get()) {
        param = prefix + "Level";
        val = (int)std::lround(kn->value());
    }
    else if (c == sl_fmul.get()) {
        param = prefix + "Frequency multiplier";
        val = (int)std::lround(kn->value());
    }
    else if (c == sl_ksl.get()) {
        param = prefix + "Key scale level";
        val = swap_ksl((int)std::lround(kn->value()));
    }
    else if (c == kn_attack.get()) {
        param = prefix + "Attack";
        val = (int)std::lround(kn->value());
    }
    else if (c == kn_decay.get()) {
        param = prefix + "Decay";
        val = (int)std::lround(kn->value());
    }
    else if (c == kn_sustain.get()) {
        param = prefix + "Sustain";
        val = (int)std::lround(kn->value());
    }
    else if (c == kn_release.get()) {
        param = prefix + "Release";
        val = (int)std::lround(kn->value());
    }
    else if (c == btn_next_wave.get() || c == btn_prev_wave.get()) {
        param = prefix + "Wave";
        val = lbl_wave->wave();
    }

    if (param.isEmpty())
        return false;

    info_->display_info(param + " = " + String(val));
    return true;
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
                 overlayOpacity="0.33" fixedSize="0" initialWidth="264" initialHeight="128">
  <BACKGROUND backgroundColour="323e44">
    <RECT pos="25 96 108 24" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
    <ROUNDRECT pos="0 0 264 128" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="7c54ff103d9f5d" memberName="kn_attack"
                    virtualName="" explicitFocusOrder="0" pos="24 3 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="be39ad00dcf6efe1" memberName="kn_decay"
                    virtualName="" explicitFocusOrder="0" pos="96 3 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="8d88729c124c7b16" memberName="kn_sustain"
                    virtualName="" explicitFocusOrder="0" pos="24 48 48 48" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="7d576b68e9b588f" memberName="kn_release"
                    virtualName="" explicitFocusOrder="0" pos="96 48 48 48" class="Styled_Knob_Default"
                    params=""/>
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="btn_prev_wave"
              virtualName="" explicitFocusOrder="0" pos="3 96 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="btn_next_wave"
              virtualName="" explicitFocusOrder="0" pos="132 96 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f60e70ed4f10ef32" memberName="btn_trem"
              virtualName="" explicitFocusOrder="0" pos="168 3 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="501ccf7ad0bc53a7" memberName="btn_vib"
              virtualName="" explicitFocusOrder="0" pos="168 20 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="3e46dd6b966c40b2" memberName="btn_sus"
              virtualName="" explicitFocusOrder="0" pos="168 37 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="eb8e9dfd42dd8f57" memberName="btn_env"
              virtualName="" explicitFocusOrder="0" pos="168 54 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="ce54b68fc1a1f1e1" memberName="lbl_level"
         virtualName="" explicitFocusOrder="0" pos="163 72 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Lv" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="dd16fb8d4c488877" memberName="lbl_wave"
                    virtualName="" explicitFocusOrder="0" pos="26 96 106 24" class="Wave_Label"
                    params="chip_waves_"/>
  <LABEL name="new label" id="664ae98bd7a6b3a5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="4 0 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="A" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="360efe252ecea296" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="76 0 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="D" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="d276bb335fab1f40" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="4 48 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="S" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="2ad02ec8c7135a27" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="76 48 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="R" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="ffcd49be138de78b" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="184 3 80 15" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="Tremolo" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="14.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="37e7947c4443e047" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="184 20 80 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Vibrato" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="f04d949c957007e" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="184 37 80 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Sustain" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e8cd7412f499955d" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="184 54 80 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Key scaling" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e77fa8c6c00316b7" memberName="lbl_fmul"
         virtualName="" explicitFocusOrder="0" pos="163 88 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="F*" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="dbcb4d45f32ea3e9" memberName="lbl_ksl" virtualName=""
         explicitFocusOrder="0" pos="163 104 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Ksl" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="d7383c8ec7f64dfc" memberName="sl_level"
                    virtualName="" explicitFocusOrder="0" pos="195 70 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="37e9a27164f2dd8e" memberName="sl_fmul"
                    virtualName="" explicitFocusOrder="0" pos="195 86 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="1836679269ce1d4f" memberName="sl_ksl"
                    virtualName="" explicitFocusOrder="0" pos="195 102 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
