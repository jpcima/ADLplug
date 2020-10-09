//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/speakerarrangement.cpp
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

#include "public.sdk/source/vst/testsuite/processing/speakerarrangement.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SpeakerArrangementTest
//------------------------------------------------------------------------
SpeakerArrangementTest::SpeakerArrangementTest (ITestPlugProvider* plugProvider,
                                                ProcessSampleSize sampl, SpeakerArrangement inSpArr,
                                                SpeakerArrangement outSpArr)
: ProcessTest (plugProvider, sampl), inSpArr (inSpArr), outSpArr (outSpArr)
{
}

//------------------------------------------------------------------------
const char* SpeakerArrangementTest::getSpeakerArrangementName (SpeakerArrangement spArr)
{
	const char* saName = nullptr;
	switch (spArr)
	{
		case SpeakerArr::kMono: saName = "Mono"; break;
		case SpeakerArr::kStereo: saName = "Stereo"; break;
		case SpeakerArr::kStereoSurround: saName = "StereoSurround"; break;
		case SpeakerArr::kStereoCenter: saName = "StereoCenter"; break;
		case SpeakerArr::kStereoSide: saName = "StereoSide"; break;
		case SpeakerArr::kStereoCLfe: saName = "StereoCLfe"; break;
		case SpeakerArr::k30Cine: saName = "30Cine"; break;
		case SpeakerArr::k30Music: saName = "30Music"; break;
		case SpeakerArr::k31Cine: saName = "31Cine"; break;
		case SpeakerArr::k31Music: saName = "31Music"; break;
		case SpeakerArr::k40Cine: saName = "40Cine"; break;
		case SpeakerArr::k40Music: saName = "40Music"; break;
		case SpeakerArr::k41Cine: saName = "41Cine"; break;
		case SpeakerArr::k41Music: saName = "41Music"; break;
		case SpeakerArr::k50: saName = "50"; break;
		case SpeakerArr::k51: saName = "51"; break;
		case SpeakerArr::k60Cine: saName = "60Cine"; break;
		case SpeakerArr::k60Music: saName = "60Music"; break;
		case SpeakerArr::k61Cine: saName = "61Cine"; break;
		case SpeakerArr::k61Music: saName = "61Music"; break;
		case SpeakerArr::k70Cine: saName = "70Cine"; break;
		case SpeakerArr::k70Music: saName = "70Music"; break;
		case SpeakerArr::k71Cine: saName = "71Cine"; break;
		case SpeakerArr::k71Music: saName = "71Music"; break;
		case SpeakerArr::k80Cine: saName = "80Cine"; break;
		case SpeakerArr::k80Music: saName = "80Music"; break;
		case SpeakerArr::k81Cine: saName = "81Cine"; break;
		case SpeakerArr::k81Music: saName = "81Music"; break;
		case SpeakerArr::k102: saName = "102"; break;
		case SpeakerArr::k122: saName = "122"; break;
		case SpeakerArr::k80Cube: saName = "80Cube"; break;
		case SpeakerArr::k90: saName = "9.0"; break;
		case SpeakerArr::k91: saName = "9.1"; break;
		case SpeakerArr::k100: saName = "10.0"; break;
		case SpeakerArr::k101: saName = "10.1"; break;
		case SpeakerArr::k110: saName = "11.0"; break;
		case SpeakerArr::k111: saName = "11.1"; break;
		case SpeakerArr::k130: saName = "13.0"; break;
		case SpeakerArr::k131: saName = "13.1"; break;
		case SpeakerArr::k222: saName = "22.2"; break;
		case SpeakerArr::kEmpty: saName = "Empty"; break;
		default: saName = "Unknown"; break;
	}
	return saName;
}

//------------------------------------------------------------------------
const char* SpeakerArrangementTest::getName () const
{
	const auto inSaName = getSpeakerArrangementName (inSpArr);
	const auto outSaName = getSpeakerArrangementName (outSpArr);
	if (inSaName && outSaName)
	{
		static std::string str;
		str = "In: ";
		str += inSaName;
		str += ": ";
		str += std::to_string (SpeakerArr::getChannelCount (inSpArr));
		str += " Channels, Out: ";
		str += outSaName;
		str += ": ";
		str += std::to_string (SpeakerArr::getChannelCount (outSpArr));
		str += " Channels";
		return str.data ();
	}
	return "error";
}

