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
#include <fmt/format.h>
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

    kn_attack->setBounds (16, 3, 40, 40);

    kn_decay.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_decay.get());
    kn_decay->setName ("new component");

    kn_decay->setBounds (64, 3, 40, 40);

    kn_sustain.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_sustain.get());
    kn_sustain->setName ("new component");

    kn_sustain->setBounds (16, 48, 40, 40);

    kn_release.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_release.get());
    kn_release->setName ("new component");

    kn_release->setBounds (64, 48, 40, 40);

    btn_ssgenable.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_ssgenable.get());
    btn_ssgenable->setButtonText (String());
    btn_ssgenable->addListener (this);
    btn_ssgenable->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_ssgenable->setBounds (4, 100, 15, 15);

    lbl_level.reset (new Label ("new label",
                                TRANS("Lv")));
    addAndMakeVisible (lbl_level.get());
    lbl_level->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_level->setJustificationType (Justification::centredLeft);
    lbl_level->setEditable (false, false, false);
    lbl_level->setColour (Label::textColourId, Colours::aliceblue);
    lbl_level->setColour (TextEditor::textColourId, Colours::black);
    lbl_level->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_level->setBounds (163, 0, 28, 16);

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

    label2->setBounds (52, 0, 20, 16);

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

    label4->setBounds (52, 48, 20, 16);

    lbl_fmul.reset (new Label ("new label",
                               TRANS("F*")));
    addAndMakeVisible (lbl_fmul.get());
    lbl_fmul->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_fmul->setJustificationType (Justification::centredLeft);
    lbl_fmul->setEditable (false, false, false);
    lbl_fmul->setColour (Label::textColourId, Colours::aliceblue);
    lbl_fmul->setColour (TextEditor::textColourId, Colours::black);
    lbl_fmul->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_fmul->setBounds (163, 16, 28, 16);

    lbl_rsl.reset (new Label ("new label",
                              TRANS("Rsl")));
    addAndMakeVisible (lbl_rsl.get());
    lbl_rsl->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_rsl->setJustificationType (Justification::centredLeft);
    lbl_rsl->setEditable (false, false, false);
    lbl_rsl->setColour (Label::textColourId, Colours::aliceblue);
    lbl_rsl->setColour (TextEditor::textColourId, Colours::black);
    lbl_rsl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_rsl->setBounds (163, 32, 28, 16);

    kn_decay2.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_decay2.get());
    kn_decay2->setName ("new component");

    kn_decay2->setBounds (112, 3, 40, 40);

    label9.reset (new Label ("new label",
                             TRANS("D")));
    addAndMakeVisible (label9.get());
    label9->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centredTop);
    label9->setEditable (false, false, false);
    label9->setColour (Label::textColourId, Colours::aliceblue);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label9->setBounds (100, 0, 20, 16);

    btn_am.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_am.get());
    btn_am->setButtonText (TRANS("AM"));
    btn_am->addListener (this);
    btn_am->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_am->setBounds (112, 55, 40, 24);

    lbl_tune.reset (new Label ("new label",
                               TRANS("Detune")));
    addAndMakeVisible (lbl_tune.get());
    lbl_tune->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_tune->setJustificationType (Justification::centred);
    lbl_tune->setEditable (false, false, false);
    lbl_tune->setColour (Label::textColourId, Colours::aliceblue);
    lbl_tune->setColour (TextEditor::textColourId, Colours::black);
    lbl_tune->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_tune->setBounds (163, 48, 96, 16);

    label5.reset (new Label ("new label",
                             TRANS("SSG-EG")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (Label::textColourId, Colours::aliceblue);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (20, 99, 56, 16);

    btn_prev_ssgwave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_prev_ssgwave.get());
    btn_prev_ssgwave->setButtonText (TRANS("<"));
    btn_prev_ssgwave->setConnectedEdges (Button::ConnectedOnRight);
    btn_prev_ssgwave->addListener (this);

    btn_prev_ssgwave->setBounds (82, 96, 23, 24);

    btn_next_ssgwave.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_next_ssgwave.get());
    btn_next_ssgwave->setButtonText (TRANS(">"));
    btn_next_ssgwave->setConnectedEdges (Button::ConnectedOnLeft);
    btn_next_ssgwave->addListener (this);

    btn_next_ssgwave->setBounds (211, 96, 23, 24);

    lbl_ssgwave.reset (new Wave_Label (ssgeg_waves_));
    addAndMakeVisible (lbl_ssgwave.get());
    lbl_ssgwave->setName ("new component");

    lbl_ssgwave->setBounds (105, 96, 106, 24);

    cb_detune.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_detune.get());
    cb_detune->setEditableText (false);
    cb_detune->setJustificationType (Justification::centredLeft);
    cb_detune->setTextWhenNothingSelected (String());
    cb_detune->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_detune->addListener (this);

    cb_detune->setBounds (163, 65, 96, 24);

    sl_level.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_level.get());
    sl_level->setName ("new component");

    sl_level->setBounds (195, -2, 64, 20);

    sl_fmul.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_fmul.get());
    sl_fmul->setName ("new component");

    sl_fmul->setBounds (195, 14, 64, 20);

    sl_rsl.reset (new Styled_Slider_DefaultSmall());
    addAndMakeVisible (sl_rsl.get());
    sl_rsl->setName ("new component");

    sl_rsl->setBounds (195, 30, 64, 20);


    //[UserPreSize]
    sl_level->add_listener(this);
    sl_level->set_range(0, 127);
    sl_level->set_max_increment(1);
    sl_fmul->add_listener(this);
    sl_fmul->set_range(0, 15);
    sl_fmul->set_max_increment(1);
    sl_rsl->add_listener(this);
    sl_rsl->set_range(0, 3);
    sl_rsl->set_max_increment(1);

    kn_attack->add_listener(this);
    kn_attack->set_max_increment(1);
    kn_decay->add_listener(this);
    kn_decay->set_max_increment(1);
    kn_decay2->add_listener(this);
    kn_decay2->set_max_increment(1);
    kn_sustain->add_listener(this);
    kn_sustain->set_max_increment(1);
    kn_release->add_listener(this);
    kn_release->set_max_increment(1);

    btn_am->setClickingTogglesState(true);
    btn_ssgenable->setClickingTogglesState(true);
    //[/UserPreSize]

    setSize (264, 128);


    //[Constructor] You can add your own custom stuff here..
    kn_attack->set_range(0, 31);
    kn_decay->set_range(0, 31);
    kn_decay2->set_range(0, 31);
    kn_sustain->set_range(0, 15);
    kn_release->set_range(0, 15);

    const char *detunes[8] = {
        u8"×1",
        u8"×(1+ε)",
        u8"×(1+2ε)",
        u8"×(1+3ε)",
        u8"×1",
        u8"×(1-ε)",
        u8"×(1-2ε)",
        u8"×(1-3ε)",
    };

    for (unsigned i = 0; i < 8; ++i) {
        std::string text = fmt::format("{:d} : {:s}", i + 1, detunes[i]);
        cb_detune->addItem(text, i + 1);
    }
    cb_detune->setScrollWheelEnabled(true);
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
    btn_ssgenable = nullptr;
    lbl_level = nullptr;
    label = nullptr;
    label2 = nullptr;
    label3 = nullptr;
    label4 = nullptr;
    lbl_fmul = nullptr;
    lbl_rsl = nullptr;
    kn_decay2 = nullptr;
    label9 = nullptr;
    btn_am = nullptr;
    lbl_tune = nullptr;
    label5 = nullptr;
    btn_prev_ssgwave = nullptr;
    btn_next_ssgwave = nullptr;
    lbl_ssgwave = nullptr;
    cb_detune = nullptr;
    sl_level = nullptr;
    sl_fmul = nullptr;
    sl_rsl = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Operator_Editor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    {
        int x = 104, y = 96, width = 108, height = 24;
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
        g.fillRoundedRectangle (x, y, width, height, 5.0f);
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

    if (buttonThatWasClicked == btn_ssgenable.get())
    {
        //[UserButtonCode_btn_ssgenable] -- add your button handler code here..
        AudioParameterBool &p = *op.p_ssgenable;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_ssgenable]
    }
    else if (buttonThatWasClicked == btn_am.get())
    {
        //[UserButtonCode_btn_am] -- add your button handler code here..
        AudioParameterBool &p = *op.p_am;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_am]
    }
    else if (buttonThatWasClicked == btn_prev_ssgwave.get())
    {
        //[UserButtonCode_btn_prev_ssgwave] -- add your button handler code here..
        AudioParameterChoice &p = *op.p_ssgwave;
        int wave = std::max(p.getIndex() - 1, 0);
        p = wave;
        p.endChangeGesture();
        lbl_ssgwave->set_wave(wave, dontSendNotification);
        //[/UserButtonCode_btn_prev_ssgwave]
    }
    else if (buttonThatWasClicked == btn_next_ssgwave.get())
    {
        //[UserButtonCode_btn_next_ssgwave] -- add your button handler code here..
        AudioParameterChoice &p = *op.p_ssgwave;
        p.beginChangeGesture();
        int wave = std::min(p.getIndex() + 1, p.choices.size() - 1);
        p = wave;
        p.endChangeGesture();
        lbl_ssgwave->set_wave(wave, dontSendNotification);
        //[/UserButtonCode_btn_next_ssgwave]
    }

    //[UserbuttonClicked_Post]
    if (display_info_for_component(btn))
        info_->expire_info_in();
    //[/UserbuttonClicked_Post]
}

