//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/legacymidiccout/source/plug.cpp
// Created by  : Steinberg, 11/2018
// Description : Plug-in Example for VST SDK 3.x using Legacy MIDI CC
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

#include "plug.h"
#include "plugcids.h" // for class ids
#include "plugparamids.h"

#include "public.sdk/source/vst/vsteventshelper.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

#include "base/source/fstreamer.h"
#include <cstdio>

namespace Steinberg {
namespace Vst {
namespace LegacyMIDICCOut {

//------------------------------------------------------------------------
// Plug Implementation
//------------------------------------------------------------------------
Plug::Plug ()
{
	// register its editor class (the same than used in plugentry.cpp)
	setControllerClass (PlugControllerUID);
}

//------------------------------------------------------------------------
tresult PLUGIN_API Plug::initialize (FUnknown* context)
{
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//---create Audio In/Out busses------
	// we want a stereo Input and a Stereo Output
	addAudioInput (STR16 ("Stereo In"), SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), SpeakerArr::kStereo);

	// one output for event and only 1 channel wanted
	addEventOutput (STR16 ("Event Out"), kMaxMIDIChannelSupported);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Plug::process (ProcessData& data)
{
	//---1) Read inputs parameter changes-----------
	IParameterChanges* paramChanges = data.inputParameterChanges;
	if (paramChanges)
	{
		IEventList* eventList = data.outputEvents;

		int32 numParamsChanged = paramChanges->getParameterCount ();
		// for each parameter which are some changes in this audio block:
		for (int32 i = 0; i < numParamsChanged; i++)
		{
			IParamValueQueue* paramQueue = paramChanges->getParameterData (i);
			if (!paramQueue)
				continue;

			int32 offsetSamples;
			double value;
			int32 numPoints = paramQueue->getPointCount ();
			ParamID id = paramQueue->getParameterId ();
			switch (id)
			{
				case kBypassId:
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						bBypass = (value > 0.5f);
					}
					break;
				case kChannelId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						mChannel =
						    Helpers::boundTo<uint8> (0, kMaxMIDIChannelSupported - 1,
						                             value * (kMaxMIDIChannelSupported - 1) + 0.5);
					}
				}
				break;
				case kControllerNumId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						mControllerNum = Helpers::getMIDICCOutValue (value);
					}
				}
				break;
				case kControllerId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						if (value != mLastController)
						{
							if (eventList && !bBypass)
							{
								Event event;
								Helpers::initLegacyMIDICCOutEvent (
								    event, mControllerNum, mChannel,
								    Helpers::getMIDICCOutValue (value));
								event.sampleOffset = offsetSamples;
								event.flags = Event::kIsLive;
								eventList->addEvent (event);
							}
							mLastController = value;
						}
					}
				}
				break;
				case kAftertouchId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						if (value != mLastAftertouch)
						{
							if (eventList && !bBypass)
							{
								Event event;
								Helpers::initLegacyMIDICCOutEvent (
								    event, kAfterTouch, mChannel,
								    Helpers::getMIDICCOutValue (value));
								event.sampleOffset = offsetSamples;
								eventList->addEvent (event);
							}
							mLastAftertouch = value;
						}
					}
				}
				break;
				case kProgramChangeId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						if (value != mLastProgramChange)
						{
							if (eventList && !bBypass)
							{
								Event event;
								Helpers::initLegacyMIDICCOutEvent (
								    event, kCtrlProgramChange, mChannel,
								    Helpers::getMIDICCOutValue (value));
								event.sampleOffset = offsetSamples;
								eventList->addEvent (event);
							}
							mLastProgramChange = value;
						}
					}
				}
				break;
				case kPolyPressureNoteId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						mPolyPressureKey = Helpers::getMIDICCOutValue (value);
					}
				}
				break;
				case kPolyPressureId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						if (value != mLastPolyPressure)
						{
							if (eventList && !bBypass)
							{
								Event event;
								Helpers::initLegacyMIDICCOutEvent (
								    event, kCtrlPolyPressure, mChannel, mPolyPressureKey,
								    Helpers::getMIDICCOutValue (value));
								event.sampleOffset = offsetSamples;
								eventList->addEvent (event);
							}
							mLastPolyPressure = value;
						}
					}
				}
				break;
				case kPitchBendId:
				{
					if (paramQueue->getPoint (numPoints - 1, offsetSamples, value) == kResultTrue)
					{
						if (value != mLastPitchBend)
						{
							if (eventList && !bBypass)
							{
								Event event;
								Helpers::initLegacyMIDICCOutEvent (event, kPitchBend, mChannel);
								Helpers::setPitchBendValue (event.midiCCOut, value);
								event.sampleOffset = offsetSamples;
								eventList->addEvent (event);
							}
							mLastPitchBend = value;
						}
					}
				}
				break;
			}
		}
	}

	//--- ----------------------------------
	//---3) Process Audio---------------------
	//--- ----------------------------------
	if (data.numInputs == 0 || data.numOutputs == 0)
	{
		// nothing to do
		return kResultOk;
	}

	// (simplification) we suppose in this example that we have the same input channel count than
	// the output
	int32 numChannels = data.inputs[0].numChannels;

	//---get audio buffers----------------
	float** in = data.inputs[0].channelBuffers32;
	float** out = data.outputs[0].channelBuffers32;

	for (int32 i = 0; i < numChannels; i++)
	{
		// do not need to be copied if the buffers are the same
		if (in[i] != out[i])
		{
			memcpy (out[i], in[i], data.numSamples * sizeof (float));
		}
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Plug::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	// read the bypass
	int32 savedBypass = 0;
	if (streamer.readInt32 (savedBypass) == false)
		return kResultFalse;

	bBypass = savedBypass > 0;

	streamer.readInt8u (mChannel);
	streamer.readInt8u (mControllerNum);
	streamer.readInt8u (mPolyPressureKey);
	streamer.readDouble (mLastController);
	streamer.readDouble (mLastProgramChange);
	streamer.readDouble (mLastAftertouch);
	streamer.readDouble (mLastPolyPressure);
	streamer.readDouble (mLastPitchBend);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API Plug::getState (IBStream* state)
{
	// here we need to save the model

	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	streamer.writeInt32 (bBypass ? 1 : 0);

	streamer.writeInt8u (mChannel);
	streamer.writeInt8u (mControllerNum);
	streamer.writeInt8u (mPolyPressureKey);

	streamer.writeDouble (mLastController);
	streamer.writeDouble (mLastProgramChange);
	streamer.writeDouble (mLastAftertouch);
	streamer.writeDouble (mLastPolyPressure);
	streamer.writeDouble (mLastPitchBend);

	return kResultOk;
}
}
}
} // namespaces
