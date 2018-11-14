//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "jack_main.h"
#include "plugin_processor.h"
#include "plugin_editor.h"
#include "plugin_version.h"
#include "utility/midi.h"
#include "utility/functional_timer.h"
#include <string.h>
#include <sys/mman.h>
extern AudioProcessor *JUCE_CALLTYPE createPluginFilter();

Application_Window::Application_Window(const String &name)
    : DocumentWindow(
        name,
        LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
        DocumentWindow::minimiseButton|DocumentWindow::closeButton,
        false)
{
#if JUCE_LINUX
    setUsingNativeTitleBar(true);
#endif
}

void Application_Window::closeButtonPressed()
{
#if defined(ADLPLUG_USE_NSM)
    Application_Jack *app = Application_Jack::getInstance();
    if (app->under_session())
        app->set_gui_visible(false);
    else
#endif
    {
        JUCEApplicationBase::quit();
    }
}

void Application_Jack::initialise(const String &args)
{
#if defined(ADLPLUG_USE_NSM)
    if (const char *nsm_url = getenv("NSM_URL")) {
        if (!initialise_session(nsm_url)) {
            setApplicationReturnValue(1);
            return quit();
        }
        return;
    }
#endif

    const StringArray argv = getCommandLineParameterArray();
    std::vector<const String *> optargs;
    bool arg_autoconnect = false;
    for (unsigned argi = 0, argc = argv.size(); argi < argc; ++argi) {
        const String &arg = argv[argi];
        if (arg == "-h") {
            fprintf(
                stderr,
                "Usage:\n" "   %s [-a]\n\n" "Options:\n"
                "   -h    show the help message\n"
                "   -a    connect to system ports\n"
                "   -v    display the version number\n",
                JucePlugin_Name);
            return quit();
        }
        if (arg == "-a")
            arg_autoconnect = true;
        else if (arg == "-v" || arg == "--version") {
            printf("%s\n", ADLplug_SemVer);
            return quit();
        } else if (arg == "--") {
            while (argi + 1 < argc)
                optargs.push_back(&argv[++argi]);
        }
        else if (!arg.isEmpty() && arg[0] == '-') {
            fprintf(stderr, "Invalid argument: '%s'\n", arg.toRawUTF8());
            setApplicationReturnValue(1);
            return quit();
        }
        else {
            optargs.push_back(&arg);
        }
    }
    if (!optargs.empty()) {
        fprintf(stderr, "Invalid arguments:");
        for (const String *arg : optargs)
            fprintf(stderr, " '%s'", arg->toRawUTF8());
        fputc('\n', stderr);
        setApplicationReturnValue(1);
        return quit();
    }

    if (!setup_audio(JucePlugin_Name)) {
        setApplicationReturnValue(1);
        return quit();
    }

    if (!start(arg_autoconnect)) {
        setApplicationReturnValue(1);
        return quit();
    }
}

void Application_Jack::shutdown()
{
    client_.reset();
    window_.reset();
    processor_.reset();
}

bool Application_Jack::setup_audio(const char *client_name)
{
    client_.reset();

    if (mlockall(MCL_CURRENT|MCL_FUTURE) != 0)
        fprintf(stderr, "could not lock memory\n");

    jack_client_t *client = jack_client_open(client_name, JackNoStartServer, nullptr);
    if (!client) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon,
            TRANS("JACK Audio"),
            TRANS("Could not create a new audio client.\n\n"
                  "Please start the JACK server and try again."));
        return false;
    }
    client_.reset(client);

    midiport_ = jack_port_register(client, "midi", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput|JackPortIsTerminal, 0);
    outport_[0] = jack_port_register(client, "left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput|JackPortIsTerminal, 0);
    outport_[1] = jack_port_register(client, "right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput|JackPortIsTerminal, 0);

    if (!midiport_ || !outport_[0] || !outport_[1]) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon,
            TRANS("JACK Audio"),
            TRANS("Could not create the synthesizer ports."));
        client_.reset();
        return false;
    }

    jack_set_process_callback(client, &process, this);

    return true;
}

