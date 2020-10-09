//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : AudioHost
// Filename    : public.sdk/samples/vst-hosting/audiohost/source/media/jack/jackclient.cpp
// Created by  : Steinberg 09.2016
// Description : Audio Host Example for VST 3 using Jack
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "public.sdk/samples/vst-hosting/audiohost/source/media/imediaserver.h"

#include <cassert>

//! Workaround for Jack on Windows
#if defined(SMTG_OS_WINDOWS) && defined(_STDINT)
#define _STDINT_H
#endif

#include <jack/jack.h>
#include <jack/midiport.h>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

static const int kJackSuccess = 0;
//------------------------------------------------------------------------
//  jack Client
//------------------------------------------------------------------------
class JackClient : public IMediaServer
{
public:
//--------------------------------------------------------------------
	using JackPorts = std::vector<jack_port_t*>;
	using JackName = std::string;

	JackClient () = default;
	virtual ~JackClient ();

	// IMediaServer interface
	bool registerAudioClient (IAudioClient* client) override;
	bool registerMidiClient (IMidiClient* client) override;

	bool initialize (JackName name);

	// jack process callback
	int process (jack_nframes_t nframes);

//--------------------------------------------------------------------
private:
	jack_client_t* registerClient (JackName name);
	bool registerAudioPorts (IAudioClient* processor);
	bool registerMidiPorts (IMidiClient* processor);
	bool addAudioOutputPort (JackName name);
	bool addAudioInputPort (JackName name);
	bool addMidiInputPort (JackName name);
	int processMidi (jack_nframes_t nframes);
	bool setupJackProcessCallbacks (jack_client_t* client);
	bool autoConnectAudioPorts (jack_client_t* client);
	bool autoConnectMidiPorts (jack_client_t* client);
	void updateAudioBuffers (jack_nframes_t nframes);

	// Jack objects
	jack_client_t* jackClient = nullptr;
	JackPorts audioOutputPorts;
	JackPorts audioInputPorts;
	JackPorts midiInputPorts;

	IAudioClient* audioClient = nullptr;
	IMidiClient* midiClient = nullptr;
	using BufferPointers = std::vector<jack_default_audio_sample_t*>;
	BufferPointers audioOutputPointers;
	BufferPointers audioInputPointers;
	IAudioClient::Buffers buffers {nullptr};
};

//------------------------------------------------------------------------
int jack_on_process (jack_nframes_t nframes, void* arg)
{
	auto client = reinterpret_cast<JackClient*> (arg);

	return client->process (nframes);
}

//------------------------------------------------------------------------
int jack_on_set_sample_rate (jack_nframes_t nframes, void* arg)
{
	auto client = reinterpret_cast<IAudioClient*> (arg);
	client->setSamplerate (static_cast<SampleRate> (nframes));
	return kJackSuccess;
}

//------------------------------------------------------------------------
int jack_on_set_block_size (jack_nframes_t nframes, void* arg)
{
	auto client = reinterpret_cast<IAudioClient*> (arg);
	client->setBlockSize (static_cast<int32> (nframes));
	return kJackSuccess;
}

//------------------------------------------------------------------------
IMediaServerPtr createMediaServer (const AudioClientName& name)
{
	auto client = std::make_shared<JackClient> ();
	client->initialize (name);
	return client;
}

//------------------------------------------------------------------------
JackClient::~JackClient ()
{
	//! We do not need to "unregister" ports. It is done automatically with "jack_client_close"
	jack_deactivate (jackClient); // Stops calls of process
	jack_client_close (jackClient); // Remove client from process graph and remove all ports
}

//------------------------------------------------------------------------
bool JackClient::registerAudioClient (IAudioClient* client)
{
	if (audioClient)
		return false;

	audioClient = client;

	//! First thing to do: register the audio ports.
	if (!registerAudioPorts (audioClient))
		return false;

	//! Setup all the callbacks like setSampleRate and process etc.
	if (!setupJackProcessCallbacks (jackClient))
		return false;

	//! Activate after defining the callbacks. It is said in the documentation.
	if (jack_activate (jackClient) != kJackSuccess)
		return false;

	//! AFTER activation, register the ports.
	if (!autoConnectAudioPorts (jackClient))
		return false;

	return true;
}

//------------------------------------------------------------------------
bool JackClient::registerMidiClient (IMidiClient* client)
{
	if (midiClient)
		return false;

	midiClient = client;

	//! Register the midi ports.
	if (!registerMidiPorts (midiClient))
		return false;

	//! Afterwards auto-connect them.
	if (!autoConnectMidiPorts (jackClient))
		return false;

	return true;
}

//------------------------------------------------------------------------
bool JackClient::initialize (JackClient::JackName name)
{
	jackClient = registerClient (name);
	if (!jackClient)
		return false;

	return true;
}

//------------------------------------------------------------------------
void JackClient::updateAudioBuffers (jack_nframes_t nframes)
{
	int outputIndex = 0;
	for (auto audioOutputPort : audioOutputPorts)
	{
		auto* portBuffer = jack_port_get_buffer (audioOutputPort, nframes);
		if (!portBuffer)
			continue;

		buffers.outputs[outputIndex++] = static_cast<jack_default_audio_sample_t*> (portBuffer);
	}

	int inputIndex = 0;
	for (auto audioInputPort : audioInputPorts)
	{
		auto* portBuffer = jack_port_get_buffer (audioInputPort, nframes);
		if (!portBuffer)
			continue;

		buffers.inputs[inputIndex++] = static_cast<jack_default_audio_sample_t*> (portBuffer);
	}
}

