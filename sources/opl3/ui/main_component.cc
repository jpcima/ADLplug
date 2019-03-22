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
#include "operator_editor.h"
#include "about_component.h"
#include "components/algorithm_help.h"
#include "ui/components/vu_meter.h"
#include "ui/components/indicator_NxM.h"
#include "ui/components/midi_keyboard_ex.h"
#include "plugin_processor.h"
#include "parameter_block.h"
#include "messages.h"
#include "resources.h"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
//[/Headers]

#include "main_component.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
RESOURCE(Res, emoji_u1f4a1);

#if 1
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[UI Main] " fmt "\n", ##__VA_ARGS__)
#endif

enum class Radio_Button_Group {
    Fm_Mode = 1,
    Algo_12,
    Algo_34,
};
//[/MiscUserDefs]

//==============================================================================
Main_Component::Main_Component (AdlplugAudioProcessor &proc, Parameter_Block &pb, Configuration &conf)
    : Generic_Main_Component(proc, pb, conf)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    ed_op2.reset (new Operator_Editor (WOPL_OP_CARRIER1, pb));
    addAndMakeVisible (ed_op2.get());
    ed_op2->setName ("new component");

    ed_op2->setBounds (300, 160, 264, 128);

    btn_4op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_4op.get());
    btn_4op->setButtonText (TRANS("4 op"));
    btn_4op->setConnectedEdges (Button::ConnectedOnRight);
    btn_4op->addListener (this);
    btn_4op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_4op->setBounds (598, 174, 56, 24);

    btn_pseudo4op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_pseudo4op.get());
    btn_pseudo4op->setButtonText (TRANS("2x2 op"));
    btn_pseudo4op->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_pseudo4op->addListener (this);
    btn_pseudo4op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_pseudo4op->setBounds (653, 174, 56, 24);

    btn_2op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_2op.get());
    btn_2op->setButtonText (TRANS("2 op"));
    btn_2op->setConnectedEdges (Button::ConnectedOnLeft);
    btn_2op->addListener (this);
    btn_2op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_2op->setBounds (708, 174, 56, 24);

    btn_fm12.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_fm12.get());
    btn_fm12->setButtonText (String());
    btn_fm12->addListener (this);
    btn_fm12->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_fm12->setBounds (606, 208, 15, 15);

    btn_am12.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_am12.get());
    btn_am12->setButtonText (String());
    btn_am12->addListener (this);
    btn_am12->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_am12->setBounds (606, 225, 15, 15);

    ed_op1.reset (new Operator_Editor (WOPL_OP_MODULATOR1, pb));
    addAndMakeVisible (ed_op1.get());
    ed_op1->setName ("new component");

    ed_op1->setBounds (16, 160, 264, 128);

    ed_op4.reset (new Operator_Editor (WOPL_OP_CARRIER2, pb));
    addAndMakeVisible (ed_op4.get());
    ed_op4->setName ("new component");

    ed_op4->setBounds (300, 316, 264, 128);

    btn_fm34.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_fm34.get());
    btn_fm34->setButtonText (String());
    btn_fm34->addListener (this);
    btn_fm34->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_fm34->setBounds (686, 208, 15, 15);

    btn_am34.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_am34.get());
    btn_am34->setButtonText (String());
    btn_am34->addListener (this);
    btn_am34->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_am34->setBounds (686, 225, 15, 15);

    ed_op3.reset (new Operator_Editor (WOPL_OP_MODULATOR2, pb));
    addAndMakeVisible (ed_op3.get());
    ed_op3->setName ("new component");

    ed_op3->setBounds (16, 316, 264, 128);

    sl_tune12.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_tune12.get());
    sl_tune12->setRange (-127, 127, 1);
    sl_tune12->setSliderStyle (Slider::IncDecButtons);
    sl_tune12->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_tune12->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_tune12->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_tune12->addListener (this);

    sl_tune12->setBounds (696, 324, 76, 20);

    sl_tune34.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_tune34.get());
    sl_tune34->setRange (-127, 127, 1);
    sl_tune34->setSliderStyle (Slider::IncDecButtons);
    sl_tune34->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_tune34->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_tune34->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_tune34->addListener (this);

    sl_tune34->setBounds (696, 348, 76, 20);

    kn_fb12.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_fb12.get());
    kn_fb12->setName ("new component");

    kn_fb12->setBounds (648, 245, 32, 32);

    kn_fb34.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_fb34.get());
    kn_fb34->setName ("new component");

    kn_fb34->setBounds (736, 245, 32, 32);

    midi_kb.reset (new Midi_Keyboard_Ex (midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard));
    addAndMakeVisible (midi_kb.get());
    midi_kb->setName ("new component");

    midi_kb->setBounds (16, 520, 730, 64);

    btn_about.reset (new ImageButton ("new button"));
    addAndMakeVisible (btn_about.get());
    btn_about->setButtonText (String());
    btn_about->addListener (this);

    btn_about->setImages (false, true, true,
                          ImageCache::getFromMemory (logo_png, logo_pngSize), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000));
    btn_about->setBounds (16, 8, 232, 40);

    label2.reset (new Label ("new label",
                             TRANS("FM synthesizer with YMF262 chip emulation")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::aliceblue);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (264, 8, 192, 40);

    vu_left.reset (new Vu_Meter());
    addAndMakeVisible (vu_left.get());
    vu_left->setName ("new component");

    vu_left->setBounds (578, 8, 92, 12);

    vu_right.reset (new Vu_Meter());
    addAndMakeVisible (vu_right.get());
    vu_right->setName ("new component");

    vu_right->setBounds (578, 24, 92, 12);

    label3.reset (new Label ("new label",
                             TRANS("CPU")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredRight);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::aliceblue);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (684, 8, 40, 24);

    lbl_cpu.reset (new Label ("new label",
                              TRANS("100%")));
    addAndMakeVisible (lbl_cpu.get());
    lbl_cpu->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_cpu->setJustificationType (Justification::centred);
    lbl_cpu->setEditable (false, false, false);
    lbl_cpu->setColour (Label::textColourId, Colours::aliceblue);
    lbl_cpu->setColour (Label::outlineColourId, Colour (0xff8e989b));
    lbl_cpu->setColour (TextEditor::textColourId, Colours::black);
    lbl_cpu->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_cpu->setBounds (724, 8, 48, 24);

    ind_midi_activity.reset (new Indicator_NxM (2, 8));
    addAndMakeVisible (ind_midi_activity.get());
    ind_midi_activity->setName ("new component");

    ind_midi_activity->setBounds (466, 8, 102, 28);

    btn_panic.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_panic.get());
    btn_panic->setButtonText (TRANS("Panic"));
    btn_panic->addListener (this);
    btn_panic->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_panic->setBounds (308, 102, 48, 24);

    edt_bank_name.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_name.get());
    edt_bank_name->setMultiLine (false);
    edt_bank_name->setReturnKeyStartsNewLine (false);
    edt_bank_name->setReadOnly (false);
    edt_bank_name->setScrollbarsShown (true);
    edt_bank_name->setCaretVisible (true);
    edt_bank_name->setPopupMenuEnabled (true);
    edt_bank_name->setColour (TextEditor::outlineColourId, Colour (0xff8e989b));
    edt_bank_name->setText (String());

    edt_bank_name->setBounds (16, 74, 215, 24);

    cb_program.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_program.get());
    cb_program->setEditableText (false);
    cb_program->setJustificationType (Justification::centredLeft);
    cb_program->setTextWhenNothingSelected (String());
    cb_program->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_program->addListener (this);

    cb_program->setBounds (16, 104, 215, 24);

    label4.reset (new Label ("new label",
                             TRANS("Part")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colours::aliceblue);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (304, 78, 56, 20);

    btn_bank_load.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_bank_load.get());
    btn_bank_load->setButtonText (String());
    btn_bank_load->setConnectedEdges (Button::ConnectedOnRight);
    btn_bank_load->addListener (this);

    btn_bank_load->setBounds (235, 74, 23, 24);

    btn_bank_save.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_bank_save.get());
    btn_bank_save->setButtonText (String());
    btn_bank_save->setConnectedEdges (Button::ConnectedOnLeft);
    btn_bank_save->addListener (this);

    btn_bank_save->setBounds (257, 74, 23, 24);

    label6.reset (new Label ("new label",
                             TRANS("FM 1-2")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centredLeft);
    label6->setEditable (false, false, false);
    label6->setColour (Label::textColourId, Colours::aliceblue);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (622, 208, 56, 15);

    label7.reset (new Label ("new label",
                             TRANS("FM 3-4")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centredLeft);
    label7->setEditable (false, false, false);
    label7->setColour (Label::textColourId, Colours::aliceblue);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (702, 208, 56, 15);

    label8.reset (new Label ("new label",
                             TRANS("AM 1-2")));
    addAndMakeVisible (label8.get());
    label8->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label8->setJustificationType (Justification::centredLeft);
    label8->setEditable (false, false, false);
    label8->setColour (Label::textColourId, Colours::aliceblue);
    label8->setColour (TextEditor::textColourId, Colours::black);
    label8->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label8->setBounds (622, 225, 56, 15);

    label9.reset (new Label ("new label",
                             TRANS("AM 3-4")));
    addAndMakeVisible (label9.get());
    label9->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label9->setJustificationType (Justification::centredLeft);
    label9->setEditable (false, false, false);
    label9->setColour (Label::textColourId, Colours::aliceblue);
    label9->setColour (TextEditor::textColourId, Colours::black);
    label9->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label9->setBounds (702, 225, 56, 15);

    label10.reset (new Label ("new label",
                              TRANS("Feedback\n"
                              "1-2")));
    addAndMakeVisible (label10.get());
    label10->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (Label::textColourId, Colours::aliceblue);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (584, 248, 72, 24);

    label12.reset (new Label ("new label",
                              TRANS("Feedback\n"
                              "3-4")));
    addAndMakeVisible (label12.get());
    label12->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label12->setJustificationType (Justification::centred);
    label12->setEditable (false, false, false);
    label12->setColour (Label::textColourId, Colours::aliceblue);
    label12->setColour (TextEditor::textColourId, Colours::black);
    label12->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label12->setBounds (672, 248, 72, 24);

    label11.reset (new Label ("new label",
                              TRANS("Note offset 1-2")));
    addAndMakeVisible (label11.get());
    label11->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label11->setJustificationType (Justification::centredLeft);
    label11->setEditable (false, false, false);
    label11->setColour (Label::textColourId, Colours::aliceblue);
    label11->setColour (TextEditor::textColourId, Colours::black);
    label11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label11->setBounds (590, 324, 104, 20);

    label13.reset (new Label ("new label",
                              TRANS("Note offset 3-4")));
    addAndMakeVisible (label13.get());
    label13->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label13->setJustificationType (Justification::centredLeft);
    label13->setEditable (false, false, false);
    label13->setColour (Label::textColourId, Colours::aliceblue);
    label13->setColour (TextEditor::textColourId, Colours::black);
    label13->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label13->setBounds (590, 348, 104, 20);

    btn_emulator.reset (new ImageButton ("new button"));
    addAndMakeVisible (btn_emulator.get());
    btn_emulator->addListener (this);

    btn_emulator->setImages (false, true, true,
                             Image(), 1.0f, Colour (0x00000000),
                             Image(), 1.0f, Colour (0x00000000),
                             Image(), 1.0f, Colour (0x00000000));
    btn_emulator->setBounds (659, 56, 76, 20);

    label14.reset (new Label ("new label",
                              TRANS("Core")));
    addAndMakeVisible (label14.get());
    label14->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label14->setJustificationType (Justification::centredLeft);
    label14->setEditable (false, false, false);
    label14->setColour (Label::textColourId, Colours::aliceblue);
    label14->setColour (TextEditor::textColourId, Colours::black);
    label14->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label14->setBounds (611, 56, 48, 20);

    sl_num_chips.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_num_chips.get());
    sl_num_chips->setRange (1, 100, 1);
    sl_num_chips->setSliderStyle (Slider::IncDecButtons);
    sl_num_chips->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_num_chips->setColour (Slider::rotarySliderOutlineColourId, Colour (0xff8e98ff));
    sl_num_chips->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_num_chips->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_num_chips->addListener (this);

    sl_num_chips->setBounds (659, 80, 76, 20);

    label15.reset (new Label ("new label",
                              TRANS("Chips")));
    addAndMakeVisible (label15.get());
    label15->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label15->setJustificationType (Justification::centredLeft);
    label15->setEditable (false, false, false);
    label15->setColour (Label::textColourId, Colours::aliceblue);
    label15->setColour (TextEditor::textColourId, Colours::black);
    label15->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label15->setBounds (611, 80, 48, 20);

    label16.reset (new Label ("new label",
                              TRANS("4 ops")));
    addAndMakeVisible (label16.get());
    label16->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label16->setJustificationType (Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (Label::textColourId, Colours::aliceblue);
    label16->setColour (TextEditor::textColourId, Colours::black);
    label16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label16->setBounds (611, 104, 48, 20);

    sl_num_4ops.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_num_4ops.get());
    sl_num_4ops->setRange (1, 600, 1);
    sl_num_4ops->setSliderStyle (Slider::IncDecButtons);
    sl_num_4ops->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_num_4ops->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_num_4ops->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_num_4ops->addListener (this);

    sl_num_4ops->setBounds (659, 104, 76, 20);

    label5.reset (new Label ("new label",
                             TRANS("Percussion key")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (590, 372, 104, 20);

    cb_percussion_key.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_percussion_key.get());
    cb_percussion_key->setEditableText (false);
    cb_percussion_key->setJustificationType (Justification::centredLeft);
    cb_percussion_key->setTextWhenNothingSelected (String());
    cb_percussion_key->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_percussion_key->addListener (this);

    cb_percussion_key->setBounds (696, 372, 74, 20);

    label17.reset (new Label ("new label",
                              TRANS("Percussion key")));
    addAndMakeVisible (label17.get());
    label17->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label17->setJustificationType (Justification::centredLeft);
    label17->setEditable (false, false, false);
    label17->setColour (Label::textColourId, Colours::aliceblue);
    label17->setColour (TextEditor::textColourId, Colours::black);
    label17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label17->setBounds (590, 372, 104, 20);

    label18.reset (new Label ("new label",
                              TRANS("Fine tune 3-4")));
    addAndMakeVisible (label18.get());
    label18->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label18->setJustificationType (Justification::centredLeft);
    label18->setEditable (false, false, false);
    label18->setColour (Label::textColourId, Colours::aliceblue);
    label18->setColour (TextEditor::textColourId, Colours::black);
    label18->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label18->setBounds (590, 396, 104, 20);

    sl_finetune34.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_finetune34.get());
    sl_finetune34->setRange (-2, 2, 0.015625);
    sl_finetune34->setSliderStyle (Slider::IncDecButtons);
    sl_finetune34->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_finetune34->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_finetune34->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_finetune34->addListener (this);

    sl_finetune34->setBounds (696, 396, 76, 20);

    sl_veloffset.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_veloffset.get());
    sl_veloffset->setRange (-127, 127, 1);
    sl_veloffset->setSliderStyle (Slider::IncDecButtons);
    sl_veloffset->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_veloffset->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_veloffset->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_veloffset->addListener (this);

    sl_veloffset->setBounds (410, 482, 76, 20);

    label19.reset (new Label ("new label",
                              TRANS("Velocity offset")));
    addAndMakeVisible (label19.get());
    label19->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label19->setJustificationType (Justification::centredLeft);
    label19->setEditable (false, false, false);
    label19->setColour (Label::textColourId, Colours::aliceblue);
    label19->setColour (TextEditor::textColourId, Colours::black);
    label19->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label19->setBounds (304, 482, 104, 20);

    sl_midi_channel.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_midi_channel.get());
    sl_midi_channel->setRange (1, 16, 1);
    sl_midi_channel->setSliderStyle (Slider::IncDecButtons);
    sl_midi_channel->setTextBoxStyle (Slider::TextBoxLeft, false, 46, 46);
    sl_midi_channel->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_midi_channel->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_midi_channel->addListener (this);

    sl_midi_channel->setBounds (364, 79, 76, 46);

    btn_deep_tremolo.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_deep_tremolo.get());
    btn_deep_tremolo->setButtonText (String());
    btn_deep_tremolo->addListener (this);
    btn_deep_tremolo->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_deep_tremolo->setBounds (552, 486, 15, 15);

    label20.reset (new Label ("new label",
                              TRANS("Deep tremolo")));
    addAndMakeVisible (label20.get());
    label20->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label20->setJustificationType (Justification::centredLeft);
    label20->setEditable (false, false, false);
    label20->setColour (Label::textColourId, Colours::aliceblue);
    label20->setColour (TextEditor::textColourId, Colours::black);
    label20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label20->setBounds (568, 486, 94, 15);

    btn_deep_vibrato.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_deep_vibrato.get());
    btn_deep_vibrato->setButtonText (String());
    btn_deep_vibrato->addListener (this);
    btn_deep_vibrato->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_deep_vibrato->setBounds (664, 486, 15, 15);

    label21.reset (new Label ("new label",
                              TRANS("Deep vibrato")));
    addAndMakeVisible (label21.get());
    label21->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label21->setJustificationType (Justification::centredLeft);
    label21->setEditable (false, false, false);
    label21->setColour (Label::textColourId, Colours::aliceblue);
    label21->setColour (TextEditor::textColourId, Colours::black);
    label21->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label21->setBounds (680, 486, 94, 15);

    label22.reset (new Label ("new label",
                              TRANS("Volume model")));
    addAndMakeVisible (label22.get());
    label22->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label22->setJustificationType (Justification::centredLeft);
    label22->setEditable (false, false, false);
    label22->setColour (Label::textColourId, Colours::aliceblue);
    label22->setColour (TextEditor::textColourId, Colours::black);
    label22->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label22->setBounds (552, 458, 106, 20);

    cb_volmodel.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_volmodel.get());
    cb_volmodel->setEditableText (false);
    cb_volmodel->setJustificationType (Justification::centredLeft);
    cb_volmodel->setTextWhenNothingSelected (String());
    cb_volmodel->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_volmodel->addListener (this);

    cb_volmodel->setBounds (664, 457, 104, 20);

    btn_algo_help.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_algo_help.get());
    btn_algo_help->setButtonText (TRANS("?"));
    btn_algo_help->addListener (this);

    btn_algo_help->setBounds (754, 136, 20, 20);

    btn_auto4ops.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_auto4ops.get());
    btn_auto4ops->setTooltip (TRANS("Choose a recommended value"));
    btn_auto4ops->setButtonText (String());
    btn_auto4ops->addListener (this);

    btn_auto4ops->setBounds (739, 102, 24, 24);

    btn_keymap.reset (new TextButton (String()));
    addAndMakeVisible (btn_keymap.get());
    btn_keymap->addListener (this);

    btn_keymap->setBounds (750, 520, 24, 24);

    btn_octave_up.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_octave_up.get());
    btn_octave_up->setTooltip (TRANS("Octave"));
    btn_octave_up->setButtonText (TRANS("+"));
    btn_octave_up->setConnectedEdges (Button::ConnectedOnBottom);
    btn_octave_up->addListener (this);

    btn_octave_up->setBounds (750, 549, 24, 18);

    btn_octave_down.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_octave_down.get());
    btn_octave_down->setTooltip (TRANS("Octave"));
    btn_octave_down->setButtonText (TRANS("-"));
    btn_octave_down->setConnectedEdges (Button::ConnectedOnTop);
    btn_octave_down->addListener (this);

    btn_octave_down->setBounds (750, 566, 24, 18);

    btn_pgm_edit.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_pgm_edit.get());
    btn_pgm_edit->setButtonText (String());
    btn_pgm_edit->setConnectedEdges (Button::ConnectedOnRight);
    btn_pgm_edit->addListener (this);

    btn_pgm_edit->setBounds (235, 104, 23, 24);

    btn_pgm_add.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_pgm_add.get());
    btn_pgm_add->setButtonText (String());
    btn_pgm_add->setConnectedEdges (Button::ConnectedOnLeft);
    btn_pgm_add->addListener (this);

    btn_pgm_add->setBounds (257, 104, 23, 24);

    kn_mastervol.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_mastervol.get());
    kn_mastervol->setName ("new component");

    kn_mastervol->setBounds (510, 78, 48, 48);

    label23.reset (new Label ("new label",
                              TRANS("Volume")));
    addAndMakeVisible (label23.get());
    label23->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label23->setJustificationType (Justification::centredLeft);
    label23->setEditable (false, false, false);
    label23->setColour (Label::textColourId, Colours::aliceblue);
    label23->setColour (TextEditor::textColourId, Colours::black);
    label23->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label23->setBounds (464, 78, 56, 22);

    lbl_mastervol.reset (new Label ("new label",
                                    TRANS("-20 dB")));
    addAndMakeVisible (lbl_mastervol.get());
    lbl_mastervol->setFont (Font (12.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_mastervol->setJustificationType (Justification::centredRight);
    lbl_mastervol->setEditable (false, false, false);
    lbl_mastervol->setColour (Label::textColourId, Colours::aliceblue);
    lbl_mastervol->setColour (Label::outlineColourId, Colour (0xff8e989b));
    lbl_mastervol->setColour (TextEditor::textColourId, Colours::black);
    lbl_mastervol->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_mastervol->setBounds (468, 102, 44, 22);

    lbl_info.reset (new Label ("new label",
                               TRANS("OPL Ready!")));
    addAndMakeVisible (lbl_info.get());
    lbl_info->setFont (Font (Font::getDefaultMonospacedFontName(), 15.0f, Font::plain));
    lbl_info->setJustificationType (Justification::centred);
    lbl_info->setEditable (false, false, false);
    lbl_info->setColour (Label::textColourId, Colours::aliceblue);
    lbl_info->setColour (TextEditor::textColourId, Colours::black);
    lbl_info->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_info->setBounds (16, 474, 264, 36);


    //[UserPreSize]
    for (Operator_Editor *ed : {ed_op1.get(), ed_op2.get(), ed_op3.get(), ed_op4.get()})
        ed->set_info_display(this);

    setup_generic_components();

    kn_fb12->add_listener(this);
    kn_fb12->set_max_increment(1);
    kn_fb34->add_listener(this);
    kn_fb34->set_max_increment(1);
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    for (TextButton *btn : {btn_4op.get(), btn_pseudo4op.get(), btn_2op.get()}) {
        btn->setClickingTogglesState(true);
        btn->setRadioGroupId((int)Radio_Button_Group::Fm_Mode);
    }
    for (TextButton *btn : {btn_fm12.get(), btn_am12.get()}) {
        btn->setClickingTogglesState(true);
        btn->setRadioGroupId((int)Radio_Button_Group::Algo_12);
    }
    for (TextButton *btn : {btn_fm34.get(), btn_am34.get()}) {
        btn->setClickingTogglesState(true);
        btn->setRadioGroupId((int)Radio_Button_Group::Algo_34);
    }

    Label *lbl_midi_channel = (Label *)(intptr_t)(int64)sl_midi_channel->getProperties()["X-Slider-Text-Box"];
    Font fnt_midi_channel(Font::getDefaultSansSerifFontName(), 30.0, Font::plain);
    lbl_midi_channel->setFont(fnt_midi_channel);

    sl_num_4ops->setNumDecimalPlacesToDisplay(0);
    sl_num_chips->setNumDecimalPlacesToDisplay(0);

    kn_fb12->set_range(0, 7);
    kn_fb34->set_range(0, 7);
    sl_tune12->setNumDecimalPlacesToDisplay(0);
    sl_tune34->setNumDecimalPlacesToDisplay(0);
    sl_finetune34->setNumDecimalPlacesToDisplay(2);

    btn_deep_tremolo->setClickingTogglesState(true);
    btn_deep_vibrato->setClickingTogglesState(true);

    create_image_overlay(*btn_auto4ops, ImageCache::getFromMemory(Res::emoji_u1f4a1.data, Res::emoji_u1f4a1.size), 0.7f);

    {
        StringArray strings = pb.p_volmodel->getAllValueStrings();
        for (unsigned i = 0, n = strings.size(); i < n; ++i)
            cb_volmodel->addItem(strings[i], i + 1);
    }
    cb_volmodel->setScrollWheelEnabled(true);
    //[/Constructor]
}

Main_Component::~Main_Component()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    ed_op2 = nullptr;
    btn_4op = nullptr;
    btn_pseudo4op = nullptr;
    btn_2op = nullptr;
    btn_fm12 = nullptr;
    btn_am12 = nullptr;
    ed_op1 = nullptr;
    ed_op4 = nullptr;
    btn_fm34 = nullptr;
    btn_am34 = nullptr;
    ed_op3 = nullptr;
    sl_tune12 = nullptr;
    sl_tune34 = nullptr;
    kn_fb12 = nullptr;
    kn_fb34 = nullptr;
    midi_kb = nullptr;
    btn_about = nullptr;
    label2 = nullptr;
    vu_left = nullptr;
    vu_right = nullptr;
    label3 = nullptr;
    lbl_cpu = nullptr;
    ind_midi_activity = nullptr;
    btn_panic = nullptr;
    edt_bank_name = nullptr;
    cb_program = nullptr;
    label4 = nullptr;
    btn_bank_load = nullptr;
    btn_bank_save = nullptr;
    label6 = nullptr;
    label7 = nullptr;
    label8 = nullptr;
    label9 = nullptr;
    label10 = nullptr;
    label12 = nullptr;
    label11 = nullptr;
    label13 = nullptr;
    btn_emulator = nullptr;
    label14 = nullptr;
    sl_num_chips = nullptr;
    label15 = nullptr;
    label16 = nullptr;
    sl_num_4ops = nullptr;
    label5 = nullptr;
    cb_percussion_key = nullptr;
    label17 = nullptr;
    label18 = nullptr;
    sl_finetune34 = nullptr;
    sl_veloffset = nullptr;
    label19 = nullptr;
    sl_midi_channel = nullptr;
    btn_deep_tremolo = nullptr;
    label20 = nullptr;
    btn_deep_vibrato = nullptr;
    label21 = nullptr;
    label22 = nullptr;
    cb_volmodel = nullptr;
    btn_algo_help = nullptr;
    btn_auto4ops = nullptr;
    btn_keymap = nullptr;
    btn_octave_up = nullptr;
    btn_octave_down = nullptr;
    btn_pgm_edit = nullptr;
    btn_pgm_add = nullptr;
    kn_mastervol = nullptr;
    label23 = nullptr;
    lbl_mastervol = nullptr;
    lbl_info = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Main_Component::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    {
        int x = 0, y = 0, width = 800, height = 600;
        Colour fillColour1 = Colour (0xff3f6a8b), fillColour2 = Colour (0xff274c70);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (ColourGradient (fillColour1,
                                       150.0f - 0.0f + x,
                                       50.0f - 0.0f + y,
                                       fillColour2,
                                       800.0f - 0.0f + x,
                                       600.0f - 0.0f + y,
                                       true));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 16, y = 288, width = 264, height = 30;
        String text (TRANS("Operator 3"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 16, y = 132, width = 264, height = 30;
        String text (TRANS("Operator 1"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 300, y = 132, width = 264, height = 30;
        String text (TRANS("Operator 2"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 300, y = 288, width = 264, height = 30;
        String text (TRANS("Operator 4"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 578, y = 8, width = 92, height = 12;
        Colour strokeColour = Colour (0xff8e989b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 578, y = 24, width = 92, height = 12;
        Colour strokeColour = Colour (0xff8e989b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 586, y = 132, width = 188, height = 30;
        String text (TRANS("Algorithm"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 586, y = 288, width = 188, height = 30;
        String text (TRANS("Tuning"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 300, y = 444, width = 188, height = 30;
        String text (TRANS("Misc"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 586, y = 422, width = 188, height = 30;
        String text (TRANS("Global"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (20.0f, Font::plain).withTypefaceStyle ("Bold Italic"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        float x = 16.0f, y = 474.0f, width = 264.0f, height = 36.0f;
        Colour fillColour = Colour (0xff3c485f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 300.0f, y = 474.0f, width = 188.0f, height = 36.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 546.0f, y = 450.0f, width = 228.0f, height = 60.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 586.0f, y = 316.0f, width = 188.0f, height = 106.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 586.0f, y = 160.0f, width = 188.0f, height = 128.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 606.0f, y = 52.0f, width = 168.0f, height = 80.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 460.0f, y = 72.0f, width = 102.0f, height = 60.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 300.0f, y = 72.0f, width = 146.0f, height = 60.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Main_Component::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Main_Component::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Button *btn = buttonThatWasClicked;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_4op.get())
    {
        //[UserButtonCode_btn_4op] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterBool &p1 = *part.p_ps4op;
            p1.beginChangeGesture();
            p1 = false;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *part.p_is4op;
            p2.beginChangeGesture();
            p2 = true;
            p2.endChangeGesture();
        }
        //[/UserButtonCode_btn_4op]
    }
    else if (buttonThatWasClicked == btn_pseudo4op.get())
    {
        //[UserButtonCode_btn_pseudo4op] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterBool &p1 = *part.p_ps4op;
            p1.beginChangeGesture();
            p1 = true;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *part.p_is4op;
            p2.beginChangeGesture();
            p2 = true;
            p2.endChangeGesture();
        }
        //[/UserButtonCode_btn_pseudo4op]
    }
    else if (buttonThatWasClicked == btn_2op.get())
    {
        //[UserButtonCode_btn_2op] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterBool &p1 = *part.p_ps4op;
            p1.beginChangeGesture();
            p1 = false;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *part.p_is4op;
            p2.beginChangeGesture();
            p2 = false;
            p2.endChangeGesture();
        }
        //[/UserButtonCode_btn_2op]
    }
    else if (buttonThatWasClicked == btn_fm12.get())
    {
        //[UserButtonCode_btn_fm12] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterChoice &p = *part.p_con12;
            p.beginChangeGesture();
            p = 0;
            p.endChangeGesture();
        }
        //[/UserButtonCode_btn_fm12]
    }
    else if (buttonThatWasClicked == btn_am12.get())
    {
        //[UserButtonCode_btn_am12] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterChoice &p = *part.p_con12;
            p.beginChangeGesture();
            p = 1;
            p.endChangeGesture();
        }
        //[/UserButtonCode_btn_am12]
    }
    else if (buttonThatWasClicked == btn_fm34.get())
    {
        //[UserButtonCode_btn_fm34] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterChoice &p = *part.p_con34;
            p.beginChangeGesture();
            p = 0;
            p.endChangeGesture();
        }
        //[/UserButtonCode_btn_fm34]
    }
    else if (buttonThatWasClicked == btn_am34.get())
    {
        //[UserButtonCode_btn_am34] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterChoice &p = *part.p_con34;
            p.beginChangeGesture();
            p = 1;
            p.endChangeGesture();
        }
        //[/UserButtonCode_btn_am34]
    }
    else if (buttonThatWasClicked == btn_about.get())
    {
        //[UserButtonCode_btn_about] -- add your button handler code here..
        popup_about_dialog();
        //[/UserButtonCode_btn_about]
    }
    else if (buttonThatWasClicked == btn_panic.get())
    {
        //[UserButtonCode_btn_panic] -- add your button handler code here..
        AdlplugAudioProcessor &proc = *proc_;
        std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
        if (proc.is_playback_ready())
            proc.panic_nonrt();
        //[/UserButtonCode_btn_panic]
    }
    else if (buttonThatWasClicked == btn_bank_load.get())
    {
        //[UserButtonCode_btn_bank_load] -- add your button handler code here..
        handle_load_bank(buttonThatWasClicked);
        //[/UserButtonCode_btn_bank_load]
    }
    else if (buttonThatWasClicked == btn_bank_save.get())
    {
        //[UserButtonCode_btn_bank_save] -- add your button handler code here..
        handle_save_bank(buttonThatWasClicked);
        //[/UserButtonCode_btn_bank_save]
    }
    else if (buttonThatWasClicked == btn_emulator.get())
    {
        //[UserButtonCode_btn_emulator] -- add your button handler code here..
        int selection = select_emulator_by_menu();
        if (selection != 0 && (unsigned)(selection - 1) != chip_settings_.emulator) {
            AudioParameterChoice &p = *pb.p_emulator;
            p.beginChangeGesture();
            p = selection - 1;
            p.endChangeGesture();
        }
        //[/UserButtonCode_btn_emulator]
    }
    else if (buttonThatWasClicked == btn_deep_tremolo.get())
    {
        //[UserButtonCode_btn_deep_tremolo] -- add your button handler code here..
        AudioParameterBool &p = *pb.p_deeptrem;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_deep_tremolo]
    }
    else if (buttonThatWasClicked == btn_deep_vibrato.get())
    {
        //[UserButtonCode_btn_deep_vibrato] -- add your button handler code here..
        AudioParameterBool &p = *pb.p_deepvib;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_deep_vibrato]
    }
    else if (buttonThatWasClicked == btn_algo_help.get())
    {
        //[UserButtonCode_btn_algo_help] -- add your button handler code here..
        CallOutBox::launchAsynchronously(new Algorithm_Help, btn_algo_help->getBounds(), this);
        //[/UserButtonCode_btn_algo_help]
    }
    else if (buttonThatWasClicked == btn_auto4ops.get())
    {
        //[UserButtonCode_btn_auto4ops] -- add your button handler code here..
        Messages::User::SelectOptimal4Ops msg;
        write_to_processor(msg.tag, &msg, sizeof(msg));
        //[/UserButtonCode_btn_auto4ops]
    }
    else if (buttonThatWasClicked == btn_keymap.get())
    {
        //[UserButtonCode_btn_keymap] -- add your button handler code here..
        handle_change_keymap();
        //[/UserButtonCode_btn_keymap]
    }
    else if (buttonThatWasClicked == btn_octave_up.get())
    {
        //[UserButtonCode_btn_octave_up] -- add your button handler code here..
        handle_change_octave(+1);
        //[/UserButtonCode_btn_octave_up]
    }
    else if (buttonThatWasClicked == btn_octave_down.get())
    {
        //[UserButtonCode_btn_octave_down] -- add your button handler code here..
        handle_change_octave(-1);
        //[/UserButtonCode_btn_octave_down]
    }
    else if (buttonThatWasClicked == btn_pgm_edit.get())
    {
        //[UserButtonCode_btn_pgm_edit] -- add your button handler code here..
        handle_edit_program();
        //[/UserButtonCode_btn_pgm_edit]
    }
    else if (buttonThatWasClicked == btn_pgm_add.get())
    {
        //[UserButtonCode_btn_pgm_add] -- add your button handler code here..
        handle_add_program();
        //[/UserButtonCode_btn_pgm_add]
    }

    //[UserbuttonClicked_Post]
    if (display_info_for_component(btn))
        expire_info_in();
    //[/UserbuttonClicked_Post]
}

void Main_Component::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Slider *sl = sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_tune12.get())
    {
        //[UserSliderCode_sl_tune12] -- add your slider handling code here..
        AudioParameterInt &p = *part.p_tune12;
        p = sl->getValue();
        //[/UserSliderCode_sl_tune12]
    }
    else if (sliderThatWasMoved == sl_tune34.get())
    {
        //[UserSliderCode_sl_tune34] -- add your slider handling code here..
        AudioParameterInt &p = *part.p_tune34;
        p = sl->getValue();
        //[/UserSliderCode_sl_tune34]
    }
    else if (sliderThatWasMoved == sl_num_chips.get())
    {
        //[UserSliderCode_sl_num_chips] -- add your slider handling code here..
        AudioParameterInt &p = *pb.p_nchip;
        set_int_parameter_with_delay(500, p, sl->getValue());
        //[/UserSliderCode_sl_num_chips]
    }
    else if (sliderThatWasMoved == sl_num_4ops.get())
    {
        //[UserSliderCode_sl_num_4ops] -- add your slider handling code here..
        AudioParameterInt &p = *pb.p_n4op;
        set_int_parameter_with_delay(500, p, sl->getValue());
        //[/UserSliderCode_sl_num_4ops]
    }
    else if (sliderThatWasMoved == sl_finetune34.get())
    {
        //[UserSliderCode_sl_finetune34] -- add your slider handling code here..
        AudioParameterInt &p = *part.p_voice2ft;
        double finetune34 = sl->getValue();
        int value;
        if(finetune34 > 0 && finetune34 <= 0.000025)
            value = +1;
        else if(finetune34 < 0 && finetune34 >= -0.000025)
            value = -1;
        else {
            value = (int)std::lround(finetune34 * (1000.0 / 15.625));
            value = (value < -127) ? -127 : value;
            value = (value > +127) ? +127 : value;
        }
        p = value;
        //[/UserSliderCode_sl_finetune34]
    }
    else if (sliderThatWasMoved == sl_veloffset.get())
    {
        //[UserSliderCode_sl_veloffset] -- add your slider handling code here..
        AudioParameterInt &p = *part.p_veloffset;
        p = sl->getValue();
        //[/UserSliderCode_sl_veloffset]
    }
    else if (sliderThatWasMoved == sl_midi_channel.get())
    {
        //[UserSliderCode_sl_midi_channel] -- add your slider handling code here..
        Messages::User::SetActivePart msg;
        msg.part = (unsigned)sl->getValue() - 1;
        write_to_processor(msg.tag, &msg, sizeof(msg));
        //[/UserSliderCode_sl_midi_channel]
    }

    //[UsersliderValueChanged_Post]
    display_info_for_component(sl);
    //[/UsersliderValueChanged_Post]
}

void Main_Component::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    ComboBox *cb = comboBoxThatHasChanged;
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cb_program.get())
    {
        //[UserComboBoxCode_cb_program] -- add your combo box handling code here..
        int selection = comboBoxThatHasChanged->getSelectedId();
        handle_selected_program(selection);
        //[/UserComboBoxCode_cb_program]
    }
    else if (comboBoxThatHasChanged == cb_percussion_key.get())
    {
        //[UserComboBoxCode_cb_percussion_key] -- add your combo box handling code here..
        AudioParameterInt &p = *part.p_drumnote;
        p.beginChangeGesture();
        p = cb->getSelectedId() - 1;
        p.endChangeGesture();
        //[/UserComboBoxCode_cb_percussion_key]
    }
    else if (comboBoxThatHasChanged == cb_volmodel.get())
    {
        //[UserComboBoxCode_cb_volmodel] -- add your combo box handling code here..
        AudioParameterChoice &p = *pb.p_volmodel;
        p.beginChangeGesture();
        p = cb->getSelectedId() - 1;
        p.endChangeGesture();
        //[/UserComboBoxCode_cb_volmodel]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Main_Component::sliderDragStarted(Slider *slider)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (slider == sl_tune12.get()) {
        AudioParameterInt &p = *part.p_tune12;
        p.beginChangeGesture();
    }
    else if (slider == sl_tune34.get()) {
        AudioParameterInt &p = *part.p_tune34;
        p.beginChangeGesture();
    }
    else if (slider == sl_finetune34.get()) {
        AudioParameterInt &p = *part.p_voice2ft;
        p.beginChangeGesture();
    }
    else if (slider == sl_veloffset.get()) {
        AudioParameterInt &p = *part.p_veloffset;
        p.beginChangeGesture();
    }

    display_info_for_component(slider);
}

void Main_Component::sliderDragEnded(Slider *slider)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (slider == sl_tune12.get()) {
        AudioParameterInt &p = *part.p_tune12;
        p.endChangeGesture();
    }
    else if (slider == sl_tune34.get()) {
        AudioParameterInt &p = *part.p_tune34;
        p.endChangeGesture();
    }
    else if (slider == sl_finetune34.get()) {
        AudioParameterInt &p = *part.p_voice2ft;
        p.endChangeGesture();
    }
    else if (slider == sl_veloffset.get()) {
        AudioParameterInt &p = *part.p_veloffset;
        p.endChangeGesture();
    }

    expire_info_in();
}

void Main_Component::knob_value_changed(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (k == kn_mastervol.get()) {
        AudioParameterFloat &p = *pb.p_mastervol;
        p = get_volume_knob_value();
        update_master_volume_label();
    }
    else if (k == kn_fb12.get()) {
        AudioParameterInt &p = *part.p_fb12;
        p = (int)std::lround(k->value());
    }
    else if (k == kn_fb34.get()) {
        AudioParameterInt &p = *part.p_fb34;
        p = (int)std::lround(k->value());
    }

    display_info_for_component(k);
}

void Main_Component::knob_drag_started(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (k == kn_mastervol.get()) {
        AudioParameterFloat &p = *pb.p_mastervol;
        p.beginChangeGesture();
    }
    else if (k == kn_fb12.get()) {
        AudioParameterInt &p = *part.p_fb12;
        p.beginChangeGesture();
    }
    else if (k == kn_fb34.get()) {
        AudioParameterInt &p = *part.p_fb34;
        p.beginChangeGesture();
    }

    display_info_for_component(k);
}

void Main_Component::knob_drag_ended(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (k == kn_mastervol.get()) {
        AudioParameterFloat &p = *pb.p_mastervol;
        p.endChangeGesture();
    }
    else if (k == kn_fb12.get()) {
        AudioParameterInt &p = *part.p_fb12;
        p.endChangeGesture();
    }
    else if (k == kn_fb34.get()) {
        AudioParameterInt &p = *part.p_fb34;
        p.endChangeGesture();
    }

    expire_info_in();
}

void Main_Component::set_instrument_parameters(const Instrument &ins, NotificationType ntf)
{
    trace("Update instrument parameters on display");

    Operator_Editor *op_editors[4] =
        { ed_op2.get(), ed_op1.get(), ed_op4.get(), ed_op3.get() };

   ((!ins.four_op()) ? btn_2op :
    (ins.pseudo_four_op()) ? btn_pseudo4op : btn_4op)->setToggleState(true, ntf);

   ((ins.con12()) ? btn_am12 : btn_fm12)->setToggleState(true, ntf);
   ((ins.con34()) ? btn_am34 : btn_fm34)->setToggleState(true, ntf);

   kn_fb12->set_value(ins.fb12(), ntf);
   kn_fb34->set_value(ins.fb34(), ntf);

   sl_tune12->setValue(ins.note_offset1, ntf);
   sl_tune34->setValue(ins.note_offset2, ntf);

   double finetune34;
   switch (ins.second_voice_detune) {
   case -1:
       finetune34 = -0.000025; break;
   case +1:
       finetune34 = +0.000025; break;
   default:
       finetune34 = ins.second_voice_detune * (15.625 / 1000.0); break;
   }
   sl_finetune34->setValue(finetune34, ntf);

   cb_percussion_key->setSelectedId(ins.percussion_key_number + 1, ntf);

   sl_veloffset->setValue(ins.midi_velocity_offset, ntf);

   for (unsigned op = 0; op < 4; ++op) {
       Operator_Editor *oped = op_editors[op];
       oped->set_operator_parameters(ins, op, ntf);
       oped->set_operator_enabled(op < 2 || (ins.inst_flags & ADLMIDI_Ins_4op));
   }
}

void Main_Component::set_chip_settings(NotificationType ntf)
{
    const Chip_Settings &cs = chip_settings_;
    update_emulator_icon();
    sl_num_chips->setValue(cs.chip_count, ntf);
    sl_num_4ops->setRange(0, 6 * cs.chip_count, 1);
    sl_num_4ops->setValue(cs.fourop_count, ntf);
}

void Main_Component::set_global_parameters(NotificationType ntf)
{
    cb_volmodel->setSelectedId(instrument_gparam_.volume_model + 1, ntf);
    btn_deep_tremolo->setToggleState(instrument_gparam_.deep_tremolo, ntf);
    btn_deep_vibrato->setToggleState(instrument_gparam_.deep_vibrato, ntf);
}

void Main_Component::on_change_midi_channel(unsigned channel)
{
    if (channel > 15)
        return;

    unsigned old_channel = midichannel_;
    if (channel == old_channel)
        return;

    trace("Change MIDI channel to %u", channel + 1);

    midichannel_ = channel;

    Operator_Editor *op_editors[4] =
        { ed_op2.get(), ed_op1.get(), ed_op4.get(), ed_op3.get() };
    for (Operator_Editor *oped : op_editors)
        oped->set_midi_channel(channel);

    midi_kb->setMidiChannel(channel + 1);
    sl_midi_channel->setValue(channel + 1, dontSendNotification);

    if (is_percussion_channel(channel) != is_percussion_channel(old_channel))
        update_instrument_choices();

    set_program_selection(midiprogram_[channel] + 1, dontSendNotification);
    reload_selected_instrument(dontSendNotification);
}

void Main_Component::popup_about_dialog()
{
    if (dlg_about_)
        return;

    DialogWindow::LaunchOptions dlgopts;
    dlgopts.dialogTitle = "About " JucePlugin_Name;
    dlgopts.content.set(new About_Component, true);
    dlgopts.componentToCentreAround = this;
    dlgopts.resizable = false;
    dlg_about_ = dlgopts.launchAsync();
}

bool Main_Component::display_info_for_component(Component *c)
{
    String param;
    int val = 0;

    Knob *kn = static_cast<Knob *>(c);

    if (c == kn_fb12.get()) {
        param = "Feedback 1-2";
        val = (int)std::lround(kn->value());
    }
    else if (c == kn_fb34.get()) {
        param = "Feedback 3-4";
        val = (int)std::lround(kn->value());
    }

    if (param.isEmpty()) {
        for (Operator_Editor *ed : { ed_op1.get(), ed_op2.get(), ed_op3.get(), ed_op4.get() }) {
            if (ed->display_info_for_component(c))
                return true;
        }
        return false;
    }

    display_info(param + " = " + String(val));
    return true;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Main_Component" componentName=""
                 parentClasses="public Generic_Main_Component&lt;Main_Component&gt;, public Knob::Listener"
                 constructorParams="AdlplugAudioProcessor &amp;proc, Parameter_Block &amp;pb, Configuration &amp;conf"
                 variableInitialisers="Generic_Main_Component(proc, pb, conf)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.66"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 800 600" fill=" radial: 150 50, 800 600, 0=ff3f6a8b, 1=ff274c70"
          hasStroke="0"/>
    <TEXT pos="16 288 264 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 3"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="16 132 264 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 1"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="300 132 264 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 2"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="300 288 264 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 4"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <RECT pos="578 8 92 12" fill="solid: 8e989b" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
    <RECT pos="578 24 92 12" fill="solid: 8e989b" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
    <TEXT pos="586 132 188 30" fill="solid: fff0f8ff" hasStroke="0" text="Algorithm"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="586 288 188 30" fill="solid: fff0f8ff" hasStroke="0" text="Tuning"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="300 444 188 30" fill="solid: fff0f8ff" hasStroke="0" text="Misc"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="586 422 188 30" fill="solid: fff0f8ff" hasStroke="0" text="Global"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <ROUNDRECT pos="16 474 264 36" cornerSize="7.0" fill="solid: ff3c485f" hasStroke="0"/>
    <ROUNDRECT pos="300 474 188 36" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="546 450 228 60" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="586 316 188 106" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="586 160 188 128" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="606 52 168 80" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
    <ROUNDRECT pos="460 72 102 60" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
    <ROUNDRECT pos="300 72 146 60" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="423f2b5d9aff978c" memberName="ed_op2"
                    virtualName="" explicitFocusOrder="0" pos="300 160 264 128" class="Operator_Editor"
                    params="WOPL_OP_CARRIER1, pb"/>
  <TEXTBUTTON name="new button" id="333aa0ccccbfed24" memberName="btn_4op"
              virtualName="" explicitFocusOrder="0" pos="598 174 56 24" bgColOn="ff42a2c8"
              buttonText="4 op" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="64d70bb49227f0d8" memberName="btn_pseudo4op"
              virtualName="" explicitFocusOrder="0" pos="653 174 56 24" bgColOn="ff42a2c8"
              buttonText="2x2 op" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="402f6ebfee1d48e6" memberName="btn_2op"
              virtualName="" explicitFocusOrder="0" pos="708 174 56 24" bgColOn="ff42a2c8"
              buttonText="2 op" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b2cb238ae0314374" memberName="btn_fm12"
              virtualName="" explicitFocusOrder="0" pos="606 208 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f884f98bb82f318" memberName="btn_am12"
              virtualName="" explicitFocusOrder="0" pos="606 225 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="a00c5401e39a953e" memberName="ed_op1"
                    virtualName="" explicitFocusOrder="0" pos="16 160 264 128" class="Operator_Editor"
                    params="WOPL_OP_MODULATOR1, pb"/>
  <GENERICCOMPONENT name="new component" id="b7424f0838e48a08" memberName="ed_op4"
                    virtualName="" explicitFocusOrder="0" pos="300 316 264 128" class="Operator_Editor"
                    params="WOPL_OP_CARRIER2, pb"/>
  <TEXTBUTTON name="new button" id="6c84b2cc5c27a17f" memberName="btn_fm34"
              virtualName="" explicitFocusOrder="0" pos="686 208 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c55383265bc18fb0" memberName="btn_am34"
              virtualName="" explicitFocusOrder="0" pos="686 225 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="4bf73df293534890" memberName="ed_op3"
                    virtualName="" explicitFocusOrder="0" pos="16 316 264 128" class="Operator_Editor"
                    params="WOPL_OP_MODULATOR2, pb"/>
  <SLIDER name="new slider" id="9cd7cc232d55ac8a" memberName="sl_tune12"
          virtualName="" explicitFocusOrder="0" pos="696 324 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="-127.0" max="127.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="314f46c5e90647" memberName="sl_tune34"
          virtualName="" explicitFocusOrder="0" pos="696 348 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="-127.0" max="127.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="new component" id="8e0915367ccd00d3" memberName="kn_fb12"
                    virtualName="" explicitFocusOrder="0" pos="648 245 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="59510781248f1393" memberName="kn_fb34"
                    virtualName="" explicitFocusOrder="0" pos="736 245 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="4d4a20a681c7e721" memberName="midi_kb"
                    virtualName="" explicitFocusOrder="0" pos="16 520 730 64" class="Midi_Keyboard_Ex"
                    params="midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard"/>
  <IMAGEBUTTON name="new button" id="1c21a98bd6493eb8" memberName="btn_about"
               virtualName="" explicitFocusOrder="0" pos="16 8 232 40" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="logo_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <LABEL name="new label" id="9fd95e8efc5b0fe1" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="264 8 192 40" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM synthesizer with YMF262 chip emulation"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="8c38ad0cbcf109f7" memberName="vu_left"
                    virtualName="" explicitFocusOrder="0" pos="578 8 92 12" class="Vu_Meter"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="16bc493366b76ca5" memberName="vu_right"
                    virtualName="" explicitFocusOrder="0" pos="578 24 92 12" class="Vu_Meter"
                    params=""/>
  <LABEL name="new label" id="bda2c045916cddf3" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="684 8 40 24" textCol="fff0f8ff" edTextCol="ff000000"
         edBkgCol="0" labelText="CPU" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="34"/>
  <LABEL name="new label" id="2067136cf573bd1a" memberName="lbl_cpu" virtualName=""
         explicitFocusOrder="0" pos="724 8 48 24" textCol="fff0f8ff" outlineCol="ff8e989b"
         edTextCol="ff000000" edBkgCol="0" labelText="100%" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="b87acb622e25d16e" memberName="ind_midi_activity"
                    virtualName="" explicitFocusOrder="0" pos="466 8 102 28" class="Indicator_NxM"
                    params="2, 8"/>
  <TEXTBUTTON name="new button" id="ec00354399da2ced" memberName="btn_panic"
              virtualName="" explicitFocusOrder="0" pos="308 102 48 24" bgColOn="ff42a2c8"
              buttonText="Panic" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="62544efea1101020" memberName="edt_bank_name"
              virtualName="" explicitFocusOrder="0" pos="16 74 215 24" outlinecol="ff8e989b"
              initialText="" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <COMBOBOX name="new combo box" id="396a835342f6b630" memberName="cb_program"
            virtualName="" explicitFocusOrder="0" pos="16 104 215 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="ac425a00ee881383" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="304 78 56 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Part" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="c78c3c0db0e0be9a" memberName="btn_bank_load"
              virtualName="" explicitFocusOrder="0" pos="235 74 23 24" buttonText=""
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="fd2ff0a2438003e5" memberName="btn_bank_save"
              virtualName="" explicitFocusOrder="0" pos="257 74 23 24" buttonText=""
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="46218839ef803ea8" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="622 208 56 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM 1-2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="7daa01d81d205529" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="702 208 56 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM 3-4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1c8f55c1c8ab019" memberName="label8" virtualName=""
         explicitFocusOrder="0" pos="622 225 56 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="AM 1-2" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="bcca96e3f9921089" memberName="label9" virtualName=""
         explicitFocusOrder="0" pos="702 225 56 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="AM 3-4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="6ff60ad947a8f168" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="584 248 72 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Feedback&#10;1-2"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="30f33e05a10bcf08" memberName="label12" virtualName=""
         explicitFocusOrder="0" pos="672 248 72 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Feedback&#10;3-4"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <LABEL name="new label" id="94205e2c6bef35b1" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="590 324 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Note offset 1-2"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="885b9d0baab4b5bb" memberName="label13" virtualName=""
         explicitFocusOrder="0" pos="590 348 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Note offset 3-4"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <IMAGEBUTTON name="new button" id="1df5353a837ca5f4" memberName="btn_emulator"
               virtualName="" explicitFocusOrder="0" pos="659 56 76 20" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="" opacityNormal="1.0" colourNormal="0" resourceOver=""
               opacityOver="1.0" colourOver="0" resourceDown="" opacityDown="1.0"
               colourDown="0"/>
  <LABEL name="new label" id="61dc1fae1b35b41b" memberName="label14" virtualName=""
         explicitFocusOrder="0" pos="611 56 48 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Core" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="4c8fba05f57b7320" memberName="sl_num_chips"
          virtualName="" explicitFocusOrder="0" pos="659 80 76 20" rotaryslideroutline="ff8e98ff"
          textboxtext="fff0f8ff" textboxoutline="ff8e989b" min="1.0" max="100.0"
          int="1.0" style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="7b8136bebc755c42" memberName="label15" virtualName=""
         explicitFocusOrder="0" pos="611 80 48 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Chips" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="e0a305f99bf634b2" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="611 104 48 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="4 ops" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="8ab12bd836d92a62" memberName="sl_num_4ops"
          virtualName="" explicitFocusOrder="0" pos="659 104 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="1.0" max="600.0" int="1.0" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="36"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="1b1aa7f42f11af53" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="590 372 104 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Percussion key" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="72b8aa8d99fdb9cc" memberName="cb_percussion_key"
            virtualName="" explicitFocusOrder="0" pos="696 372 74 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="5917d9d90d014675" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="590 372 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Percussion key"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="new label" id="4acd0a642a0be89a" memberName="label18" virtualName=""
         explicitFocusOrder="0" pos="590 396 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Fine tune 3-4" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <SLIDER name="new slider" id="3cc0e1cfce5f68cd" memberName="sl_finetune34"
          virtualName="" explicitFocusOrder="0" pos="696 396 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="-2.0" max="2.0" int="0.015625"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="new slider" id="902cdb29676bc0b6" memberName="sl_veloffset"
          virtualName="" explicitFocusOrder="0" pos="410 482 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="-127.0" max="127.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="b2db0b3cb9c241dc" memberName="label19" virtualName=""
         explicitFocusOrder="0" pos="304 482 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Velocity offset"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <SLIDER name="new slider" id="59a5ffdc8abc84a6" memberName="sl_midi_channel"
          virtualName="" explicitFocusOrder="0" pos="364 79 76 46" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="1.0" max="16.0" int="1.0" style="IncDecButtons"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="46"
          textBoxHeight="46" skewFactor="1.0" needsCallback="1"/>
  <TEXTBUTTON name="new button" id="fd73fd08b6289ecb" memberName="btn_deep_tremolo"
              virtualName="" explicitFocusOrder="0" pos="552 486 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="bf1838a34852c77b" memberName="label20" virtualName=""
         explicitFocusOrder="0" pos="568 486 94 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Deep tremolo" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="e446fd30ddc83460" memberName="btn_deep_vibrato"
              virtualName="" explicitFocusOrder="0" pos="664 486 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="f2fdd9093e241907" memberName="label21" virtualName=""
         explicitFocusOrder="0" pos="680 486 94 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Deep vibrato" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="96b421fbb370c8db" memberName="label22" virtualName=""
         explicitFocusOrder="0" pos="552 458 106 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Volume model" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="7289bfbfa6c092b0" memberName="cb_volmodel"
            virtualName="" explicitFocusOrder="0" pos="664 457 104 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TEXTBUTTON name="new button" id="5a50a3a87b4f5d76" memberName="btn_algo_help"
              virtualName="" explicitFocusOrder="0" pos="754 136 20 20" buttonText="?"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="988d753c82121162" memberName="btn_auto4ops"
              virtualName="" explicitFocusOrder="0" pos="739 102 24 24" tooltip="Choose a recommended value"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="" id="39bffafade9476c7" memberName="btn_keymap" virtualName=""
              explicitFocusOrder="0" pos="750 520 24 24" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ecb42a974d834d93" memberName="btn_octave_up"
              virtualName="" explicitFocusOrder="0" pos="750 549 24 18" tooltip="Octave"
              buttonText="+" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="4187eb5f8c289604" memberName="btn_octave_down"
              virtualName="" explicitFocusOrder="0" pos="750 566 24 18" tooltip="Octave"
              buttonText="-" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ec8a402c551d38af" memberName="btn_pgm_edit"
              virtualName="" explicitFocusOrder="0" pos="235 104 23 24" buttonText=""
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="44f25f4ad0c28c0f" memberName="btn_pgm_add"
              virtualName="" explicitFocusOrder="0" pos="257 104 23 24" buttonText=""
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="9d73506941d5ef91" memberName="kn_mastervol"
                    virtualName="" explicitFocusOrder="0" pos="510 78 48 48" class="Styled_Knob_Default"
                    params=""/>
  <LABEL name="new label" id="72b9b823ebcf0c96" memberName="label23" virtualName=""
         explicitFocusOrder="0" pos="464 78 56 22" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Volume" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="7108230db9c0dcf2" memberName="lbl_mastervol"
         virtualName="" explicitFocusOrder="0" pos="468 102 44 22" textCol="fff0f8ff"
         outlineCol="ff8e989b" edTextCol="ff000000" edBkgCol="0" labelText="-20 dB"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="new label" id="d7c1d56634e337b7" memberName="lbl_info"
         virtualName="" explicitFocusOrder="0" pos="16 474 264 36" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="OPL Ready!" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default monospaced font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: logo_png, 7636, "../../resources/ui/logo.png"
static const unsigned char resource_Main_Component_logo_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,240,0,0,0,39,8,6,0,0,0,49,110,197,5,0,0,0,4,115,66,73,84,8,8,8,8,124,8,100,136,0,0,
0,9,112,72,89,115,0,0,14,196,0,0,14,196,1,149,43,14,27,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,0,29,81,73,68,
65,84,120,156,229,157,121,124,92,197,149,239,191,117,239,237,69,173,214,46,89,187,44,201,182,44,91,54,139,13,4,75,16,48,139,237,96,120,64,2,9,239,205,64,156,9,147,121,51,36,19,67,66,146,151,100,18,103,
102,62,67,38,201,4,156,100,30,153,124,94,96,248,76,38,1,195,203,2,216,193,56,60,59,182,21,54,3,222,45,47,178,44,107,181,22,107,235,150,122,187,183,222,31,173,110,107,233,123,111,183,36,27,146,249,125,
62,247,243,145,110,157,58,117,78,85,157,58,167,150,91,45,152,5,122,182,62,90,20,38,220,6,104,169,228,147,130,123,203,110,123,226,133,153,150,123,230,149,141,197,14,157,206,153,230,143,65,64,88,34,124,
72,217,15,162,21,193,17,36,175,71,8,108,159,127,251,147,3,51,229,219,177,117,99,11,80,153,130,32,157,165,183,61,81,58,211,242,204,208,190,117,227,33,1,203,102,154,223,48,140,154,242,59,126,112,210,44,
189,243,229,191,93,41,133,178,111,166,252,129,29,165,235,159,88,147,40,161,101,231,38,183,115,116,112,44,37,110,146,189,165,183,63,113,189,89,114,231,203,27,55,74,193,227,41,202,152,8,126,96,12,232,4,
90,144,242,93,169,136,189,165,190,142,223,139,143,63,175,207,148,105,199,214,141,111,3,87,165,146,71,153,105,97,0,97,66,247,147,162,241,2,40,146,13,179,41,119,174,32,193,1,50,7,193,66,132,188,25,228,223,
34,228,47,52,225,234,234,124,121,227,243,29,219,30,185,242,253,150,241,253,132,80,196,77,86,233,82,88,167,255,9,35,29,200,7,46,3,238,68,136,111,9,201,107,157,233,37,237,157,219,54,254,99,219,43,15,231,
94,42,65,82,54,190,201,80,54,128,228,151,157,165,108,105,47,183,165,94,150,57,196,215,107,143,33,97,237,153,87,54,22,87,174,123,162,107,118,229,195,195,7,47,167,43,144,54,227,252,46,85,103,158,43,72,121,
218,24,203,51,7,185,42,103,128,12,45,226,146,130,123,144,198,199,58,182,110,252,169,91,232,95,200,187,237,135,195,169,242,126,178,101,1,191,239,45,48,77,111,200,235,227,115,11,78,205,88,246,100,241,116,
107,21,219,207,21,38,77,255,55,213,205,124,56,191,23,129,184,9,248,55,51,58,137,88,45,128,237,231,10,121,186,181,42,105,254,159,40,59,203,221,37,201,7,80,127,119,116,25,39,125,94,211,244,143,150,118,240,
241,210,182,164,249,181,142,166,243,229,195,203,147,166,79,132,76,71,152,98,119,128,74,207,40,87,102,13,176,60,107,8,85,80,36,37,95,83,116,249,80,199,214,135,31,41,93,255,248,211,51,225,253,179,182,10,
94,238,42,49,77,95,154,57,204,55,106,143,2,179,48,224,182,151,30,190,6,228,82,128,31,183,46,224,200,72,166,109,158,215,135,115,249,235,234,102,242,156,33,205,97,112,63,240,157,153,150,31,195,59,35,57,
52,251,205,27,55,85,184,84,131,181,5,221,60,84,121,138,101,153,195,2,120,48,32,213,235,207,110,251,220,250,138,219,126,216,156,10,175,19,126,47,123,6,243,77,211,243,211,66,179,21,55,41,52,143,121,44,229,
152,138,249,61,126,62,156,223,11,176,90,74,132,16,200,169,52,251,246,125,198,33,186,185,30,1,47,245,148,166,196,191,190,160,63,105,90,128,253,190,108,222,29,204,54,77,95,145,155,218,108,199,167,171,41,
201,155,12,138,92,1,62,81,218,198,103,230,159,38,219,17,206,6,249,84,231,182,141,55,12,248,178,63,83,247,241,77,41,53,116,203,88,186,165,124,17,33,226,127,207,56,132,86,133,220,0,112,120,36,147,35,163,
153,160,98,251,68,132,194,47,187,203,162,12,230,42,140,78,162,220,84,158,32,10,47,246,150,176,110,223,245,124,245,248,114,198,116,21,96,177,38,213,61,221,47,126,54,121,55,3,209,218,181,42,111,86,19,152,
57,148,99,202,211,120,161,243,20,180,191,242,249,132,174,170,184,199,115,13,2,175,148,130,198,161,188,212,234,89,36,226,56,11,249,103,82,143,115,220,111,186,35,110,54,183,46,162,225,245,155,248,245,185,
232,114,134,148,124,50,219,59,240,172,220,114,175,154,146,108,34,121,125,83,246,192,21,53,21,213,153,217,233,171,195,134,124,64,83,5,91,206,149,131,154,124,139,60,215,83,206,95,86,156,6,88,210,182,245,
243,31,42,95,191,249,205,100,242,213,213,213,57,7,141,209,251,0,254,231,15,246,100,255,244,161,241,181,10,69,164,84,126,178,144,8,158,233,170,228,128,63,155,159,93,246,38,57,90,168,88,87,181,173,221,219,
191,120,117,209,218,239,249,167,210,151,47,174,250,111,134,66,54,128,63,168,123,211,93,170,189,108,23,193,128,203,150,84,223,38,145,249,0,35,99,225,236,204,52,7,136,212,234,168,37,228,165,51,224,166,196,
29,64,213,197,77,192,193,9,252,23,73,228,170,215,143,247,220,185,170,102,30,39,71,189,244,68,220,209,142,149,44,44,68,41,93,82,117,63,32,238,248,242,115,142,237,223,92,27,125,169,96,45,191,21,191,218,
234,26,132,188,22,96,231,193,174,171,111,188,188,56,229,250,72,5,131,134,147,135,142,173,160,201,159,193,87,170,155,64,138,187,59,211,203,30,3,190,148,136,190,188,182,114,173,33,68,33,192,121,95,48,47,
215,235,178,239,55,234,44,60,176,161,138,218,191,88,93,243,33,77,21,233,97,41,248,85,95,89,194,81,162,62,183,63,225,251,99,99,153,28,242,103,1,32,16,159,74,165,108,33,113,9,137,203,136,224,188,160,204,
197,125,246,251,179,121,224,200,53,4,13,5,96,137,30,9,39,12,251,13,112,198,228,147,82,70,107,248,125,240,192,2,35,117,57,18,121,225,225,168,23,150,130,73,11,85,210,208,203,132,196,85,85,224,173,3,104,
28,202,79,189,94,45,244,22,140,203,174,227,138,191,156,5,63,105,24,74,172,62,34,82,215,98,133,92,236,126,243,195,142,69,252,168,109,97,76,138,47,180,189,180,241,198,68,242,25,40,142,120,191,54,198,53,
73,193,3,167,212,133,10,47,43,76,7,165,228,186,186,194,122,128,29,3,69,156,55,156,211,10,80,53,201,99,139,14,162,105,50,161,0,91,206,149,199,228,252,68,219,150,135,103,190,2,21,211,192,66,217,101,153,
195,220,62,175,51,254,220,90,112,142,107,114,206,83,230,25,75,186,49,222,245,231,240,88,235,146,241,2,197,95,181,191,244,200,229,115,33,219,7,53,132,70,133,189,195,241,48,250,6,185,115,83,60,82,19,66,
45,206,77,119,59,10,178,220,213,48,110,232,169,118,240,185,14,161,103,226,76,109,100,92,147,223,61,169,223,220,152,215,195,149,89,3,100,56,35,73,235,249,157,246,90,246,141,228,2,40,138,194,230,164,67,
233,20,250,77,74,33,180,26,113,215,92,81,153,151,91,146,235,89,2,240,92,111,121,148,225,20,172,204,24,96,97,154,143,171,179,207,243,250,112,222,180,244,95,13,148,242,119,198,81,156,138,145,173,164,203,
187,128,95,164,34,199,100,161,72,40,67,12,159,40,58,203,95,20,182,36,76,235,14,185,121,177,191,132,127,235,94,64,119,200,109,89,204,83,189,85,220,87,120,150,90,207,136,170,168,242,107,192,199,109,101,
139,53,132,85,250,165,128,141,28,30,85,103,204,80,136,57,108,128,198,145,184,1,103,182,143,14,172,4,222,172,174,174,206,10,33,211,239,168,175,88,160,42,138,195,0,94,247,231,77,226,173,10,137,67,24,4,12,
139,2,83,213,123,174,235,49,230,225,44,240,221,133,7,200,119,76,95,123,210,165,224,216,104,6,207,244,84,241,66,95,25,33,195,188,112,29,193,87,207,44,227,183,203,246,160,10,121,89,135,183,244,110,192,254,
252,67,10,250,166,166,186,20,53,247,175,94,180,74,8,148,158,176,139,93,190,121,9,71,136,181,121,221,0,172,205,237,78,152,62,32,157,236,24,140,111,107,108,72,73,134,68,202,36,49,58,75,41,35,225,136,49,
26,142,24,163,177,172,69,206,0,159,41,62,205,174,203,118,114,83,110,143,37,31,93,17,108,238,174,137,241,186,251,236,111,62,107,190,206,31,131,93,40,116,113,166,97,73,203,177,36,99,152,195,87,189,34,79,
94,181,141,133,30,255,164,180,46,61,141,211,129,116,0,212,232,118,18,145,180,72,41,192,85,11,242,106,1,142,140,102,49,40,29,147,242,93,149,57,64,243,213,219,216,191,242,85,74,210,2,115,163,247,92,123,
244,100,120,142,35,98,200,96,56,98,140,70,12,25,132,232,0,181,44,125,152,239,86,29,224,87,75,27,153,231,54,209,113,252,57,18,204,226,213,161,104,95,87,164,252,155,164,100,75,38,196,31,71,210,30,184,178,
174,178,200,64,100,93,81,149,187,10,224,151,3,101,68,76,38,218,107,178,163,6,188,38,167,155,77,93,117,9,105,158,27,44,103,125,110,23,192,45,29,219,31,41,47,93,251,253,182,100,101,153,4,13,107,45,198,135,
168,214,30,255,193,79,124,231,181,31,3,40,42,98,245,178,146,162,91,87,148,45,187,182,166,224,214,12,39,217,79,47,124,155,117,39,174,231,216,152,249,118,216,182,145,34,206,71,156,228,106,33,77,117,168,
31,5,126,100,41,155,106,35,91,114,1,213,236,97,34,135,75,51,200,81,195,2,160,62,187,143,147,125,147,183,227,26,125,249,84,187,253,72,184,9,120,44,98,104,37,10,146,170,194,204,218,88,250,84,190,13,217,
125,0,20,104,65,28,154,1,70,2,121,82,245,152,118,245,56,19,15,156,100,207,127,190,177,229,153,39,126,125,104,31,64,110,186,219,177,102,101,105,229,173,87,150,126,168,174,44,187,225,138,244,65,245,63,106,
222,100,253,137,15,19,145,230,163,200,127,246,87,242,145,236,110,36,226,195,231,126,243,249,194,194,59,55,159,179,44,84,177,145,111,66,191,73,90,117,221,16,53,247,212,87,47,240,122,28,133,0,91,134,202,
19,142,12,11,61,62,170,93,209,69,218,249,206,81,106,211,135,19,210,253,126,116,30,61,97,55,128,34,195,198,3,201,202,49,13,51,152,31,25,58,242,181,3,157,93,95,121,250,173,29,159,254,193,158,191,31,242,
7,59,52,97,240,88,217,33,75,94,17,69,97,199,72,44,114,16,183,206,90,182,75,21,66,155,141,232,19,202,111,200,152,190,232,184,119,52,30,70,55,236,124,122,131,91,32,139,11,179,211,220,121,94,87,37,64,227,
232,244,249,111,125,70,223,5,166,115,165,247,92,215,99,50,30,46,65,191,57,239,15,132,159,221,221,124,242,211,155,119,255,236,201,87,155,54,27,134,52,150,165,13,243,64,193,25,75,94,123,198,242,24,137,174,
159,169,186,166,220,60,151,250,38,167,250,141,55,106,134,148,213,31,89,89,86,15,176,127,44,155,227,225,76,80,197,180,103,77,246,228,193,101,77,214,185,132,116,17,69,225,133,161,232,158,176,16,124,82,202,
25,6,148,9,120,79,122,198,185,38,58,140,0,208,220,53,236,123,234,119,39,127,14,112,117,250,121,22,120,252,150,252,246,141,141,159,146,147,172,176,149,45,182,253,97,246,40,151,40,134,86,44,100,24,71,189,
183,15,101,74,218,31,198,10,24,111,150,52,25,150,107,132,196,125,215,181,243,23,41,138,80,34,82,225,205,64,222,36,250,52,135,193,74,207,128,125,185,34,69,189,173,228,159,9,63,176,239,55,54,120,102,199,
137,166,163,109,3,191,7,184,47,183,205,146,87,68,81,57,16,200,137,102,148,70,18,253,198,70,182,84,13,184,180,251,108,117,129,215,153,190,168,36,235,42,128,231,70,42,76,71,135,181,25,209,240,89,74,25,1,
88,155,153,120,30,140,10,91,70,226,199,47,23,117,109,125,248,186,100,100,153,174,108,98,222,83,71,171,242,2,111,43,70,120,155,128,214,169,44,182,52,54,159,242,5,245,62,128,91,51,206,89,242,59,20,138,159,
8,42,235,222,254,197,121,115,33,219,69,135,153,28,19,202,207,81,67,44,241,12,77,74,63,143,147,99,193,12,0,60,14,231,109,0,87,86,69,231,191,251,3,217,248,133,54,137,254,106,239,121,156,98,66,204,156,130,
119,179,132,157,183,188,24,30,120,28,183,173,40,223,107,24,114,143,84,24,154,202,230,165,183,219,222,0,168,115,15,81,234,178,222,213,56,24,204,138,149,109,111,192,23,97,27,169,230,193,181,75,87,56,84,
197,29,148,10,191,241,149,36,100,156,239,12,177,34,237,60,0,111,159,234,125,21,224,114,247,16,69,174,196,19,253,147,122,6,239,141,143,76,146,232,201,174,148,145,100,40,36,132,144,237,39,218,59,218,154,
90,94,53,36,187,153,224,145,13,29,217,125,222,223,12,80,155,150,56,228,143,61,77,122,38,17,25,173,182,136,17,178,62,80,155,66,67,92,84,36,97,192,0,13,222,190,105,52,141,99,209,48,58,211,235,184,14,160,
178,200,187,24,160,49,48,61,124,110,72,239,157,204,112,174,244,126,31,182,145,98,60,51,211,157,193,206,19,103,154,58,230,181,252,82,17,156,153,200,226,183,251,206,182,234,134,17,6,88,236,177,238,55,135,
67,81,3,150,8,251,67,216,201,108,251,77,32,181,68,254,226,197,25,2,89,188,106,201,188,122,128,87,70,139,25,18,211,247,126,81,225,150,140,110,20,32,18,49,198,190,245,179,125,219,66,17,195,47,144,150,94,
237,57,127,69,180,32,193,189,221,219,191,152,110,171,92,170,202,38,104,220,206,227,45,199,13,67,57,54,241,93,247,249,177,46,128,5,14,159,37,191,176,34,24,48,28,209,76,134,58,125,143,44,21,217,222,239,
57,176,58,153,172,193,147,96,30,28,28,55,96,183,163,246,234,69,249,121,217,94,87,57,192,222,64,193,52,218,235,60,125,147,25,154,233,63,215,251,192,23,209,3,199,177,139,72,129,39,119,151,64,250,98,175,
130,97,195,240,141,133,123,0,22,56,253,150,252,122,100,124,155,50,87,110,218,100,45,241,92,206,129,61,34,82,243,161,197,249,121,133,57,105,139,1,182,248,19,47,94,161,194,186,244,232,199,69,173,253,35,
135,251,124,225,80,107,143,239,32,192,218,244,46,211,60,191,30,43,37,32,85,128,140,136,30,254,152,157,60,211,96,215,16,38,26,234,46,223,254,137,94,120,56,16,242,1,100,106,97,91,158,195,227,6,172,72,204,
79,216,195,7,199,128,147,244,192,215,166,245,77,59,124,243,70,40,159,136,84,16,138,80,63,187,190,238,118,129,16,65,169,240,110,36,123,18,93,134,35,194,114,215,148,40,115,174,244,158,97,27,207,138,103,
2,188,243,206,59,97,137,114,120,226,187,177,144,158,84,191,25,22,241,101,101,229,116,195,88,134,165,108,115,24,66,11,41,244,69,255,253,250,69,245,2,33,186,244,52,246,132,167,143,188,168,144,166,233,92,
231,142,142,192,239,157,58,127,0,224,173,147,61,7,0,26,92,125,120,181,196,39,88,70,132,198,43,129,162,113,97,82,59,90,25,215,192,74,89,147,209,254,220,193,115,126,33,100,60,230,27,13,134,131,0,233,66,
183,55,96,25,53,96,169,200,63,41,3,246,138,8,203,221,131,147,104,124,138,198,193,112,52,252,171,41,205,90,5,176,47,148,75,64,81,39,209,93,155,214,143,58,117,157,240,82,121,224,84,249,205,196,3,143,35,
168,166,77,58,21,20,214,101,0,32,93,177,62,161,53,44,28,241,60,158,72,48,199,82,190,185,242,192,229,139,203,139,133,34,50,151,87,231,173,2,120,33,80,134,174,137,11,123,175,19,158,235,61,189,164,9,29,105,
72,253,231,187,155,15,3,60,187,231,212,17,221,48,194,78,97,112,163,167,39,97,62,52,216,18,140,134,209,82,114,67,202,95,252,152,240,140,63,22,26,234,186,26,143,249,12,93,74,32,218,9,109,120,26,177,121,
181,180,106,106,162,149,61,67,217,230,20,177,125,197,68,207,56,70,131,145,243,0,215,165,245,77,163,217,27,142,134,209,138,34,20,24,255,127,10,77,131,59,58,22,250,131,250,249,56,211,185,210,123,174,235,
49,182,15,108,245,152,12,10,189,71,142,248,164,32,24,251,223,144,209,126,163,40,214,253,70,159,248,1,130,46,236,67,104,43,217,212,201,164,166,144,66,173,249,31,55,212,212,120,93,106,62,192,11,161,10,204,
150,182,215,185,163,171,207,61,195,129,163,29,125,254,81,128,158,129,96,176,115,96,172,9,96,173,187,219,52,239,158,72,1,157,122,26,128,208,85,237,147,150,202,77,83,118,230,91,12,234,132,149,69,175,199,
233,6,24,70,179,230,167,10,92,177,149,86,137,245,149,47,194,70,182,75,182,141,132,109,249,29,253,163,199,1,234,93,253,211,232,26,245,201,151,18,52,70,10,166,209,92,231,138,142,133,237,125,35,23,214,22,
146,216,6,73,10,118,91,62,51,10,161,103,190,141,36,13,6,99,127,187,52,37,13,96,68,56,44,249,185,213,11,209,201,152,38,173,251,141,221,246,227,196,193,192,140,71,93,93,157,83,72,170,214,93,89,90,15,240,
118,36,151,83,120,19,186,116,69,149,220,236,28,55,224,161,177,29,19,249,28,108,233,223,15,112,179,171,7,77,51,18,230,55,84,193,11,225,248,150,210,39,109,39,249,19,97,23,10,89,180,133,68,198,71,82,143,
83,77,3,240,161,217,242,76,19,209,107,143,12,145,76,67,88,60,31,160,16,250,112,235,64,19,192,213,90,63,78,117,114,59,237,211,115,9,142,175,188,251,164,198,1,153,51,41,61,79,11,82,171,141,0,112,180,117,
240,184,109,185,127,196,33,52,128,42,46,244,27,167,166,185,33,102,192,230,252,210,212,72,60,191,28,179,27,248,109,100,75,230,99,134,129,176,191,186,56,215,227,173,42,202,88,1,176,37,82,97,170,216,10,117,
128,124,37,122,240,123,249,252,220,71,222,252,151,59,167,209,100,137,16,215,58,251,217,27,73,124,197,204,150,72,57,159,227,36,2,89,217,113,205,224,141,192,255,179,84,50,6,219,89,188,121,82,68,26,97,109,
156,32,61,205,145,14,48,44,157,182,13,56,79,9,68,139,22,194,254,72,156,21,175,75,189,10,61,21,19,222,189,118,160,253,196,221,171,230,203,52,161,139,21,206,1,222,208,47,44,176,7,80,121,199,200,165,94,237,
227,77,61,111,218,17,218,85,90,31,2,73,196,32,180,167,169,251,244,221,245,149,23,248,39,170,255,247,251,99,6,108,248,217,192,80,148,136,48,162,30,213,233,84,211,1,70,98,3,191,9,10,181,192,248,95,50,80,
121,215,230,105,123,202,147,48,39,31,51,8,81,243,224,186,197,43,53,69,184,198,80,121,201,40,53,29,17,214,58,147,187,218,106,141,195,252,80,71,139,240,242,182,30,63,229,180,33,41,134,36,230,149,172,151,
19,66,196,227,154,162,28,79,41,192,105,153,110,201,47,67,139,144,33,34,227,249,101,135,165,108,127,68,30,184,165,107,104,100,200,31,236,4,104,112,246,78,163,221,171,71,231,193,141,70,130,237,35,71,52,
124,238,29,244,55,7,2,225,11,174,102,174,60,176,93,27,95,12,15,108,197,83,215,37,64,73,110,154,59,221,165,228,1,156,54,137,78,99,79,177,22,115,186,74,151,217,169,192,56,102,251,57,97,117,117,117,86,72,
200,194,107,23,21,54,0,108,213,75,24,81,204,79,87,255,220,152,207,75,65,251,91,81,227,39,119,76,176,197,40,231,26,181,31,1,31,235,223,246,185,207,38,117,145,156,93,101,91,164,169,8,39,72,138,243,211,211,
138,115,60,53,0,239,144,107,41,227,82,37,62,120,202,81,205,125,194,82,182,15,138,7,54,147,99,74,221,156,233,241,55,93,94,229,42,173,87,251,248,151,41,244,141,20,0,77,52,146,63,141,87,131,22,93,192,106,
238,26,105,154,148,160,66,194,174,58,147,16,122,54,233,137,202,159,133,7,22,8,39,192,159,221,184,232,178,216,182,218,97,37,203,82,175,165,106,180,43,75,100,147,57,85,146,242,217,25,112,88,163,166,161,
174,112,222,188,108,247,2,128,45,210,60,124,6,104,33,133,75,229,44,248,188,68,41,127,207,33,60,232,158,160,84,63,14,252,31,91,126,10,51,54,96,69,40,78,67,234,124,235,190,21,119,106,138,112,141,72,141,
29,162,200,82,198,21,74,124,145,245,212,130,91,255,217,62,20,154,77,250,92,193,172,67,76,169,155,253,167,251,154,46,175,202,189,121,165,114,158,52,85,103,108,66,166,253,34,155,54,233,225,168,200,156,196,
171,88,140,81,45,162,31,175,52,30,63,151,156,1,207,36,132,158,97,27,155,98,54,33,180,16,142,121,94,135,115,221,85,21,119,1,188,42,139,240,91,56,56,128,21,74,244,140,184,34,177,191,71,59,133,105,97,162,
82,133,84,88,116,223,245,213,245,128,104,147,30,94,87,172,15,28,205,21,124,104,108,147,37,220,35,218,144,176,129,100,12,216,172,147,196,96,81,25,247,52,148,85,173,94,94,118,207,229,85,185,171,1,190,205,
82,203,72,3,96,157,18,93,172,19,200,215,108,101,75,182,227,73,188,29,47,127,254,203,182,252,172,160,242,235,210,143,108,62,158,48,205,78,142,113,252,230,173,179,39,31,184,169,198,112,8,169,92,173,245,
179,91,94,56,234,29,65,225,9,22,99,76,153,255,54,136,104,248,28,214,141,192,111,247,181,183,46,45,203,188,176,55,110,86,247,51,9,161,173,218,120,166,33,244,12,32,119,110,210,190,246,252,222,203,238,109,
88,184,193,235,82,243,71,81,249,71,150,89,242,43,99,140,229,34,58,214,11,69,204,190,223,76,192,180,222,90,178,108,65,153,67,226,93,86,145,123,45,192,243,74,5,134,205,182,213,92,226,57,57,159,123,100,27,
64,125,251,214,71,106,202,214,127,223,58,76,85,5,54,75,205,32,65,192,77,29,91,31,222,7,210,1,228,0,217,64,252,68,204,14,138,248,15,181,202,146,215,82,57,204,74,25,29,73,13,169,110,177,85,70,177,222,198,
138,201,6,100,34,196,183,109,249,89,64,24,242,20,96,98,192,38,114,92,40,31,128,142,62,255,104,255,72,240,108,126,166,171,178,65,233,99,183,152,124,151,244,11,84,48,181,126,26,100,31,72,232,30,24,59,49,
22,140,76,254,250,87,99,26,125,180,92,97,109,144,83,97,219,198,41,242,3,208,108,44,196,136,143,174,143,117,108,125,248,75,96,164,131,200,233,28,29,204,126,104,253,50,23,68,63,117,254,154,184,156,118,205,
99,201,234,207,228,25,68,244,115,187,174,34,95,251,94,91,217,108,251,205,5,125,167,25,176,48,100,205,3,183,46,90,226,113,105,185,18,193,243,106,249,165,187,57,2,120,157,60,206,70,210,169,144,126,129,212,
63,9,124,205,50,131,221,72,106,0,58,128,204,1,86,78,77,238,193,205,83,106,53,79,42,11,209,109,20,253,162,126,52,214,16,135,74,214,127,127,151,77,201,209,145,212,98,236,27,52,156,28,52,172,15,115,217,97,
169,28,66,179,235,189,102,114,72,32,50,249,213,153,115,195,199,243,51,11,42,175,163,111,90,221,70,18,212,79,67,36,234,129,143,119,12,77,159,219,153,121,146,120,155,36,9,59,143,148,42,191,100,60,176,17,
39,173,142,86,212,100,1,246,139,28,190,173,44,101,143,98,126,113,63,64,129,12,242,169,200,233,113,150,242,127,39,245,211,43,118,242,77,208,119,146,1,47,92,184,208,21,212,141,249,55,95,22,253,238,247,13,
37,143,179,154,249,247,5,107,244,46,62,102,180,219,202,51,21,207,170,243,217,169,36,254,18,79,34,120,94,150,243,133,72,19,66,136,7,228,150,123,191,97,169,180,77,227,190,42,138,233,22,23,238,187,26,17,
14,134,113,48,36,156,180,139,52,154,133,23,35,137,17,106,173,222,205,90,163,59,38,228,38,219,149,196,152,108,22,13,177,83,157,199,78,172,191,72,180,195,225,224,54,114,164,205,189,225,102,114,36,184,45,
227,189,211,125,77,87,45,42,88,187,76,14,146,161,70,162,219,35,38,152,47,253,148,202,232,13,69,123,143,156,155,110,192,102,11,140,169,122,75,155,129,48,101,126,73,24,240,102,185,152,180,241,209,77,71,
97,88,56,24,194,193,128,112,114,82,100,208,43,92,214,12,198,177,41,124,136,140,40,159,190,112,72,88,223,224,18,131,221,226,167,32,177,1,135,92,161,5,37,57,153,25,243,11,211,175,4,120,78,179,94,188,250,
243,240,25,110,214,173,183,66,19,151,47,217,233,48,239,184,207,43,21,60,28,105,66,129,178,46,79,201,45,192,118,83,98,155,85,232,61,106,1,123,176,30,37,237,80,109,248,120,60,242,238,248,127,242,149,146,
219,55,255,50,169,140,118,13,113,169,144,130,28,47,190,209,122,234,193,53,181,17,77,160,93,75,31,59,212,34,83,218,235,34,209,213,103,41,101,255,239,246,183,79,223,82,51,51,188,68,215,236,88,193,110,203,
45,85,126,49,158,22,120,74,173,158,1,211,201,216,16,62,205,93,122,212,193,73,120,180,234,238,39,6,109,178,68,145,66,123,77,50,96,105,40,53,15,174,89,124,149,38,20,135,79,104,108,117,155,111,13,185,165,
78,131,17,13,159,90,186,135,223,238,247,133,108,127,47,163,32,195,85,48,191,48,99,229,13,70,15,14,197,32,108,50,183,110,195,195,27,90,62,245,145,62,164,194,6,172,12,216,110,116,158,37,42,13,63,207,6,255,
64,150,12,33,160,75,195,249,169,164,51,95,10,3,22,216,123,160,20,22,109,122,134,131,193,222,161,224,153,121,217,238,133,13,70,47,59,156,230,6,220,16,138,26,176,161,203,221,97,221,152,46,133,153,225,69,
18,188,179,130,93,61,166,202,111,150,219,72,201,224,222,208,89,254,33,52,126,31,190,144,207,150,221,182,249,223,147,206,108,39,223,132,154,142,27,112,197,242,138,28,35,44,10,174,89,52,175,1,224,101,71,
41,163,170,57,151,134,112,31,110,169,99,24,210,120,244,153,183,254,179,173,199,63,106,74,60,142,197,229,217,25,207,108,252,240,10,175,140,136,85,178,143,221,154,185,23,126,206,53,159,250,72,31,72,238,
106,125,249,175,115,110,251,242,238,105,191,134,0,92,212,3,17,235,195,157,124,215,191,63,102,188,67,72,227,142,121,183,127,183,59,105,6,151,160,163,36,5,51,3,48,49,252,211,231,134,142,207,203,118,47,108,
208,123,77,229,23,72,234,163,151,152,16,146,114,87,66,34,179,182,153,233,231,132,102,184,196,39,177,172,224,150,58,95,25,59,202,95,6,227,63,163,181,71,232,99,159,78,137,137,221,128,53,33,226,136,171,110,
232,74,205,234,229,37,197,249,153,174,42,128,45,105,230,215,230,160,194,45,145,104,63,238,29,30,59,149,140,241,2,28,111,27,28,233,27,14,182,0,220,170,155,159,202,66,133,109,238,146,232,249,82,132,91,85,
92,247,217,42,59,71,143,166,74,86,235,61,60,235,111,228,39,190,183,200,146,33,16,116,130,184,165,228,246,31,188,147,140,158,23,75,182,132,207,108,228,48,233,248,111,158,232,107,2,168,53,134,201,21,161,
132,121,23,203,17,10,140,232,145,224,190,161,177,93,41,149,251,199,112,22,58,197,39,91,9,177,33,124,154,223,15,255,46,110,188,66,202,109,206,136,115,125,201,29,63,73,202,62,146,214,119,66,187,107,241,
44,186,178,240,158,134,202,122,128,118,53,141,227,174,76,178,100,216,180,140,155,195,209,185,111,32,44,95,52,12,185,199,84,22,65,37,66,196,191,84,56,222,54,116,176,160,206,93,125,75,232,28,223,243,154,
243,71,192,239,92,133,220,29,104,143,253,158,240,79,19,145,101,40,17,178,132,5,31,19,104,72,114,141,16,185,70,136,121,70,128,229,145,65,174,12,159,231,178,240,32,94,57,33,38,19,188,232,144,142,191,154,
183,62,5,207,59,142,52,97,204,72,182,84,32,146,88,192,113,163,39,148,195,171,76,208,83,81,131,16,189,98,118,235,219,237,167,31,186,125,73,88,67,113,220,164,247,176,67,157,254,211,164,171,35,241,181,143,
142,251,190,249,187,19,184,197,135,166,210,100,40,137,203,117,167,180,100,28,253,78,217,170,30,221,34,181,73,176,138,156,113,187,164,73,131,92,35,72,158,12,81,161,251,185,34,60,192,21,145,1,106,194,195,
19,118,3,100,64,160,124,171,120,95,246,119,196,166,77,41,207,208,221,34,113,189,197,224,21,23,234,79,3,40,169,171,46,75,211,84,239,178,249,57,215,2,148,233,99,28,233,217,154,84,97,85,69,222,127,239,60,
113,198,244,120,88,233,194,133,62,161,233,113,3,222,190,191,227,192,117,117,133,119,85,232,126,142,246,38,87,134,132,107,182,124,243,134,165,107,191,241,219,105,105,91,251,119,37,197,35,21,72,137,225,
15,70,222,242,186,213,111,148,222,182,121,135,125,142,196,248,167,145,253,252,211,200,254,185,20,109,70,248,186,239,8,95,247,29,177,164,73,115,170,221,64,49,192,144,63,16,238,61,31,104,46,206,243,212,
110,30,178,62,56,36,17,59,205,210,182,247,39,247,61,138,29,126,49,208,56,39,124,98,168,141,12,39,221,247,82,129,174,27,163,225,136,124,214,147,166,254,67,241,71,30,63,51,83,62,143,250,142,241,168,239,
152,61,33,49,15,108,200,197,15,172,94,88,231,118,106,89,169,21,37,90,138,215,61,110,217,51,58,78,157,234,42,173,173,12,11,162,87,18,188,250,110,91,199,87,238,185,172,63,221,165,165,116,188,43,221,173,
61,0,36,167,85,10,144,72,25,12,25,190,225,209,80,103,123,255,232,153,99,157,67,45,167,207,156,127,238,199,207,238,221,53,215,101,125,144,145,225,113,118,129,191,56,246,127,115,247,72,83,113,158,167,214,
46,159,72,246,171,177,63,49,232,134,17,30,11,25,195,253,195,129,214,179,61,190,51,199,218,135,154,222,107,29,254,215,23,95,108,76,254,151,203,231,0,90,101,101,165,59,2,21,219,222,109,107,62,212,210,111,
121,104,162,162,200,123,232,209,59,46,143,27,172,16,90,50,177,189,174,10,209,97,72,42,99,47,62,247,228,235,143,101,121,212,132,27,105,159,90,93,251,242,138,133,121,190,169,239,207,244,248,117,96,13,64,
115,215,224,200,195,63,249,131,245,1,143,36,112,110,56,56,214,220,53,28,47,75,32,125,210,112,238,110,63,113,194,250,43,35,27,124,233,169,215,191,235,210,44,86,0,103,137,79,175,91,254,226,21,229,153,147,
234,94,58,180,105,251,121,95,122,234,173,31,120,156,147,207,134,186,52,231,208,247,62,117,245,180,80,166,99,164,103,16,28,241,43,79,255,249,133,247,94,251,191,141,153,111,79,164,209,20,101,236,241,79,
175,250,205,196,119,163,233,89,147,58,236,209,246,225,193,68,109,83,81,144,121,228,209,187,150,31,154,248,206,234,195,246,145,64,32,98,213,198,197,121,233,199,191,250,209,43,222,155,248,46,168,135,3,102,
244,0,63,250,109,83,227,214,183,206,30,176,162,177,67,216,144,70,107,223,136,175,103,32,24,255,38,24,41,219,12,111,104,119,231,59,157,169,205,117,167,224,127,61,243,246,191,122,93,138,35,81,90,186,219,
209,255,237,251,175,153,22,13,138,242,37,11,150,73,105,172,74,166,0,195,112,252,170,243,196,137,62,123,202,201,40,173,173,174,17,200,27,146,161,85,80,222,61,219,212,60,109,177,168,174,174,206,57,164,143,
166,118,91,71,146,144,10,67,66,167,169,189,120,254,81,118,237,74,117,83,2,128,210,197,85,247,8,129,245,93,71,115,132,160,234,249,69,239,145,35,211,6,57,128,242,37,149,119,73,41,76,55,190,165,100,160,227,
120,75,194,31,216,42,91,90,245,231,24,152,254,90,164,148,198,169,142,227,173,211,66,230,202,202,74,119,196,45,238,183,146,89,72,241,94,219,241,211,182,7,249,203,150,84,61,72,18,151,252,75,33,15,117,28,
59,243,134,29,93,73,77,101,173,162,136,235,237,232,82,134,64,130,232,52,84,113,168,243,112,115,219,76,217,148,214,86,223,46,144,197,118,116,66,200,222,182,99,103,126,61,245,189,38,165,172,73,170,36,41,
70,103,98,188,0,194,23,106,35,195,33,147,106,24,244,26,224,93,82,63,95,99,11,41,8,32,8,74,131,49,129,209,39,84,173,215,57,38,123,79,159,62,29,61,105,126,60,241,175,24,254,87,129,212,149,78,33,140,5,166,
233,40,151,52,60,252,64,64,96,96,136,0,130,128,192,240,233,66,246,162,106,189,158,128,232,57,117,234,84,208,158,193,197,197,255,7,45,51,80,138,69,148,252,106,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* Main_Component::logo_png = (const char*) resource_Main_Component_logo_png;
const int Main_Component::logo_pngSize = 7636;


//[EndFile] You can add extra defines here...
#include "ui/generic_main_component.tcc"
template class Generic_Main_Component<Main_Component>;
//[/EndFile]
