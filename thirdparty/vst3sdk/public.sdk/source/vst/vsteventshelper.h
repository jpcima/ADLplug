//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vsteventshelper.h
// Created by  : Steinberg, 11/2018
// Description : common defines
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once
#include "pluginterfaces/vst/ivstevents.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace Helpers {
//------------------------------------------------------------------------
/** bound a value between a min and max */
template<class T>inline T boundTo (T minval, T maxval, T x)
{
	if (x < minval)
		return minval;
	else if (x > maxval)
		return maxval;
	return x;
}

//------------------------------------------------------------------------
/** Initialized a Event */
inline Event& init (Event& event, uint16 type, int32 busIndex = 0, int32 sampleOffset = 0,
                    TQuarterNotes ppqPosition = 0, uint16 flags = 0)
{
	event.busIndex = busIndex;
	event.sampleOffset = sampleOffset;
	event.ppqPosition = ppqPosition;
	event.flags = flags;
	event.type = type;
	return event;
}

//------------------------------------------------------------------------
/** Returns normalized value of a LegacyMIDICCOutEvent value [0, 127] */
inline ParamValue getMIDINormValue (uint8 value)
{
	if (value >= 127)
		return 1.0;
	return (ParamValue)value / 127;
}

//------------------------------------------------------------------------
/** Returns LegacyMIDICCOut value [0, 127] from a normalized value [0., 1.] */
inline int8 getMIDICCOutValue (ParamValue value)
{
	return boundTo<int8> (0, 127, (int8)(value * 127 + 0.5));
}

//------------------------------------------------------------------------
/** Returns pitchbend value from a PitchBend LegacyMIDICCOut Event */
inline int16 getPitchBendValue (LegacyMIDICCOutEvent& e)
{
	return ((e.value & 0x7F)| ((e.value2 & 0x7F) << 7));
}

//------------------------------------------------------------------------
/** set a normalized pitchbend value to a LegacyMIDICCOut Event */
inline void setPitchBendValue (LegacyMIDICCOutEvent& e, ParamValue value)
{
	int16 tmp = static_cast<int16> (value * 0x3FFF);
	e.value = (tmp & 0x7F);
	e.value2 = ((tmp >> 7) & 0x7F);
}

//------------------------------------------------------------------------
/** Returns normalized pitchbend value from a PitchBend LegacyMIDICCOut Event */
inline float getNormPitchBendValue (LegacyMIDICCOutEvent& e)
{
	float val = (float)getPitchBendValue (e) / (float)0x3FFF;
	if (val < 0)
		val = 0;
	if (val > 1)
		val = 1;
	return val;
}

//------------------------------------------------------------------------
/** Initialized a LegacyMIDICCOutEvent */
inline LegacyMIDICCOutEvent& initLegacyMIDICCOutEvent (Event& event, uint8 controlNumber,
                                                       uint8 channel = 0, int8 value = 0,
                                                       int8 value2 = 0)
{
	init (event, Event::kLegacyMIDICCOutEvent);
	event.midiCCOut.channel = channel;
	event.midiCCOut.controlNumber = controlNumber;
	event.midiCCOut.value = value;
	event.midiCCOut.value2 = value2;
	return event.midiCCOut;
}
/*@}*/

//------------------------------------------------------------------------
} // namespace Helpers
} // namespace Vst
} // namespace Steinberg
