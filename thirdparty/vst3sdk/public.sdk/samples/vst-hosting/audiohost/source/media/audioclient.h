//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : AudioHost
// Filename    : public.sdk/samples/vst-hosting/audiohost/source/audioclient.h
// Created by  : Steinberg 09.2016
// Description : Audio Host Example for VST 3
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

#pragma once

#include "public.sdk/samples/vst-hosting/audiohost/source/media/imediaserver.h"
#include "public.sdk/samples/vst-hosting/audiohost/source/media/iparameterclient.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/hosting/parameterchanges.h"
#include "public.sdk/source/vst/hosting/processdata.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include <array>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class IMidiMapping;
class IComponent;

enum
{
	kMaxMidiMappingBusses = 4,
	kMaxMidiChannels = 16
};
using Controllers = std::vector<int32>;
using Channels = std::array<Controllers, kMaxMidiChannels>;
using Busses = std::array<Channels, kMaxMidiMappingBusses>;
using MidiCCMapping = Busses;

//------------------------------------------------------------------------
using AudioClientPtr = std::shared_ptr<class AudioClient>;
//------------------------------------------------------------------------
class AudioClient : public IAudioClient, public IMidiClient, public IParameterClient
{
public:
//--------------------------------------------------------------------
	using Name = std::string;

	AudioClient ();
	virtual ~AudioClient ();

	static AudioClientPtr create (const Name& name, IComponent* component,
	                              IMidiMapping* midiMapping);

	// IAudioClient
	bool process (Buffers& buffers, int64_t continousFrames) override;
	bool setSamplerate (SampleRate value) override;
	bool setBlockSize (int32 value) override;
	IAudioClient::IOSetup getIOSetup () const override;

	// IMidiClient
	bool onEvent (const Event& event, int32_t port) override;
	IMidiClient::IOSetup getMidiIOSetup () const override;

	// IParameterClient
	void setParameter (ParamID id, ParamValue value, int32 sampleOffset) override;

	bool initialize (const Name& name, IComponent* component, IMidiMapping* midiMapping);

//--------------------------------------------------------------------
private:
	void createLocalMediaServer (const Name& name);
	void terminate ();
	void updateBusBuffers (Buffers& buffers, HostProcessData& processData);
	void initProcessData ();
	void initProcessContext ();
	bool updateProcessSetup ();
	void preprocess (Buffers& buffers, int64_t continousFrames);
	void postprocess (Buffers& buffers);
	bool isPortInRange (int32 port, int32 channel) const;
	bool processVstEvent (const IMidiClient::Event& event, int32 port);
	bool processParamChange (const IMidiClient::Event& event, int32 port);

	SampleRate sampleRate = 0;
	int32 blockSize = 0;
	HostProcessData processData;
	ProcessContext processContext;
	EventList eventList;
	ParameterChanges inputParameterChanges;
	IComponent* component = nullptr;
	ParameterChangeTransfer paramTransferrer;

	MidiCCMapping midiCCMapping;
	IMediaServerPtr mediaServer;
	bool isProcessing = false;

	Name name;
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
