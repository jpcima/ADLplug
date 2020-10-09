//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/utility/mpeprocessor.h
// Created by  : Steinberg, 07/2017
// Description : VST 3 AUv3Wrapper
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

#include <cstdint>
#include <cstdlib>
#include <memory>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace MPE {

using NoteID = int32_t;
using Pitch = uint32_t;
using Channel = uint32_t;
using Velocity = float;
using NormalizedValue = double;

//------------------------------------------------------------------------
enum class Controller
{
	Pressure,
	X,
	Y,
	None
};

//------------------------------------------------------------------------
struct Handler
{
	virtual bool generateNewNoteID (NoteID& outNoteID) = 0;
	virtual void releaseNoteID (NoteID noteID) = 0;

	virtual void onMPENoteOn (NoteID noteID, Pitch pitch, Velocity velocity) = 0;
	virtual void onMPENoteOff (NoteID noteID, Pitch pitch, Velocity velocity) = 0;
	virtual void onMPEControllerChange (NoteID noteID, Controller cc, NormalizedValue value) = 0;

	virtual void onOtherInput (const uint8_t* data, size_t dataSize) = 0;
	virtual void onSysexInput (const uint8_t* data, size_t dataSize) = 0;

	// error handling
	virtual void errorNoteDroppedBecauseNoNoteID (Pitch pitch) = 0;
	virtual void errorNoteDroppedBecauseNoteStackFull (Channel channel, Pitch pitch) = 0;
	virtual void errorNoteForNoteOffNotFound (Channel channel, Pitch pitch) = 0;
	virtual void errorProgramChangeReceivedInMPEZone () = 0;
};

//------------------------------------------------------------------------
enum InputMIDIMessage
{
	MIDICC_0 = 0,
	MIDICC_127 = 127,
	ChannelPressure = 128,
	PitchBend = 129,
	Aftertouch = 130,
};

//------------------------------------------------------------------------
struct Setup
{
	Channel masterChannel {0};
	Channel memberChannelBegin {1};
	Channel memberChannelEnd {14};
	InputMIDIMessage pressure {ChannelPressure};
	InputMIDIMessage x {PitchBend};
	InputMIDIMessage y {static_cast<InputMIDIMessage> (74)};
};

//------------------------------------------------------------------------
class Processor
{
public:
	Processor (Handler* delegate, size_t maxNotesPerChannel = 16);
	~Processor () noexcept;

	const Setup& getSetup () const;
	void changeSetup (const Setup& setup);
	void reset ();

	void processMIDIInput (const uint8_t* data, size_t dataSize);

private:
	int32_t onNoteOn (const uint8_t* data, size_t dataSize);
	int32_t onNoteOff (const uint8_t* data, size_t dataSize);
	int32_t onAftertouch (const uint8_t* data, size_t dataSize);
	int32_t onController (const uint8_t* data, size_t dataSize);
	int32_t onProgramChange (const uint8_t* data, size_t dataSize);
	int32_t onChannelPressure (const uint8_t* data, size_t dataSize);
	int32_t onPitchWheel (const uint8_t* data, size_t dataSize);

	struct Impl;
	std::unique_ptr<Impl> impl;
};

//------------------------------------------------------------------------
} // MPE
} // Vst
} // Steinberg
