//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
class AdlplugAudioProcessor;
class Custom_Look_And_Feel;
class Main_Component;

//==============================================================================
/**
 */
class AdlplugAudioProcessorEditor : public AudioProcessorEditor {
public:
    AdlplugAudioProcessorEditor(AdlplugAudioProcessor &);
    ~AdlplugAudioProcessorEditor();

    //==========================================================================
    void paint(Graphics &) override;
    void resized() override;

private:
    std::unique_ptr<Custom_Look_And_Feel> lnf_;
    std::unique_ptr<Main_Component> main_;
    TooltipWindow tooltip_window_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessorEditor)
};
