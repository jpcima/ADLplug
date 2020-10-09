//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/processing/process.cpp
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

#include "public.sdk/source/vst/testsuite/processing/process.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ProcessTest
//------------------------------------------------------------------------
ProcessTest::ProcessTest (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: TestEnh (plugProvider, sampl)
{
	processData.numSamples = TestDefaults::instance ().defaultBlockSize;
	processData.symbolicSampleSize = sampl;

	processSetup.processMode = kRealtime;
	processSetup.symbolicSampleSize = sampl;
	processSetup.maxSamplesPerBlock = TestDefaults::instance ().maxBlockSize;
	processSetup.sampleRate = TestDefaults::instance ().defaultSampleRate;
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessTest::setup ()
{
	if (!TestEnh::setup ())
		return false;
	if (!vstPlug || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (audioEffect->canProcessSampleSize (processSetup.symbolicSampleSize) != kResultOk)
		return true; // this fails in run (..)

	prepareProcessing ();

	if (vstPlug->setActive (true) != kResultTrue)
		return false;
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessTest::run (ITestResult* testResult)
{
	if (!testResult || !audioEffect)
		return false;

	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (!canProcessSampleSize (testResult))
		return true;

	audioEffect->setProcessing (true);

	for (int32 i = 0; i < TestDefaults::instance ().numAudioBlocksToProcess; ++i)
	{
		if (!preProcess (testResult))
			return false;
		tresult result = audioEffect->process (processData);
		if (result != kResultOk)
		{
			if (processSetup.symbolicSampleSize == kSample32)
				addErrorMessage (testResult,
				                 STR ("IAudioProcessor::process (..with kSample32..) failed."));
			else
				addErrorMessage (testResult,
				                 STR ("IAudioProcessor::process (..with kSample64..) failed."));

			audioEffect->setProcessing (false);
			return false;
		}
		if (!postProcess (testResult))
		{
			audioEffect->setProcessing (false);
			return false;
		}
	}

	audioEffect->setProcessing (false);
	return true;
}

//------------------------------------------------------------------------
bool ProcessTest::preProcess (ITestResult* /*testResult*/)
{
	return true;
}

//------------------------------------------------------------------------
bool ProcessTest::postProcess (ITestResult* /*testResult*/)
{
	return true;
}

//------------------------------------------------------------------------
bool ProcessTest::canProcessSampleSize (ITestResult* testResult)
{
	if (!testResult || !audioEffect)
		return false;
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	if (audioEffect->canProcessSampleSize (processSetup.symbolicSampleSize) != kResultOk)
	{
		if (processSetup.symbolicSampleSize == kSample32)
			addMessage (testResult, STR ("32bit Audio Processing not supported."));
		else
			addMessage (testResult, STR ("64bit Audio Processing not supported."));
		return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool PLUGIN_API ProcessTest::teardown ()
{
	unprepareProcessing ();
	if (!vstPlug || (vstPlug->setActive (false) != kResultOk))
		return false;
	return TestEnh::teardown ();
}

//------------------------------------------------------------------------
bool ProcessTest::prepareProcessing ()
{
	if (!vstPlug || !audioEffect)
		return false;

	if (audioEffect->setupProcessing (processSetup) == kResultOk)
	{
		processData.prepare (*vstPlug, 0, processSetup.symbolicSampleSize);

		for (BusDirection dir = kInput; dir <= kOutput; dir++)
		{
			int32 numBusses = vstPlug->getBusCount (kAudio, dir);
			AudioBusBuffers* audioBuffers =
			    dir == kInput ? processData.inputs :
			                    processData.outputs; // new AudioBusBuffers [numBusses];
			if (!setupBuffers (numBusses, audioBuffers, dir))
				return false;

			if (dir == kInput)
			{
				processData.numInputs = numBusses;
				processData.inputs = audioBuffers;
			}
			else
			{
				processData.numOutputs = numBusses;
				processData.outputs = audioBuffers;
			}
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------
bool ProcessTest::setupBuffers (int32 numBusses, AudioBusBuffers* audioBuffers, BusDirection dir)
{
	if (((numBusses > 0) && !audioBuffers) || !vstPlug)
		return false;
	for (int32 busIndex = 0; busIndex < numBusses; busIndex++) // buses
	{
		BusInfo busInfo;
		if (vstPlug->getBusInfo (kAudio, dir, busIndex, busInfo) == kResultTrue)
		{
			if (!setupBuffers (audioBuffers[busIndex]))
				return false;

			if ((busInfo.flags & BusInfo::kDefaultActive) != 0)
			{
				for (int32 chIdx = 0; chIdx < busInfo.channelCount; chIdx++) // channels per bus
					audioBuffers[busIndex].silenceFlags |=
					    (TestDefaults::instance ().channelIsSilent << chIdx);
			}
		}
		else
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool ProcessTest::setupBuffers (AudioBusBuffers& audioBuffers)
{
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	audioBuffers.silenceFlags = 0;
	for (int32 chIdx = 0; chIdx < audioBuffers.numChannels; chIdx++)
	{
		if (processSetup.symbolicSampleSize == kSample32)
		{
			if (audioBuffers.channelBuffers32)
			{
				audioBuffers.channelBuffers32[chIdx] =
				    new Sample32[processSetup.maxSamplesPerBlock];
				if (audioBuffers.channelBuffers32[chIdx])
					memset (audioBuffers.channelBuffers32[chIdx], 0,
					        processSetup.maxSamplesPerBlock * sizeof (Sample32));
				else
					return false;
			}
			else
				return false;
		}
		else if (processSetup.symbolicSampleSize == kSample64)
		{
			if (audioBuffers.channelBuffers64)
			{
				audioBuffers.channelBuffers64[chIdx] =
				    new Sample64[processSetup.maxSamplesPerBlock];
				if (audioBuffers.channelBuffers64[chIdx])
					memset (audioBuffers.channelBuffers64[chIdx], 0,
					        processSetup.maxSamplesPerBlock * sizeof (Sample64));
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

//------------------------------------------------------------------------
bool ProcessTest::unprepareProcessing ()
{
	bool ret = true;
	ret &= freeBuffers (processData.numInputs, processData.inputs);
	ret &= freeBuffers (processData.numOutputs, processData.outputs);
	processData.unprepare ();
	return ret;
}

//------------------------------------------------------------------------
bool ProcessTest::freeBuffers (int32 numBuses, AudioBusBuffers* buses)
{
	if (processSetup.symbolicSampleSize != processData.symbolicSampleSize)
		return false;
	for (int32 busIndex = 0; busIndex < numBuses; busIndex++)
	{
		for (int32 chIdx = 0; chIdx < buses[busIndex].numChannels; chIdx++)
		{
			if (processSetup.symbolicSampleSize == kSample32)
				delete[] buses[busIndex].channelBuffers32[chIdx];
			else if (processSetup.symbolicSampleSize == kSample64)
				delete[] buses[busIndex].channelBuffers64[chIdx];
			else
				return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