bool Application_Jack::start(bool autoconnect)
{
    jack_client_t *client = client_.get();
    jack_nframes_t buffer_size = jack_get_buffer_size(client);
    unsigned sample_rate = jack_get_sample_rate(client);

    AdlplugAudioProcessor *processor = static_cast<AdlplugAudioProcessor *>(createPluginFilter());
    processor_.reset(processor);
    processor->prepareToPlay(sample_rate, buffer_size);
    processor->setPlayConfigDetails(0, 2, sample_rate, buffer_size);

    if (jack_activate(client) != 0) {
        AlertWindow::showMessageBox(
            AlertWindow::WarningIcon,
            TRANS("JACK Audio"),
            TRANS("Could not start the synthesizer client."));
        return false;
    }

    if (autoconnect) {
        std::unique_ptr<const char *[], void(*)(void *)> ports(
            jack_get_ports(client, nullptr, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput),
            &free);
        if (ports) {
            // identify the default device (the first one)
            std::string device;
            if (ports[0]) {
                if (const char *separator = strchr(ports[0], ':'))
                    device.assign(ports[0], separator);
            }
            if (!device.empty()) {
                unsigned nports = 0;
                for (const char **p = ports.get(), *port; nports < 2 && (port = *p); ++p) {
                    bool is_of_device = strlen(port) > device.size() &&
                        port[device.size()] == ':' &&
                        !memcmp(port, device.data(), device.size());
                    if (is_of_device)
                        jack_connect(client, jack_port_name(outport_[nports++]), port);
                }
            }
        }
    }

    Application_Window *window = window_.get();
    String window_title = getApplicationName() + " [jack:" + (const char *)jack_get_client_name(client) + "]";

    if (window)
        window->setName(window_title);
    else {
        window = new Application_Window(window_title);
        window_.reset(window);

        bool visible = true;
#if defined(ADLPLUG_USE_NSM)
        if (under_session()) {
            const CSimpleIniA &conf = nsm_session_conf_;
            visible = conf.GetBoolValue("gui", "window-visible", false);
        }
#endif
        window->setVisible(visible);
        window->addToDesktop();

#if defined(ADLPLUG_USE_NSM)
        if (under_session()) {
            nsm_client_t *nsm = nsm_.get();
            (visible ? nsm_send_gui_is_shown : nsm_send_gui_is_hidden)(nsm);
        }
#endif
    }

    AdlplugAudioProcessorEditor *editor = static_cast<AdlplugAudioProcessorEditor *>(processor->createEditor());
    window->setContentOwned(editor, true);

    return true;
}

int Application_Jack::process(jack_nframes_t nframes, void *user_data)
{
    Application_Jack *self = reinterpret_cast<Application_Jack *>(user_data);
    AdlplugAudioProcessor *processor = self->processor_.get();

    void *midi = jack_port_get_buffer(self->midiport_, nframes);
    float *outputs[2] = {
        (float *)jack_port_get_buffer(self->outport_[0], nframes),
        (float *)jack_port_get_buffer(self->outport_[1], nframes),
    };

    const ScopedLock lock(processor->getCallbackLock());
    if (processor->isSuspended()) {
        for (float *output : outputs)
            memset(output, 0, nframes * sizeof(float));
        return 0;
    }

    struct Midi_Cb_Context {
        void *port_buffer;
        jack_nframes_t index;
        jack_nframes_t nframes;
    };

    auto midi_cb = [](void *user_data) -> Midi_Input_Message {
        Midi_Cb_Context &ctx = *reinterpret_cast<Midi_Cb_Context *>(user_data);
        jack_midi_event_t event;
        return (jack_midi_event_get(&event, ctx.port_buffer, ctx.index++) == 0) ?
            Midi_Input_Message(event.buffer, event.size, event.time) : Midi_Input_Message();
    };
    Midi_Cb_Context midi_ctx = { midi, 0, nframes };

    Midi_Input_Source midi_source(+midi_cb, &midi_ctx);
    processor->process(outputs, nframes, midi_source);

    return 0;
}

