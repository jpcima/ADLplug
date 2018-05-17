//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "ui/main_component.h"
#include "ui/look_and_feel.h"
#include "plugin_editor.h"
#include "plugin_processor.h"

//==============================================================================
AdlplugAudioProcessorEditor::AdlplugAudioProcessorEditor(AdlplugAudioProcessor &p)
    : AudioProcessorEditor(&p)
{
    Custom_Look_And_Feel *lnf = new Custom_Look_And_Feel;
    lnf_.reset(lnf);
    LookAndFeel::setDefaultLookAndFeel(lnf);

    Main_Component *main = new Main_Component(p);
    main_.reset(main);
    addAndMakeVisible(main);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(main->getWidth(), main->getHeight());
}

AdlplugAudioProcessorEditor::~AdlplugAudioProcessorEditor()
{
    Styled_Knobs::release_skins();
}

//==============================================================================
void AdlplugAudioProcessorEditor::paint(Graphics &g)
{
    LookAndFeel &lnf = getLookAndFeel();

    // (Our component is opaque, so we must completely fill the background with a
    // solid colour)
    g.fillAll(lnf.findColour(ResizableWindow::backgroundColourId));
}

void AdlplugAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    Rectangle<int> bounds = getLocalBounds();

    Main_Component *main = main_.get();
    main->setBounds(bounds);
}
