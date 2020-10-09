//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/processformat.cpp
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

#include "public.sdk/source/vst/testsuite/processing/processformat.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ProcessFormatTest
//------------------------------------------------------------------------
ProcessFormatTest::ProcessFormatTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: ProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessFormatTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;

	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (testResult);

	int32 numFails = 0;
	const int32 numRates = 12;
	SampleRate sampleRateFormats[numRates] = {22050.,    32000.,    44100.,    48000.,
	                                          88200.,    96000.,    192000.,   384000.,
	                                          1234.5678, 12345.678, 123456.78, 1234567.8};

	tresult result = vstPlug->setActive (false);
	if (result != kResultOk)
	{
		addErrorMessage (testResult, STR ("IComponent::setActive (false) failed."));
		return false;
	}

	addMessage (testResult, STR ("***Tested Sample Rates***"));

	for (int32 i = 0; i < numRates; ++i)
	{
		processSetup.sampleRate = sampleRateFormats[i];
		result = audioEffect->setupProcessing (processSetup);
		if (result == kResultOk)
		{
			result = vstPlug->setActive (true);
			if (result != kResultOk)
			{
				addErrorMessage (testResult, STR ("IComponent::setActive (true) failed."));
				return false;
			}

			audioEffect->setProcessing (true);
			result = audioEffect->process (processData);
			audioEffect->setProcessing (false);

			if (result == kResultOk)
			{
				addMessage (testResult,
				            printf (" %10.10G Hz - processed successfully!", sampleRateFormats[i]));
			}
			else
			{
				numFails++;
				addErrorMessage (testResult,
				                 printf (" %10.10G Hz - failed to process!", sampleRateFormats[i]));
			}

			result = vstPlug->setActive (false);
			if (result != kResultOk)
			{
				addErrorMessage (testResult, STR ("IComponent::setActive (false) failed."));
				return false;
			}
		}
		else if (sampleRateFormats[i] > 0.)
		{
			addErrorMessage (
			    testResult,
			    printf ("IAudioProcessor::setupProcessing (..) failed for samplerate %.3f Hz! ",
			            sampleRateFormats[i]));
			// return false;
		}
	}

	result = vstPlug->setActive (true);
	if (result != kResultOk)
		return false;

	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
