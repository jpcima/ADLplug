//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/noteexpression/keyswitch.cpp
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

#include "public.sdk/source/vst/testsuite/noteexpression/keyswitch.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// KeyswitchTest
//------------------------------------------------------------------------
KeyswitchTest::KeyswitchTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API KeyswitchTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<IKeyswitchController> keyswitch (controller);
	if (!keyswitch)
	{
		addMessage (testResult, STR ("No Keyswitch interface supplied!"));
		return true;
	}

	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);

	for (int32 bus = 0; bus < eventBusCount; bus++)
	{
		BusInfo busInfo;
		vstPlug->getBusInfo (kEvent, kInput, bus, busInfo);

		for (int16 channel = 0; channel < busInfo.channelCount; channel++)
		{
			int32 count = keyswitch->getKeyswitchCount (bus, channel);

			if (count > 0)
			{
				addMessage (testResult, printf ("Keyswitch support bus[%d], channel[%d]: %d", bus,
				                                channel, count));
			}

			for (int32 i = 0; i < count; ++i)
			{
				KeyswitchInfo info;
				if (keyswitch->getKeyswitchInfo (bus, channel, i, info) == kResultTrue)
				{
				}
				else
				{
					addErrorMessage (
					    testResult,
					    printf ("Keyswitch getKeyswitchInfo (%d, %d, %d) return kResultFalse!", bus,
					            channel, i));
					return false;
				}
			}
		}
	}

	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
