//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/silenceprocessing.cpp
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

#include "public.sdk/source/vst/testsuite/processing/silenceprocessing.h"
#include <cmath>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// SilenceProcessingTest
//------------------------------------------------------------------------
SilenceProcessingTest::SilenceProcessingTest (ITestPlugProvider* plugProvider,
                                              ProcessSampleSize sampl)
: ProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool SilenceProcessingTest::isBufferSilent (void* buffer, int32 numSamples, ProcessSampleSize sampl)
{
	if (sampl == kSample32)
	{
		const float kSilenceThreshold = 0.000132184039f;

		float* floatBuffer = (float*)buffer;
		while (numSamples--)
		{
			if (fabsf (*floatBuffer) > kSilenceThreshold)
				return false;
			floatBuffer++;
		}
	}
	else if (sampl == kSample64)
	{
		const double kSilenceThreshold = 0.000132184039;

		double* floatBuffer = (double*)buffer;
		while (numSamples--)
		{
			if (fabs (*floatBuffer) > kSilenceThreshold)
				return false;
			floatBuffer++;
		}
	}
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API SilenceProcessingTest::run (ITestResult* testResult)
{
	if (!vstPlug || !testResult || !audioEffect)
		return false;

	if (!canProcessSampleSize (testResult))
		return true;

	printTestHeader (testResult);

	if (processData.inputs != nullptr)
	{
		audioEffect->setProcessing (true);

		for (int32 busIndex = 0; busIndex < processData.numInputs; busIndex++)
		{
			processData.inputs[busIndex].silenceFlags = 0;
			for (int32 channelIndex = 0; channelIndex < processData.inputs[busIndex].numChannels;
			     channelIndex++)
			{
				processData.inputs[busIndex].silenceFlags |= (uint64)1 << (uint64)channelIndex;
				if (processData.symbolicSampleSize == kSample32)
					memset (processData.inputs[busIndex].channelBuffers32[channelIndex], 0,
					        sizeof (float) * processData.numSamples);
				else if (processData.symbolicSampleSize == kSample64)
					memset (processData.inputs[busIndex].channelBuffers32[channelIndex], 0,
					        sizeof (double) * processData.numSamples);
			}
		}

		for (int32 busIndex = 0; busIndex < processData.numOutputs; busIndex++)
		{
			if (processData.numInputs > busIndex)
				processData.outputs[busIndex].silenceFlags =
				    processData.inputs[busIndex].silenceFlags;
			else
			{
				processData.outputs[busIndex].silenceFlags = 0;
				for (int32 channelIndex = 0;
				     channelIndex < processData.inputs[busIndex].numChannels; channelIndex++)
					processData.outputs[busIndex].silenceFlags |= (uint64)1 << (uint64)channelIndex;
			}
		}

		tresult result = audioEffect->process (processData);
		if (result != kResultOk)
		{
			addErrorMessage (testResult, printf ("%s", "The component failed to process!"));

			audioEffect->setProcessing (false);
			return false;
		}

		for (int32 busIndex = 0; busIndex < processData.numOutputs; busIndex++)
		{
			for (int32 channelIndex = 0; channelIndex < processData.outputs[busIndex].numChannels;
			     channelIndex++)
			{
				bool channelShouldBeSilent = (processData.outputs[busIndex].silenceFlags &
				                              (uint64)1 << (uint64)channelIndex) != 0;
				bool channelIsSilent =
				    isBufferSilent (processData.outputs[busIndex].channelBuffers32[channelIndex],
				                    processData.numSamples, processData.symbolicSampleSize);
				if (channelShouldBeSilent != channelIsSilent)
				{
					constexpr auto silentText = STR (
					    "The component reported a wrong silent flag for its output buffer! : output is silent but silenceFlags not set !");
					constexpr auto nonSilentText = STR (
					    "The component reported a wrong silent flag for its output buffer! : silenceFlags is set to silence but output is not silent");
					addMessage (testResult, channelIsSilent ? silentText : nonSilentText);
					break;
				}
			}
		}
	}
	else if (processData.numInputs > 0)
	{
		addErrorMessage (testResult,
		                 STR ("ProcessData::inputs are 0 but ProcessData::numInputs are nonzero."));
		return false;
	}

	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
