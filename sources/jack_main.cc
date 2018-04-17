//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "plugin_processor.h"
#include "plugin_editor.h"
#include <memory>
#include <jack/jack.h>
#include <jack/midiport.h>

class Application_Window : public DocumentWindow
{
public:
    explicit Application_Window(const String &name)
        : DocumentWindow(
            name,
            LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton|DocumentWindow::closeButton) {}

    void closeButtonPressed() override
        { JUCEApplicationBase::quit(); }
};

class Application_Jack : public JUCEApplication
{
public:
    void initialise(const String &args) override;
    void shutdown() override;

    const String getApplicationName() override
        { return JucePlugin_Name; }
    const String getApplicationVersion() override
        { return JucePlugin_VersionString; }

private:
    static int process(jack_nframes_t nframes, void *user_data);

    std::unique_ptr<AdlplugAudioProcessor> processor_;
    std::unique_ptr<AdlplugAudioProcessorEditor> editor_;
    Application_Window *window_ = nullptr;

    struct jack_client_deleter {
        void operator()(jack_client_t *c) const { jack_client_close(c); }
    };

    std::unique_ptr<jack_client_t, jack_client_deleter> client_;
    jack_port_t *midiport_ = nullptr;
    jack_port_t *outport_[2] = {nullptr, nullptr};
};

void Application_Jack::initialise(const String &args)
{
    jack_client_t *client = jack_client_open("ADLplug", JackNoStartServer, nullptr);
    if (!client)
        throw std::runtime_error("error creating Jack client");
    client_.reset(client);

    midiport_ = jack_port_register(client, "midi", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput|JackPortIsTerminal, 0);
    outport_[0] = jack_port_register(client, "left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput|JackPortIsTerminal, 0);
    outport_[1] = jack_port_register(client, "right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput|JackPortIsTerminal, 0);

    if (!midiport_ || !outport_[0] || !outport_[1])
        throw std::runtime_error("error creating Jack ports");

    jack_nframes_t buffer_size = jack_get_buffer_size(client);
    unsigned sample_rate = jack_get_sample_rate(client);

    AdlplugAudioProcessor *processor = new AdlplugAudioProcessor;
    processor_.reset(processor);
    processor->prepareToPlay(sample_rate, buffer_size);
    processor->setPlayConfigDetails(0, 2, sample_rate, buffer_size);

    AdlplugAudioProcessorEditor *editor = new AdlplugAudioProcessorEditor(*processor);
    editor_.reset(editor);

    jack_set_process_callback(client, &process, this);

    if (jack_activate(client) != 0)
        throw std::runtime_error("error activating Jack client");

    Application_Window *window = new Application_Window(
        getApplicationName() + " [jack:" + (const char *)jack_get_client_name(client) + "]");
    window_ = window;
    window->setContentOwned(editor, true);
    window->setSize(editor->getWidth(), editor->getHeight());
    window->setVisible(true);
}

void Application_Jack::shutdown()
{
    window_ = nullptr;
    client_.reset();
}

int Application_Jack::process(jack_nframes_t nframes, void *user_data)
{
    Application_Jack *self = reinterpret_cast<Application_Jack *>(user_data);

    void *midi = jack_port_get_buffer(self->midiport_, nframes);
    float *outputs[2] = {
        (float *)jack_port_get_buffer(self->outport_[0], nframes),
        (float *)jack_port_get_buffer(self->outport_[1], nframes),
    };

    struct Midi_Cb_Context {
        void *port_buffer;
        jack_nframes_t index;
        jack_nframes_t nframes;
    };

    auto midi_cb = [](void *user_data) -> std::pair<const uint8_t *, unsigned> {
        Midi_Cb_Context &ctx = *reinterpret_cast<Midi_Cb_Context *>(user_data);
        jack_midi_event_t event;
        if (jack_midi_event_get(&event, ctx.port_buffer, ctx.index++) != 0)
            return {nullptr, 0};
        return {event.buffer, event.size};
    };

    Midi_Cb_Context midi_ctx = { midi, 0, nframes };
    self->processor_->process(outputs, nframes, +midi_cb, &midi_ctx);
    return 0;
}

JUCE_CREATE_APPLICATION_DEFINE(Application_Jack);
JUCE_MAIN_FUNCTION_DEFINITION;
