//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin_editor.h"
#include "plugin_processor.h"
#include "messages.h"
#include "ui/main_component.h"
#include "ui/look_and_feel.h"
#include "ui/components/algorithm_help.h"
#include "utility/functional_timer.h"
#include <cassert>

AdlplugAudioProcessorEditor::AdlplugAudioProcessorEditor(AdlplugAudioProcessor &p, Parameter_Block &pb)
    : AudioProcessorEditor(&p), proc_(p)
{
    Custom_Look_And_Feel *lnf = new Custom_Look_And_Feel;
    lnf_.reset(lnf);
    LookAndFeel::setDefaultLookAndFeel(lnf);

    TooltipWindow *tooltip_window = new TooltipWindow(this);
    tooltip_window_.reset(tooltip_window);

    Algorithm_Help *algorithm_help = new Algorithm_Help;
    lnf->add_custom_tooltip("Algorithms", algorithm_help, true);

    Main_Component *main = new Main_Component(p, pb);
    main_.reset(main);
    addAndMakeVisible(main);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(main->getWidth(), main->getHeight());

    discard_notifications();
    Timer *timer = Functional_Timer::create([this]() { process_notifications(); });
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
    Main_Component *main = main_.get();
    Simple_Fifo &queue = proc.message_queue_to_ui();

    while (Buffered_Message msg = read_message(queue)) {
        Fx_Message tag = (Fx_Message)msg.header->tag;

        switch (tag) {
        case Fx_Message::NotifyBankSlots: {
            auto &body = *(const Messages::Fx::NotifyBankSlots *)msg.data;
            main->receive_bank_slots(body);
            break;
        }
        case Fx_Message::NotifyGlobalParameters: {
            auto &body = *(const Messages::Fx::NotifyGlobalParameters *)msg.data;
            main->receive_global_parameters(body);
            break;
        }
        case Fx_Message::NotifyInstrument: {
            auto &body = *(const Messages::Fx::NotifyInstrument *)msg.data;
            main->receive_instrument(body.bank, body.program, body.instrument);
            break;
        }
        default:
            assert(false);
        }
        finish_read_message(queue, msg);
    }
}

void AdlplugAudioProcessorEditor::discard_notifications()
{
    AdlplugAudioProcessor &proc = proc_;
    Simple_Fifo &queue = proc.message_queue_to_ui();
    while (Buffered_Message msg = read_message(queue))
        finish_read_message(queue, msg);
}