void Operator_Editor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Parameter_Block::Operator &op = part.nth_operator(operator_id_);
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cb_detune.get())
    {
        //[UserComboBoxCode_cb_detune] -- add your combo box handling code here..
        AudioParameterInt &p = *op.p_detune;
        p.beginChangeGesture();
        p = cb_detune->getSelectedId() - 1;
        p.endChangeGesture();
        //[/UserComboBoxCode_cb_detune]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Operator_Editor::set_operator_parameters(const Instrument &ins, unsigned op, NotificationType ntf)
{
    kn_attack->set_value(ins.attack(op), ntf);
    kn_decay->set_value(ins.decay1(op), ntf);
    kn_decay2->set_value(ins.decay2(op), ntf);
    kn_sustain->set_value(ins.sustain(op), ntf);
    kn_release->set_value(ins.release(op), ntf);

    sl_level->set_value(ins.level(op), ntf);
    sl_fmul->set_value(ins.fmul(op), ntf);
    sl_rsl->set_value(ins.ratescale(op), ntf);
    cb_detune->setSelectedId(ins.detune(op) + 1, ntf);

    btn_am->setToggleState(ins.am(op), ntf);
    btn_ssgenable->setToggleState(ins.ssgenable(op), ntf);
    lbl_ssgwave->set_wave(ins.ssgwave(op), ntf);
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
    else if (k == sl_rsl.get()) {
        AudioParameterInt &p = *op.p_ratescale;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay1;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_decay2.get()) {
        AudioParameterInt &p = *op.p_decay2;
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
    else if (k == sl_rsl.get()) {
        AudioParameterInt &p = *op.p_ratescale;
        p.beginChangeGesture();
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p.beginChangeGesture();
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay1;
        p.beginChangeGesture();
    }
    else if (k == kn_decay2.get()) {
        AudioParameterInt &p = *op.p_decay2;
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
    else if (k == sl_rsl.get()) {
        AudioParameterInt &p = *op.p_ratescale;
        p.endChangeGesture();
    }
    else if (k == kn_attack.get()) {
        AudioParameterInt &p = *op.p_attack;
        p.endChangeGesture();
    }
    else if (k == kn_decay.get()) {
        AudioParameterInt &p = *op.p_decay1;
        p.endChangeGesture();
    }
    else if (k == kn_decay2.get()) {
        AudioParameterInt &p = *op.p_decay2;
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
    const char *prefixes[4] = {"Op1 ", "Op3 ", "Op2 ", "Op4 "};
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
    else if (c == sl_rsl.get()) {
        param = prefix + "Rate scale level";
        val = (int)std::lround(kn->value());
    }
    else if (kn == kn_attack.get()) {
        param = prefix + "Attack";
        val = (int)std::lround(kn->value());
    }
    else if (kn == kn_decay.get()) {
        param = prefix + "Primary Decay";
        val = (int)std::lround(kn->value());
    }
    else if (kn == kn_decay2.get()) {
        param = prefix + "Secondary Decay";
        val = (int)std::lround(kn->value());
    }
    else if (kn == kn_sustain.get()) {
        param = prefix + "Sustain";
        val = (int)std::lround(kn->value());
    }
    else if (kn == kn_release.get()) {
        param = prefix + "Release";
        val = (int)std::lround(kn->value());
    }
    else if (c == btn_next_ssgwave.get() || c == btn_prev_ssgwave.get()) {
        param = prefix + "SSG-EG Wave";
        val = lbl_ssgwave->wave();
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
    <RECT pos="104 96 108 24" fill="solid: 0" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
    <ROUNDRECT pos="0 0 264 128" cornerSize="5.0" fill="solid: 662e4c4d" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="7c54ff103d9f5d" memberName="kn_attack"
                    virtualName="" explicitFocusOrder="0" pos="16 3 40 40" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="be39ad00dcf6efe1" memberName="kn_decay"
                    virtualName="" explicitFocusOrder="0" pos="64 3 40 40" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="8d88729c124c7b16" memberName="kn_sustain"
                    virtualName="" explicitFocusOrder="0" pos="16 48 40 40" class="Styled_Knob_Default"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="7d576b68e9b588f" memberName="kn_release"
                    virtualName="" explicitFocusOrder="0" pos="64 48 40 40" class="Styled_Knob_Default"
                    params=""/>
  <TEXTBUTTON name="new button" id="f60e70ed4f10ef32" memberName="btn_ssgenable"
              virtualName="" explicitFocusOrder="0" pos="4 100 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="ce54b68fc1a1f1e1" memberName="lbl_level"
         virtualName="" explicitFocusOrder="0" pos="163 0 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Lv" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="664ae98bd7a6b3a5" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="4 0 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="A" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="360efe252ecea296" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="52 0 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="D" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="d276bb335fab1f40" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="4 48 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="S" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="2ad02ec8c7135a27" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="52 48 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="R" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <LABEL name="new label" id="e77fa8c6c00316b7" memberName="lbl_fmul"
         virtualName="" explicitFocusOrder="0" pos="163 16 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="F*" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="dbcb4d45f32ea3e9" memberName="lbl_rsl" virtualName=""
         explicitFocusOrder="0" pos="163 32 28 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Rsl" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="d3ebb26d44d99705" memberName="kn_decay2"
                    virtualName="" explicitFocusOrder="0" pos="112 3 40 40" class="Styled_Knob_Default"
                    params=""/>
  <LABEL name="new label" id="739d50c231ea95f4" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="100 0 20 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="D" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="12"/>
  <TEXTBUTTON name="new button" id="17b8bc55dead908d" memberName="btn_am" virtualName=""
              explicitFocusOrder="0" pos="112 55 40 24" bgColOn="ff42a2c8"
              buttonText="AM" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="13046618f23c99f6" memberName="lbl_tune"
         virtualName="" explicitFocusOrder="0" pos="163 48 96 16" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Detune" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="1e3f006eb3ae12a5" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="20 99 56 16" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="SSG-EG" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="14.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="btn_prev_ssgwave"
              virtualName="" explicitFocusOrder="0" pos="82 96 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="btn_next_ssgwave"
              virtualName="" explicitFocusOrder="0" pos="211 96 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="dd16fb8d4c488877" memberName="lbl_ssgwave"
                    virtualName="" explicitFocusOrder="0" pos="105 96 106 24" class="Wave_Label"
                    params="ssgeg_waves_"/>
  <COMBOBOX name="new combo box" id="53eb24e86e9015c0" memberName="cb_detune"
            virtualName="" explicitFocusOrder="0" pos="163 65 96 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="new component" id="d7383c8ec7f64dfc" memberName="sl_level"
                    virtualName="" explicitFocusOrder="0" pos="195 -2 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="a54ced31a407c9a2" memberName="sl_fmul"
                    virtualName="" explicitFocusOrder="0" pos="195 14 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="b3eb1a9c8b111b7c" memberName="sl_rsl"
                    virtualName="" explicitFocusOrder="0" pos="195 30 64 20" class="Styled_Slider_DefaultSmall"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
