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

    Main_Component *main = main_ = new Main_Component;
    addAndMakeVisible(main);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(main->getWidth(), main->getHeight());
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
    g.drawText("FM synthesizer with YMF262 chip emulation", toprow, Justification::centred);
}

void AdlplugAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    Rectangle<int> bounds = getLocalBounds();

    Main_Component *main = main_;
    main->setBounds(bounds);
}
