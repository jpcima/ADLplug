//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/utility/processcontextrequirements.h
// Created by  : Steinberg, 12/2019
// Description : Helper class to work with IProcessContextRequirements
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

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class ProcessContextRequirements
{
private:
	using Self = ProcessContextRequirements;

public:
	ProcessContextRequirements (uint32 inFlags = 0) : flags (inFlags) {}

	bool wantsNone () const { return flags == 0; }
	bool wantsSystemTime () const
	{
		return (flags & IProcessContextRequirements::kNeedSystemTime) != 0;
	}
	bool wantsContinousTimeSamples () const
	{
		return (flags & IProcessContextRequirements::kNeedContinousTimeSamples) != 0;
	}
	bool wantsProjectTimeMusic () const
	{
		return (flags & IProcessContextRequirements::kNeedProjectTimeMusic) != 0;
	}
	bool wantsBarPositionMusic () const
	{
		return (flags & IProcessContextRequirements::kNeedBarPositionMusic) != 0;
	}
	bool wantsCycleMusic () const
	{
		return (flags & IProcessContextRequirements::kNeedCycleMusic) != 0;
	}
	bool wantsSamplesToNextClock () const
	{
		return (flags & IProcessContextRequirements::kNeedSamplesToNextClock) != 0;
	}
	bool wantsTempo () const { return (flags & IProcessContextRequirements::kNeedTempo) != 0; }
	bool wantsTimeSignature () const
	{
		return (flags & IProcessContextRequirements::kNeedTimeSignature) != 0;
	}
	bool wantsChord () const { return (flags & IProcessContextRequirements::kNeedChord) != 0; }
	bool wantsFrameRate () const
	{
		return (flags & IProcessContextRequirements::kNeedFrameRate) != 0;
	}
	bool wantsTransportState () const
	{
		return (flags & IProcessContextRequirements::kNeedTransportState) != 0;
	}

	/** set SystemTime as requested */
	Self& needSystemTime ()
	{
		flags |= IProcessContextRequirements::kNeedSystemTime;
		return *this;
	}
	/** set ContinousTimeSamples as requested */
	Self& needContinousTimeSamples ()
	{
		flags |= IProcessContextRequirements::kNeedContinousTimeSamples;
		return *this;
	}
	/** set ProjectTimeMusic as requested */
	Self& needProjectTimeMusic ()
	{
		flags |= IProcessContextRequirements::kNeedProjectTimeMusic;
		return *this;
	}
	/** set BarPositionMusic as needed */
	Self& needBarPositionMusic ()
	{
		flags |= IProcessContextRequirements::kNeedBarPositionMusic;
		return *this;
	}
	/** set CycleMusic as needed */
	Self& needCycleMusic ()
	{
		flags |= IProcessContextRequirements::kNeedCycleMusic;
		return *this;
	}
	/** set SamplesToNextClock as needed */
	Self& needSamplesToNextClock ()
	{
		flags |= IProcessContextRequirements::kNeedSamplesToNextClock;
		return *this;
	}
	/** set Tempo as needed */
	Self& needTempo ()
	{
		flags |= IProcessContextRequirements::kNeedTempo;
		return *this;
	}
	/** set TimeSignature as needed */
	Self& needTimeSignature ()
	{
		flags |= IProcessContextRequirements::kNeedTimeSignature;
		return *this;
	}
	/** set Chord as needed */
	Self& needChord ()
	{
		flags |= IProcessContextRequirements::kNeedChord;
		return *this;
	}
	/** set FrameRate as needed */
	Self& needFrameRate ()
	{
		flags |= IProcessContextRequirements::kNeedFrameRate;
		return *this;
	}
	/** set TransportState as needed */
	Self& needTransportState ()
	{
		flags |= IProcessContextRequirements::kNeedTransportState;
		return *this;
	}

	uint32 flags {0};
};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