#if defined(ADLPLUG_USE_NSM)
bool Application_Jack::initialise_session(const char *nsm_url)
{
    nsm_client_t *nsm = nsm_new();
    nsm_.reset(nsm);
    nsm_set_log_callback(nsm, &session_log, this);
    nsm_set_open_callback(nsm, &session_open, this);
    nsm_set_save_callback(nsm, &session_save, this);
    nsm_set_show_optional_gui_callback(nsm, &show_optional_gui, this);
    nsm_set_hide_optional_gui_callback(nsm, &hide_optional_gui, this);
    if (nsm_init(nsm, nsm_url) != 0) {
        fprintf(stderr, "error initializing session management.\n");
        return false;
    }

    String progname = File::getSpecialLocation(File::currentApplicationFile).getFileName();
    nsm_send_announce(nsm, JucePlugin_Name, ":switch:optional-gui:", progname.toRawUTF8());
    nsm_send_gui_is_hidden(nsm);

    Timer *nsm_timer = Functional_Timer::create(
        [nsm]() { nsm_check_nowait(nsm); });
    nsm_timer_.reset(nsm_timer);
    nsm_timer->startTimer(50);

    return true;
}

bool Application_Jack::under_session() const
{
    return nsm_ != nullptr;
}

void Application_Jack::load_session_conf()
{
    std::string path = nsm_path_  + "/session.conf";
    CSimpleIniA &conf = nsm_session_conf_;
    conf.LoadFile(path.c_str());
}

void Application_Jack::save_session_conf() const
{
    std::string path = nsm_path_  + "/session.conf";
    const CSimpleIniA &conf = nsm_session_conf_;
    conf.SaveFile(path.c_str());
}

void Application_Jack::session_log(void *user_data, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

int Application_Jack::session_open(const char *path, const char *display_name, const char *client_id, char **out_msg, void *user_data)
{
    Application_Jack *self = (Application_Jack *)user_data;

    self->nsm_path_ = path;
    File(path).createDirectory();

    self->load_session_conf();

    if (!self->setup_audio(client_id))
        return 1;

    if (!self->start(false))
        return 1;

    AdlplugAudioProcessor &processor = *self->processor_;

    File statefile(String(path) + "/state.dat");
    MemoryBlock state;
    if (statefile.loadFileAsData(state))
        processor.setStateInformation(state.getData(), state.getSize());

    return 0;
}

int Application_Jack::session_save(char **out_msg, void *user_data)
{
    Application_Jack *self = (Application_Jack *)user_data;
    AdlplugAudioProcessor *processor = self->processor_.get();

    if (!processor)
        return 1;

    MemoryBlock state;
    processor->getStateInformation(state);

    File statefile(self->nsm_path_ + "/state.dat");
    statefile.replaceWithData(state.getData(), state.getSize());

    self->save_session_conf();

    return 0;
}

void Application_Jack::show_optional_gui(void *user_data)
{
    Application_Jack *self = (Application_Jack *)user_data;
    self->set_gui_visible(true);
}

void Application_Jack::hide_optional_gui(void *user_data)
{
    Application_Jack *self = (Application_Jack *)user_data;
    self->set_gui_visible(false);
}

void Application_Jack::set_gui_visible(bool visible)
{
    Application_Window *window = window_.get();
    if (!window)
        return;

    window->setVisible(visible);

    CSimpleIniA &conf = nsm_session_conf_;
    conf.SetBoolValue("gui", "window-visible", visible);

    nsm_client_t *nsm = nsm_.get();
    (visible ? nsm_send_gui_is_shown : nsm_send_gui_is_hidden)(nsm);
}
#endif

JUCE_CREATE_APPLICATION_DEFINE(Application_Jack);
JUCE_MAIN_FUNCTION_DEFINITION;
