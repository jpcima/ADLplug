//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/utility/mpeprocessor.cpp
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

#include "mpeprocessor.h"
#include <array>
#include <vector>
#include <cassert>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace MPE {

//------------------------------------------------------------------------
struct Note
{
	NoteID noteID;
	Pitch pitch;
};

//------------------------------------------------------------------------
struct ChannelData
{
	using NoteList = std::vector<Note>;

	NoteList notes;
	NormalizedValue pressure {0.};
	NormalizedValue x {0.5};
	NormalizedValue y {0.};
};

//------------------------------------------------------------------------
struct Processor::Impl
{
	using ChannelDataList = std::array<ChannelData, 16>;

	Handler* delegate {nullptr};
	Setup setup;
	ChannelDataList channelData {};
	size_t dataBufferUsed {0};
	size_t maxNotesPerChannel {16};
	bool inSysex {false};

	Impl (Handler* delegate, size_t maxNotesPerChannel)
	: delegate (delegate), maxNotesPerChannel (maxNotesPerChannel)
	{
		for (auto& cd : channelData)
			cd.notes.reserve (maxNotesPerChannel);
	}

	bool inMPEZone (uint8_t channel) const
	{
		return channel >= setup.memberChannelBegin && channel <= setup.memberChannelEnd;
	}

	Controller getController (InputMIDIMessage input) const
	{
		if (setup.pressure == input)
			return Controller::Pressure;
		if (setup.x == input)
			return Controller::X;
		if (setup.y == input)
			return Controller::Y;
		return Controller::None;
	}

	NormalizedValue getControllerValue (Controller controller, const ChannelData& data) const
	{
		switch (controller)
		{
			case Controller::Pressure: return data.pressure;
			case Controller::X: return data.x;
			case Controller::Y: return data.y;
			case Controller::None: assert (false); break;
		}
		return 0.;
	}

	void setControllerValue (Controller controller, ChannelData& data, NormalizedValue value)
	{
		switch (controller)
		{
			case Controller::Pressure: data.pressure = value; break;
			case Controller::X: data.x = value; break;
			case Controller::Y: data.y = value; break;
			case Controller::None: assert (false); break;
		}
	}
};

//------------------------------------------------------------------------
Processor::Processor (Handler* delegate, size_t maxNotesPerChannel)
{
	impl = std::make_unique<Impl> (delegate, maxNotesPerChannel);
}

//------------------------------------------------------------------------
Processor::~Processor () noexcept = default;

//------------------------------------------------------------------------
const Setup& Processor::getSetup () const
{
	return impl->setup;
}

//------------------------------------------------------------------------
void Processor::changeSetup (const Setup& setup)
{
	impl->setup = setup;
}

//------------------------------------------------------------------------
void Processor::reset ()
{
	for (auto& cd : impl->channelData)
	{
		for (auto& note : cd.notes)
		{
			impl->delegate->onMPENoteOff (note.noteID, note.pitch, 0.f);
			impl->delegate->releaseNoteID (note.noteID);
		}
		cd.notes.clear ();
	}
}

//------------------------------------------------------------------------
int32_t Processor::onNoteOn (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 2);
	if (data[2] == 0)
		return onNoteOff (data, dataSize);
	auto channel = data[0] & 0x0F;
	if (impl->inMPEZone (channel))
	{
		auto& channelData = impl->channelData[channel];
		auto pitch = data[1];
		if (channelData.notes.size () >= impl->maxNotesPerChannel)
		{
			// error note stack full
			impl->delegate->errorNoteDroppedBecauseNoteStackFull (channel, pitch);
		}
		else
		{
			Note note;
			if (impl->delegate->generateNewNoteID (note.noteID))
			{
				note.pitch = pitch;
				channelData.notes.push_back (note);
				auto velocity = static_cast<Velocity> (data[2]) / 127.f;
				impl->delegate->onMPENoteOn (note.noteID, note.pitch, velocity);
				impl->delegate->onMPEControllerChange (note.noteID, Controller::Pressure,
				                                       channelData.pressure);
				impl->delegate->onMPEControllerChange (note.noteID, Controller::X, channelData.x);
				impl->delegate->onMPEControllerChange (note.noteID, Controller::Y, channelData.y);
			}
			else
			{
				// error: note will be dropped
				impl->delegate->errorNoteDroppedBecauseNoNoteID (pitch);
			}
		}
	}
	else
	{
		impl->delegate->onOtherInput (data, 3);
	}
	return 3;
}

//------------------------------------------------------------------------
int32_t Processor::onNoteOff (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 2);
	auto channel = data[0] & 0x0F;
	if (impl->inMPEZone (channel))
	{
		bool noteFound = false;
		auto& notes = impl->channelData[channel].notes;
		for (auto it = notes.begin (); it != notes.end (); ++it)
		{
			if (it->pitch == data[1])
			{
				auto velocity = static_cast<Velocity> (data[2]) / 127.f;
				impl->delegate->onMPENoteOff (it->noteID, it->pitch, velocity);
				impl->delegate->releaseNoteID (it->noteID);
				notes.erase (it);
				noteFound = true;
				break;
			}
		}
		if (!noteFound)
		{
			// error: no note for note off found
			impl->delegate->errorNoteForNoteOffNotFound (channel, data[1]);
		}
	}
	else
	{
		impl->delegate->onOtherInput (data, 3);
	}
	return 3;
}

