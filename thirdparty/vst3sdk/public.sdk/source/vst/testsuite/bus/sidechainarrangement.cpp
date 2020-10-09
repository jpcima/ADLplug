//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/bus/sidechainarrangement.cpp
// Created by  : Steinberg, 11/2019
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

#include "public.sdk/source/vst/testsuite/bus/sidechainarrangement.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SideChainArrangementTest
//------------------------------------------------------------------------
SideChainArrangementTest::SideChainArrangementTest (ITestPlugProvider* plugProvider)
: TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API SideChainArrangementTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	bool failed = false;

	FUnknownPtr<IAudioProcessor> audioEffect (vstPlug);
	if (!audioEffect)
		return failed;
	// get the side chain arrangements
	// set Main/first Input and output to Mono
	// get the current arrangement and compare

	// check if Audio sideChain is supported
	bool hasInputSideChain = false;
	int32 numInBusses = vstPlug->getBusCount (kAudio, kInput);
	if (numInBusses < 2)
		return true;

	for (int32 busIndex = 0; busIndex < numInBusses; busIndex++)
	{
		BusInfo info;
		if (vstPlug->getBusInfo (kAudio, kInput, busIndex, info) != kResultTrue)
		{
			addErrorMessage (testResult, STR ("IComponent::getBusInfo (..) failed."));
			continue;
		}
		if (info.busType == kAux)
			hasInputSideChain = true;
	}
	if (!hasInputSideChain)
		return true;

	auto* inputArrArray = new SpeakerArrangement[numInBusses];
	for (int32 busIndex = 0; busIndex < numInBusses; busIndex++)
	{
		if (audioEffect->getBusArrangement (kInput, busIndex, inputArrArray[busIndex]) !=
		    kResultTrue)
		{
			addErrorMessage (testResult, STR ("IComponent::getBusArrangement (..) failed."));
		}
	}

	int32 numOutBusses = vstPlug->getBusCount (kAudio, kOutput);
	SpeakerArrangement* outputArrArray = nullptr;
	if (numOutBusses > 0)
	{
		outputArrArray = new SpeakerArrangement[numOutBusses];
		for (int32 busIndex = 0; busIndex < numOutBusses; busIndex++)
		{
			if (audioEffect->getBusArrangement (kOutput, busIndex, outputArrArray[busIndex]) !=
			    kResultTrue)
			{
				addErrorMessage (testResult, STR ("IComponent::getBusArrangement (..) failed."));
			}
		}
		outputArrArray[0] = kSpeakerM;
	}
	inputArrArray[0] = kSpeakerM;

	if (audioEffect->setBusArrangements (inputArrArray, numInBusses, outputArrArray,
	                                     numOutBusses) == kResultTrue)
	{
		for (int32 busIndex = 0; busIndex < numInBusses; busIndex++)
		{
			SpeakerArrangement tmp;
			if (audioEffect->getBusArrangement (kInput, busIndex, tmp) == kResultTrue)
			{
				if (tmp != inputArrArray[busIndex])
				{
					addErrorMessage (
					    testResult,
					    printf (
					        "Input %d: setBusArrangements was returning kResultTrue but getBusArrangement returns different arrangement!",
					        busIndex));
					failed = true;
				}
			}
		}
		for (int32 busIndex = 0; busIndex < numOutBusses; busIndex++)
		{
			SpeakerArrangement tmp;
			if (audioEffect->getBusArrangement (kOutput, busIndex, tmp) != kResultTrue)
			{
				if (tmp != outputArrArray[busIndex])
				{
					addErrorMessage (
					    testResult,
					    printf (
					        "Output %d: setBusArrangements was returning kResultTrue but getBusArrangement returns different arrangement!",
					        busIndex));
					failed = true;
				}
			}
		}
	}

	return failed == false;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
