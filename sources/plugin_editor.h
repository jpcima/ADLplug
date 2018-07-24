//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
class AdlplugAudioProcessor;
class Custom_Look_And_Feel;
class Main_Component;
struct Parameter_Block;

//==============================================================================
/**
 */
class AdlplugAudioProcessorEditor : public AudioProcessorEditor {
public:
    AdlplugAudioProcessorEditor(AdlplugAudioProcessor &, Parameter_Block &);
    ~AdlplugAudioProcessorEditor();

    //==========================================================================
    void paint(Graphics &) override;
    void resized() override;

private:
    AdlplugAudioProcessor &proc_;
    std::unique_ptr<Custom_Look_And_Feel> lnf_;
    std::unique_ptr<Main_Component> main_;
    std::unique_ptr<TooltipWindow> tooltip_window_;

    class Notification_Timer;
    std::unique_ptr<Notification_Timer> notification_timer_;

    void process_notifications();
    void discard_notifications();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AdlplugAudioProcessorEditor)
};
