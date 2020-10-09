//-----------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/unit/scanprograms.cpp
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

#include "public.sdk/source/vst/testsuite/unit/scanprograms.h"
#include "public.sdk/source/vst/utility/stringconvert.h"
#include "pluginterfaces/vst/ivstunits.h"
#include "pluginterfaces/vst/vstpresetkeys.h"
#include <memory>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
// ProgramInfoTest
//------------------------------------------------------------------------
ProgramInfoTest::ProgramInfoTest (ITestPlugProvider* plugProvider) : TestBase (plugProvider)
{
}

//------------------------------------------------------------------------
bool ProgramInfoTest::run (ITestResult* testResult)
{
	if (!testResult || !vstPlug)
		return false;

	printTestHeader (testResult);

	FUnknownPtr<IUnitInfo> iUnitInfo (controller);
	if (iUnitInfo)
	{
		int32 programListCount = iUnitInfo->getProgramListCount ();
		if (programListCount == 0)
		{
			addMessage (testResult, STR ("This component does not export any programs."));
			return true;
		}
		else if (programListCount < 0)
		{
			addErrorMessage (testResult,
			                 STR ("IUnitInfo::getProgramListCount () returned a negative number."));
			return false;
		}

		// used to check double IDs
		auto programListIds = std::unique_ptr<int32[]> (new int32[programListCount]);

		for (int32 programListIndex = 0; programListIndex < programListCount; programListIndex++)
		{
			// get programm list info
			ProgramListInfo programListInfo;
			if (iUnitInfo->getProgramListInfo (programListIndex, programListInfo) == kResultOk)
			{
				int32 programListId = programListInfo.id;
				programListIds[programListIndex] = programListId;
				if (programListId < 0)
				{
					addErrorMessage (testResult,
					                 printf ("Programlist %03d: Invalid ID!!!", programListIndex));
					return false;
				}

				// check if ID is already used by another parameter
				for (int32 idIndex = 0; idIndex < programListIndex; idIndex++)
				{
					if (programListIds[idIndex] == programListIds[programListIndex])
					{
						addErrorMessage (testResult, printf ("Programlist %03d: ID already used!!!",
						                                     programListIndex));
						return false;
					}
				}

				auto programListName = VST3::StringConvert::convert (programListInfo.name);
				if (programListName.empty ())
				{
					addErrorMessage (testResult, printf ("Programlist %03d (id=%d): No name!!!",
					                                     programListIndex, programListId));
					return false;
				}

				int32 programCount = programListInfo.programCount;
				if (programCount <= 0)
				{
					addMessage (
					    testResult,
					    printf (
					        "Programlist %03d (id=%d): \"%s\" No programs!!! (programCount is null!)",
					        programListIndex, programListId,
					        VST3::StringConvert::convert (programListName).data ()));
					// return false;
				}

				addMessage (testResult, printf ("Programlist %03d (id=%d):  \"%s\" (%d programs).",
				                                programListIndex, programListId,
				                                programListName.data (), programCount));

				for (int32 programIndex = 0; programIndex < programCount; programIndex++)
				{
					TChar programName[256];
					if (iUnitInfo->getProgramName (programListId, programIndex, programName) ==
					    kResultOk)
					{
						if (programName[0] == 0)
						{
							addErrorMessage (
							    testResult,
							    printf ("Programlist %03d->Program %03d: has no name!!!",
							            programListIndex, programIndex));
							return false;
						}

						auto programNameUTF8 = VST3::StringConvert::convert (programName);
						auto msg = printf ("Programlist %03d->Program %03d: \"%s\"",
						                   programListIndex, programIndex, programNameUTF8.data ());

						String128 programInfo {};
						if (iUnitInfo->getProgramInfo (programListId, programIndex,
						                               PresetAttributes::kInstrument,
						                               programInfo) == kResultOk)
						{
							auto programInfoUTF8 = VST3::StringConvert::convert (programInfo);
							msg += VST3::StringConvert::convert (" (instrument = \"");
							msg += (const char16_t*)programInfo;
							msg += VST3::StringConvert::convert ("\")");
						}

						addMessage (testResult, msg.data ());

						if (iUnitInfo->hasProgramPitchNames (programListId, programIndex) ==
						    kResultOk)
						{
							addMessage (testResult, printf (" => \"%s\": supports PitchNames",
							                                programNameUTF8.data ()));

							String128 pitchName = {0};
							for (int16 midiPitch = 0; midiPitch < 128; midiPitch++)
							{
								if (iUnitInfo->getProgramPitchName (programListId, programIndex,
								                                    midiPitch,
								                                    pitchName) == kResultOk)
								{
									msg = printf ("   => MIDI Pitch %d => \"", midiPitch);
									msg += (const char16_t*)pitchName;
									msg += VST3::StringConvert::convert ("\"");
									addMessage (testResult, msg.data ());
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		addMessage (testResult, STR ("This component does not export any programs."));

		// check if not more than 1 program change parameter is defined
		int32 numPrgChanges = 0;
		for (int32 i = 0; i < controller->getParameterCount (); ++i)
		{
			ParameterInfo paramInfo = {};
			if (controller->getParameterInfo (i, paramInfo) != kResultOk)
			{
				if (paramInfo.flags & ParameterInfo::kIsProgramChange)
					numPrgChanges++;
			}
		}
		if (numPrgChanges > 1)
		{
			addErrorMessage (
			    testResult,
			    printf ("More than 1 programChange Parameter (%d) without support of IUnitInfo!!!",
			            numPrgChanges));
		}
	}
	return true;
}

//------------------------------------------------------------------------
} // Vst
} // Steinberg
