//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin_editor.h"
#include "plugin_processor.h"

//==============================================================================
AdlplugAudioProcessorEditor::AdlplugAudioProcessorEditor(AdlplugAudioProcessor &p)
    : AudioProcessorEditor(&p) , processor(p)
{
#warning XXX test only
    knob_ = new Knob;
    addAndMakeVisible(knob_);
    knob_->load_skin_data(BinaryData::knobskin_png, BinaryData::knobskin_pngSize, 64);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
}

AdlplugAudioProcessorEditor::~AdlplugAudioProcessorEditor()
{
}

//==============================================================================
void AdlplugAudioProcessorEditor::paint(Graphics &g)
{
    LookAndFeel &lnf = getLookAndFeel();

    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(lnf.findColour(ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void AdlplugAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    Rectangle<int> bounds = getLocalBounds();

#warning XXX test only
    Knob *knob = knob_;
    Rectangle<int> knob_bounds = Rectangle<int>(bounds.getTopLeft(), {100, 100});
    knob->setBounds(knob_bounds);
}
