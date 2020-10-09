//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/testbase.cpp
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

#include "public.sdk/source/vst/testsuite/testbase.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include <cstdarg>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// TestBase
//------------------------------------------------------------------------
TestBase::TestBase (ITestPlugProvider* plugProvider)
: plugProvider (plugProvider)
, vstPlug (nullptr)
, controller (nullptr) {FUNKNOWN_CTOR}

//------------------------------------------------------------------------
TestBase::TestBase ()
: plugProvider (nullptr)
, vstPlug (nullptr)
, controller (nullptr) {FUNKNOWN_CTOR}

//------------------------------------------------------------------------
TestBase::~TestBase () {FUNKNOWN_DTOR}

//------------------------------------------------------------------------
IMPLEMENT_FUNKNOWN_METHODS (TestBase, ITest, ITest::iid);

//------------------------------------------------------------------------
bool TestBase::setup ()
{
	if (plugProvider)
	{
		vstPlug = plugProvider->getComponent ();
		controller = plugProvider->getController ();

		return activateMainIOBusses (true);
	}
	return false;
}

//------------------------------------------------------------------------
bool TestBase::teardown ()
{
	if (vstPlug)
	{
		activateMainIOBusses (false);
		plugProvider->releasePlugIn (vstPlug, controller);
	}
	return true;
}

//------------------------------------------------------------------------
bool TestBase::activateMainIOBusses (bool val)
{
	if (vstPlug)
	{
		if (auto countIn = vstPlug->getBusCount (kAudio, kInput) > 0)
			vstPlug->activateBus (kAudio, kInput, 0, val);
		if (auto countOut = vstPlug->getBusCount (kAudio, kOutput) > 0)
			vstPlug->activateBus (kAudio, kOutput, 0, val);

		return true;
	}

	return false;
}

//------------------------------------------------------------------------
void TestBase::printTestHeader (ITestResult* testResult)
{
	using VST3::StringConvert::convert;

	std::string str = "===";
	str += getName ();
	str += " ====================================";
	addMessage (testResult, convert (str));
}

//------------------------------------------------------------------------
// Component Initialize / Terminate
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// VstTestEnh
//------------------------------------------------------------------------
TestEnh::TestEnh (ITestPlugProvider* plugProvider, ProcessSampleSize sampl)
: TestBase (plugProvider), audioEffect (nullptr)
{
	// process setup defaults
	memset (&processSetup, 0, sizeof (ProcessSetup));

	processSetup.processMode = kRealtime;
	processSetup.symbolicSampleSize = sampl;
	processSetup.maxSamplesPerBlock = kMaxSamplesPerBlock;
	processSetup.sampleRate = kSampleRate;
}

//------------------------------------------------------------------------
TestEnh::~TestEnh ()
{
}

//------------------------------------------------------------------------
bool TestEnh::setup ()
{
	bool res = TestBase::setup ();

	if (vstPlug)
	{
		tresult check = vstPlug->queryInterface (IAudioProcessor::iid, (void**)&audioEffect);
		if (check != kResultTrue)
			return false;
	}

	return (res && audioEffect);
}

//------------------------------------------------------------------------
bool TestEnh::teardown ()
{
	if (audioEffect)
		audioEffect->release ();

	bool res = TestBase::teardown ();

	return res && audioEffect;
}

//------------------------------------------------------------------------
void addMessage (ITestResult* testResult, const std::u16string& str)
{
	testResult->addMessage (reinterpret_cast<const tchar*> (str.data ()));
}

//------------------------------------------------------------------------
void addMessage (ITestResult* testResult, const tchar* str)
{
	testResult->addMessage (str);
}

//------------------------------------------------------------------------
void addErrorMessage (ITestResult* testResult, const tchar* str)
{
	testResult->addErrorMessage (str);
}

//------------------------------------------------------------------------
void addErrorMessage (ITestResult* testResult, const std::u16string& str)
{
	testResult->addErrorMessage (reinterpret_cast<const tchar*> (str.data ()));
}

//------------------------------------------------------------------------
std::u16string printf (const char8* format, ...)
{
	using VST3::StringConvert::convert;

	char8 string[1024 * 4];

	va_list marker;
	va_start (marker, format);

	vsnprintf (string, kPrintfBufferSize, format, marker);
	return convert (string).data ();
}

IMPLEMENT_FUNKNOWN_METHODS (ParamChanges, IParamValueQueue, IParamValueQueue::iid)

//------------------------------------------------------------------------
ParamChanges::ParamChanges () {FUNKNOWN_CTOR}

//------------------------------------------------------------------------
ParamChanges::~ParamChanges ()
{
	if (points)
		delete[] points;
	FUNKNOWN_DTOR
}

//------------------------------------------------------------------------
void ParamChanges::init (ParamID _id, int32 _numPoints)
{
	id = _id;
	numPoints = _numPoints;
	numUsedPoints = 0;
	if (points)
		delete[] points;
	points = new ParamPoint[numPoints];
	processedFrames = 0;
}

//------------------------------------------------------------------------
bool ParamChanges::setPoint (int32 index, int32 offsetSamples, double value)
{
	if (points && (index >= 0) && (index == numUsedPoints) && (index < numPoints))
	{
		points[index].set (offsetSamples, value);
		numUsedPoints++;
		return true;
	}
	if (!points)
		return true;
	return false;
}

//------------------------------------------------------------------------
void ParamChanges::resetPoints ()
{
	numUsedPoints = 0;
	processedFrames = 0;
}

//------------------------------------------------------------------------
int32 ParamChanges::getProcessedFrames () const
{
	return processedFrames;
}

//------------------------------------------------------------------------
void ParamChanges::setProcessedFrames (int32 amount)
{
	processedFrames = amount;
}

//------------------------------------------------------------------------
bool ParamChanges::havePointsBeenRead (bool atAll)
{
	for (int32 i = 0; i < getPointCount (); ++i)
	{
		if (points[i].wasRead ())
		{
			if (atAll)
				return true;
		}
		else if (!atAll)
			return false;
	}
	return !atAll;
}

//------------------------------------------------------------------------
ParamID PLUGIN_API ParamChanges::getParameterId ()
{
	return id;
}

//------------------------------------------------------------------------
int32 PLUGIN_API ParamChanges::getPointCount ()
{
	return numUsedPoints;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ParamChanges::getPoint (int32 index, int32& offsetSamples, double& value)
{
	if (points && (index < numUsedPoints) && (index >= 0))
	{
		points[index].get (offsetSamples, value);
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ParamChanges::addPoint (int32 /*offsetSamples*/, double /*value*/,
                                           int32& /*index*/)
{
	return kResultFalse;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
