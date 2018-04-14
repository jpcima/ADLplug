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
    logo_ = ImageFileFormat::loadFrom(BinaryData::logo_png, BinaryData::logo_pngSize);

    for (int i = 0; i < 4; ++i) {
        Operator_Editor *oped = oped_[i] = new Operator_Editor;
        addAndMakeVisible(oped);
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
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

    Rectangle<int> bounds = getLocalBounds();

    //
    const Image &logo = logo_;
    bounds.removeFromTop(4);
    Rectangle<int> toprow = bounds.removeFromTop(logo.getHeight());
    toprow.removeFromLeft(8);
    Point<int> logo_pos = toprow.getTopLeft();
    g.drawImageAt(logo, logo_pos.getX(), logo_pos.getY());
    toprow.removeFromLeft(logo.getWidth() + 8);
    toprow.removeFromRight(24);
    g.setColour(Colours::white);
    g.setFont(15.0f);
    g.drawText("FM synthesizer with YMF262chip emulation", toprow, Justification::centred);
}

void AdlplugAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    Rectangle<int> bounds = getLocalBounds();

#warning XXX test only
    bounds.removeFromTop(100);
    for (int i = 0; i < 4; ++i) {
        Operator_Editor *oped = oped_[i];
        Rectangle<int> oped_bounds = bounds.removeFromTop(100);
        // fprintf(stderr, "%s\n", oped_bounds.toString().toRawUTF8());
        oped->setBounds(oped_bounds);
        bounds.removeFromTop(8);
    }
}
