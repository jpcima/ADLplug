//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : AudioHost
// Filename    : public.sdk/samples/vst-hosting/audiohost/source/media/audioclient.cpp
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

#include "audioclient.h"

#include "miditovst.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "public.sdk/source/vst/hosting/parameterchanges.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include <functional>

#include <cassert>
#include <limits>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// From Vst2Wrapper
static MidiCCMapping initMidiCtrlerAssignment (IComponent* component, IMidiMapping* midiMapping)
{
	MidiCCMapping midiCCMapping {};

	if (!midiMapping || !component)
		return midiCCMapping;

	int32 busses = std::min<int32> (component->getBusCount (kEvent, kInput), kMaxMidiMappingBusses);

	if (midiCCMapping[0][0].empty ())
	{
		for (int32 b = 0; b < busses; b++)
			for (int32 i = 0; i < kMaxMidiChannels; i++)
				midiCCMapping[b][i].resize (Vst::kCountCtrlNumber);
	}

	ParamID paramID;
	for (int32 b = 0; b < busses; b++)
	{
		for (int16 ch = 0; ch < kMaxMidiChannels; ch++)
		{
			for (int32 i = 0; i < Vst::kCountCtrlNumber; i++)
			{
				paramID = kNoParamId;
				if (midiMapping->getMidiControllerAssignment (b, ch, (CtrlNumber)i, paramID) ==
				    kResultTrue)
				{
					// TODO check if tag is associated to a parameter
					midiCCMapping[b][ch][i] = paramID;
				}
				else
					midiCCMapping[b][ch][i] = kNoParamId;
			}
		}
	}
	return midiCCMapping;
}

//------------------------------------------------------------------------
static void assignBusBuffers (const IAudioClient::Buffers& buffers, HostProcessData& processData,
                              bool unassign = false)
{
	// Set outputs
	auto bufferIndex = 0;
	for (auto busIndex = 0; busIndex < processData.numOutputs; busIndex++)
	{
		auto channelCount = processData.outputs[busIndex].numChannels;
		for (auto chanIndex = 0; chanIndex < channelCount; chanIndex++)
		{
			if (bufferIndex < buffers.numOutputs)
			{
				processData.setChannelBuffer (BusDirections::kOutput, busIndex, chanIndex,
				                              unassign ? nullptr : buffers.outputs[bufferIndex]);
				bufferIndex++;
			}
		}
	}

	// Set inputs
	bufferIndex = 0;
	for (auto busIndex = 0; busIndex < processData.numInputs; busIndex++)
	{
		auto channelCount = processData.inputs[busIndex].numChannels;
		for (auto chanIndex = 0; chanIndex < channelCount; chanIndex++)
		{
			if (bufferIndex < buffers.numInputs)
			{
				processData.setChannelBuffer (BusDirections::kInput, busIndex, chanIndex,
				                              unassign ? nullptr : buffers.inputs[bufferIndex]);

				bufferIndex++;
			}
		}
	}
}

//------------------------------------------------------------------------
static void unassignBusBuffers (const IAudioClient::Buffers& buffers, HostProcessData& processData)
{
	assignBusBuffers (buffers, processData, true);
}

//------------------------------------------------------------------------
//  Vst3Processor
//------------------------------------------------------------------------
AudioClient::AudioClient ()
{
}

//------------------------------------------------------------------------
AudioClient::~AudioClient ()
{

	terminate ();
}

//------------------------------------------------------------------------
AudioClientPtr AudioClient::create (const Name& name, IComponent* component,
                                    IMidiMapping* midiMapping)
{
	auto newProcessor = std::make_shared<AudioClient> ();
	newProcessor->initialize (name, component, midiMapping);
	return newProcessor;
}

//------------------------------------------------------------------------
void AudioClient::initProcessContext ()
{
	processContext = {};
	processContext.tempo = 120;
}

