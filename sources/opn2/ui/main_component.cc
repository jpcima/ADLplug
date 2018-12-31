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
#include <fmt/format.h>
#include <memory>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
//[/Headers]

#include "main_component.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
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

    ed_op2.reset (new Operator_Editor (2, pb));
    addAndMakeVisible (ed_op2.get());
    ed_op2->setName ("new component");

    ed_op2->setBounds (300, 160, 264, 128);

    ed_op1.reset (new Operator_Editor (0, pb));
    addAndMakeVisible (ed_op1.get());
    ed_op1->setName ("new component");

    ed_op1->setBounds (16, 160, 264, 128);

    ed_op4.reset (new Operator_Editor (3, pb));
    addAndMakeVisible (ed_op4.get());
    ed_op4->setName ("new component");

    ed_op4->setBounds (300, 316, 264, 128);

    ed_op3.reset (new Operator_Editor (1, pb));
    addAndMakeVisible (ed_op3.get());
    ed_op3->setName ("new component");

    ed_op3->setBounds (16, 316, 264, 128);

    sl_tune.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_tune.get());
    sl_tune->setRange (-127, 127, 1);
    sl_tune->setSliderStyle (Slider::IncDecButtons);
    sl_tune->setTextBoxStyle (Slider::TextBoxLeft, false, 36, 20);
    sl_tune->setColour (Slider::textBoxTextColourId, Colours::aliceblue);
    sl_tune->setColour (Slider::textBoxOutlineColourId, Colour (0xff8e989b));
    sl_tune->addListener (this);

    sl_tune->setBounds (696, 324, 76, 20);

    midi_kb.reset (new Midi_Keyboard_Ex (midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard));
    addAndMakeVisible (midi_kb.get());
    midi_kb->setName ("new component");

    midi_kb->setBounds (16, 520, 730, 64);

    btn_about.reset (new ImageButton ("new button"));
    addAndMakeVisible (btn_about.get());
    btn_about->setButtonText (String());
    btn_about->addListener (this);

    btn_about->setImages (false, true, true,
                          ImageCache::getFromMemory (opnmidi_png, opnmidi_pngSize), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000));
    btn_about->setBounds (16, 8, 232, 40);

    label2.reset (new Label ("new label",
                             TRANS("FM synthesizer with YM2612 chip emulation")));
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

    label11.reset (new Label ("new label",
                              TRANS("Note offset")));
    addAndMakeVisible (label11.get());
    label11->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label11->setJustificationType (Justification::centredLeft);
    label11->setEditable (false, false, false);
    label11->setColour (Label::textColourId, Colours::aliceblue);
    label11->setColour (TextEditor::textColourId, Colours::black);
    label11->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label11->setBounds (590, 324, 104, 20);

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

    label5.reset (new Label ("new label",
                             TRANS("Percussion key")));
    addAndMakeVisible (label5.get());
    label5->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label5->setBounds (590, 348, 104, 20);

    cb_percussion_key.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_percussion_key.get());
    cb_percussion_key->setEditableText (false);
    cb_percussion_key->setJustificationType (Justification::centredLeft);
    cb_percussion_key->setTextWhenNothingSelected (String());
    cb_percussion_key->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_percussion_key->addListener (this);

    cb_percussion_key->setBounds (696, 348, 74, 20);

    label17.reset (new Label ("new label",
                              TRANS("Percussion key")));
    addAndMakeVisible (label17.get());
    label17->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label17->setJustificationType (Justification::centredLeft);
    label17->setEditable (false, false, false);
    label17->setColour (Label::textColourId, Colours::aliceblue);
    label17->setColour (TextEditor::textColourId, Colours::black);
    label17->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label17->setBounds (590, 348, 104, 20);

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

    btn_lfo_enable.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_lfo_enable.get());
    btn_lfo_enable->setButtonText (String());
    btn_lfo_enable->addListener (this);
    btn_lfo_enable->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_lfo_enable->setBounds (552, 486, 15, 15);

    label20.reset (new Label ("new label",
                              TRANS("LFO enabled")));
    addAndMakeVisible (label20.get());
    label20->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label20->setJustificationType (Justification::centredLeft);
    label20->setEditable (false, false, false);
    label20->setColour (Label::textColourId, Colours::aliceblue);
    label20->setColour (TextEditor::textColourId, Colours::black);
    label20->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label20->setBounds (568, 486, 94, 15);

    cb_lfofreq.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_lfofreq.get());
    cb_lfofreq->setEditableText (false);
    cb_lfofreq->setJustificationType (Justification::centredLeft);
    cb_lfofreq->setTextWhenNothingSelected (String());
    cb_lfofreq->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_lfofreq->addListener (this);

    cb_lfofreq->setBounds (664, 482, 104, 20);

    cb_algorithm.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_algorithm.get());
    cb_algorithm->setEditableText (false);
    cb_algorithm->setJustificationType (Justification::centredLeft);
    cb_algorithm->setTextWhenNothingSelected (String());
    cb_algorithm->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_algorithm->addListener (this);

    cb_algorithm->setBounds (590, 168, 180, 24);

    kn_feedback.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_feedback.get());
    kn_feedback->setName ("new component");

    kn_feedback->setBounds (692, 192, 32, 32);

    label10.reset (new Label ("new label",
                              TRANS("Feedback")));
    addAndMakeVisible (label10.get());
    label10->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label10->setJustificationType (Justification::centred);
    label10->setEditable (false, false, false);
    label10->setColour (Label::textColourId, Colours::aliceblue);
    label10->setColour (TextEditor::textColourId, Colours::black);
    label10->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label10->setBounds (628, 195, 72, 24);

    kn_ams.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_ams.get());
    kn_ams->setName ("new component");

    kn_ams->setBounds (636, 254, 32, 32);

    label6.reset (new Label ("new label",
                             TRANS("AM")));
    addAndMakeVisible (label6.get());
    label6->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label6->setJustificationType (Justification::centred);
    label6->setEditable (false, false, false);
    label6->setColour (Label::textColourId, Colours::aliceblue);
    label6->setColour (TextEditor::textColourId, Colours::black);
    label6->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label6->setBounds (608, 257, 36, 24);

    kn_fms.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_fms.get());
    kn_fms->setName ("new component");

    kn_fms->setBounds (714, 254, 32, 32);

    label7.reset (new Label ("new label",
                             TRANS("FM")));
    addAndMakeVisible (label7.get());
    label7->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label7->setJustificationType (Justification::centred);
    label7->setEditable (false, false, false);
    label7->setColour (Label::textColourId, Colours::aliceblue);
    label7->setColour (TextEditor::textColourId, Colours::black);
    label7->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label7->setBounds (686, 257, 36, 24);

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

    kn_mastervol.reset (new Styled_Knob_Default());
    addAndMakeVisible (kn_mastervol.get());
    kn_mastervol->setName ("new component");

    kn_mastervol->setBounds (510, 78, 48, 48);

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

    label16.reset (new Label ("new label",
                              TRANS("Rate")));
    addAndMakeVisible (label16.get());
    label16->setFont (Font (14.0f, Font::plain).withTypefaceStyle ("Regular"));
    label16->setJustificationType (Justification::centredLeft);
    label16->setEditable (false, false, false);
    label16->setColour (Label::textColourId, Colours::aliceblue);
    label16->setColour (TextEditor::textColourId, Colours::black);
    label16->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label16->setBounds (611, 104, 48, 20);

    cb_chip_type.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_chip_type.get());
    cb_chip_type->setEditableText (false);
    cb_chip_type->setJustificationType (Justification::centredLeft);
    cb_chip_type->setTextWhenNothingSelected (String());
    cb_chip_type->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_chip_type->addListener (this);

    cb_chip_type->setBounds (659, 104, 110, 20);

    lbl_info.reset (new Label ("new label",
                               TRANS("OPN Ready!")));
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

    kn_mastervol->add_listener(this);
    kn_feedback->add_listener(this);
    kn_ams->add_listener(this);
    kn_fms->add_listener(this);
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    Label *lbl_midi_channel = (Label *)(intptr_t)(int64)sl_midi_channel->getProperties()["X-Slider-Text-Box"];
    Font fnt_midi_channel(Font::getDefaultSansSerifFontName(), 30.0, Font::plain);
    lbl_midi_channel->setFont(fnt_midi_channel);

    kn_mastervol->set_range(0, 1);

    sl_num_chips->setNumDecimalPlacesToDisplay(0);

    kn_feedback->set_range(0, 7);
    sl_tune->setNumDecimalPlacesToDisplay(0);

    kn_ams->set_range(0, 3);
    kn_fms->set_range(0, 7);

    btn_lfo_enable->setClickingTogglesState(true);

    {
        StringArray strings = pb.p_volmodel->getAllValueStrings();
        for (unsigned i = 0, n = strings.size(); i < n; ++i)
            cb_volmodel->addItem(strings[i], i + 1);
    }
    cb_volmodel->setScrollWheelEnabled(true);

    {
        StringArray strings = pb.p_lfofreq->getAllValueStrings();
        for (unsigned i = 0, n = strings.size(); i < n; ++i)
            cb_lfofreq->addItem(strings[i], i + 1);
    }
    cb_lfofreq->setScrollWheelEnabled(true);

    const char *algorithms[8] = {
        u8"1→2→3→4",
        u8"(1+2)→3→4",
        u8"(1+(2→3))→4",
        u8"((1→2)+3)→4",
        u8"(1→2)+(3→4)",
        u8"(1→2)+(1→3)+(1→4)",
        u8"(1→2)+3+4",
        u8"1+2+3+4",
    };
    unsigned num_algorithms = sizeof(algorithms) / sizeof(algorithms[0]);
    for (unsigned i = 0; i < num_algorithms; ++i) {
        std::string text = fmt::format("{:d} : {:s}", i + 1, algorithms[i]);
        cb_algorithm->addItem(text, i + 1);
    }
    cb_algorithm->setScrollWheelEnabled(true);

    unsigned num_chip_types = pb.p_chiptype->choices.size();
    for (unsigned i = 0; i < num_chip_types; ++i)
        cb_chip_type->addItem(pb.p_chiptype->choices[i], i + 1);
    cb_chip_type->setScrollWheelEnabled(true);
    //[/Constructor]
}