//------------------------------------------------------------------------
int32_t Processor::onAftertouch (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 2);
	auto controller = impl->getController (ChannelPressure);
	if (controller != Controller::None)
	{
		auto channel = data[0] & 0x0F;
		if (impl->inMPEZone (channel))
		{
			auto& channelData = impl->channelData[channel];
			auto value = static_cast<NormalizedValue> (data[2]) / 127.;
			impl->setControllerValue (controller, channelData, value);
			for (auto& note : channelData.notes)
			{
				impl->delegate->onMPEControllerChange (note.noteID, controller, value);
			}
			return 3;
		}
	}
	impl->delegate->onOtherInput (data, 3);
	return 3;
}

//------------------------------------------------------------------------
int32_t Processor::onController (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 2);
	auto cc = data[1];
	auto controller = impl->getController (static_cast<InputMIDIMessage> (cc));
	if (controller != Controller::None)
	{
		auto channel = data[0] & 0x0F;
		if (impl->inMPEZone (channel))
		{
			auto& channelData = impl->channelData[channel];
			auto value = static_cast<NormalizedValue> (data[2]) / 127.;
			impl->setControllerValue (controller, channelData, value);
			for (auto& note : channelData.notes)
			{
				impl->delegate->onMPEControllerChange (note.noteID, controller, value);
			}
			return 3;
		}
	}
	impl->delegate->onOtherInput (data, 3);
	return 3;
}

//------------------------------------------------------------------------
int32_t Processor::onProgramChange (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 1);
	auto channel = data[0] & 0x0F;
	if (impl->inMPEZone (channel))
	{
		// error: program change send in MPE zone
		impl->delegate->errorProgramChangeReceivedInMPEZone ();
	}
	else
	{
		impl->delegate->onOtherInput (data, 2);
	}
	return 2;
}

//------------------------------------------------------------------------
int32_t Processor::onChannelPressure (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 1);
	auto controller = impl->getController (ChannelPressure);
	if (controller != Controller::None)
	{
		auto channel = data[0] & 0x0F;
		if (impl->inMPEZone (channel))
		{
			auto& channelData = impl->channelData[channel];
			auto value = static_cast<NormalizedValue> (data[1]) / 127.;
			impl->setControllerValue (controller, channelData, value);
			for (auto& note : channelData.notes)
			{
				impl->delegate->onMPEControllerChange (note.noteID, controller, value);
			}
			return 2;
		}
	}
	impl->delegate->onOtherInput (data, 2);
	return 2;
}

//------------------------------------------------------------------------
int32_t Processor::onPitchWheel (const uint8_t* data, size_t dataSize)
{
	assert (dataSize >= 2);
	auto controller = impl->getController (PitchBend);
	if (controller != Controller::None)
	{
		auto channel = data[0] & 0x0F;
		if (impl->inMPEZone (channel))
		{
			auto& channelData = impl->channelData[channel];
			auto value =
			    static_cast<NormalizedValue> ((data[1] & 0x7F) + ((data[2] & 0x7F) << 7)) / 16383.;
			impl->setControllerValue (controller, channelData, value);
			for (auto& note : channelData.notes)
			{
				impl->delegate->onMPEControllerChange (note.noteID, controller, value);
			}
			return 3;
		}
	}
	impl->delegate->onOtherInput (data, 3);
	return 3;
}

//------------------------------------------------------------------------
void Processor::processMIDIInput (const uint8_t* data, size_t dataSize)
{
	assert (dataSize > 0);
	if (impl->inSysex || data[0] == 0xf0)
	{
		for (size_t index = 0; index < dataSize; ++index)
		{
			if (data[index] == 0xf7)
			{
				++index;
				impl->delegate->onSysexInput (data, index);
				impl->inSysex = false;
				auto dataLeft = dataSize - index;
				if (dataLeft != 0)
				{
					processMIDIInput (data + index, dataSize - index);
				}
				return;
			}
		}
		impl->inSysex = true;
		impl->delegate->onSysexInput (data, dataSize);
		return;
	}

	auto status = data[0] & 0xF0;
	int32_t packetSize = 0;
	switch (status)
	{
		case 0x90: // Note On
		{
			packetSize = onNoteOn (data, dataSize);
			break;
		}
		case 0x80: // Note Off
		{
			packetSize = onNoteOff (data, dataSize);
			break;
		}
		case 0xa0: // Aftertouch
		{
			packetSize = onAftertouch (data, dataSize);
			break;
		}
		case 0xb0: // Controller
		{
			packetSize = onController (data, dataSize);
			break;
		}
		case 0xc0: // Program Change
		{
			packetSize = onProgramChange (data, dataSize);
			break;
		}
		case 0xd0: // Channel Pressure
		{
			packetSize = onChannelPressure (data, dataSize);
			break;
		}
		case 0xe0: // Pitch Wheel
		{
			packetSize = onPitchWheel (data, dataSize);
			break;
		}
		case 0xf0: // System Realtime Messages
		{
			impl->delegate->onOtherInput (data, 1);
			packetSize = 1;
			break;
		}
		default:
		{
			// Ehm...
			assert (false);
			return;
		}
	}
	if (dataSize > static_cast<size_t> (packetSize))
		processMIDIInput (data + packetSize, dataSize - packetSize);
}

//------------------------------------------------------------------------
} // MPE
} // Vst
} // Steinberg