//------------------------------------------------------------------------
void AudioClient::createLocalMediaServer (const Name& name)
{
	mediaServer = createMediaServer (name);
	mediaServer->registerAudioClient (this);
	mediaServer->registerMidiClient (this);
}

//------------------------------------------------------------------------
bool AudioClient::initialize (const Name& name, IComponent* _component, IMidiMapping* midiMapping)
{
	component = _component;
	if (!component)
		return false;

	initProcessData ();

	paramTransferrer.setMaxParameters (1000);

	if (midiMapping)
		midiCCMapping = initMidiCtrlerAssignment (component, midiMapping);

	createLocalMediaServer (name);
	return true;
}

//------------------------------------------------------------------------
void AudioClient::terminate ()
{
	mediaServer = nullptr;

	FUnknownPtr<IAudioProcessor> processor = component;
	if (!processor)
		return;

	processor->setProcessing (false);
	component->setActive (false);
}

//------------------------------------------------------------------------
void AudioClient::initProcessData ()
{
	// processData.prepare will be done in setBlockSize

	processData.inputEvents = &eventList;
	processData.inputParameterChanges = &inputParameterChanges;
	processData.processContext = &processContext;

	initProcessContext ();
}

//------------------------------------------------------------------------
IMidiClient::IOSetup AudioClient::getMidiIOSetup () const
{
	IMidiClient::IOSetup iosetup;
	auto count = component->getBusCount (MediaTypes::kEvent, BusDirections::kInput);
	for (int32_t i = 0; i < count; i++)
	{
		BusInfo info;
		if (component->getBusInfo (MediaTypes::kEvent, BusDirections::kInput, i, info) != kResultOk)
			continue;

		auto busName = VST3::StringConvert::convert (info.name, 128); // TODO: 128???
		iosetup.inputs.push_back (busName);
	}

	count = component->getBusCount (MediaTypes::kEvent, BusDirections::kOutput);
	for (int32_t i = 0; i < count; i++)
	{
		BusInfo info;
		if (component->getBusInfo (MediaTypes::kEvent, BusDirections::kOutput, i, info) !=
		    kResultOk)
			continue;

		auto busName = VST3::StringConvert::convert (info.name, 128); // TODO: 128???
		iosetup.outputs.push_back (busName);
	}

	return iosetup;
}

//------------------------------------------------------------------------
IAudioClient::IOSetup AudioClient::getIOSetup () const
{
	IAudioClient::IOSetup iosetup;
	auto count = component->getBusCount (MediaTypes::kAudio, BusDirections::kOutput);
	for (int32_t i = 0; i < count; i++)
	{
		BusInfo info;
		if (component->getBusInfo (MediaTypes::kAudio, BusDirections::kOutput, i, info) !=
		    kResultOk)
			continue;

		for (int32_t j = 0; j < info.channelCount; j++)
		{
			auto channelName = VST3::StringConvert::convert (info.name, 128); // TODO: 128???
			iosetup.outputs.push_back (channelName + " " + std::to_string (j));
		}
	}

	count = component->getBusCount (MediaTypes::kAudio, BusDirections::kInput);
	for (int32_t i = 0; i < count; i++)
	{
		BusInfo info;
		if (component->getBusInfo (MediaTypes::kAudio, BusDirections::kInput, i, info) != kResultOk)
			continue;

		for (int32_t j = 0; j < info.channelCount; j++)
		{
			auto channelName = VST3::StringConvert::convert (info.name, 128); // TODO: 128???
			iosetup.inputs.push_back (channelName + " " + std::to_string (j));
		}
	}

	return iosetup;
}

//------------------------------------------------------------------------
void AudioClient::preprocess (Buffers& buffers, int64_t continousFrames)
{
	processData.numSamples = buffers.numSamples;
	processContext.continousTimeSamples = continousFrames;
	assignBusBuffers (buffers, processData);
	paramTransferrer.transferChangesTo (inputParameterChanges);
}

