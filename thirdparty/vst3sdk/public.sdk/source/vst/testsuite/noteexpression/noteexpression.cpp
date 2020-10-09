//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/noteexpression/noteexpression.cpp
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

#include "public.sdk/source/vst/testsuite/noteexpression/noteexpression.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstphysicalui.h"

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// NoteExpressionTest
//------------------------------------------------------------------------
NoteExpressionTest::NoteExpressionTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool PLUGIN_API NoteExpressionTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	if (!controller)
	{
		addMessage (testResult, STR ("No Edit Controller supplied!"));
		return true;
	}

	FUnknownPtr<INoteExpressionController> noteExpression (controller);
	if (!noteExpression)
	{
		addMessage (testResult, STR ("No Note Expression interface supplied!"));
		return true;
	}

	FUnknownPtr<INoteExpressionPhysicalUIMapping> noteExpressionPUIMapping (controller);
	if (!noteExpressionPUIMapping)
	{
		addMessage (testResult, STR ("No Note Expression PhysicalUIMapping interface supplied!"));
	}

	int32 eventBusCount = vstPlug->getBusCount (kEvent, kInput);

	const uint32 maxPUI = kPUITypeCount;
	PhysicalUIMap puiArray[maxPUI];
	PhysicalUIMapList puiMap;
	puiMap.count = maxPUI;
	puiMap.map = puiArray;
	for (uint32 i = 0; i < maxPUI; i++)
	{
		puiMap.map[i].physicalUITypeID = static_cast<PhysicalUITypeID> (i);
	}

	for (int32 bus = 0; bus < eventBusCount; bus++)
	{
		BusInfo busInfo;
		vstPlug->getBusInfo (kEvent, kInput, bus, busInfo);

		for (int16 channel = 0; channel < busInfo.channelCount; channel++)
		{
			int32 count = noteExpression->getNoteExpressionCount (bus, channel);
			if (count > 0)
			{
				addMessage (testResult, printf ("Note Expression count bus[%d], channel[%d]: %d",
				                                bus, channel, count));
			}

			for (int32 i = 0; i < count; ++i)
			{
				NoteExpressionTypeInfo info;
				if (noteExpression->getNoteExpressionInfo (bus, channel, i, info) == kResultTrue)
				{
					addMessage (testResult,
					            printf ("Note Expression TypeID: %d [%s]", info.typeId,
					                    VST3::StringConvert::convert (info.title).data ()));
					NoteExpressionTypeID id = info.typeId;
					NoteExpressionValue valueNormalized = info.valueDesc.defaultValue;
					String128 string;
					if (noteExpression->getNoteExpressionStringByValue (
					        bus, channel, id, valueNormalized, string) != kResultTrue)
					{
						addMessage (
						    testResult,
						    printf (
						        "Note Expression getNoteExpressionStringByValue (%d, %d, %d) return kResultFalse!",
						        bus, channel, id));
					}

					if (noteExpression->getNoteExpressionValueByString (
					        bus, channel, id, string, valueNormalized) != kResultTrue)
					{
						addMessage (
						    testResult,
						    printf (
						        "Note Expression getNoteExpressionValueByString (%d, %d, %d) return kResultFalse!",
						        bus, channel, id));
					}
				}
				else
				{
					addErrorMessage (
					    testResult,
					    printf (
					        "Note Expression getNoteExpressionInfo (%d, %d, %d) return kResultFalse!",
					        bus, channel, i));
					return false;
				}
			}

			if (noteExpressionPUIMapping)
			{
				for (uint32 i = 0; i < maxPUI; i++)
				{
					puiMap.map[i].noteExpressionTypeID = kInvalidTypeID;
				}

				if (noteExpressionPUIMapping->getPhysicalUIMapping (bus, channel, puiMap) ==
				    kResultFalse)
				{
					addMessage (
					    testResult,
					    printf (
					        "Note Expression getPhysicalUIMapping (%d, %d, ...) return kResultFalse!",
					        bus, channel));
				}
				else
				{
					for (int32 i = 0; i < maxPUI; i++)
					{
						addMessage (testResult,
						            printf ("Note Expression PhysicalUIMapping: %d => %d",
						                    puiMap.map[i].noteExpressionTypeID,
						                    puiMap.map[i].physicalUITypeID));
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
