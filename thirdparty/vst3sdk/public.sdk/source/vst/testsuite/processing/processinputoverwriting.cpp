//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/processinputoverwriting.cpp
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

#include "public.sdk/source/vst/testsuite/processing/processinputoverwriting.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ProcessInputOverwritingTest
//------------------------------------------------------------------------
ProcessInputOverwritingTest::ProcessInputOverwritingTest (ITestPlugProvider* plugProvider,
                                                          ProcessSampleSize sampl)
: ProcessTest (plugProvider, sampl)
{
}

//------------------------------------------------------------------------
bool ProcessInputOverwritingTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	bool ret = ProcessTest::run (testResult);
	return ret;
}

//------------------------------------------------------------------------
bool ProcessInputOverwritingTest::preProcess (ITestResult* /*testResult*/)
{
	int32 min = processData.numInputs < processData.numOutputs ? processData.numInputs :
	                                                             processData.numOutputs;
	noNeedtoProcess = true;

	for (int32 i = 0; i < min; i++)
	{
		if (!noNeedtoProcess)
			break;

		int32 minChannel = processData.inputs[i].numChannels < processData.outputs[i].numChannels ?
		                       processData.inputs[i].numChannels :
		                       processData.outputs[i].numChannels;

		auto ptrIn = processData.inputs[i].channelBuffers32;
		auto ptrOut = processData.outputs[i].channelBuffers32;
		for (int32 j = 0; j < minChannel; j++)
		{
			if (ptrIn[j] != ptrOut[j])
			{
				noNeedtoProcess = false;
				break;
			}
		}
	}
	if (noNeedtoProcess)
		return true;

	for (int32 i = 0; i < processData.numInputs; i++)
	{
		if (processSetup.symbolicSampleSize == kSample32)
		{
			auto ptr = processData.inputs[i].channelBuffers32;
			if (ptr)
			{
				float inc = 1.0 / (processData.numSamples - 1);
				for (int32 c = 0; c < processData.inputs[i].numChannels; c++)
				{
					auto chaBuf = ptr[c];
					for (int32 j = 0; j < processData.numSamples; j++)
					{
						*chaBuf = inc * j;
						chaBuf++;
					}
				}
			}
		}
		else if (processSetup.symbolicSampleSize == kSample64)
		{
			auto ptr = processData.inputs[i].channelBuffers64;
			if (ptr)
			{
				float inc = 1.0 / (processData.numSamples - 1);
				for (int32 c = 0; c < processData.inputs[i].numChannels; c++)
				{
					auto chaBuf = ptr[c];
					for (int32 j = 0; j < processData.numSamples; j++)
					{
						*chaBuf = inc * j;
						chaBuf++;
					}
				}
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------
bool ProcessInputOverwritingTest::postProcess (ITestResult* testResult)
{
	if (noNeedtoProcess)
		return true;

	for (int32 i = 0; i < processData.numInputs; i++)
	{
		if (processSetup.symbolicSampleSize == kSample32)
		{
			auto ptr = processData.inputs[i].channelBuffers32;
			if (ptr)
			{
				float inc = 1.0 / (processData.numSamples - 1);
				for (int32 c = 0; c < processData.inputs[i].numChannels; c++)
				{
					auto chaBuf = ptr[c];
					for (int32 j = 0; j < processData.numSamples; j++)
					{
						if (*chaBuf != inc * j)
						{
							addErrorMessage (
							    testResult,
							    STR (
							        "IAudioProcessor::process overwrites input buffer (..with kSample32..)!"));
							return false;
						}
						chaBuf++;
					}
				}
			}
		}
		else if (processSetup.symbolicSampleSize == kSample64)
		{
			auto ptr = processData.inputs[i].channelBuffers64;
			if (ptr)
			{
				float inc = 1.0 / (processData.numSamples - 1);
				for (int32 c = 0; c < processData.inputs[i].numChannels; c++)
				{
					auto chaBuf = ptr[c];
					for (int32 j = 0; j < processData.numSamples; j++)
					{
						if (*chaBuf != inc * j)
						{
							addErrorMessage (
							    testResult,
							    STR (
							        "IAudioProcessor::process overwrites input buffer (..with kSample64..)!"));
							return false;
						}
						chaBuf++;
					}
				}
			}
		}
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
