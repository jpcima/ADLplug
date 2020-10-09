//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/hostcheck/source/hostcheck.h
// Created by  : Steinberg, 04/2012
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

#pragma once

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"
#include <vector>
#include <set>
#include "logevents.h"
#include "eventlogger.h"
#include "processsetupcheck.h"
#include "processcontextcheck.h"
#include "eventlistcheck.h"
#include "parameterchangescheck.h"

namespace Steinberg {
namespace Vst {
class IEventList;
class IComponent;
}
}
//------------------------------------------------------------------------
//	ProcessDataValidator
//------------------------------------------------------------------------
class HostCheck
{
public:
//------------------------------------------------------------------------
	static HostCheck& Instance ()
	{
		static HostCheck instance;
		return instance;
	}

	using ParamIDs = std::set<Steinberg::Vst::ParamID>;

	HostCheck ();
	void addParameter (Steinberg::Vst::ParamID paramId);
	void setProcessSetup (Steinberg::Vst::ProcessSetup& setup);
	void setComponent (Steinberg::Vst::IComponent* component);
	bool validate (Steinberg::Vst::ProcessData& data, Steinberg::int32 minInputBufferCount,
	               Steinberg::int32 minOutputBufferCount);

	const EventLogger::Codes& getEventLogs () const { return mEventLogger.getLogEvents (); }

	EventLogger& getEventLogger ()
	{
		return mEventLogger;
	} /// Caution logger is used by audio thread...!!!
//------------------------------------------------------------------------
private:
	void addLogEvent (Steinberg::int32 logId);
	void checkAudioBuffers (Steinberg::Vst::AudioBusBuffers* buffers, Steinberg::int32 numBuffers,
	                        Steinberg::Vst::BusDirection dir, Steinberg::int32 symbolicSampleSize,
	                        Steinberg::int32 minBufferCount);

	Steinberg::Vst::IComponent* mComponent {nullptr};
	ParamIDs mParameterIds;

	ProcessSetupCheck mProcessSetupCheck;
	ProcessContextCheck mProcessContextCheck;
	EventListCheck mEventListCheck;
	ParameterChangesCheck mParamChangesCheck;
	EventLogger mEventLogger;
};

//------------------------------------------------------------------------
