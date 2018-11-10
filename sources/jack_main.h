//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "JuceHeader.h"
#if defined(ADLPLUG_USE_NSM)
#include <nsm.h>
#endif
#include <jack/jack.h>
#include <jack/midiport.h>
#include <memory>
class AdlplugAudioProcessor;

#if defined(ADLPLUG_USE_NSM)
struct Nsm_Deleter {
    void operator()(nsm_client_t *x) { nsm_free(x); }
};
typedef std::unique_ptr<nsm_client_t, Nsm_Deleter> nsm_client_u;
#endif

class Application_Window : public DocumentWindow
{
public:
    explicit Application_Window(const String &name);
    void closeButtonPressed() override;
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
    bool setup_audio(const char *client_name);
    bool start(bool autoconnect);

#if defined(ADLPLUG_USE_NSM)
    bool initialise_session(const char *nsm_url);
    static void session_log(void *user_data, const char *fmt, ...);
    static int session_open(const char *path, const char *display_name, const char *client_id, char **out_msg, void *user_data);
    static int session_save(char **out_msg, void *user_data);
#endif

    static int process(jack_nframes_t nframes, void *user_data);

    std::unique_ptr<AdlplugAudioProcessor> processor_;
    std::unique_ptr<Application_Window> window_;

    struct jack_client_deleter {
        void operator()(jack_client_t *c) const { jack_client_close(c); }
    };
    std::unique_ptr<jack_client_t, jack_client_deleter> client_;
    jack_port_t *midiport_ = nullptr;
    jack_port_t *outport_[2] = {nullptr, nullptr};

#if defined(ADLPLUG_USE_NSM)
    nsm_client_u nsm_;
    std::string nsm_path_;
    std::unique_ptr<Timer> nsm_timer_;
#endif
};
