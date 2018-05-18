//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin_editor.h"
#include "plugin_processor.h"
#include "messages.h"
#include "ui/main_component.h"
#include "ui/look_and_feel.h"

class AdlplugAudioProcessorEditor::Notification_Timer : public Timer {
public:
    Notification_Timer(AdlplugAudioProcessorEditor &ed) : ed_(ed) {}
    void timerCallback() override { ed_.process_notifications(); }
private:
    AdlplugAudioProcessorEditor &ed_;
};

//==============================================================================
AdlplugAudioProcessorEditor::AdlplugAudioProcessorEditor(AdlplugAudioProcessor &p)
    : AudioProcessorEditor(&p), proc_(p)
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

    Notification_Timer *timer = new Notification_Timer(*this);
    notification_timer_.reset(timer);
    timer->startTimer(10);
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

void AdlplugAudioProcessorEditor::process_notifications()
{
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_ui();

    while (Buffered_Message msg = read_message(queue)) {
        Fx_Message tag = (Fx_Message)msg.header->tag;

        switch (tag) {
            case Fx_Message::NotifyInstrument: {
                auto &body = *(const Messages::Fx::NotifyInstrument *)msg.data;

                // fprintf(stderr, "Notify instrument %u:%u:%u:%u\n",
                //         body.bank.percussive, body.bank.msb, body.bank.lsb,
                //         body.program);

                break;
            }
        }
        finish_read_message(queue, msg);
    }
}