Main_Component::~Main_Component()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    ed_op2 = nullptr;
    ed_op1 = nullptr;
    ed_op4 = nullptr;
    ed_op3 = nullptr;
    sl_tune = nullptr;
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
    label11 = nullptr;
    btn_emulator = nullptr;
    label14 = nullptr;
    sl_num_chips = nullptr;
    label15 = nullptr;
    label5 = nullptr;
    cb_percussion_key = nullptr;
    label17 = nullptr;
    sl_veloffset = nullptr;
    label19 = nullptr;
    sl_midi_channel = nullptr;
    label22 = nullptr;
    cb_volmodel = nullptr;
    btn_algo_help = nullptr;
    btn_lfo_enable = nullptr;
    label20 = nullptr;
    cb_lfofreq = nullptr;
    cb_algorithm = nullptr;
    kn_feedback = nullptr;
    label10 = nullptr;
    kn_ams = nullptr;
    label6 = nullptr;
    kn_fms = nullptr;
    label7 = nullptr;
    btn_keymap = nullptr;
    btn_octave_up = nullptr;
    btn_octave_down = nullptr;
    btn_pgm_edit = nullptr;
    btn_pgm_add = nullptr;
    label23 = nullptr;
    kn_mastervol = nullptr;
    lbl_mastervol = nullptr;
    label16 = nullptr;
    cb_chip_type = nullptr;
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
        Colour fillColour1 = Colour (0xff8b3f4b), fillColour2 = Colour (0xff602731);
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
        int x = 586, y = 224, width = 188, height = 30;
        String text (TRANS("LFO Sensitivity"));
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
        float x = 586.0f, y = 316.0f, width = 188.0f, height = 60.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 586.0f, y = 252.0f, width = 188.0f, height = 36.0f;
        Colour fillColour = Colour (0x662e4c4d);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRoundedRectangle (x, y, width, height, 7.0f);
    }

    {
        float x = 586.0f, y = 160.0f, width = 188.0f, height = 64.0f;
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

void Main_Component::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Slider *sl = sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_tune.get())
    {
        //[UserSliderCode_sl_tune] -- add your slider handling code here..
        AudioParameterInt &p = *part.p_tune;
        p = sl->getValue();
        //[/UserSliderCode_sl_tune]
    }
    else if (sliderThatWasMoved == sl_num_chips.get())
    {
        //[UserSliderCode_sl_num_chips] -- add your slider handling code here..
        AudioParameterInt &p = *pb.p_nchip;
        set_int_parameter_with_delay(500, p, sl->getValue());
        //[/UserSliderCode_sl_num_chips]
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

void Main_Component::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];
    Button *btn = buttonThatWasClicked;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_about.get())
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
    else if (buttonThatWasClicked == btn_algo_help.get())
    {
        //[UserButtonCode_btn_algo_help] -- add your button handler code here..
        CallOutBox::launchAsynchronously(new Algorithm_Help, btn_algo_help->getBounds(), this);
        //[/UserButtonCode_btn_algo_help]
    }
    else if (buttonThatWasClicked == btn_lfo_enable.get())
    {
        //[UserButtonCode_btn_lfo_enable] -- add your button handler code here..
        AudioParameterBool &p = *pb.p_lfoenable;
        p.beginChangeGesture();
        p = btn->getToggleState();
        p.endChangeGesture();
        //[/UserButtonCode_btn_lfo_enable]
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
    else if (comboBoxThatHasChanged == cb_lfofreq.get())
    {
        //[UserComboBoxCode_cb_lfofreq] -- add your combo box handling code here..
        AudioParameterChoice &p = *pb.p_lfofreq;
        p.beginChangeGesture();
        p = cb->getSelectedId() - 1;
        p.endChangeGesture();
        //[/UserComboBoxCode_cb_lfofreq]
    }
    else if (comboBoxThatHasChanged == cb_algorithm.get())
    {
        //[UserComboBoxCode_cb_algorithm] -- add your combo box handling code here..
        AudioParameterInt &p = *part.p_algorithm;
        p.beginChangeGesture();
        p = cb->getSelectedId() - 1;
        p.endChangeGesture();
        //[/UserComboBoxCode_cb_algorithm]
    }
    else if (comboBoxThatHasChanged == cb_chip_type.get())
    {
        //[UserComboBoxCode_cb_chip_type] -- add your combo box handling code here..
        int selection = cb_chip_type->getSelectedId();
        if (selection != 0 && (unsigned)(selection - 1) != chip_settings_.chip_type) {
            AudioParameterChoice &p = *pb.p_chiptype;
            p.beginChangeGesture();
            p = selection - 1;
            p.endChangeGesture();
        }
        //[/UserComboBoxCode_cb_chip_type]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Main_Component::sliderDragStarted(Slider *slider)
{
    Parameter_Block &pb = *parameter_block_;
    Parameter_Block::Part &part = pb.part[midichannel_];

    if (slider == sl_tune.get()) {
        AudioParameterInt &p = *part.p_tune;
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

    if (slider == sl_tune.get()) {
        AudioParameterInt &p = *part.p_tune;
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
    else if (k == kn_feedback.get()) {
        AudioParameterInt &p = *part.p_feedback;
        p = std::lround(k->value());
    }
    else if (k == kn_ams.get()) {
        AudioParameterInt &p = *part.p_ams;
        p = std::lround(k->value());
    }
    else if (k == kn_fms.get()) {
        AudioParameterInt &p = *part.p_fms;
        p = std::lround(k->value());
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
    else if (k == kn_feedback.get()) {
        AudioParameterInt &p = *part.p_feedback;
        p.beginChangeGesture();
    }
    else if (k == kn_ams.get()) {
        AudioParameterInt &p = *part.p_ams;
        p.beginChangeGesture();
    }
    else if (k == kn_fms.get()) {
        AudioParameterInt &p = *part.p_fms;
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
    else if (k == kn_feedback.get()) {
        AudioParameterInt &p = *part.p_feedback;
        p.endChangeGesture();
    }
    else if (k == kn_ams.get()) {
        AudioParameterInt &p = *part.p_ams;
        p.endChangeGesture();
    }
    else if (k == kn_fms.get()) {
        AudioParameterInt &p = *part.p_fms;
        p.endChangeGesture();
    }

    expire_info_in();
}

void Main_Component::set_instrument_parameters(const Instrument &ins, NotificationType ntf)
{
    trace("Update instrument parameters on display");

    Operator_Editor *op_editors[4] =
        { ed_op1.get(), ed_op3.get(), ed_op2.get(), ed_op4.get() };

    cb_algorithm->setSelectedId(ins.algorithm() + 1, ntf);

    kn_feedback->set_value(ins.feedback(), ntf);

    sl_tune->setValue(ins.note_offset, ntf);

    kn_ams->set_value(ins.ams(), ntf);
    kn_fms->set_value(ins.fms(), ntf);

    cb_percussion_key->setSelectedId(ins.percussion_key_number + 1, ntf);

    sl_veloffset->setValue(ins.midi_velocity_offset, ntf);

    for (unsigned op = 0; op < 4; ++op) {
        Operator_Editor *oped = op_editors[op];
        oped->set_operator_parameters(ins, op, ntf);
        oped->set_operator_enabled(true);
    }
}

void Main_Component::set_chip_settings(NotificationType ntf)
{
    const Chip_Settings &cs = chip_settings_;
    update_emulator_icon();
    sl_num_chips->setValue(cs.chip_count, ntf);
    cb_chip_type->setSelectedId(cs.chip_type + 1, ntf);
}

void Main_Component::set_global_parameters(NotificationType ntf)
{
    cb_volmodel->setSelectedId(instrument_gparam_.volume_model + 1, ntf);
    btn_lfo_enable->setToggleState(instrument_gparam_.lfo_enable, ntf);
    cb_lfofreq->setSelectedId(instrument_gparam_.lfo_frequency + 1, ntf);
}

void Main_Component::on_change_midi_channel(unsigned channel)
{
    if (channel > 15)
        return;

    trace("Change MIDI channel to %u", channel + 1);

    midichannel_ = channel;

    Operator_Editor *op_editors[4] =
        { ed_op2.get(), ed_op1.get(), ed_op4.get(), ed_op3.get() };
    for (Operator_Editor *oped : op_editors)
        oped->set_midi_channel(channel);

    midi_kb->setMidiChannel(channel + 1);
    sl_midi_channel->setValue(channel + 1, dontSendNotification);
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

    if (c == kn_feedback.get()) {
        param = "Feedback";
        val = (int)lround(kn->value());
    }
    else if (c == kn_ams.get()) {
        param = "AM sensitivity";
        val = (int)lround(kn->value());
    }
    else if (c == kn_fms.get()) {
        param = "FM sensitivity";
        val = (int)lround(kn->value());
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
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.66"
                 fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="0 0 800 600" fill=" radial: 150 50, 800 600, 0=ff8b3f4b, 1=ff602731"
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
    <TEXT pos="586 224 188 30" fill="solid: fff0f8ff" hasStroke="0" text="LFO Sensitivity"
          fontname="Default font" fontsize="20.0" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <ROUNDRECT pos="16 474 264 36" cornerSize="7.0" fill="solid: ff3c485f" hasStroke="0"/>
    <ROUNDRECT pos="300 474 188 36" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="546 450 228 60" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="586 316 188 60" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="586 252 188 36" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="586 160 188 64" cornerSize="7.0" fill="solid: 662e4c4d"
               hasStroke="0"/>
    <ROUNDRECT pos="606 52 168 80" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
    <ROUNDRECT pos="460 72 102 60" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
    <ROUNDRECT pos="300 72 146 60" cornerSize="7.0" fill="solid: 662e4c4d" hasStroke="0"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="423f2b5d9aff978c" memberName="ed_op2"
                    virtualName="" explicitFocusOrder="0" pos="300 160 264 128" class="Operator_Editor"
                    params="2, pb"/>
  <GENERICCOMPONENT name="new component" id="a00c5401e39a953e" memberName="ed_op1"
                    virtualName="" explicitFocusOrder="0" pos="16 160 264 128" class="Operator_Editor"
                    params="0, pb"/>
  <GENERICCOMPONENT name="new component" id="b7424f0838e48a08" memberName="ed_op4"
                    virtualName="" explicitFocusOrder="0" pos="300 316 264 128" class="Operator_Editor"
                    params="3, pb"/>
  <GENERICCOMPONENT name="new component" id="4bf73df293534890" memberName="ed_op3"
                    virtualName="" explicitFocusOrder="0" pos="16 316 264 128" class="Operator_Editor"
                    params="1, pb"/>
  <SLIDER name="new slider" id="9cd7cc232d55ac8a" memberName="sl_tune"
          virtualName="" explicitFocusOrder="0" pos="696 324 76 20" textboxtext="fff0f8ff"
          textboxoutline="ff8e989b" min="-127.0" max="127.0" int="1.0"
          style="IncDecButtons" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="36" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="new component" id="4d4a20a681c7e721" memberName="midi_kb"
                    virtualName="" explicitFocusOrder="0" pos="16 520 730 64" class="Midi_Keyboard_Ex"
                    params="midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard"/>
  <IMAGEBUTTON name="new button" id="1c21a98bd6493eb8" memberName="btn_about"
               virtualName="" explicitFocusOrder="0" pos="16 8 232 40" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="opnmidi_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <LABEL name="new label" id="9fd95e8efc5b0fe1" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="264 8 192 40" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM synthesizer with YM2612 chip emulation"
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
  <LABEL name="new label" id="94205e2c6bef35b1" memberName="label11" virtualName=""
         explicitFocusOrder="0" pos="590 324 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Note offset" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
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
  <LABEL name="new label" id="1b1aa7f42f11af53" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="590 348 104 20" edTextCol="ff000000"
         edBkgCol="0" labelText="Percussion key" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="72b8aa8d99fdb9cc" memberName="cb_percussion_key"
            virtualName="" explicitFocusOrder="0" pos="696 348 74 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="5917d9d90d014675" memberName="label17" virtualName=""
         explicitFocusOrder="0" pos="590 348 104 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Percussion key"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="14.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
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
  <TEXTBUTTON name="new button" id="fd73fd08b6289ecb" memberName="btn_lfo_enable"
              virtualName="" explicitFocusOrder="0" pos="552 486 15 15" bgColOn="ff42a2c8"
              buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="bf1838a34852c77b" memberName="label20" virtualName=""
         explicitFocusOrder="0" pos="568 486 94 15" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="LFO enabled" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="e1df7689d7d9db6e" memberName="cb_lfofreq"
            virtualName="" explicitFocusOrder="0" pos="664 482 104 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="new combo box" id="53eb24e86e9015c0" memberName="cb_algorithm"
            virtualName="" explicitFocusOrder="0" pos="590 168 180 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <GENERICCOMPONENT name="new component" id="8e0915367ccd00d3" memberName="kn_feedback"
                    virtualName="" explicitFocusOrder="0" pos="692 192 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <LABEL name="new label" id="6ff60ad947a8f168" memberName="label10" virtualName=""
         explicitFocusOrder="0" pos="628 195 72 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Feedback" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="68cccf59111ab75a" memberName="kn_ams"
                    virtualName="" explicitFocusOrder="0" pos="636 254 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <LABEL name="new label" id="bc635c855613f35a" memberName="label6" virtualName=""
         explicitFocusOrder="0" pos="608 257 36 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="AM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="3d549e9f57e5f19c" memberName="kn_fms"
                    virtualName="" explicitFocusOrder="0" pos="714 254 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <LABEL name="new label" id="5775323ab5508477" memberName="label7" virtualName=""
         explicitFocusOrder="0" pos="686 257 36 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="" id="39bffafade9476c7" memberName="btn_keymap" virtualName=""
              explicitFocusOrder="0" pos="750 520 24 24" buttonText="" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ecb42a974d834d93" memberName="btn_octave_up"
              virtualName="" explicitFocusOrder="0" pos="750 549 24 18" tooltip="Octave"
              buttonText="+" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="4187eb5f8c289604" memberName="btn_octave_down"
              virtualName="" explicitFocusOrder="0" pos="750 566 24 18" tooltip="Octave"
              buttonText="-" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="ea10a8acbf1cc61a" memberName="btn_pgm_edit"
              virtualName="" explicitFocusOrder="0" pos="235 104 23 24" buttonText=""
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="1330c86e50189ade" memberName="btn_pgm_add"
              virtualName="" explicitFocusOrder="0" pos="257 104 23 24" buttonText=""
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="72b9b823ebcf0c96" memberName="label23" virtualName=""
         explicitFocusOrder="0" pos="464 78 56 22" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Volume" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="9d73506941d5ef91" memberName="kn_mastervol"
                    virtualName="" explicitFocusOrder="0" pos="510 78 48 48" class="Styled_Knob_Default"
                    params=""/>
  <LABEL name="new label" id="7108230db9c0dcf2" memberName="lbl_mastervol"
         virtualName="" explicitFocusOrder="0" pos="468 102 44 22" textCol="fff0f8ff"
         outlineCol="ff8e989b" edTextCol="ff000000" edBkgCol="0" labelText="-20 dB"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.0" kerning="0.0" bold="0"
         italic="0" justification="34"/>
  <LABEL name="new label" id="e0a305f99bf634b2" memberName="label16" virtualName=""
         explicitFocusOrder="0" pos="611 104 48 20" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Rate" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="14.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="5e6f2bbe82f06203" memberName="cb_chip_type"
            virtualName="" explicitFocusOrder="0" pos="659 104 110 20" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="d7c1d56634e337b7" memberName="lbl_info"
         virtualName="" explicitFocusOrder="0" pos="16 474 264 36" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="OPN Ready!" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default monospaced font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: opnmidi_png, 9727, "../../../resources/ui/opnmidi.png"
static const unsigned char resource_Main_Component_opnmidi_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,246,0,0,0,40,8,6,0,0,0,205,38,7,151,0,0,0,4,115,66,73,84,8,8,8,8,124,8,100,136,
0,0,0,9,112,72,89,115,0,0,14,196,0,0,14,196,1,149,43,14,27,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,119,119,119,46,105,110,107,115,99,97,112,101,46,111,114,103,155,238,60,26,0,0,32,0,73,
68,65,84,120,156,229,125,121,120,28,197,181,239,175,170,123,246,209,104,22,237,187,100,217,146,45,219,96,240,198,30,48,97,177,113,76,8,203,35,185,144,188,220,124,144,112,67,108,39,185,73,94,110,146,235,
151,229,133,155,197,96,146,220,44,55,185,1,66,30,92,72,8,224,13,8,171,193,96,27,111,24,75,150,108,217,26,237,219,104,153,209,236,211,93,117,255,24,141,52,163,153,238,158,22,14,249,190,247,206,247,245,
247,73,211,213,191,58,85,93,167,206,169,83,231,84,19,204,147,90,159,218,102,44,44,24,191,2,140,92,66,8,46,2,71,29,64,42,193,97,5,129,29,64,0,192,56,8,198,193,225,3,112,6,4,71,9,23,94,169,216,240,211,238,
249,214,155,162,254,221,155,63,74,56,173,255,160,56,0,0,194,25,35,152,4,163,99,6,89,110,43,221,180,99,120,62,48,3,123,182,220,12,70,74,242,172,51,82,190,254,161,199,9,1,215,42,58,242,218,125,118,41,100,
250,164,30,94,38,194,133,143,180,220,190,45,174,192,231,167,192,136,77,15,222,12,81,198,203,15,185,126,71,182,109,99,74,69,122,95,216,234,22,36,220,58,47,124,0,160,240,86,172,127,240,165,92,183,206,236,
185,223,100,99,226,167,245,192,49,240,254,170,155,30,218,173,116,191,111,231,151,47,160,132,175,209,203,102,238,186,216,4,21,48,200,169,208,93,121,253,246,222,243,129,57,176,107,203,45,0,41,210,243,140,
168,191,146,47,93,204,65,239,3,153,188,21,140,58,0,32,99,104,146,153,191,28,0,28,73,129,7,0,92,7,14,112,200,232,223,189,229,4,1,249,207,88,156,63,90,255,241,135,38,245,242,144,172,146,124,30,132,223,50,
159,103,115,17,225,0,8,131,36,18,244,239,222,218,197,193,95,226,32,191,168,222,240,224,251,58,152,250,6,215,49,64,250,247,108,182,2,59,126,173,85,78,10,91,60,156,200,154,229,210,169,204,62,246,36,128,
156,130,205,57,30,0,225,85,122,240,102,31,38,24,92,57,126,20,192,17,165,34,2,227,55,114,2,93,252,102,214,129,103,1,228,20,108,35,147,172,156,8,186,176,9,240,50,0,69,193,38,68,190,129,19,242,128,62,38,
149,234,34,224,12,0,99,232,223,189,101,152,3,251,9,225,79,177,32,125,190,250,246,7,35,243,193,228,132,124,19,224,23,235,121,38,111,193,238,221,249,165,133,148,210,31,115,96,211,76,133,140,35,218,57,130,
104,231,40,98,253,147,72,140,6,33,135,227,0,227,160,70,1,66,129,25,212,110,132,88,104,133,169,206,3,235,162,82,136,110,43,0,44,231,224,15,25,141,228,95,251,247,108,121,96,176,36,252,224,202,149,191,73,
232,97,60,69,210,100,4,9,95,112,62,143,206,16,53,137,16,108,38,8,118,35,136,81,4,192,235,9,112,47,1,191,183,127,247,214,93,162,196,62,167,71,139,75,99,33,36,38,194,138,247,133,2,51,140,165,5,32,156,252,
164,103,207,253,47,215,172,255,217,217,124,177,163,221,227,224,9,89,177,29,166,106,87,190,80,136,15,79,65,158,138,230,93,222,84,233,4,181,24,192,41,93,7,21,193,230,12,215,128,0,114,32,130,248,72,254,239,
198,80,100,135,232,180,228,93,62,214,51,1,22,151,148,241,220,182,212,120,203,139,88,52,129,88,223,188,244,12,0,64,176,26,147,99,201,101,5,161,4,0,74,9,112,11,56,185,133,218,216,208,192,238,173,15,148,
91,11,127,65,174,222,166,204,180,10,37,124,33,72,147,42,227,202,98,128,177,210,9,32,79,193,238,219,189,249,94,2,178,29,128,21,0,34,157,163,24,127,161,13,254,131,93,144,167,98,249,115,70,0,75,67,17,156,
31,89,4,247,53,77,160,54,163,11,28,255,86,62,98,189,163,111,247,151,239,172,218,176,253,116,254,96,73,154,124,235,44,6,127,187,95,239,99,185,217,163,4,166,26,55,108,75,202,224,186,182,25,150,134,34,0,
252,38,89,36,199,122,119,111,254,120,245,134,29,7,243,193,241,237,62,137,209,103,142,43,222,167,54,35,22,237,184,13,134,34,187,93,96,194,35,252,169,219,62,66,110,127,58,183,180,206,161,238,31,188,128,
248,200,84,206,123,150,134,34,52,110,255,68,62,48,0,128,145,255,251,46,38,222,56,147,119,249,202,251,174,132,251,186,197,0,112,13,128,31,41,22,36,184,6,0,70,158,121,15,190,103,223,203,27,191,252,179,151,
162,232,99,203,242,46,223,179,253,21,68,189,99,138,247,75,63,185,10,37,183,95,148,55,94,172,103,2,157,95,251,75,222,229,149,136,8,20,134,146,2,216,154,74,81,176,170,6,5,171,106,65,141,98,25,7,127,168,
63,60,249,169,158,61,247,223,169,103,50,79,145,111,215,251,170,253,105,91,90,129,134,239,111,4,0,80,53,32,190,109,27,237,223,189,117,7,1,249,21,0,107,124,104,10,93,15,188,132,211,95,127,22,190,215,78,
35,17,78,128,9,52,255,139,82,132,188,227,232,127,228,0,218,62,255,4,70,159,59,1,46,51,128,227,34,2,246,206,192,174,173,87,232,109,44,8,209,199,131,202,37,19,130,112,239,4,70,95,60,133,211,255,252,23,116,
61,240,18,164,169,40,56,80,78,65,118,14,61,255,197,188,214,244,92,131,39,41,42,161,231,23,251,146,75,24,130,203,7,236,149,95,201,183,185,90,253,171,135,24,213,215,119,83,39,7,166,27,136,203,91,159,218,
102,204,133,57,188,107,107,3,144,92,126,77,157,28,212,133,207,41,201,5,57,191,190,152,7,30,39,218,152,121,141,35,0,209,145,41,140,189,217,9,239,246,87,113,234,243,79,98,116,215,73,112,198,65,128,85,2,
23,14,12,238,221,188,74,23,115,208,30,87,44,173,189,170,26,123,96,165,127,7,192,191,8,0,19,111,157,69,239,111,246,131,69,36,128,10,122,121,202,34,41,42,163,255,241,195,152,56,216,141,250,175,94,3,131,
219,230,230,224,123,122,119,110,93,87,189,241,193,67,185,158,41,107,108,44,54,210,120,1,0,4,163,9,91,129,217,0,78,8,248,121,224,39,23,249,143,244,33,242,141,231,177,240,7,55,193,224,180,22,75,130,248,
95,156,99,77,186,195,171,118,113,109,57,151,185,5,0,98,9,102,50,26,40,144,7,79,83,39,135,48,250,66,27,138,111,92,2,112,124,183,119,247,214,189,115,214,243,66,205,162,154,90,0,120,228,133,246,202,79,223,
176,48,249,43,165,202,216,42,130,93,177,116,65,181,24,79,24,0,64,98,76,20,41,5,136,10,86,46,158,219,70,82,147,145,205,101,155,88,11,96,95,234,94,81,83,83,129,149,71,138,59,134,38,239,88,80,94,8,41,24,
67,164,103,82,23,62,135,162,32,146,154,69,53,245,0,240,221,199,143,23,126,247,174,105,55,134,22,255,68,185,63,26,27,27,29,113,26,47,2,128,129,241,176,187,194,99,3,240,183,25,75,137,169,56,250,31,61,132,
201,3,94,212,125,245,26,24,156,214,34,198,201,222,254,189,155,47,171,188,113,71,71,174,103,170,155,170,43,8,39,102,0,136,37,100,147,201,64,193,117,180,87,177,229,253,123,54,127,17,36,41,212,35,123,219,
208,245,139,55,33,197,217,121,211,142,169,43,120,110,12,237,223,222,141,216,80,0,32,176,83,202,159,31,126,110,115,105,46,158,4,33,177,148,81,97,29,163,194,186,241,64,172,24,0,184,78,173,163,247,138,142,
133,209,245,224,235,0,7,8,176,106,112,239,150,77,233,60,73,16,46,74,241,20,136,36,10,1,128,209,252,176,251,159,56,130,232,128,31,0,76,20,252,177,116,45,88,87,87,103,72,225,190,235,29,185,44,245,187,42,
182,138,96,19,198,46,73,225,37,36,102,74,98,233,235,187,120,48,134,72,207,4,0,128,131,92,147,142,111,22,98,75,24,21,214,129,208,155,1,96,170,117,8,178,78,124,40,107,88,154,226,189,109,96,234,202,153,190,
208,210,216,68,89,99,199,141,168,78,97,118,14,6,26,147,157,116,126,52,182,210,53,213,233,195,233,111,239,65,98,34,2,112,120,56,35,127,234,122,109,155,57,23,127,50,17,47,78,241,23,138,74,5,64,126,99,61,
69,57,53,246,224,158,251,151,48,142,31,3,192,216,254,46,244,252,241,72,254,90,154,36,157,8,44,46,129,39,20,119,68,50,40,54,25,195,233,127,123,5,139,191,183,30,162,221,84,42,139,228,119,0,110,202,231,217,
164,121,162,204,155,96,49,160,124,83,246,186,77,14,199,17,233,155,68,224,253,65,48,5,103,84,138,2,157,99,152,56,210,3,215,202,26,48,142,45,0,158,85,229,137,170,243,148,34,38,3,93,191,126,27,205,223,185,
30,68,160,23,22,218,38,183,1,248,166,234,51,130,160,136,157,254,98,243,33,78,105,94,124,166,83,160,109,8,150,90,23,144,92,103,111,155,173,156,87,82,129,144,10,143,165,57,89,110,88,55,182,154,32,230,34,
166,193,191,110,83,28,249,189,183,15,66,145,241,8,58,31,124,13,205,223,185,1,68,164,75,77,97,255,191,0,248,118,94,252,105,140,43,78,53,4,59,46,209,159,137,2,49,71,135,2,232,250,253,33,48,66,161,108,37,
37,5,185,244,186,38,184,87,86,195,82,237,76,121,4,145,8,68,17,56,49,136,225,87,78,35,216,233,83,101,58,226,11,163,235,183,7,177,112,203,149,224,192,134,193,61,91,110,44,95,255,208,94,213,135,48,45,216,
42,154,74,176,154,80,182,177,69,241,190,28,142,195,251,135,195,24,123,171,75,181,158,145,55,206,194,181,178,6,4,184,100,232,197,175,218,202,174,255,73,104,190,60,165,211,212,185,113,12,238,106,67,197,
166,165,32,192,215,250,118,110,217,89,181,241,161,119,148,202,51,170,140,205,85,76,207,156,229,85,176,148,200,223,62,130,210,27,23,3,192,154,84,63,212,213,213,153,37,66,220,215,44,171,44,183,24,68,7,0,
4,78,141,232,198,214,43,216,156,170,91,41,122,241,144,199,123,91,252,205,117,160,150,105,195,138,115,72,161,4,228,96,12,145,129,0,2,167,134,49,213,49,12,173,200,132,169,174,9,12,236,106,69,229,205,203,
192,193,191,210,243,220,23,127,89,179,233,231,3,90,236,113,162,222,94,70,84,4,251,229,95,222,125,147,40,36,205,44,239,31,143,65,146,56,160,50,75,20,182,148,161,241,222,181,48,56,178,45,10,131,195,12,207,
229,245,240,92,86,143,145,55,207,161,235,177,195,224,146,178,22,31,59,62,128,226,147,67,112,46,45,3,231,216,6,64,83,176,161,161,177,121,154,22,235,27,11,159,76,72,114,140,16,34,216,205,162,195,89,96,172,
22,173,70,195,130,123,46,69,108,50,134,192,41,229,29,173,192,153,113,112,153,129,8,212,40,201,210,26,0,175,42,214,73,244,105,194,222,157,109,40,188,176,18,182,90,151,64,40,30,29,122,241,171,43,214,222,
251,167,156,102,4,167,231,81,99,43,240,105,112,152,225,186,176,130,27,61,54,210,191,179,53,227,157,249,79,251,102,250,129,73,242,21,0,94,224,38,94,14,78,200,85,45,101,205,0,16,159,136,32,52,18,204,24,
55,70,183,5,37,87,52,240,168,47,68,198,14,246,228,28,7,122,39,38,166,210,23,243,193,75,58,207,212,223,155,181,198,3,193,102,200,121,175,18,203,16,238,15,160,231,137,163,152,60,57,164,138,211,191,167,29,
197,87,55,194,88,104,177,136,162,225,11,200,67,107,107,106,108,37,83,188,102,89,141,203,108,50,124,11,0,166,58,199,48,222,54,162,46,212,75,74,209,188,249,114,16,145,66,146,89,248,232,57,223,235,239,156,
26,105,237,25,11,7,220,54,209,188,188,206,83,189,182,185,244,210,98,135,185,177,228,202,6,24,156,22,180,63,188,31,156,43,79,105,189,59,219,146,130,13,172,30,220,187,121,85,249,141,59,222,85,109,172,134,
16,177,180,37,196,246,231,78,60,185,191,117,120,36,245,127,121,145,205,242,171,207,95,122,95,153,203,218,84,119,247,197,56,254,173,23,149,113,18,12,113,127,20,38,183,21,148,241,10,85,158,242,52,197,103,
31,0,206,252,238,16,150,127,251,90,80,131,176,80,150,18,63,2,144,211,83,174,106,138,235,116,252,40,153,178,230,90,55,26,62,187,154,0,192,68,219,48,166,58,103,183,148,88,156,33,228,157,128,125,129,7,60,
105,142,191,144,128,88,65,193,176,168,202,217,12,0,254,142,209,44,220,194,11,171,80,245,241,101,4,0,38,219,70,17,15,100,239,159,207,207,43,126,254,76,113,228,97,138,243,105,200,211,3,254,3,67,227,225,
1,131,32,24,108,86,209,86,225,178,214,121,28,166,122,107,165,131,52,125,249,42,156,125,236,8,70,222,84,182,2,153,12,12,191,118,14,213,55,183,128,131,127,134,115,124,71,43,10,81,123,172,231,18,236,143,
124,68,44,141,248,54,84,151,216,46,2,128,193,125,231,212,77,92,139,1,11,255,113,21,136,72,49,17,140,245,126,229,183,135,118,180,246,142,7,210,203,60,127,176,183,27,192,91,255,231,211,171,62,186,110,121,
197,173,174,229,229,164,244,163,139,48,240,178,242,222,169,255,220,4,66,125,126,216,170,10,193,24,189,13,128,134,96,171,155,79,106,247,6,125,161,200,111,94,108,127,250,59,255,227,162,111,89,203,29,16,
28,22,36,130,202,251,242,137,96,28,38,183,21,90,225,125,122,76,241,20,5,7,131,232,126,182,13,245,183,45,3,64,190,176,247,199,27,246,126,244,219,123,178,202,169,154,226,122,215,148,10,166,108,186,9,91,
208,92,10,255,185,137,140,251,19,237,163,176,47,240,0,132,175,3,0,2,86,97,50,80,90,229,177,46,2,128,201,246,209,44,220,194,37,179,254,80,165,54,168,120,197,115,243,175,97,154,234,54,237,137,250,120,73,
167,119,59,124,199,30,222,117,242,88,250,111,151,181,148,150,124,253,150,11,62,93,234,180,44,108,248,212,10,76,158,25,67,84,37,64,103,248,80,47,170,111,110,1,128,170,190,61,155,87,3,234,113,18,90,227,
138,231,242,138,87,141,120,47,254,248,101,53,107,69,66,13,92,102,24,125,127,100,70,59,228,186,202,215,45,132,209,105,65,66,102,209,111,60,242,238,207,231,10,117,58,125,243,209,119,255,250,94,215,248,107,
0,80,179,113,49,96,54,170,98,251,142,15,78,115,202,215,171,53,20,152,213,142,106,87,138,204,2,205,138,236,56,124,198,55,163,193,141,197,118,117,156,233,113,194,136,198,254,255,244,204,170,247,234,123,
245,28,252,103,198,0,128,20,88,196,95,87,151,216,178,194,166,82,230,103,206,75,167,198,86,236,187,180,193,227,92,82,146,117,127,226,244,180,6,231,184,240,209,237,183,85,18,2,231,141,43,171,171,69,129,
90,1,96,226,244,88,230,51,162,0,199,162,98,205,54,204,87,99,43,93,122,77,241,148,198,206,103,44,229,162,253,173,195,35,159,217,190,239,161,96,36,49,66,13,2,170,214,55,171,98,133,199,163,136,12,39,5,95,
224,228,18,45,238,82,206,78,229,107,142,96,23,53,53,21,16,208,150,150,90,247,2,0,240,123,39,144,136,203,89,174,244,244,171,120,117,245,116,109,248,247,93,123,15,255,168,212,230,126,36,97,164,207,129,242,
19,132,243,172,144,185,31,252,233,248,78,153,177,132,104,53,192,115,113,165,42,246,196,153,177,84,63,47,30,219,115,191,67,181,177,26,155,246,92,152,29,44,223,191,107,229,94,38,144,23,9,39,51,235,215,59,
174,108,88,2,36,195,99,195,147,81,85,44,131,221,148,234,180,81,245,23,160,206,147,165,202,145,243,119,153,18,180,255,225,24,164,152,4,66,72,197,3,119,175,206,74,252,224,121,182,53,31,98,68,1,39,77,176,
29,13,110,192,108,200,184,63,217,61,153,218,73,160,101,38,203,38,0,184,116,113,105,51,0,68,70,67,8,251,51,251,209,82,237,132,209,97,154,173,119,30,219,83,185,251,89,99,187,75,175,41,158,199,118,87,138,
74,156,166,1,112,146,21,223,57,30,138,38,246,181,13,189,4,0,69,43,42,192,141,162,42,222,84,175,63,217,39,4,154,33,114,90,227,42,189,189,20,72,238,65,114,198,133,114,151,181,6,0,166,186,3,201,89,85,225,
50,56,109,176,149,219,1,0,38,35,121,18,0,142,28,57,146,24,62,113,118,164,175,205,123,48,42,218,158,6,200,120,58,83,221,67,83,161,254,241,72,27,0,56,22,21,169,226,251,251,167,82,158,69,26,133,160,26,99,
152,52,79,148,177,24,201,156,101,251,127,126,247,192,218,37,165,103,55,172,170,174,249,241,255,92,125,195,237,151,55,124,6,0,124,173,195,136,69,100,69,28,152,140,51,131,147,131,171,122,70,146,38,162,50,
79,11,239,92,1,98,51,231,188,23,154,136,161,243,153,54,0,64,99,185,99,245,230,77,75,51,130,255,25,85,193,38,122,53,118,110,172,116,193,166,6,1,5,11,60,25,247,37,25,240,119,37,99,170,29,54,227,181,0,208,
88,86,216,12,0,227,103,198,178,240,156,77,153,9,111,138,125,60,159,237,46,149,126,214,61,81,64,99,44,165,89,68,215,93,80,221,218,215,113,238,9,74,133,183,9,37,25,142,206,167,223,242,190,7,0,162,213,0,
107,165,83,21,47,208,151,52,116,9,215,22,108,166,99,172,139,0,8,231,100,17,1,80,104,49,120,0,32,56,22,206,152,157,230,146,165,170,32,201,12,144,8,114,118,98,238,253,209,214,214,96,93,93,221,110,201,68,
63,1,194,103,204,201,62,95,200,91,83,100,187,192,94,85,168,138,207,36,142,120,48,14,99,129,17,132,147,90,0,138,193,224,41,141,173,136,149,110,158,80,122,122,32,60,137,135,62,151,153,128,21,25,143,160,
253,233,54,85,28,215,34,15,72,242,190,100,38,76,125,45,68,213,121,50,187,45,88,116,91,11,90,255,152,213,117,0,128,190,131,253,40,90,94,142,162,150,98,220,122,105,253,63,244,250,130,15,207,180,135,82,69,
108,166,83,99,43,241,57,119,29,231,106,46,198,216,217,204,117,246,248,153,49,184,22,121,224,46,48,173,177,89,140,127,45,117,89,26,147,191,143,103,97,186,154,50,93,18,76,200,93,239,252,246,177,207,231,
118,87,62,59,11,25,152,172,167,173,179,181,162,165,97,138,2,215,167,126,108,235,25,247,199,18,114,208,100,16,236,150,138,2,248,7,115,199,246,3,64,112,52,148,130,173,211,98,79,107,172,167,91,108,180,174,
165,174,148,112,152,1,192,108,18,157,0,16,13,196,84,109,121,115,81,82,91,115,192,187,112,253,207,114,122,155,188,94,111,148,114,41,99,63,118,96,60,232,3,0,115,161,73,99,173,32,32,54,237,53,101,224,101,
170,173,37,243,95,23,69,198,35,232,122,181,11,239,252,244,29,68,66,146,42,70,217,202,202,233,222,195,1,207,250,159,41,250,19,128,217,160,25,181,181,122,249,170,74,20,93,80,161,88,166,245,233,54,196,67,
113,24,69,106,187,231,186,166,79,165,176,85,215,148,186,215,216,185,215,108,124,142,96,59,23,22,101,149,241,157,77,106,108,187,217,80,113,239,117,205,43,68,74,76,224,192,88,215,100,102,91,13,34,156,11,
92,115,234,85,226,95,231,196,164,185,198,214,235,60,203,99,44,229,128,28,104,61,215,195,8,201,112,129,7,163,210,40,0,152,92,22,85,188,120,108,102,219,207,118,248,240,61,185,247,209,82,252,105,173,177,
211,189,226,9,160,140,0,160,2,136,40,16,35,0,36,18,92,213,251,70,45,51,245,251,213,24,233,57,221,227,173,106,106,8,167,180,118,40,34,71,0,64,48,139,154,222,71,41,158,106,48,87,205,187,211,242,20,70,195,
9,188,51,199,217,40,197,24,226,193,24,18,145,52,87,128,10,134,189,212,134,178,139,146,59,92,28,228,183,170,140,231,193,83,106,79,99,201,109,75,48,222,227,71,60,152,157,54,29,9,37,208,250,76,7,86,220,181,
12,46,187,185,102,230,89,21,175,56,211,235,124,82,224,115,46,78,97,181,3,212,98,132,20,155,237,47,127,255,20,164,168,4,209,44,98,253,202,170,141,0,16,28,10,34,26,146,50,250,178,160,166,16,162,37,115,188,
50,37,111,188,78,175,56,59,207,94,241,20,111,106,164,184,31,69,113,26,50,102,146,132,226,113,57,6,0,212,108,80,197,76,204,10,54,42,71,76,78,168,248,111,244,120,197,69,206,72,49,1,96,51,27,5,76,207,71,
50,35,224,42,154,78,48,205,236,146,41,39,135,38,137,129,176,62,128,44,2,128,184,36,37,61,46,2,85,197,7,0,198,146,93,72,8,85,159,197,136,58,175,28,192,228,160,2,155,26,60,0,128,104,22,113,193,93,203,147,
241,245,28,173,149,225,190,199,181,158,225,84,171,125,201,1,103,180,27,208,114,219,18,28,125,44,247,89,14,67,173,62,12,28,27,70,197,138,217,173,34,78,5,69,108,221,9,12,10,124,166,52,54,7,231,96,96,68,
32,130,179,209,133,209,246,89,183,137,12,96,188,107,18,37,139,139,80,96,53,148,1,128,239,220,68,22,158,103,145,7,0,192,24,151,40,37,34,0,112,65,161,13,122,35,213,180,198,145,78,60,104,140,37,53,26,40,
62,55,80,53,92,207,83,179,147,196,121,12,72,202,138,26,102,66,154,157,42,136,196,237,80,115,204,106,140,171,140,0,21,202,73,1,192,17,138,198,101,206,193,8,1,37,70,117,141,42,203,211,66,7,152,20,11,165,
152,37,194,24,231,201,89,201,98,50,152,0,64,142,203,154,51,163,96,156,158,60,24,83,157,60,230,179,103,156,47,89,93,102,172,184,99,49,10,74,109,0,16,97,132,255,99,62,121,211,154,60,165,41,146,210,37,69,
168,88,85,129,190,35,185,253,113,39,119,117,194,221,224,132,185,48,217,213,74,218,14,152,71,128,135,2,159,169,153,159,51,112,95,48,230,45,113,152,23,184,23,184,49,124,58,243,16,2,223,217,164,96,207,252,
127,206,159,133,231,110,76,154,225,190,64,244,108,137,211,210,164,214,6,253,166,243,121,222,199,198,7,24,75,175,67,98,139,249,20,5,113,0,128,72,147,10,73,146,213,173,95,46,206,10,106,130,24,114,158,120,
51,83,86,43,102,35,67,99,19,110,35,28,96,50,184,44,179,152,40,82,139,96,17,85,215,166,137,68,82,176,57,135,93,141,17,0,72,112,57,40,78,143,100,219,180,96,75,9,166,138,15,0,130,113,250,62,33,234,71,112,
232,141,242,202,131,76,54,3,106,86,150,161,241,242,74,136,102,17,0,100,206,249,167,171,111,202,239,160,133,124,147,43,226,18,11,25,69,106,91,114,83,35,124,221,1,132,39,178,221,21,177,56,199,241,103,59,
177,246,238,150,180,237,152,243,19,121,166,196,103,186,131,198,59,28,232,40,113,152,23,120,26,93,89,101,71,187,103,157,66,156,3,163,61,193,140,50,84,164,112,213,22,2,0,58,135,166,218,51,4,59,151,165,160,
215,121,166,21,121,54,47,231,217,252,199,18,1,13,2,220,1,0,6,145,90,0,32,17,87,31,235,212,56,27,35,70,18,113,213,227,108,152,70,228,89,70,18,8,101,68,228,36,41,168,49,153,133,69,145,90,196,2,147,186,211,
41,52,163,180,106,57,7,81,11,133,19,153,148,192,180,53,93,230,182,20,3,64,196,31,215,236,64,83,193,180,5,206,137,242,17,25,208,14,179,35,2,65,97,137,242,50,157,138,4,162,69,132,201,44,194,81,102,131,179,
194,14,79,181,61,229,1,7,128,30,2,126,87,229,77,59,246,41,130,100,241,148,223,100,179,239,228,192,238,203,91,202,175,51,155,4,231,133,183,52,97,255,163,109,57,195,109,135,187,2,232,58,60,132,250,85,101,
170,177,226,92,211,163,59,167,188,146,128,165,13,144,67,103,198,218,87,47,44,89,239,40,179,65,180,155,16,79,243,75,76,250,162,136,133,18,48,217,12,240,15,4,17,79,100,230,21,184,106,29,16,68,10,14,206,
15,118,140,156,190,180,57,185,237,149,116,246,228,138,21,215,235,21,87,119,142,206,117,2,106,81,94,217,93,42,60,82,206,19,169,183,103,52,144,100,142,190,134,96,139,5,51,70,47,27,170,73,168,59,101,53,148,
88,70,218,38,167,92,72,137,229,100,48,62,108,51,137,158,130,98,171,170,202,247,251,166,207,100,35,176,247,238,189,191,1,80,62,230,69,52,136,178,52,61,15,84,56,45,229,0,16,24,139,168,226,27,45,34,140,86,
195,116,21,178,234,217,61,90,230,137,217,110,196,85,247,46,87,131,200,73,50,227,163,2,37,255,30,143,227,161,250,143,239,208,117,16,150,182,243,44,57,56,2,145,68,232,241,87,59,31,253,220,245,77,95,242,
212,57,72,253,37,21,232,60,48,152,243,153,147,175,244,161,120,129,83,213,20,103,186,147,30,212,77,113,0,120,254,64,207,217,47,220,208,148,16,40,53,184,235,157,24,104,207,8,79,192,168,55,128,170,22,15,
70,188,129,44,172,226,134,228,249,91,147,193,120,223,240,68,100,198,242,202,39,148,53,47,254,53,178,211,230,165,177,63,192,178,142,81,34,19,198,97,16,40,177,154,12,46,0,136,133,37,245,177,62,27,184,51,
170,117,238,159,46,231,25,97,92,230,36,233,212,24,245,71,250,43,61,214,37,5,165,86,213,153,97,42,32,205,204,212,148,11,155,0,108,87,42,27,39,130,129,130,193,102,49,138,149,69,182,197,0,48,62,24,86,197,
183,151,206,88,248,220,32,155,85,207,65,211,210,142,249,56,67,36,153,133,163,9,22,241,135,226,195,253,227,65,239,168,63,190,239,162,13,238,237,243,61,96,81,211,20,79,27,111,255,241,82,123,235,37,139,139,
247,181,212,184,175,106,185,166,10,67,93,1,4,124,217,22,89,92,6,222,125,222,11,73,197,66,209,155,221,165,100,18,167,227,248,67,209,196,168,63,122,182,204,101,109,46,106,40,68,223,153,204,141,144,209,238,
32,170,90,60,24,238,14,102,97,21,213,39,205,240,238,145,96,123,38,254,135,164,177,231,185,221,53,95,162,140,11,28,192,13,23,85,86,139,148,152,0,96,98,52,170,138,105,243,36,15,111,36,128,230,145,220,90,
227,42,99,187,139,131,198,1,46,2,192,233,254,201,238,11,27,60,40,170,46,0,49,137,144,85,82,44,123,78,77,98,225,202,98,80,224,190,222,167,182,254,82,233,104,85,49,158,48,48,42,224,139,27,154,87,137,2,181,
50,198,209,215,25,80,29,132,69,13,51,167,26,183,22,111,250,145,242,238,62,242,9,80,153,125,185,63,120,234,232,15,142,117,141,207,120,29,25,3,250,125,161,89,231,28,1,99,132,29,29,104,235,62,142,127,213,
62,239,91,145,39,141,0,149,185,244,141,223,31,250,211,147,95,91,183,216,102,49,148,172,188,185,1,175,60,214,1,46,103,87,239,27,154,102,85,1,91,183,233,169,16,224,49,215,9,215,57,48,213,94,230,178,54,23,
215,59,178,202,15,246,78,225,2,153,99,116,48,51,168,73,52,82,184,43,146,71,151,31,61,235,203,20,108,133,254,153,239,153,103,74,164,63,187,75,255,228,152,241,44,136,145,128,227,142,43,27,215,1,64,96,44,
138,169,41,73,241,125,1,128,179,210,62,253,44,142,41,22,154,38,93,1,42,28,124,198,99,243,228,91,93,39,57,227,178,96,160,40,105,80,15,133,59,125,108,12,178,196,192,129,5,130,141,255,138,63,117,91,206,169,
132,17,193,126,245,178,138,242,245,23,215,220,10,36,39,132,72,148,171,98,151,47,72,206,244,84,225,108,233,172,198,230,25,102,55,22,140,71,122,71,66,225,212,53,35,212,4,140,115,214,201,121,252,217,129,
182,238,99,80,217,174,204,135,180,66,74,231,70,57,140,4,98,177,95,238,61,245,123,206,193,92,165,86,44,185,172,66,51,180,49,247,53,143,124,236,60,66,83,15,116,12,119,0,128,195,99,134,209,145,25,10,59,229,
151,209,115,106,18,9,57,243,61,120,106,29,32,2,1,99,156,61,123,160,59,99,57,165,28,2,122,158,248,159,39,30,180,198,18,21,84,7,6,129,108,252,242,166,229,171,22,86,56,214,2,192,169,3,35,234,227,192,32,162,
184,218,62,253,44,209,116,204,42,133,0,231,14,41,21,200,4,24,119,1,73,237,53,48,17,105,175,244,88,91,22,92,228,65,111,151,178,178,12,4,18,56,177,127,4,43,174,42,3,7,238,30,176,87,46,28,216,189,245,187,
33,34,189,150,138,70,235,127,241,203,213,39,59,125,247,53,87,59,239,18,5,106,141,69,36,28,219,55,164,58,235,56,139,45,112,151,91,167,251,153,60,175,217,88,29,33,165,22,145,132,56,199,36,5,143,115,32,6,
145,140,139,50,31,65,4,67,94,111,119,254,7,108,127,64,158,120,14,69,242,244,254,174,179,87,46,43,127,113,245,194,226,27,155,215,150,160,207,27,196,152,210,254,187,18,238,60,79,41,205,198,201,100,112,215,
209,94,239,150,155,151,70,69,74,205,37,117,5,240,158,202,116,57,188,127,96,36,11,167,164,54,25,118,60,22,140,121,135,39,35,209,37,105,231,157,43,105,218,92,253,162,202,191,150,198,214,189,221,149,191,
198,230,2,191,180,111,215,102,17,160,86,74,224,228,96,101,227,83,241,219,221,5,166,6,0,24,234,9,226,92,135,95,85,91,87,46,116,192,96,18,0,64,34,140,238,212,172,83,107,92,165,159,82,202,185,60,145,158,
133,184,247,112,239,203,159,187,190,169,165,188,174,0,174,10,59,198,134,149,63,94,208,118,108,28,212,64,177,252,210,18,16,142,75,56,248,94,43,23,88,255,238,45,62,0,54,72,204,182,180,206,13,0,136,4,19,
120,245,47,61,8,199,212,79,100,105,90,149,76,239,35,192,251,229,235,31,124,67,171,177,90,219,93,233,247,190,247,15,171,118,63,242,95,15,231,127,144,246,60,73,247,65,11,211,244,141,71,15,239,252,243,255,
186,122,153,203,102,174,90,123,99,53,246,254,241,108,242,4,155,60,73,183,198,206,99,141,13,0,145,136,36,15,79,68,206,84,122,108,203,74,106,11,112,238,116,230,132,31,8,202,89,239,180,180,38,169,137,186,
134,3,25,102,56,144,244,123,48,33,71,187,116,31,237,116,158,15,90,200,103,141,157,154,44,56,190,74,8,1,192,145,138,234,112,79,123,184,7,188,65,188,181,183,95,125,251,145,0,139,215,148,164,254,222,83,177,
241,167,234,103,135,65,167,87,220,0,12,166,71,92,252,199,75,237,173,55,95,82,231,45,114,152,234,86,173,43,199,222,167,187,115,174,247,82,116,226,240,56,6,122,195,88,190,182,8,21,85,54,16,129,80,0,51,233,
60,145,176,132,179,109,126,156,60,50,142,68,76,86,141,6,42,173,180,160,161,57,105,134,51,78,126,170,213,80,32,31,79,161,254,117,214,7,165,124,131,102,74,156,150,12,23,120,40,18,151,182,63,219,246,187,
255,125,231,138,127,113,184,140,226,133,87,148,225,208,27,249,127,70,108,62,103,140,105,121,197,83,116,122,48,208,94,233,177,45,43,173,182,105,214,99,50,11,112,150,36,157,66,135,206,140,101,9,246,249,
10,80,81,219,33,152,15,94,62,1,42,241,216,172,180,112,206,17,143,202,136,199,100,68,163,50,124,131,17,12,116,135,224,27,137,38,213,191,10,86,125,179,3,158,82,203,116,189,244,135,121,241,167,25,144,147,
38,216,221,167,186,135,43,23,215,71,83,137,32,0,240,159,127,237,120,242,159,111,89,254,53,79,137,153,46,95,83,140,99,7,85,183,146,49,226,75,224,229,93,131,48,152,4,184,92,6,152,173,2,228,4,71,40,36,97,
114,34,62,187,98,85,153,109,76,38,138,75,174,173,152,94,126,146,183,43,15,23,254,33,191,198,230,175,177,63,44,210,10,36,72,205,250,171,155,74,58,25,225,97,202,201,76,206,249,75,71,123,251,175,189,176,
252,185,171,90,202,63,177,104,185,11,61,221,97,12,244,230,103,146,207,207,121,150,67,99,231,192,121,243,228,96,199,213,75,203,97,119,24,96,115,154,146,78,33,5,42,169,177,79,123,17,120,244,249,3,61,89,
91,161,231,45,64,69,75,99,255,13,2,84,254,252,7,175,54,142,70,160,80,65,129,136,85,87,206,132,9,63,81,181,126,251,129,124,216,211,212,216,233,233,182,0,24,103,244,92,122,129,63,191,221,117,238,221,206,
145,23,0,96,217,197,110,44,90,234,156,142,203,85,191,226,18,199,240,104,28,221,221,17,244,13,68,49,225,151,166,227,166,213,47,193,44,226,35,27,42,81,80,104,0,128,16,21,112,143,218,215,28,51,26,75,136,
38,254,135,78,84,157,167,20,25,4,34,27,41,222,192,156,0,159,111,255,225,240,95,71,38,163,157,132,0,151,93,83,10,131,69,204,171,255,245,7,168,228,230,51,117,190,119,122,224,209,75,71,251,123,227,18,11,
1,64,105,141,77,149,135,178,234,164,143,68,102,228,160,63,20,205,218,50,84,124,118,30,217,93,170,125,49,143,83,74,243,237,231,249,94,38,187,1,215,108,172,130,201,44,128,0,131,162,196,183,230,221,94,13,
254,210,215,243,20,0,226,16,79,128,32,67,144,190,246,251,119,159,239,245,133,222,35,4,88,115,101,49,90,86,184,167,83,195,206,239,101,182,27,113,195,199,42,80,86,110,6,0,6,144,79,149,223,240,96,107,254,
47,67,195,83,168,55,49,226,60,144,166,183,54,173,172,183,213,59,4,138,140,196,236,88,130,177,31,62,117,244,247,146,204,99,86,187,136,213,87,148,124,184,94,241,28,125,150,144,25,31,28,15,119,0,64,89,149,
85,149,143,242,74,235,116,59,228,156,62,18,197,58,231,21,43,126,254,188,226,92,133,183,243,113,21,184,205,184,113,83,37,156,46,3,0,68,64,249,237,122,62,246,168,213,222,244,0,37,10,0,190,142,142,41,198,
104,198,90,40,18,147,216,61,15,191,245,155,161,137,112,7,37,192,170,53,46,172,187,190,20,182,66,245,243,202,244,92,245,139,28,184,249,214,74,20,21,155,64,128,4,231,228,179,149,27,30,124,78,215,203,208,
202,161,253,27,37,136,168,242,164,113,14,219,220,1,220,215,218,117,100,238,137,51,111,119,140,142,190,120,172,247,41,0,104,92,104,67,109,99,129,102,127,234,205,238,74,153,178,217,56,185,251,236,84,223,
100,7,0,84,84,88,192,196,220,60,152,11,140,40,116,38,163,6,199,131,137,220,130,173,196,191,238,88,113,173,254,152,159,243,236,124,95,212,36,98,217,10,23,54,221,82,145,234,155,48,37,248,68,197,141,59,222,
210,195,158,86,62,118,206,15,6,148,219,157,135,70,194,99,213,156,147,130,212,111,227,161,104,226,147,63,125,99,199,207,238,89,115,71,75,141,251,170,218,26,11,170,42,205,56,125,54,130,246,211,33,140,250,
84,147,81,114,146,40,16,212,213,152,177,108,105,1,138,61,51,25,153,62,206,201,157,85,55,61,248,178,94,60,173,96,144,191,135,41,174,181,45,145,131,100,142,216,235,132,24,111,6,159,221,162,248,254,83,199,
222,90,94,239,185,176,218,99,91,118,217,101,46,12,142,38,16,142,40,39,151,233,205,199,86,62,65,37,101,138,147,140,37,194,235,39,6,219,111,184,168,10,22,171,128,66,183,17,19,147,217,235,236,242,202,25,
87,205,212,15,159,105,63,10,96,65,190,245,206,43,164,244,239,180,221,165,70,4,128,179,80,68,113,145,17,229,101,38,212,213,90,96,50,146,84,29,231,40,195,109,229,172,29,215,115,0,0,5,112,73,68,65,84,27,
31,58,170,23,87,171,189,25,94,241,212,31,71,142,28,73,148,53,54,190,34,136,210,6,2,50,35,113,161,72,92,250,236,142,55,255,248,79,27,150,180,222,114,105,253,109,118,179,88,188,120,145,21,139,23,89,17,142,
48,244,15,197,224,27,151,16,152,146,16,12,201,136,197,56,36,57,249,229,54,209,64,96,54,18,216,109,2,156,133,2,202,138,141,168,40,51,66,76,63,194,135,240,39,5,193,176,185,236,250,159,140,96,30,84,93,101,
198,181,86,143,226,125,65,231,113,65,231,131,26,23,216,80,84,172,156,209,106,54,101,191,156,254,246,254,177,202,150,186,35,68,38,51,95,97,100,50,248,119,30,63,250,216,175,255,233,178,109,102,19,181,173,
191,174,8,147,1,101,167,149,209,168,175,173,45,75,10,80,91,155,157,32,99,54,207,242,199,41,252,132,161,16,0,94,123,127,96,48,26,151,252,102,163,88,120,229,101,110,132,194,217,147,140,219,149,28,82,4,216,
183,255,204,168,148,171,23,174,184,204,13,153,101,239,180,120,220,170,169,247,89,180,102,181,19,241,184,178,43,198,229,204,251,243,239,0,128,66,135,136,107,175,86,30,75,90,100,54,83,152,141,20,118,187,
0,163,33,243,93,48,198,195,148,226,97,202,34,223,171,216,248,27,125,1,10,211,180,112,129,5,37,197,57,63,114,58,83,127,138,50,90,62,212,217,57,90,217,216,248,50,49,178,235,56,227,25,118,221,47,118,183,
29,127,252,245,115,173,155,63,182,120,237,154,69,165,87,20,57,76,117,86,11,197,194,122,11,22,214,67,23,113,206,165,193,201,200,235,197,133,214,175,215,109,220,161,123,230,74,167,194,2,1,133,5,31,254,58,
90,141,220,46,113,102,128,235,161,254,86,239,123,21,77,245,53,148,96,198,101,218,214,51,238,255,211,219,93,143,127,242,202,5,247,206,23,87,137,74,138,12,40,41,82,23,38,65,34,3,140,242,194,212,255,125,
99,225,246,198,114,199,154,210,98,3,0,229,103,153,202,151,82,106,171,53,211,248,243,162,170,114,229,65,62,31,50,153,40,26,106,115,126,35,111,94,20,141,75,254,97,127,180,107,194,31,123,97,233,2,251,143,
170,55,252,114,66,251,41,101,242,184,12,240,184,242,155,252,178,70,73,127,103,103,95,201,210,250,221,70,153,92,155,126,16,33,144,76,8,248,238,19,199,222,4,240,230,37,45,165,37,31,93,94,209,180,168,210,
181,208,101,51,150,23,90,13,69,6,145,218,114,85,18,75,200,65,127,56,62,56,58,25,25,120,239,220,68,235,171,239,13,61,181,247,175,239,106,7,159,168,80,107,207,228,169,96,44,145,51,87,155,82,194,154,171,
156,89,201,35,6,163,81,245,40,167,15,74,167,122,38,142,185,11,77,189,185,238,45,40,115,156,51,25,133,57,107,23,97,46,143,220,42,11,175,199,4,233,19,169,196,28,0,216,241,220,201,35,213,110,219,51,238,66,
83,214,135,10,140,2,141,55,86,22,158,155,251,123,192,226,81,92,39,181,246,76,30,180,152,132,140,119,107,54,10,145,134,50,71,70,34,2,229,156,1,210,0,32,44,78,253,246,210,123,253,251,99,146,156,145,56,78,
41,97,77,21,133,103,8,157,53,221,185,204,50,34,169,134,253,209,112,107,239,68,86,234,107,145,195,52,90,226,180,206,221,79,61,174,196,123,36,28,151,114,225,164,200,109,55,143,149,185,45,25,167,144,80,134,
83,74,229,1,160,163,127,178,215,83,104,206,59,45,55,23,37,18,60,30,138,198,67,254,176,20,28,245,71,38,222,62,53,220,251,94,247,184,143,128,30,236,105,235,204,223,25,156,131,218,122,38,142,185,28,198,156,
73,34,102,131,16,109,40,119,120,231,254,174,104,187,85,85,85,89,96,23,87,131,144,133,249,30,70,85,226,50,153,108,38,163,88,94,104,182,2,192,112,32,22,241,5,226,177,212,150,7,167,240,27,8,223,231,109,245,
170,127,216,72,129,42,155,106,175,38,132,54,106,149,227,4,177,254,83,93,143,205,167,14,221,60,45,110,216,64,184,250,39,127,0,64,52,243,167,189,199,189,121,165,127,150,47,173,93,44,72,244,242,124,202,114,
142,201,254,142,174,167,53,249,92,82,127,123,202,164,206,69,140,99,120,160,163,43,43,132,183,174,174,206,44,153,201,93,106,216,4,188,167,183,221,155,245,125,164,226,150,22,187,73,14,223,169,246,44,23,
248,187,253,173,94,69,65,158,38,161,170,185,254,179,26,101,146,188,48,114,188,247,244,57,213,175,199,0,64,205,146,198,22,198,228,75,243,193,156,23,113,222,75,141,236,96,207,251,61,243,210,210,21,205,245,
27,41,160,126,144,39,0,66,248,104,239,41,111,214,215,95,21,237,186,190,190,190,8,128,55,170,90,170,222,103,146,105,49,33,114,35,1,81,181,125,70,38,98,49,32,22,235,26,154,202,248,18,37,225,124,132,112,
246,126,79,123,143,23,64,126,223,214,253,255,152,6,79,118,159,170,106,170,171,5,33,213,127,111,94,188,94,111,180,170,185,97,12,224,138,139,79,14,170,249,165,200,255,215,137,19,68,9,227,163,76,224,195,
70,35,233,202,119,18,255,91,145,230,130,173,175,181,111,28,201,115,189,223,169,95,90,95,28,231,188,28,18,119,81,16,7,39,212,198,9,23,201,244,98,139,3,9,14,30,35,160,65,206,200,36,192,124,176,199,122,7,
142,12,204,203,89,48,151,40,88,135,204,72,238,147,8,210,136,80,250,161,77,30,156,226,125,158,224,138,7,77,164,200,150,176,233,234,3,102,143,239,195,148,177,70,171,156,8,150,215,214,132,32,201,135,37,80,
197,137,153,11,68,49,41,128,240,196,1,153,11,138,95,100,145,205,225,156,203,143,26,179,57,54,56,21,122,83,141,47,130,132,234,87,85,166,137,49,198,85,113,102,11,10,234,97,146,41,18,226,3,76,162,121,97,
42,145,145,34,202,9,143,133,152,57,232,107,239,80,77,47,214,75,156,224,4,147,185,102,94,3,163,200,153,188,244,223,99,107,45,192,181,30,88,44,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* Main_Component::opnmidi_png = (const char*) resource_Main_Component_opnmidi_png;
const int Main_Component::opnmidi_pngSize = 9727;


//[EndFile] You can add extra defines here...
#include "ui/generic_main_component.tcc"
template class Generic_Main_Component<Main_Component>;
//[/EndFile]
