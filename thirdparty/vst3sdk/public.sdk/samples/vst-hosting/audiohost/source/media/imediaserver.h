//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : AudioHost
// Filename    : public.sdk/samples/vst-hosting/audiohost/source/media/imediaserver.h
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

#include <memory>
#include <string>
#include <vector>
#include <pluginterfaces/vst/vsttypes.h>

//----------------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
using IOName = std::string;
using IONames = std::vector<IOName>;
using AudioClientName = std::string;

struct IAudioClient
{
	struct Buffers
	{
		float** inputs;
		int32_t numInputs;
		float** outputs;
		int32_t numOutputs;
		int32_t numSamples;
	};

	struct IOSetup
	{
		IONames inputs;
		IONames outputs;
	};

	virtual bool process (Buffers& buffers, int64_t continousFrames) = 0;
	virtual bool setSamplerate (SampleRate value) = 0;
	virtual bool setBlockSize (int32 value) = 0;
	virtual IOSetup getIOSetup () const = 0;

	virtual ~IAudioClient () {}
};

//----------------------------------------------------------------------------------
struct IMidiClient
{
	using MidiData = uint8_t;

	struct Event
	{
		MidiData type;
		MidiData channel;
		MidiData data0;
		MidiData data1;
		int64_t timestamp;
	};

	struct IOSetup
	{
		IONames inputs;
		IONames outputs;
	};

	virtual bool onEvent (const Event& event, int32_t port) = 0;
	virtual IOSetup getMidiIOSetup () const = 0;

	virtual ~IMidiClient () {}
};

//----------------------------------------------------------------------------------
struct IMediaServer
{
	virtual bool registerAudioClient (IAudioClient* client) = 0;
	virtual bool registerMidiClient (IMidiClient* client) = 0;

	virtual ~IMediaServer () {}
};

//----------------------------------------------------------------------------------
using IMediaServerPtr = std::shared_ptr<IMediaServer>;

IMediaServerPtr createMediaServer (const AudioClientName& name);
//----------------------------------------------------------------------------------
} // Vst
} // Steinberg
