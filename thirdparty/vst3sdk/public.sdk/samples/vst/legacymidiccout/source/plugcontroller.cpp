//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/legacymidiccout/source/plugcontroller.cpp
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

#include "plugcontroller.h"
#include "plugparamids.h"

#include "public.sdk/source/vst/vsteventshelper.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstcomponent.h"

namespace Steinberg {
namespace Vst {
namespace LegacyMIDICCOut {
//------------------------------------------------------------------------
// PlugController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::initialize (FUnknown* context)
{
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	//---Create Parameters------------

	//---Bypass parameter---
	int32 stepCount = 1;
	ParamValue defaultVal = 0;
	int32 flags = ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass;
	int32 tag = kBypassId;
	parameters.addParameter (String ("Bypass"), nullptr, stepCount, defaultVal, flags, tag);

	//---Controller parameter - only for demo ---
	parameters.addParameter (new RangeParameter (
	    STR16 ("MIDI Channel"), kChannelId, nullptr, 1, kMaxMIDIChannelSupported, 1,
	    kMaxMIDIChannelSupported - 1, ParameterInfo::kNoFlags));
	parameters.addParameter (new RangeParameter (STR16 ("Controller Num"), kControllerNumId,
	                                             nullptr, 0, 127, 0, 127,
	                                             ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("Controller"), kControllerId, nullptr, 0,
	                                             127, 0, 127, ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("PitchBend"), kPitchBendId, nullptr,
	                                             -0x2000, 0x1FFF, 0, 0x3FFF,
	                                             ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("ProgramChange"), kProgramChangeId, nullptr,
	                                             0, 127, 0, 127, ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("PolyPressure Key"), kPolyPressureNoteId,
	                                             nullptr, 0, 127, 0, 127,
	                                             ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("PolyPressure"), kPolyPressureId, nullptr,
	                                             0, 127, 0, 127, ParameterInfo::kCanAutomate));
	parameters.addParameter (new RangeParameter (STR16 ("Aftertouch"), kAftertouchId, nullptr, 0,
	                                             127, 0, 127, ParameterInfo::kCanAutomate));

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState (IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read only the gain and bypass value...
	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	// read the bypass
	int32 bypassState = 0;
	if (streamer.readInt32 (bypassState) == false)
		return kResultFalse;
	setParamNormalized (kBypassId, bypassState ? 1 : 0);

	uint8 val;
	if (streamer.readInt8u (val) == true)
		setParamNormalized (kChannelId, (ParamValue)val / kMaxMIDIChannelSupported);

	if (streamer.readInt8u (val) == true)
		setParamNormalized (kControllerNumId, Helpers::getMIDINormValue (val));

	if (streamer.readInt8u (val) == true)
		setParamNormalized (kPolyPressureNoteId, Helpers::getMIDINormValue (val));

	double dVal;
	if (streamer.readDouble (dVal) == true)
		setParamNormalized (kPolyPressureNoteId, Helpers::getMIDINormValue (dVal));
	if (streamer.readDouble (dVal) == true)
		setParamNormalized (kProgramChangeId, Helpers::getMIDINormValue (dVal));
	if (streamer.readDouble (dVal) == true)
		setParamNormalized (kAftertouchId, Helpers::getMIDINormValue (dVal));
	if (streamer.readDouble (dVal) == true)
		setParamNormalized (kPolyPressureId, Helpers::getMIDINormValue (dVal));
	if (streamer.readDouble (dVal) == true)
		setParamNormalized (kPitchBendId, Helpers::getMIDINormValue (dVal));
	return kResultOk;
}
}
}
} // namespaces
