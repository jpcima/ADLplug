//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/syncdelay/source/syncdelaycontroller.cpp
// Created by  : Steinberg, 01/2020
// Description :
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

#include "syncdelaycontroller.h"
#include "sync.h"
#include "syncdelayids.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"

namespace Steinberg {
namespace Vst {

//-----------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayController::initialize (FUnknown* context)
{
	auto result = EditController::initialize (context);
	if (result == kResultTrue)
	{
		auto delayParam = new StringListParameter (STR16 ("Delay"), kDelayId, nullptr);
		for (const auto& entry : Synced)
			delayParam->appendString (entry.title);

		parameters.addParameter (delayParam); // parameters takes ownership of delayParam

		parameters.addParameter (STR16 ("Bypass"), nullptr, 1, 0,
		                         ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass, kBypassId);
	}
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SyncDelayController::setComponentState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	uint32 savedDelay = 0.f;
	if (streamer.readInt32u (savedDelay) == false)
		return kResultFalse;
	int32 savedBypassState = 0;
	if (streamer.readInt32 (savedBypassState) == false)
		return kResultFalse;

	setParamNormalized (kDelayId, ToNormalized<ParamValue> (savedDelay, static_cast<int32> (Synced.size () - 1)));
	setParamNormalized (kBypassId, savedBypassState ? 1 : 0);

	getComponentHandler ()->restartComponent (RestartFlags::kParamValuesChanged);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
