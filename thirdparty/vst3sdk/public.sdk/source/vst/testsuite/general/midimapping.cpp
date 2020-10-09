//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/general/midimapping.cpp
// Created by  : Steinberg, 04/2005
// Description : VST Test Suite
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

#include "public.sdk/source/vst/testsuite/general/midimapping.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include <unordered_set>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// MidiMappingTest
//------------------------------------------------------------------------
MidiMappingTest::MidiMappingTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API MidiMappingTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<IMidiMapping> midiMapping (controller);
	if (!midiMapping)
	{
		addMessage (testResult, STR ("No MIDI Mapping interface supplied!"));
		return true;
	}

	int32 numParameters = controller->getParameterCount ();
	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);
	bool interruptProcess = false;

	std::unordered_set<ParamID> parameterIds;
	for (int32 i = 0; i < numParameters; ++i)
	{
		ParameterInfo parameterInfo;
		if (controller->getParameterInfo (i, parameterInfo) == kResultTrue)
			parameterIds.insert (parameterInfo.id);
	}
	for (int32 bus = 0; bus < eventBusCount + 1; bus++)
	{
		if (interruptProcess)
			break;

		BusInfo info;
		if (vstPlug->getBusInfo (kEvent, kInput, bus, info) == kResultTrue)
		{
			if (bus >= eventBusCount)
			{
				addMessage (testResult, STR ("getBusInfo supplied for an unknown event bus"));
				break;
			}
		}
		else
			break;

		for (int16 channel = 0; channel < info.channelCount; channel++)
		{
			if (interruptProcess)
				break;

			int32 foundCount = 0;
			// test with the cc outside the valid range too (>=kCountCtrlNumber)
			for (CtrlNumber cc = 0; cc < kCountCtrlNumber + 1; cc++)
			{
				ParamID tag;
				if (midiMapping->getMidiControllerAssignment (bus, channel, cc, tag) == kResultTrue)
				{
					if (bus >= eventBusCount)
					{
						addMessage (testResult,
						            STR ("MIDI Mapping supplied for an unknown event bus"));
						interruptProcess = true;
						break;
					}
					if (cc >= kCountCtrlNumber)
					{
						addMessage (
						    testResult,
						    STR (
						        "MIDI Mapping supplied for a wrong ControllerNumbers value (bigger than the max)"));
						break;
					}
					if (parameterIds.find (tag) == parameterIds.end ())
					{
						addErrorMessage (
						    testResult,
						    printf ("Unknown ParamID [%d] returned for MIDI Mapping", tag));
						return false;
					}
					foundCount++;
				}
				else
				{
					if (bus >= eventBusCount)
						interruptProcess = true;
				}
			}
			if (foundCount == 0 && (bus < eventBusCount))
			{
				addMessage (
				    testResult,
				    printf (
				        "MIDI Mapping getMidiControllerAssignment (%d, %d) : no assignment available!",
				        bus, channel));
			}
		}
	}

	return true;
}
//------------------------------------------------------------------------
} // Vst
} // Steinberg
