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
#include "ui/operator_editor.h"
#include "ui/vu_meter.h"
#include "ui/indicator_NxM.h"
#include "ui/about_component.h"
#include "adl/instrument.h"
#include "midi/insnames.h"
#include "plugin_processor.h"
#include "parameter_block.h"
#include "messages.h"
#include <wopl/wopl_file.h>
#include <memory>
#include <cstdio>
#include <cmath>
#include <cassert>
//[/Headers]

#include "main_component.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#if 0
#   define trace(fmt, ...)
#else
#   define trace(fmt, ...) fprintf(stderr, "[UI Main] " fmt "\n", ##__VA_ARGS__)
#endif

#if !JUCE_LINUX
static constexpr bool prefer_native_file_dialog = true;
#else
static constexpr bool prefer_native_file_dialog = false;
#endif

enum class Radio_Button_Group {
    Fm_Mode = 1,
    Algo_12,
    Algo_34,
};
//[/MiscUserDefs]

//==============================================================================
Main_Component::Main_Component (AdlplugAudioProcessor &proc, Parameter_Block &pb)
{
    //[Constructor_pre] You can add your own custom stuff here..
    parameter_block_ = &pb;
    //[/Constructor_pre]

    ed_op2.reset (new Operator_Editor (WOPL_OP_CARRIER1, pb));
    addAndMakeVisible (ed_op2.get());
    ed_op2->setName ("new component");

    ed_op2->setBounds (16, 160, 352, 128);

    btn_4op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_4op.get());
    btn_4op->setButtonText (TRANS("4 op"));
    btn_4op->setConnectedEdges (Button::ConnectedOnRight);
    btn_4op->addListener (this);
    btn_4op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_4op->setBounds (176, 134, 56, 24);

    btn_pseudo4op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_pseudo4op.get());
    btn_pseudo4op->setButtonText (TRANS("2x2 op"));
    btn_pseudo4op->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    btn_pseudo4op->addListener (this);
    btn_pseudo4op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_pseudo4op->setBounds (232, 134, 56, 24);

    btn_2op.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_2op.get());
    btn_2op->setButtonText (TRANS("2 op"));
    btn_2op->setConnectedEdges (Button::ConnectedOnLeft);
    btn_2op->addListener (this);
    btn_2op->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_2op->setBounds (288, 134, 56, 24);

    btn_fm12.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_fm12.get());
    btn_fm12->setButtonText (TRANS("FM"));
    btn_fm12->setConnectedEdges (Button::ConnectedOnBottom);
    btn_fm12->addListener (this);
    btn_fm12->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_fm12->setBounds (376, 200, 36, 24);

    btn_am12.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_am12.get());
    btn_am12->setButtonText (TRANS("AM"));
    btn_am12->setConnectedEdges (Button::ConnectedOnTop);
    btn_am12->addListener (this);
    btn_am12->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_am12->setBounds (376, 224, 36, 24);

    ed_op1.reset (new Operator_Editor (WOPL_OP_MODULATOR1, pb));
    addAndMakeVisible (ed_op1.get());
    ed_op1->setName ("new component");

    ed_op1->setBounds (421, 160, 352, 128);

    ed_op4.reset (new Operator_Editor (WOPL_OP_CARRIER2, pb));
    addAndMakeVisible (ed_op4.get());
    ed_op4->setName ("new component");

    ed_op4->setBounds (16, 328, 352, 128);

    btn_fm34.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_fm34.get());
    btn_fm34->setButtonText (TRANS("FM"));
    btn_fm34->setConnectedEdges (Button::ConnectedOnBottom);
    btn_fm34->addListener (this);
    btn_fm34->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_fm34->setBounds (376, 368, 36, 24);

    btn_am34.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_am34.get());
    btn_am34->setButtonText (TRANS("AM"));
    btn_am34->setConnectedEdges (Button::ConnectedOnTop);
    btn_am34->addListener (this);
    btn_am34->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_am34->setBounds (376, 392, 36, 24);

    ed_op3.reset (new Operator_Editor (WOPL_OP_MODULATOR2, pb));
    addAndMakeVisible (ed_op3.get());
    ed_op3->setName ("new component");

    ed_op3->setBounds (421, 328, 352, 128);

    sl_tune12.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_tune12.get());
    sl_tune12->setRange (-127, 127, 0);
    sl_tune12->setSliderStyle (Slider::LinearHorizontal);
    sl_tune12->setTextBoxStyle (Slider::TextBoxRight, false, 36, 20);
    sl_tune12->addListener (this);

    sl_tune12->setBounds (568, 136, 150, 24);

    sl_tune34.reset (new Slider ("new slider"));
    addAndMakeVisible (sl_tune34.get());
    sl_tune34->setRange (-127, 127, 0);
    sl_tune34->setSliderStyle (Slider::LinearHorizontal);
    sl_tune34->setTextBoxStyle (Slider::TextBoxRight, false, 36, 20);
    sl_tune34->addListener (this);

    sl_tune34->setBounds (568, 304, 150, 24);

    kn_fb12.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_fb12.get());
    kn_fb12->setName ("new component");

    kn_fb12->setBounds (496, 128, 32, 32);

    kn_fb34.reset (new Styled_Knob_DefaultSmall());
    addAndMakeVisible (kn_fb34.get());
    kn_fb34->setName ("new component");

    kn_fb34->setBounds (496, 296, 32, 32);

    midi_kb.reset (new MidiKeyboardComponent (midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard));
    addAndMakeVisible (midi_kb.get());
    midi_kb->setName ("new component");

    midi_kb->setBounds (16, 480, 757, 56);

    btn_about.reset (new ImageButton ("new button"));
    addAndMakeVisible (btn_about.get());
    btn_about->setButtonText (String());
    btn_about->addListener (this);

    btn_about->setImages (false, true, true,
                          ImageCache::getFromMemory (logo_png, logo_pngSize), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000),
                          Image(), 1.0f, Colour (0x00000000));
    btn_about->setBounds (16, 8, 232, 40);

    label.reset (new Label ("new label",
                            TRANS("Chips")));
    addAndMakeVisible (label.get());
    label->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::aliceblue);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label->setBounds (480, 40, 48, 24);

    lbl_num_chips.reset (new Label ("new label",
                                    TRANS("100")));
    addAndMakeVisible (lbl_num_chips.get());
    lbl_num_chips->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_num_chips->setJustificationType (Justification::centred);
    lbl_num_chips->setEditable (false, false, false);
    lbl_num_chips->setColour (Label::textColourId, Colours::aliceblue);
    lbl_num_chips->setColour (Label::outlineColourId, Colour (0xff8e989b));
    lbl_num_chips->setColour (TextEditor::textColourId, Colours::black);
    lbl_num_chips->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_num_chips->setBounds (528, 40, 32, 24);

    btn_less_chips.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_less_chips.get());
    btn_less_chips->setButtonText (TRANS("<"));
    btn_less_chips->setConnectedEdges (Button::ConnectedOnRight);
    btn_less_chips->addListener (this);

    btn_less_chips->setBounds (568, 40, 23, 24);

    btn_more_chips.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_more_chips.get());
    btn_more_chips->setButtonText (TRANS(">"));
    btn_more_chips->setConnectedEdges (Button::ConnectedOnLeft);
    btn_more_chips->addListener (this);

    btn_more_chips->setBounds (591, 40, 23, 24);

    cb_emulator.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_emulator.get());
    cb_emulator->setEditableText (false);
    cb_emulator->setJustificationType (Justification::centredLeft);
    cb_emulator->setTextWhenNothingSelected (String());
    cb_emulator->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_emulator->addListener (this);

    cb_emulator->setBounds (272, 40, 198, 24);

    label2.reset (new Label ("new label",
                             TRANS("FM synthesizer with YMF262 chip emulation")));
    addAndMakeVisible (label2.get());
    label2->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::aliceblue);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label2->setBounds (280, 8, 280, 24);

    vu_left.reset (new Vu_Meter());
    addAndMakeVisible (vu_left.get());
    vu_left->setName ("new component");

    vu_left->setBounds (680, 8, 92, 12);

    vu_right.reset (new Vu_Meter());
    addAndMakeVisible (vu_right.get());
    vu_right->setName ("new component");

    vu_right->setBounds (680, 24, 92, 12);

    label3.reset (new Label ("new label",
                             TRANS("CPU")));
    addAndMakeVisible (label3.get());
    label3->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (Label::textColourId, Colours::aliceblue);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label3->setBounds (684, 40, 40, 24);

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

    lbl_cpu->setBounds (724, 40, 48, 24);

    ind_midi_activity.reset (new Indicator_NxM (2, 8));
    addAndMakeVisible (ind_midi_activity.get());
    ind_midi_activity->setName ("new component");

    ind_midi_activity->setBounds (568, 8, 102, 28);

    btn_panic.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_panic.get());
    btn_panic->setButtonText (TRANS("Panic"));
    btn_panic->setConnectedEdges (Button::ConnectedOnBottom);
    btn_panic->addListener (this);
    btn_panic->setColour (TextButton::buttonOnColourId, Colour (0xff42a2c8));

    btn_panic->setBounds (622, 40, 48, 24);

    edt_bank_name.reset (new TextEditor ("new text editor"));
    addAndMakeVisible (edt_bank_name.get());
    edt_bank_name->setMultiLine (false);
    edt_bank_name->setReturnKeyStartsNewLine (false);
    edt_bank_name->setReadOnly (false);
    edt_bank_name->setScrollbarsShown (true);
    edt_bank_name->setCaretVisible (true);
    edt_bank_name->setPopupMenuEnabled (true);
    edt_bank_name->setColour (TextEditor::outlineColourId, Colour (0xff8e989b));
    edt_bank_name->setText (TRANS("Bank name"));

    edt_bank_name->setBounds (16, 80, 208, 24);

    cb_program.reset (new ComboBox ("new combo box"));
    addAndMakeVisible (cb_program.get());
    cb_program->setEditableText (false);
    cb_program->setJustificationType (Justification::centredLeft);
    cb_program->setTextWhenNothingSelected (String());
    cb_program->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    cb_program->addListener (this);

    cb_program->setBounds (312, 80, 280, 24);

    label4.reset (new Label ("new label",
                             TRANS("Channel")));
    addAndMakeVisible (label4.get());
    label4->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (Label::textColourId, Colours::aliceblue);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    label4->setBounds (620, 80, 60, 24);

    lbl_channel.reset (new Label ("new label",
                                  TRANS("1")));
    addAndMakeVisible (lbl_channel.get());
    lbl_channel->setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
    lbl_channel->setJustificationType (Justification::centred);
    lbl_channel->setEditable (false, false, false);
    lbl_channel->setColour (Label::textColourId, Colours::aliceblue);
    lbl_channel->setColour (Label::outlineColourId, Colour (0xff8e989b));
    lbl_channel->setColour (TextEditor::textColourId, Colours::black);
    lbl_channel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    lbl_channel->setBounds (684, 80, 32, 24);

    btn_prev_channel.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_prev_channel.get());
    btn_prev_channel->setButtonText (TRANS("<"));
    btn_prev_channel->setConnectedEdges (Button::ConnectedOnRight);
    btn_prev_channel->addListener (this);

    btn_prev_channel->setBounds (724, 80, 23, 24);

    btn_next_channel.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_next_channel.get());
    btn_next_channel->setButtonText (TRANS(">"));
    btn_next_channel->setConnectedEdges (Button::ConnectedOnLeft);
    btn_next_channel->addListener (this);

    btn_next_channel->setBounds (747, 80, 23, 24);

    btn_bank_load.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_bank_load.get());
    btn_bank_load->setButtonText (String());
    btn_bank_load->setConnectedEdges (Button::ConnectedOnRight);
    btn_bank_load->addListener (this);

    btn_bank_load->setBounds (232, 80, 23, 24);

    btn_bank_save.reset (new TextButton ("new button"));
    addAndMakeVisible (btn_bank_save.get());
    btn_bank_save->setButtonText (String());
    btn_bank_save->setConnectedEdges (Button::ConnectedOnLeft);
    btn_bank_save->addListener (this);

    btn_bank_save->setBounds (255, 80, 23, 24);


    //[UserPreSize]
    kn_fb12->add_listener(this);
    kn_fb34->add_listener(this);
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    proc_ = &proc;
    bank_directory_ = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory();

    midi_kb_state_.addListener(this);

    midi_kb->setLowestVisibleKey(24);

    btn_bank_load->setTooltip(TRANS("Load bank"));
    btn_bank_save->setTooltip(TRANS("Save bank"));

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

    kn_fb12->set_range(0, 7);
    kn_fb34->set_range(0, 7);
    sl_tune12->setNumDecimalPlacesToDisplay(0);
    sl_tune34->setNumDecimalPlacesToDisplay(0);
    ed_op1->set_op_label(TRANS("Modulator"));
    ed_op2->set_op_label(TRANS("Carrier"));
    ed_op3->set_op_label(TRANS("Modulator"));
    ed_op4->set_op_label(TRANS("Carrier"));

    lbl_num_chips->setText(String(proc.get_num_chips()), juce::dontSendNotification);

    std::vector<std::string> emus = proc.enumerate_emulators();
    for (size_t i = 0, n = emus.size(); i < n; ++i)
        cb_emulator->addItem(emus[i], i + 1);

    lbl_channel->setText(String(1 + midichannel_), dontSendNotification);

    vu_timer_.reset(new Vu_Timer(this));
    vu_timer_->startTimer(10);

    cpu_load_timer_.reset(new Cpu_Load_Timer(this));
    cpu_load_timer_->startTimer(500);
    lbl_cpu->setText("0%", dontSendNotification);

    midi_activity_timer_.reset(new Midi_Activity_Timer(this));
    midi_activity_timer_->startTimer(100);

    {
        std::unique_lock<std::mutex> lock(proc.acquire_player_nonrt());
        cb_emulator->setSelectedId(1 + proc.chip_emulator_nonrt());
    }
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
    label = nullptr;
    lbl_num_chips = nullptr;
    btn_less_chips = nullptr;
    btn_more_chips = nullptr;
    cb_emulator = nullptr;
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
    lbl_channel = nullptr;
    btn_prev_channel = nullptr;
    btn_next_channel = nullptr;
    btn_bank_load = nullptr;
    btn_bank_save = nullptr;


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
        int x = 28, y = 132, width = 92, height = 30;
        String text (TRANS("Operator 1-2"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 28, y = 300, width = 92, height = 30;
        String text (TRANS("Operator 3-4"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Bold"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 532, y = 132, width = 36, height = 30;
        String text (TRANS("Tune"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 532, y = 300, width = 36, height = 30;
        String text (TRANS("Tune"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 428, y = 300, width = 60, height = 30;
        String text (TRANS("Feedback"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 428, y = 132, width = 60, height = 30;
        String text (TRANS("Feedback"));
        Colour fillColour = Colours::aliceblue;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.0f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centred, true);
    }

    {
        int x = 680, y = 8, width = 92, height = 12;
        Colour strokeColour = Colour (0xff8e989b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

    }

    {
        int x = 680, y = 24, width = 92, height = 12;
        Colour strokeColour = Colour (0xff8e989b);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 1);

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
    Button *btn = buttonThatWasClicked;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btn_4op.get())
    {
        //[UserButtonCode_btn_4op] -- add your button handler code here..
        if (btn->getToggleState()) {
            AudioParameterBool &p1 = *pb.p_ps4op;
            p1.beginChangeGesture();
            p1 = false;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *pb.p_4op;
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
            AudioParameterBool &p1 = *pb.p_ps4op;
            p1.beginChangeGesture();
            p1 = true;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *pb.p_4op;
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
            AudioParameterBool &p1 = *pb.p_ps4op;
            p1.beginChangeGesture();
            p1 = false;
            p1.endChangeGesture();
            AudioParameterBool &p2 = *pb.p_4op;
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
            AudioParameterChoice &p = *pb.p_con12;
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
            AudioParameterChoice &p = *pb.p_con12;
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
            AudioParameterChoice &p = *pb.p_con34;
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
            AudioParameterChoice &p = *pb.p_con34;
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
    else if (buttonThatWasClicked == btn_less_chips.get())
    {
        //[UserButtonCode_btn_less_chips] -- add your button handler code here..
        AdlplugAudioProcessor &proc = *proc_;
        unsigned nchips = proc.get_num_chips();
        if (nchips > 1) {
            std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
            proc.set_num_chips_nonrt(nchips - 1);
            lock.unlock();
            lbl_num_chips->setText(String(proc.get_num_chips()), juce::dontSendNotification);
        }
        //[/UserButtonCode_btn_less_chips]
    }
    else if (buttonThatWasClicked == btn_more_chips.get())
    {
        //[UserButtonCode_btn_more_chips] -- add your button handler code here..
        AdlplugAudioProcessor &proc = *proc_;
        unsigned nchips = proc.get_num_chips();
        if (nchips < 100) {
            std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
            proc.set_num_chips_nonrt(proc.get_num_chips() + 1);
            lock.unlock();
            lbl_num_chips->setText(String(proc.get_num_chips()), juce::dontSendNotification);
        }
        //[/UserButtonCode_btn_more_chips]
    }
    else if (buttonThatWasClicked == btn_panic.get())
    {
        //[UserButtonCode_btn_panic] -- add your button handler code here..
        AdlplugAudioProcessor &proc = *proc_;
        std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
        proc.panic_nonrt();
        //[/UserButtonCode_btn_panic]
    }
    else if (buttonThatWasClicked == btn_prev_channel.get())
    {
        //[UserButtonCode_btn_prev_channel] -- add your button handler code here..
        on_change_midi_channel(midichannel_ - 1);
        //[/UserButtonCode_btn_prev_channel]
    }
    else if (buttonThatWasClicked == btn_next_channel.get())
    {
        //[UserButtonCode_btn_next_channel] -- add your button handler code here..
        on_change_midi_channel(midichannel_ + 1);
        //[/UserButtonCode_btn_next_channel]
    }
    else if (buttonThatWasClicked == btn_bank_load.get())
    {
        //[UserButtonCode_btn_bank_load] -- add your button handler code here..
        FileChooser chooser(TRANS("Load bank..."), bank_directory_, "*.wopl", prefer_native_file_dialog);
        if (chooser.browseForFileToOpen()) {
            File file = chooser.getResult();
            bank_directory_ = file.getParentDirectory();
            load_bank(file);
        }
        //[/UserButtonCode_btn_bank_load]
    }
    else if (buttonThatWasClicked == btn_bank_save.get())
    {
        //[UserButtonCode_btn_bank_save] -- add your button handler code here..
        FileChooser chooser(TRANS("Save bank..."), bank_directory_, "*.wopl", prefer_native_file_dialog);
        if (chooser.browseForFileToSave(false)) {
            File file = chooser.getResult();
            file = file.withFileExtension(".wopl");

            bool confirm = true;
            if (file.exists()) {
                String title = TRANS("File already exists");
                String message = TRANS("There's already a file called: ")
                    + file.getFullPathName() + "\n\n" +
                    TRANS("Are you sure you want to overwrite it?");
                confirm = AlertWindow::showOkCancelBox(
                    AlertWindow::WarningIcon, title, message,
                    TRANS("Overwrite"), TRANS("Cancel"), this);
            }

            if (confirm) {
                bank_directory_ = file.getParentDirectory();
                save_bank(file);
            }
        }
        //[/UserButtonCode_btn_bank_save]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Main_Component::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    Parameter_Block &pb = *parameter_block_;
    Slider *sl = sliderThatWasMoved;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == sl_tune12.get())
    {
        //[UserSliderCode_sl_tune12] -- add your slider handling code here..
        AudioParameterInt &p = *pb.p_tune12;
        p.beginChangeGesture();
        p = std::lround(sl->getValue());
        p.endChangeGesture();
        //[/UserSliderCode_sl_tune12]
    }
    else if (sliderThatWasMoved == sl_tune34.get())
    {
        //[UserSliderCode_sl_tune34] -- add your slider handling code here..
        AudioParameterInt &p = *pb.p_tune34;
        p.beginChangeGesture();
        p = std::lround(sl->getValue());
        p.endChangeGesture();
        //[/UserSliderCode_sl_tune34]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void Main_Component::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == cb_emulator.get())
    {
        //[UserComboBoxCode_cb_emulator] -- add your combo box handling code here..
        AdlplugAudioProcessor &proc = *proc_;
        std::unique_lock<std::mutex> lock = proc.acquire_player_nonrt();
        proc.set_chip_emulator_nonrt(comboBoxThatHasChanged->getSelectedId() - 1);
        //[/UserComboBoxCode_cb_emulator]
    }
    else if (comboBoxThatHasChanged == cb_program.get())
    {
        //[UserComboBoxCode_cb_program] -- add your combo box handling code here..
        int selection = comboBoxThatHasChanged->getSelectedId();

        trace("Select program by UI %s",
              program_selection_to_string(selection).toRawUTF8());

        if (selection != 0) {
            unsigned insno = ((unsigned)selection - 1) & 255;
            unsigned psid = ((unsigned)selection - 1) >> 8;

            unsigned channel = midichannel_;
            bool isdrum = is_percussion_channel(channel);
            if (isdrum && insno >= 128) {
                trace("Assign %s to percussion channel %u",
                      program_selection_to_string(selection).toRawUTF8(),
                      channel + 1);
                midiprogram_[channel] = (psid << 8) | insno;
                // percussion bank change LSB only
                send_program_change(channel, psid);
            }
            else if (!isdrum && insno < 128) {
                trace("Assign %s to melodic channel %u",
                      program_selection_to_string(selection).toRawUTF8(),
                      channel + 1);
                midiprogram_[channel] = (psid << 8) | insno;
                send_controller(channel, 0, psid >> 7);
                send_controller(channel, 32, psid & 127);
                send_program_change(channel, insno);
            }

            send_selection_update();
        }
        reload_selected_instrument(dontSendNotification);

        //[/UserComboBoxCode_cb_program]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void Main_Component::handleNoteOn(MidiKeyboardState *, int channel_, int note, float velocity)
{
    unsigned channel = (unsigned)(channel_ - 1);
    Simple_Fifo &queue = proc_->message_queue_for_ui();

    if (is_percussion_channel(channel))
        note = (midiprogram_[channel] - 128) & 127;

    Message_Header msghdr(User_Message::Midi, 3);
    Buffered_Message msg = write_message_retrying(queue, msghdr, std::chrono::milliseconds(1));
    msg.data[0] = channel | (0b1001u << 4);
    msg.data[1] = note;
    msg.data[2] = velocity * 127;
    finish_write_message(queue, msg);
}

void Main_Component::handleNoteOff(MidiKeyboardState *, int channel_, int note, float velocity)
{
    unsigned channel = (unsigned)(channel_ - 1);

    if (is_percussion_channel(channel))
        note = (midiprogram_[channel] - 128) & 127;

    Simple_Fifo &queue = proc_->message_queue_for_ui();
    Message_Header msghdr(User_Message::Midi, 3);
    Buffered_Message msg = write_message_retrying(queue, msghdr, std::chrono::milliseconds(1));
    msg.data[0] = channel | (0b1000u << 4);
    msg.data[1] = note;
    msg.data[2] = velocity * 127;
    finish_write_message(queue, msg);
}

void Main_Component::knob_value_changed(Knob *k)
{
    Parameter_Block &pb = *parameter_block_;

    if (k == kn_fb12.get()) {
        AudioParameterInt &p = *pb.p_fb12;
        p.beginChangeGesture();
        p = std::lround(k->value());
        p.endChangeGesture();
    }
    else if (k == kn_fb34.get()) {
        AudioParameterInt &p = *pb.p_fb34;
        p.beginChangeGesture();
        p = std::lround(k->value());
        p.endChangeGesture();
    }
}

void Main_Component::send_controller(unsigned channel, unsigned ctl, unsigned value)
{
    Simple_Fifo &queue = proc_->message_queue_for_ui();
    Message_Header msghdr(User_Message::Midi, 3);
    Buffered_Message msg = write_message_retrying(queue, msghdr, std::chrono::milliseconds(1));
    msg.data[0] = (channel & 15) | (0b1011u << 4);
    msg.data[1] = ctl & 127;
    msg.data[2] = value & 127;
    finish_write_message(queue, msg);
}

void Main_Component::send_program_change(unsigned channel, unsigned value)
{
    Simple_Fifo &queue = proc_->message_queue_for_ui();
    Message_Header msghdr(User_Message::Midi, 2);
    Buffered_Message msg = write_message_retrying(queue, msghdr, std::chrono::milliseconds(1));
    msg.data[0] = (channel & 15) | (0b1100u << 4);
    msg.data[1] = value & 127;
    finish_write_message(queue, msg);
}

bool Main_Component::is_percussion_channel(unsigned channel) const
{
    return channel == 9;
}

Instrument *Main_Component::find_instrument(uint32_t program, Instrument *if_not_found)
{
    uint32_t psid = program >> 8;
    auto it = instrument_map_.find(psid);
    if (it == instrument_map_.end())
        return if_not_found;
    return &it->second.ins[program & 255];
}

void Main_Component::reload_selected_instrument(NotificationType ntf)
{
    int selection = cb_program->getSelectedId();

    trace("Reload selected instrument %s", program_selection_to_string(selection).toRawUTF8());

    Instrument ins_empty, *ins = &ins_empty;
    if (selection != 0) {
        uint32_t program = (unsigned)selection - 1;
        ins = find_instrument(program, &ins_empty);
    }
    set_instrument_parameters(*ins, ntf);
}

void Main_Component::send_selection_update()
{
    int selection = cb_program->getSelectedId();

    unsigned insno = 0;
    unsigned psid = 0;
    if (selection != 0) {
        insno = ((unsigned)selection - 1) & 255;
        psid = ((unsigned)selection - 1) >> 8;
        trace("Send selection update %s",
              program_selection_to_string(selection).toRawUTF8());
    }
    else {
        trace("Send selection update 0:0:0 because of empty selection");
    }

    Simple_Fifo &queue = proc_->message_queue_for_ui();
    Message_Header msghdr(User_Message::SelectProgram, sizeof(Messages::User::SelectProgram));
    Buffered_Message msg = write_message_retrying(queue, msghdr, std::chrono::milliseconds(1));
    auto &body = *(Messages::User::SelectProgram *)msg.data;
    body.bank = Bank_Id(psid >> 7, psid & 127, insno >= 128);
    body.program = insno & 127;
    finish_write_message(queue, msg);
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

   sl_tune12->setValue(ins.note_offset1, ntf);
   sl_tune34->setValue(ins.note_offset2, ntf);

   kn_fb12->set_value(ins.fb12(), ntf);
   kn_fb34->set_value(ins.fb34(), ntf);

   for (unsigned op = 0; op < 4; ++op) {
       Operator_Editor *oped = op_editors[op];
       oped->set_operator_parameters(ins, op, ntf);
       oped->set_operator_enabled(op < 2 || (ins.inst_flags & ADLMIDI_Ins_4op));
   }
}

void Main_Component::receive_bank_slots(const Messages::Fx::NotifyBankSlots &msg)
{
    unsigned count = msg.count;
    bool update = false;
    auto &imap = instrument_map_;

    trace("Receive %u bank slots", count);

    // delete bank entries not in the slots
    for (auto it = imap.begin(), end = imap.end(); it != end;) {
        uint32_t psid = it->first;
        bool found = false;
        for (unsigned slotno = 0; slotno < count && !found; ++slotno)
            found = msg.entry[slotno].bank.msb == (psid >> 7) &&
                msg.entry[slotno].bank.lsb == (psid & 127);
        if (!found)
            ++it;
        else {
            imap.erase(it++);
            update = true;
        }
    }

    // enable or disable instruments according to slots
    for (unsigned slotno = 0; slotno < count; ++slotno) {
        uint32_t psid = msg.entry[slotno].bank.pseudo_id();
        bool percussive = msg.entry[slotno].bank.percussive;
        Editor_Bank &e_bank = imap[psid];
        for (unsigned i = 0; i < 128; ++i) {
            unsigned insno = i + (percussive ? 128 : 0);
            bool isblank = !msg.entry[slotno].ins_mask[i];
            if (e_bank.ins[insno].blank() != isblank) {
                e_bank.ins[insno].blank(isblank);
                update = true;
            }
        }
    }

    if (update) {
        trace("Refresh choices because of received slots");
        update_instrument_choices();
    }
}

void Main_Component::receive_instrument(Bank_Id bank, unsigned pgm, const Instrument &ins)
{
    assert(pgm < 128);

    Editor_Bank *e_bank;
    bool update;
    unsigned insno = pgm + (bank.percussive ? 128 : 0);
    uint32_t psid = bank.pseudo_id();

    trace("Receive instrument %u:%u:%u", bank.msb, bank.lsb, insno);

    auto &instrument_map = instrument_map_;
    auto it = instrument_map.find(psid);
    if (it == instrument_map.end()) {
        if (ins.blank())
            return;
        it = instrument_map.insert({psid, Editor_Bank()}).first;
        e_bank = &it->second;
        e_bank->ins[insno] = ins;
        update = true;
    }
    else {
        e_bank = &it->second;
        update = !e_bank->ins[insno].equal_instrument(ins);
        if (update)
            e_bank->ins[insno] = ins;
    }

    bool empty_bank = ins.blank();
    for (unsigned i = 0; i < 256 && empty_bank; ++i)
        empty_bank = e_bank->ins[i].blank();

    if (empty_bank) {
        instrument_map.erase(it);
        update = true;
    }

    if (update) {
        trace("Refresh choices because of received instrument");
        update_instrument_choices();
    }
}

void Main_Component::update_instrument_choices()
{
    ComboBox &cb = *cb_program;
    cb.clear(dontSendNotification);
    PopupMenu *menu = cb.getRootMenu();

    auto &instrument_map = instrument_map_;
    auto it = instrument_map.begin();

    for (; it != instrument_map.end(); ++it) {
        uint32_t psid = it->first;
        Editor_Bank &e_bank = it->second;

        char bank_sid[64];
        std::sprintf(bank_sid, "Bank %03u:%03u", psid >> 7, psid & 127);

        e_bank.ins_menu.clear();
        for (unsigned i = 0; i < 256; ++i) {
            const Instrument &ins = e_bank.ins[i];
            if(ins.blank())
                continue;
            char ins_sid[64];

#pragma message("TODO instrument names from WOPL")
            const Midi_Program_Ex *ex = midi_db.find_ex(psid >> 7, psid & 127, i);
            const char *name = ex ?
                ex->name : (i < 128) ?
                midi_db.inst(i) : midi_db.perc(i & 127).name;

            std::sprintf(ins_sid, "%c%03u %s", "MP"[i >= 128], i & 127, name);
            uint32_t program = (psid << 8) + i;
            e_bank.ins_menu.addItem(program + 1, ins_sid);

            if (false)
                trace("Add choice %s %s",
                      program_selection_to_string(program + 1).toRawUTF8(),
                      ins_sid);
        }

        menu->addSubMenu(bank_sid, e_bank.ins_menu);
    }

    unsigned channel = midichannel_;
    set_program_selection(midiprogram_[channel] + 1, dontSendNotification);
    reload_selected_instrument(dontSendNotification);
    send_selection_update();
}

void Main_Component::set_program_selection(int selection, NotificationType ntf)
{
    trace("Change program selection %s to %s",
          program_selection_to_string(cb_program->getSelectedId()).toRawUTF8(),
          program_selection_to_string(selection).toRawUTF8());

    cb_program->setSelectedId(selection, ntf);

    trace("New program selection %s",
          program_selection_to_string(cb_program->getSelectedId()).toRawUTF8());
}

String Main_Component::program_selection_to_string(int selection)
{
    if (selection == 0)
        return "(nil)";

    unsigned insno = ((unsigned)selection - 1) & 255;
    unsigned psid = ((unsigned)selection - 1) >> 8;

    char buf[64];
    std::sprintf(buf, "%u:%u:%u", psid >> 7, psid & 127, insno);
    return buf;
}

void Main_Component::load_bank(const File &file)
{
    trace("Load from WOPL file: %s", file.getFullPathName().toRawUTF8());

    WOPLFile_Ptr wopl;
    std::unique_ptr<uint8_t[]> filedata;
    std::unique_ptr<FileInputStream> stream(file.createInputStream());
    uint64_t length;
    constexpr uint64_t max_length = 8 * 1024 * 1024;
    const char *error_title = "Error loading bank";

    if (stream->failedToOpen()) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The file could not be opened.");
        return;
    }

    if ((length = stream->getTotalLength()) >= max_length) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The selected file is too large to be valid.");
        return;
    }

    filedata.reset(new uint8_t[length]);
    if (stream->read(filedata.get(), length) != length) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The input operation has failed.");
        return;
    }

    wopl.reset(WOPL_LoadBankFromMem(filedata.get(), length, nullptr));
    if (!wopl) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The input file is not in WOPL format.");
        return;
    }

    Simple_Fifo &queue = proc_->message_queue_for_ui();
    auto send_bank = [&queue](const WOPLBank &bank, bool percussive) {
                         for (unsigned i = 0; i < 128; ++i) {
                             Message_Header hdr(User_Message::LoadInstrument, sizeof(Messages::User::LoadInstrument));
                             Buffered_Message msg = write_message_retrying(queue, hdr, std::chrono::milliseconds(1));
                             auto &data = *(Messages::User::LoadInstrument *)msg.data;
                             data.bank.lsb = bank.bank_midi_lsb;
                             data.bank.msb = bank.bank_midi_msb;
                             data.bank.percussive = percussive;
                             data.program = i;
                             data.instrument = Instrument::from_wopl(bank.ins[i]);
                             finish_write_message(queue, msg);
                         }
                     };
    edt_bank_name->setText(file.getFileNameWithoutExtension());
    edt_bank_name->setCaretPosition(0);

#pragma message("TODO clear existing banks")
    

    for (unsigned i = 0, n = wopl->banks_count_melodic; i < n; ++i)
        send_bank(wopl->banks_melodic[i], false);
    for (unsigned i = 0, n = wopl->banks_count_percussion; i < n; ++i)
        send_bank(wopl->banks_percussive[i], true);
}

void Main_Component::save_bank(const File &file)
{
    trace("Save to WOPL file: %s", file.getFullPathName().toRawUTF8());

#pragma message("TODO save WOPL file")

    const auto &instrument_map = instrument_map_;
    size_t max_bank_count = instrument_map.size();

    std::vector<WOPLBank> melo_array;
    std::vector<WOPLBank> drum_array;
    melo_array.reserve(max_bank_count);
    drum_array.reserve(max_bank_count);

    for (auto &entry : instrument_map) {
        uint32_t psid = entry.first;

        WOPLBank melo;
        WOPLBank drum;
        std::memset(&melo, 0, sizeof(WOPLBank));
        std::memset(&drum, 0, sizeof(WOPLBank));

        melo.bank_midi_msb = drum.bank_midi_msb = psid >> 7;
        melo.bank_midi_lsb = drum.bank_midi_lsb = psid & 127;

        size_t melo_count = 0;
        size_t drum_count = 0;
        for (size_t i = 0; i < 256; ++i) {
            WOPLInstrument ins = entry.second.ins[i].to_wopl();
            if (i < 128) {
                melo.ins[i] = ins;
                if (!(ins.inst_flags & WOPL_Ins_IsBlank))
                    ++melo_count;
            }
            else {
                drum.ins[i - 128] = ins;
                if (!(ins.inst_flags & WOPL_Ins_IsBlank))
                    ++drum_count;
            }
        }

        if (melo_count > 0)
            melo_array.push_back(melo);
        if (drum_count > 0)
            drum_array.push_back(drum);
    }

    WOPLFile wopl;
    wopl.version = 3;

#pragma message("TODO global chip flags, volume model")
    wopl.opl_flags = 0;
    wopl.volume_model = 0;

    wopl.banks_count_melodic = melo_array.size();
    wopl.banks_count_percussion = drum_array.size();
    wopl.banks_melodic = melo_array.data();
    wopl.banks_percussive = drum_array.data();

    size_t filesize = WOPL_CalculateBankFileSize(&wopl, wopl.version);
    std::unique_ptr<uint8_t> filedata(new uint8_t[filesize]);

    const char *error_title = "Error saving bank";

    if (WOPL_SaveBankToMem(&wopl, filedata.get(), filesize, wopl.version, 0) != 0) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The bank could not be converted to WOPL.");
        return;
    }

    std::unique_ptr<FileOutputStream> stream(file.createOutputStream());

    if (stream->failedToOpen()) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The file could not be opened.");
        return;
    }

    stream->setPosition(0);
    stream->truncate();
    stream->write(filedata.get(), filesize);
    stream->flush();

    if (!stream->getStatus()) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon, error_title, "The output operation has failed.");
        return;
    }
}

void Main_Component::on_change_midi_channel(unsigned channel)
{
    if (channel > 15)
        return;

    trace("Change MIDI channel to %u", channel + 1);

    midichannel_ = channel;
    lbl_channel->setText(String(channel + 1), dontSendNotification);
    midi_kb->setMidiChannel(channel + 1);
    set_program_selection(midiprogram_[channel] + 1, dontSendNotification);
    reload_selected_instrument(dontSendNotification);
    send_selection_update();
}

void Main_Component::vu_update()
{
    AdlplugAudioProcessor &proc = *proc_;
    vu_left->set_value(proc.vu_level(0));
    vu_right->set_value(proc.vu_level(1));
}

void Main_Component::cpu_load_update()
{
    AdlplugAudioProcessor &proc = *proc_;
    String text = String((int)(100.0 * proc.cpu_load())) + "%";
    lbl_cpu->setText(text, dontSendNotification);
}

void Main_Component::midi_activity_update()
{
    AdlplugAudioProcessor &proc = *proc_;
    for (unsigned i = 0; i < 16; ++i) {
        bool active = proc.midi_channel_note_count(i) > 0;
        unsigned columns = ind_midi_activity->columns();
        ind_midi_activity->set_value(i / columns, i % columns, active);
    }
}

void Main_Component::popup_about_dialog()
{
    DialogWindow::LaunchOptions dlgopts;
    dlgopts.dialogTitle = "About " JucePlugin_Name;
    dlgopts.content.set(new About_Component, true);
    dlgopts.componentToCentreAround = this;
    dlgopts.resizable = false;
    dlgopts.runModal();
}

void Main_Component::paintOverChildren(Graphics &g)
{
    Image img;
    Component *cpt;
    Rectangle<float> target;

    img = ImageCache::getFromMemory(BinaryData::emoji_u1f4c2_png, BinaryData::emoji_u1f4c2_pngSize);
    cpt = btn_bank_load.get();
    target = cpt->getBounds().toFloat();
    target = target.withSizeKeepingCentre(0.7f * target.getWidth(), 0.7f * target.getHeight());
    g.drawImage(img, target, RectanglePlacement::centred);

    img = ImageCache::getFromMemory(BinaryData::emoji_u1f4be_png, BinaryData::emoji_u1f4be_pngSize);
    cpt = btn_bank_save.get();
    target = cpt->getBounds().toFloat();
    target = target.withSizeKeepingCentre(0.7f * target.getWidth(), 0.7f * target.getHeight());
    g.drawImage(img, target, RectanglePlacement::centred);

    Component::paintOverChildren(g);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Main_Component" componentName=""
                 parentClasses="public Component, public MidiKeyboardStateListener, public Knob::Listener"
                 constructorParams="AdlplugAudioProcessor &amp;proc, Parameter_Block &amp;pb"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.33" fixedSize="0" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ff323e44">
    <TEXT pos="28 132 92 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 1-2"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="28 300 92 30" fill="solid: fff0f8ff" hasStroke="0" text="Operator 3-4"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="1"
          italic="0" justification="33" typefaceStyle="Bold"/>
    <TEXT pos="532 132 36 30" fill="solid: fff0f8ff" hasStroke="0" text="Tune"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="532 300 36 30" fill="solid: fff0f8ff" hasStroke="0" text="Tune"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="428 300 60 30" fill="solid: fff0f8ff" hasStroke="0" text="Feedback"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="428 132 60 30" fill="solid: fff0f8ff" hasStroke="0" text="Feedback"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <RECT pos="680 8 92 12" fill="solid: 8e989b" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
    <RECT pos="680 24 92 12" fill="solid: 8e989b" hasStroke="1" stroke="1, mitered, butt"
          strokeColour="solid: ff8e989b"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="423f2b5d9aff978c" memberName="ed_op2"
                    virtualName="" explicitFocusOrder="0" pos="16 160 352 128" class="Operator_Editor"
                    params="WOPL_OP_CARRIER1, pb"/>
  <TEXTBUTTON name="new button" id="333aa0ccccbfed24" memberName="btn_4op"
              virtualName="" explicitFocusOrder="0" pos="176 134 56 24" bgColOn="ff42a2c8"
              buttonText="4 op" connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="64d70bb49227f0d8" memberName="btn_pseudo4op"
              virtualName="" explicitFocusOrder="0" pos="232 134 56 24" bgColOn="ff42a2c8"
              buttonText="2x2 op" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="402f6ebfee1d48e6" memberName="btn_2op"
              virtualName="" explicitFocusOrder="0" pos="288 134 56 24" bgColOn="ff42a2c8"
              buttonText="2 op" connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b2cb238ae0314374" memberName="btn_fm12"
              virtualName="" explicitFocusOrder="0" pos="376 200 36 24" bgColOn="ff42a2c8"
              buttonText="FM" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f884f98bb82f318" memberName="btn_am12"
              virtualName="" explicitFocusOrder="0" pos="376 224 36 24" bgColOn="ff42a2c8"
              buttonText="AM" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="a00c5401e39a953e" memberName="ed_op1"
                    virtualName="" explicitFocusOrder="0" pos="421 160 352 128" class="Operator_Editor"
                    params="WOPL_OP_MODULATOR1, pb"/>
  <GENERICCOMPONENT name="new component" id="b7424f0838e48a08" memberName="ed_op4"
                    virtualName="" explicitFocusOrder="0" pos="16 328 352 128" class="Operator_Editor"
                    params="WOPL_OP_CARRIER2, pb"/>
  <TEXTBUTTON name="new button" id="6c84b2cc5c27a17f" memberName="btn_fm34"
              virtualName="" explicitFocusOrder="0" pos="376 368 36 24" bgColOn="ff42a2c8"
              buttonText="FM" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c55383265bc18fb0" memberName="btn_am34"
              virtualName="" explicitFocusOrder="0" pos="376 392 36 24" bgColOn="ff42a2c8"
              buttonText="AM" connectedEdges="4" needsCallback="1" radioGroupId="0"/>
  <GENERICCOMPONENT name="new component" id="4bf73df293534890" memberName="ed_op3"
                    virtualName="" explicitFocusOrder="0" pos="421 328 352 128" class="Operator_Editor"
                    params="WOPL_OP_MODULATOR2, pb"/>
  <SLIDER name="new slider" id="9cd7cc232d55ac8a" memberName="sl_tune12"
          virtualName="" explicitFocusOrder="0" pos="568 136 150 24" min="-127.0"
          max="127.0" int="0.0" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="36" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="314f46c5e90647" memberName="sl_tune34"
          virtualName="" explicitFocusOrder="0" pos="568 304 150 24" min="-127.0"
          max="127.0" int="0.0" style="LinearHorizontal" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="36" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GENERICCOMPONENT name="new component" id="8e0915367ccd00d3" memberName="kn_fb12"
                    virtualName="" explicitFocusOrder="0" pos="496 128 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="59510781248f1393" memberName="kn_fb34"
                    virtualName="" explicitFocusOrder="0" pos="496 296 32 32" class="Styled_Knob_DefaultSmall"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="4d4a20a681c7e721" memberName="midi_kb"
                    virtualName="" explicitFocusOrder="0" pos="16 480 757 56" class="MidiKeyboardComponent"
                    params="midi_kb_state_, MidiKeyboardComponent::horizontalKeyboard"/>
  <IMAGEBUTTON name="new button" id="1c21a98bd6493eb8" memberName="btn_about"
               virtualName="" explicitFocusOrder="0" pos="16 8 232 40" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="logo_png" opacityNormal="1.0" colourNormal="0"
               resourceOver="" opacityOver="1.0" colourOver="0" resourceDown=""
               opacityDown="1.0" colourDown="0"/>
  <LABEL name="new label" id="5628e38331cdf4ec" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="480 40 48 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Chips" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="be41a2433f6d1f03" memberName="lbl_num_chips"
         virtualName="" explicitFocusOrder="0" pos="528 40 32 24" textCol="fff0f8ff"
         outlineCol="ff8e989b" edTextCol="ff000000" edBkgCol="0" labelText="100"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="cbf65c7349d1d293" memberName="btn_less_chips"
              virtualName="" explicitFocusOrder="0" pos="568 40 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="6fc5dc04c6c5d6b9" memberName="btn_more_chips"
              virtualName="" explicitFocusOrder="0" pos="591 40 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <COMBOBOX name="new combo box" id="8f8a11ca0d94343f" memberName="cb_emulator"
            virtualName="" explicitFocusOrder="0" pos="272 40 198 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="9fd95e8efc5b0fe1" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="280 8 280 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="FM synthesizer with YMF262 chip emulation"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <GENERICCOMPONENT name="new component" id="8c38ad0cbcf109f7" memberName="vu_left"
                    virtualName="" explicitFocusOrder="0" pos="680 8 92 12" class="Vu_Meter"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="16bc493366b76ca5" memberName="vu_right"
                    virtualName="" explicitFocusOrder="0" pos="680 24 92 12" class="Vu_Meter"
                    params=""/>
  <LABEL name="new label" id="bda2c045916cddf3" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="684 40 40 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="CPU" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="2067136cf573bd1a" memberName="lbl_cpu" virtualName=""
         explicitFocusOrder="0" pos="724 40 48 24" textCol="fff0f8ff"
         outlineCol="ff8e989b" edTextCol="ff000000" edBkgCol="0" labelText="100%"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <GENERICCOMPONENT name="new component" id="b87acb622e25d16e" memberName="ind_midi_activity"
                    virtualName="" explicitFocusOrder="0" pos="568 8 102 28" class="Indicator_NxM"
                    params="2, 8"/>
  <TEXTBUTTON name="new button" id="ec00354399da2ced" memberName="btn_panic"
              virtualName="" explicitFocusOrder="0" pos="622 40 48 24" bgColOn="ff42a2c8"
              buttonText="Panic" connectedEdges="8" needsCallback="1" radioGroupId="0"/>
  <TEXTEDITOR name="new text editor" id="62544efea1101020" memberName="edt_bank_name"
              virtualName="" explicitFocusOrder="0" pos="16 80 208 24" outlinecol="ff8e989b"
              initialText="Bank name" multiline="0" retKeyStartsLine="0" readonly="0"
              scrollbars="1" caret="1" popupmenu="1"/>
  <COMBOBOX name="new combo box" id="396a835342f6b630" memberName="cb_program"
            virtualName="" explicitFocusOrder="0" pos="312 80 280 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="ac425a00ee881383" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="620 80 60 24" textCol="fff0f8ff"
         edTextCol="ff000000" edBkgCol="0" labelText="Channel" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="new label" id="1c00855603b30379" memberName="lbl_channel"
         virtualName="" explicitFocusOrder="0" pos="684 80 32 24" textCol="fff0f8ff"
         outlineCol="ff8e989b" edTextCol="ff000000" edBkgCol="0" labelText="1"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="6ba2e6591ffa67ab" memberName="btn_prev_channel"
              virtualName="" explicitFocusOrder="0" pos="724 80 23 24" buttonText="&lt;"
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="f058380e7bbf63b0" memberName="btn_next_channel"
              virtualName="" explicitFocusOrder="0" pos="747 80 23 24" buttonText="&gt;"
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="c78c3c0db0e0be9a" memberName="btn_bank_load"
              virtualName="" explicitFocusOrder="0" pos="232 80 23 24" buttonText=""
              connectedEdges="2" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="fd2ff0a2438003e5" memberName="btn_bank_save"
              virtualName="" explicitFocusOrder="0" pos="255 80 23 24" buttonText=""
              connectedEdges="1" needsCallback="1" radioGroupId="0"/>
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
//[/EndFile]
