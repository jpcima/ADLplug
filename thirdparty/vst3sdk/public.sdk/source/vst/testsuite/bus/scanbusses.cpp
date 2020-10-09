//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/bus/scanbusses.cpp
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

#include "public.sdk/source/vst/testsuite/bus/scanbusses.h"
#include "public.sdk/source/vst/utility/stringconvert.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ScanBussesTest
//------------------------------------------------------------------------
ScanBussesTest::ScanBussesTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API ScanBussesTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	int32 numBusses = 0;

	for (MediaType mediaType = kAudio; mediaType < kNumMediaTypes; mediaType++)
	{
		int32 numInputs = vstPlug->getBusCount (mediaType, kInput);
		int32 numOutputs = vstPlug->getBusCount (mediaType, kOutput);

		numBusses += (numInputs + numOutputs);

		if ((mediaType == (kNumMediaTypes - 1)) && (numBusses == 0))
		{
			addErrorMessage (testResult, STR ("This component does not export any buses!!!"));
			return false;
		}

		addMessage (testResult,
		            printf ("=> %s Buses: [%d In(s) => %d Out(s)]",
		                    mediaType == kAudio ? "Audio" : "Event", numInputs, numOutputs));

		for (int32 i = 0; i < numInputs + numOutputs; ++i)
		{
			BusDirection busDirection = i < numInputs ? kInput : kOutput;
			int32 busIndex = busDirection == kInput ? i : i - numInputs;

			BusInfo busInfo = {};
			if (vstPlug->getBusInfo (mediaType, busDirection, busIndex, busInfo) == kResultTrue)
			{
				auto busName = VST3::StringConvert::convert (busInfo.name);

				if (busName.empty ())
				{
					addErrorMessage (testResult, printf ("Bus %d has no name!!!", busIndex));
					return false;
				}
				addMessage (
				    testResult,
				    printf ("     %s[%d]: \"%s\" (%s-%s) ", busDirection == kInput ? "In " : "Out",
				            busIndex, busName.data (), busInfo.busType == kMain ? "Main" : "Aux",
				            busInfo.kDefaultActive ? "Default Active" : "Default Inactive"));
			}
			else
				return false;
		}
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