//------------------------------------------------------------------------
bool SpeakerArrangementTest::prepareProcessing ()
{
	if (!vstPlug || !audioEffect)
		return false;

	bool ret = true;
	int32 is = vstPlug->getBusCount (kAudio, kInput);
	auto* inSpArrs = new SpeakerArrangement[is];
	for (int32 i = 0; i < is; ++i)
		inSpArrs[i] = inSpArr;

	int32 os = vstPlug->getBusCount (kAudio, kOutput);
	auto* outSpArrs = new SpeakerArrangement[os];
	for (int32 o = 0; o < os; o++)
		outSpArrs[o] = outSpArr;

	if (audioEffect->setBusArrangements (inSpArrs, is, outSpArrs, os) != kResultTrue)
		ret = false;

	// activate only the extra IO (index > 0), the main ones (index 0) were already activated in
	// TestBase::setup ()
	for (int32 i = 1; i < is; i++)
		vstPlug->activateBus (kAudio, kInput, i, true);

	for (int32 i = 1; i < os; i++)
		vstPlug->activateBus (kAudio, kOutput, i, true);

	ret &= ProcessTest::prepareProcessing ();

	delete[] inSpArrs;
	delete[] outSpArrs;

	return ret;
}

//------------------------------------------------------------------------
bool SpeakerArrangementTest::run (ITestResult* testResult)
{
	if (!testResult || !audioEffect || !vstPlug)
		return false;

	printTestHeader (testResult);

	SpeakerArrangement spArr = SpeakerArr::kEmpty;
	SpeakerArrangement compareSpArr = SpeakerArr::kEmpty;
	BusDirections bd = kInput;
	BusInfo busInfo = {};
	int32 count = 0;
	do
	{
		count++;
		int32 numBusses = 0;
		if (bd == kInput)
		{
			numBusses = processData.numInputs;
			compareSpArr = inSpArr;
		}
		else
		{
			numBusses = processData.numOutputs;
			compareSpArr = outSpArr;
		}
		for (int32 i = 0; i < numBusses; ++i)
		{
			if (audioEffect->getBusArrangement (bd, i, spArr) != kResultTrue)
			{
				addErrorMessage (testResult,
				                 STR ("IAudioProcessor::getBusArrangement (..) failed."));
				return false;
			}
			if (spArr != compareSpArr)
			{
				addMessage (
				    testResult,
				    printf ("    %s %sSpeakerArrangement is not supported. Plug-in suggests: %s.",
				            getSpeakerArrangementName (compareSpArr),
				            bd == kInput ? "Input-" : "Output-",
				            getSpeakerArrangementName (spArr)));
			}
			if (vstPlug->getBusInfo (kAudio, bd, i, busInfo) != kResultTrue)
			{
				addErrorMessage (testResult, STR ("IComponent::getBusInfo (..) failed."));
				return false;
			}
			if (spArr == compareSpArr &&
			    SpeakerArr::getChannelCount (spArr) != busInfo.channelCount)
			{
				addErrorMessage (
				    testResult,
				    STR ("SpeakerArrangement mismatch (BusInfo::channelCount inconsistency)."));
				return false;
			}
		}
		bd = kOutput;
	} while (count < 2);

	bool ret = true;

	// not a Pb ret &= verifySA (processData.numInputs, processData.inputs, inSpArr, testResult);
	// not a Pb ret &= verifySA (processData.numOutputs, processData.outputs, outSpArr, testResult);
	ret &= ProcessTest::run (testResult);

	return ret;
}

//------------------------------------------------------------------------
bool SpeakerArrangementTest::verifySA (int32 numBusses, AudioBusBuffers* buses,
                                       SpeakerArrangement spArr, ITestResult* testResult)
{
	if (!testResult || !buses)
		return false;
	for (int32 i = 0; i < numBusses; ++i)
	{
		if (buses[i].numChannels != SpeakerArr::getChannelCount (spArr))
		{
			addErrorMessage (testResult, STR ("ChannelCount is not matching SpeakerArrangement."));
			return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