//------------------------------------------------------------------------
bool AudioClient::process (Buffers& buffers, int64_t continousFrames)
{
	FUnknownPtr<IAudioProcessor> processor = component;
	if (!processor || !isProcessing)
		return false;

	preprocess (buffers, continousFrames);

	if (processor->process (processData) != kResultOk)
		return false;

	postprocess (buffers);

	return true;
}
//------------------------------------------------------------------------
void AudioClient::postprocess (Buffers& buffers)
{
	eventList.clear ();
	inputParameterChanges.clearQueue ();
	unassignBusBuffers (buffers, processData);
}

//------------------------------------------------------------------------
bool AudioClient::setSamplerate (SampleRate value)
{
	if (sampleRate == value)
		return true;

	sampleRate = value;
	processContext.sampleRate = sampleRate;
	if (blockSize == 0)
		return true;

	return updateProcessSetup ();
}

//------------------------------------------------------------------------
bool AudioClient::setBlockSize (int32 value)
{
	if (blockSize == value)
		return true;

	blockSize = value;
	if (sampleRate == 0)
		return true;

	processData.prepare (*component, blockSize, kSample32);
	return updateProcessSetup ();
}

//------------------------------------------------------------------------
bool AudioClient::updateProcessSetup ()
{
	FUnknownPtr<IAudioProcessor> processor = component;
	if (!processor)
		return false;

	if (isProcessing)
	{
		if (processor->setProcessing (false) != kResultOk)
			return false;

		if (component->setActive (false) != kResultOk)
			return false;
	}

	ProcessSetup setup {kRealtime, kSample32, blockSize, sampleRate};

	if (processor->setupProcessing (setup) != kResultOk)
		return false;

	if (component->setActive (true) != kResultOk)
		return false;

	processor->setProcessing (true); // != kResultOk
	/*
if (processor->setProcessing(true) != kResultOk)
return false;*/

	isProcessing = true;
	return isProcessing;
}

//------------------------------------------------------------------------
bool AudioClient::isPortInRange (int32 port, int32 channel) const
{
	return port < kMaxMidiMappingBusses && !midiCCMapping[port][channel].empty ();
}

//------------------------------------------------------------------------
bool AudioClient::processVstEvent (const IMidiClient::Event& event, int32 port)
{
	auto vstEvent = midiToEvent (event.type, event.channel, event.data0, event.data1);
	if (vstEvent)
	{
		vstEvent->busIndex = port;
		if (eventList.addEvent (*vstEvent) != kResultOk)
		{
			assert (false && "Event was not added to EventList!");
		}

		return true;
	}

	return false;
}

//------------------------------------------------------------------------
bool AudioClient::processParamChange (const IMidiClient::Event& event, int32 port)
{
	auto paramMapping = [port, this] (int32 channel, MidiData data1) -> ParamID {
		if (!isPortInRange (port, channel))
			return kNoParamId;

		return midiCCMapping[port][channel][data1];
	};

	auto paramChange =
	    midiToParameter (event.type, event.channel, event.data0, event.data1, paramMapping);
	if (paramChange)
	{
		int32 index = 0;
		IParamValueQueue* queue =
		    inputParameterChanges.addParameterData ((*paramChange).first, index);
		if (queue)
		{
			if (queue->addPoint (event.timestamp, (*paramChange).second, index) != kResultOk)
			{
				assert (false && "Parameter point was not added to ParamValueQueue!");
			}
		}

		return true;
	}

	return false;
}

//------------------------------------------------------------------------
bool AudioClient::onEvent (const IMidiClient::Event& event, int32_t port)
{
	// Try to create Event first.
	if (processVstEvent (event, port))
		return true;

	// In case this is no event it must be a parameter.
	if (processParamChange (event, port))
		return true;

	// TODO: Something else???

	return true;
}

//------------------------------------------------------------------------
void AudioClient::setParameter (ParamID id, ParamValue value, int32 sampleOffset)
{
	paramTransferrer.addChange (id, value, sampleOffset);
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