//------------------------------------------------------------------------
int JackClient::process (jack_nframes_t nframes)
{
	processMidi (nframes);
	buffers.numSamples = nframes;

	updateAudioBuffers (nframes);
	if (!audioClient)
		return 0;

	if (audioClient->process (buffers, jack_last_frame_time (jackClient)) == false)
	{
		assert (false);
	}

	return kJackSuccess;
}

//------------------------------------------------------------------------
bool JackClient::registerAudioPorts (IAudioClient* processor)
{
	auto ioSetup = processor->getIOSetup ();

	for (const auto& output : ioSetup.outputs)
		addAudioOutputPort (output);

	for (const auto& input : ioSetup.inputs)
		addAudioInputPort (input);

	buffers.inputs = audioInputPointers.data ();
	buffers.numInputs = (int32_t)audioInputPointers.size ();
	buffers.numOutputs = (int32_t)audioOutputPointers.size ();
	buffers.outputs = audioOutputPointers.data ();

	return true;
}

//------------------------------------------------------------------------
bool JackClient::registerMidiPorts (IMidiClient* processor)
{
	const auto ioSetup = processor->getMidiIOSetup ();
	for (const auto& input : ioSetup.inputs)
		addMidiInputPort (input);

	return true;
}

//------------------------------------------------------------------------
bool JackClient::addAudioOutputPort (JackClient::JackName name)
{
	auto port =
	    jack_port_register (jackClient, name.data (), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	if (!port)
		return false;

	audioOutputPorts.push_back (port);
	audioOutputPointers.resize (audioOutputPorts.size ());
	return true;
}

//------------------------------------------------------------------------
bool JackClient::addAudioInputPort (JackClient::JackName name)
{
	auto port =
	    jack_port_register (jackClient, name.data (), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	if (!port)
		return false;

	audioInputPorts.push_back (port);
	audioInputPointers.resize (audioOutputPorts.size ());
	return true;
}

//------------------------------------------------------------------------
bool JackClient::addMidiInputPort (JackClient::JackName name)
{
	auto port =
	    jack_port_register (jackClient, name.data (), JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
	if (!port)
		return false;

	midiInputPorts.push_back (port);
	return true;
}

//------------------------------------------------------------------------
int JackClient::processMidi (jack_nframes_t nframes)
{
	static const uint8_t kChannelMask = 0x0F;
	static const uint8_t kStatusMask = 0xF0;
	static const uint32_t kDataMask = 0x7F;

	for (int32_t portIndex = 0, count = static_cast<int32_t> (midiInputPorts.size ());
	     portIndex < count; ++portIndex)
	{
		auto midiInputPort = midiInputPorts.at (portIndex);
		auto* portBuffer = jack_port_get_buffer (midiInputPort, nframes);
		if (!portBuffer)
			continue;

		jack_midi_event_t in_event;
		auto event_count = jack_midi_get_event_count (portBuffer);
		for (uint32_t i = 0; i < event_count; i++)
		{
			jack_midi_event_get (&in_event, portBuffer, i);
			if (in_event.size == 0)
				continue;

			auto midiData = in_event.buffer;
			Steinberg::Vst::IMidiClient::MidiData channel = midiData[0] & kChannelMask;
			Steinberg::Vst::IMidiClient::MidiData status = midiData[0] & kStatusMask;
			Steinberg::Vst::IMidiClient::MidiData data0 = midiData[1];
			Steinberg::Vst::IMidiClient::MidiData data1 = midiData[2];
			midiClient->onEvent ({status, channel, data0, data1, in_event.time}, portIndex);
		}
	}

	return kJackSuccess;
}

//------------------------------------------------------------------------
jack_client_t* JackClient::registerClient (JackClient::JackName name)
{
	jack_options_t options = JackNullOption;
	jack_status_t status;

	jackClient = jack_client_open (name.data (), options, &status, nullptr);
	return jackClient;

	/* Use the status to check for errors:
	    if (status & JackServerFailed)
	    {
	        fprintf (stderr, "Unable to connect to JACK server\n");
	    }*/
}

//------------------------------------------------------------------------
bool JackClient::setupJackProcessCallbacks (jack_client_t* client)
{
	if (jack_set_process_callback (client, jack_on_process, this) != kJackSuccess)
		return false;

	if (jack_set_sample_rate_callback (client, jack_on_set_sample_rate, audioClient) !=
	    kJackSuccess)
		return false;

	if (jack_set_buffer_size_callback (client, jack_on_set_block_size, audioClient) != kJackSuccess)
		return false;

	return true;
}

//------------------------------------------------------------------------
bool JackClient::autoConnectAudioPorts (jack_client_t* client)
{
	int portIndex = 0;

	//! Connect Audio Outputs
	auto ports = jack_get_ports (client, nullptr, nullptr, JackPortIsPhysical | JackPortIsInput);
	for (auto& port : audioOutputPorts)
	{
		if (!ports[portIndex])
			break;

		auto output__port_name = ports[portIndex++];
		auto res = jack_connect (client, jack_port_name (port), output__port_name);
		if (res != 0)
			break;
	}

	jack_free (ports);
	return true;
}

//------------------------------------------------------------------------
bool JackClient::autoConnectMidiPorts (jack_client_t* client)
{
	int portIndex = 1;

	//! Connect MIDI Inputs
	auto ports = jack_get_ports (client, nullptr, "midi", JackPortIsPhysical | JackPortIsOutput);
	if (!ports)
		return false;

	for (auto& port : midiInputPorts)
	{
		if (!ports[portIndex])
			break;

		auto inputPortName = ports[portIndex++];
		auto res = jack_connect (client, inputPortName, jack_port_name (port));
		if (res != 0)
			continue;
	}

	jack_free (ports);
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
