//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Validator
// Filename    : public.sdk/source/vst/testsuite/vststructsizecheck.h
// Created by  : Steinberg, 09/2010
// Description : struct size test. Checks that struct sizes and alignments do not change after publicly released
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

#include "pluginterfaces/vst/ivstattributes.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstcomponent.h"
#include "pluginterfaces/vst/ivstcontextmenu.h"
#include "pluginterfaces/vst/ivsteditcontroller.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivsthostapplication.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstplugview.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "pluginterfaces/vst/ivstrepresentation.h"
#include "pluginterfaces/vst/ivstunits.h"
#include "pluginterfaces/vst/vstpresetkeys.h"
#include "pluginterfaces/vst/vsttypes.h"

#include "pluginterfaces/base/typesizecheck.h"

#include <cstdio>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

#define SMTG_VST_COMPILE_TIME_STRUCT_CHECK 1

#if SMTG_VST_COMPILE_TIME_STRUCT_CHECK

// ipluginbase.h
SMTG_TYPE_SIZE_CHECK (PFactoryInfo, 452, 452, 452, 452)
SMTG_TYPE_SIZE_CHECK (PClassInfo, 116, 116, 116, 116)
SMTG_TYPE_SIZE_CHECK (PClassInfo2, 440, 440, 440, 440)
SMTG_TYPE_SIZE_CHECK (PClassInfoW, 696, 696, 696, 696)
SMTG_TYPE_ALIGN_CHECK (PFactoryInfo, 4, 4, 4, 4)
SMTG_TYPE_ALIGN_CHECK (PClassInfo, 4, 4, 4, 4)
SMTG_TYPE_ALIGN_CHECK (PClassInfo2, 4, 4, 4, 4)
SMTG_TYPE_ALIGN_CHECK (PClassInfoW, 4, 4, 4, 4)
// ivstaudioprocessor.h
SMTG_TYPE_SIZE_CHECK (ProcessSetup, 24, 20, 24, 24)
SMTG_TYPE_SIZE_CHECK (AudioBusBuffers, 24, 16, 24, 24)
SMTG_TYPE_SIZE_CHECK (ProcessData, 80, 48, 48, 48)
SMTG_TYPE_ALIGN_CHECK (ProcessSetup, 8, 1, 8, 8)
SMTG_TYPE_ALIGN_CHECK (AudioBusBuffers, 8, 1, 8, 8)
SMTG_TYPE_ALIGN_CHECK (ProcessData, 8, 1, 4, 4)
// ivstcomponent.h
SMTG_TYPE_SIZE_CHECK (BusInfo, 276, 276, 276, 276)
SMTG_TYPE_SIZE_CHECK (RoutingInfo, 12, 12, 12, 12)
SMTG_TYPE_ALIGN_CHECK (BusInfo, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (RoutingInfo, 4, 1, 4, 4)
// ivstcontextmenu.h
SMTG_TYPE_SIZE_CHECK (IContextMenuItem, 264, 264, 264, 264)
SMTG_TYPE_ALIGN_CHECK (IContextMenuItem, 4, 1, 4, 4)
// ivsteditcontroller.h
SMTG_TYPE_SIZE_CHECK (ParameterInfo, 792, 792, 792, 792)
SMTG_TYPE_ALIGN_CHECK (ParameterInfo, 8, 1, 8, 8)
// ivstevents.h
SMTG_TYPE_SIZE_CHECK (NoteOnEvent, 20, 20, 20, 20)
SMTG_TYPE_SIZE_CHECK (NoteOffEvent, 16, 16, 16, 16)
SMTG_TYPE_SIZE_CHECK (DataEvent, 16, 12, 12, 12)
SMTG_TYPE_SIZE_CHECK (PolyPressureEvent, 12, 12, 12, 12)
SMTG_TYPE_SIZE_CHECK (ChordEvent, 16, 12, 12, 12)
SMTG_TYPE_SIZE_CHECK (ScaleEvent, 16, 10, 12, 12)
SMTG_TYPE_SIZE_CHECK (LegacyMIDICCOutEvent, 4, 4, 4, 4)
SMTG_TYPE_SIZE_CHECK (Event, 48, 40, 40, 48)
SMTG_TYPE_ALIGN_CHECK (NoteOnEvent, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (NoteOffEvent, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (DataEvent, 8, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (PolyPressureEvent, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (ChordEvent, 8, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (ScaleEvent, 8, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (LegacyMIDICCOutEvent, 1, 1, 1, 1)
SMTG_TYPE_ALIGN_CHECK (Event, 8, 1, 8, 8)
// ivstnoteexpression.h
SMTG_TYPE_SIZE_CHECK (NoteExpressionValueDescription, 32, 28, 32, 32)
SMTG_TYPE_SIZE_CHECK (NoteExpressionValueEvent, 16, 16, 16, 16)
SMTG_TYPE_SIZE_CHECK (NoteExpressionTextEvent, 24, 16, 16, 16)
SMTG_TYPE_SIZE_CHECK (NoteExpressionTypeInfo, 816, 812, 816, 816)
SMTG_TYPE_SIZE_CHECK (KeyswitchInfo, 536, 536, 536, 536)
SMTG_TYPE_ALIGN_CHECK (NoteExpressionValueDescription, 8, 1, 8, 8)
SMTG_TYPE_ALIGN_CHECK (NoteExpressionValueEvent, 8, 1, 4, 8)
SMTG_TYPE_ALIGN_CHECK (NoteExpressionTextEvent, 8, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (NoteExpressionTypeInfo, 8, 1, 8, 8)
SMTG_TYPE_ALIGN_CHECK (KeyswitchInfo, 4, 1, 4, 4)
// ivstprocesscontext.h
SMTG_TYPE_SIZE_CHECK (FrameRate, 8, 8, 8, 8)
SMTG_TYPE_SIZE_CHECK (Chord, 4, 4, 4, 4)
SMTG_TYPE_SIZE_CHECK (ProcessContext, 112, 104, 112, 112)
SMTG_TYPE_ALIGN_CHECK (FrameRate, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (Chord, 2, 1, 2, 2)
SMTG_TYPE_ALIGN_CHECK (ProcessContext, 8, 1, 8, 8)
// ivstrepresentation.h
SMTG_TYPE_SIZE_CHECK (RepresentationInfo, 256, 256, 256, 256)
SMTG_TYPE_ALIGN_CHECK (RepresentationInfo, 1, 1, 1, 1)
// ivstunits.h
SMTG_TYPE_SIZE_CHECK (UnitInfo, 268, 268, 268, 268)
SMTG_TYPE_SIZE_CHECK (ProgramListInfo, 264, 264, 264, 264)
SMTG_TYPE_ALIGN_CHECK (UnitInfo, 4, 1, 4, 4)
SMTG_TYPE_ALIGN_CHECK (ProgramListInfo, 4, 1, 4, 4)

#endif // SMTG_VST_COMPILE_TIME_STRUCT_CHECK

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#define SMTG_PRINT_TYPE_SIZE_ALIGN(T)                                          \
	{                                                                          \
		auto len = strlen (#T);                                                \
		std::printf (#T);                                                      \
		for (auto i = len; i < 35; ++i)                                        \
			std::printf (" ");                                                 \
		std::printf ("size = %3zu | align = %2zu\n", sizeof (T), alignof (T)); \
	}
//------------------------------------------------------------------------
inline void printStructSizes ()
{
	// ipluginbase.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (PFactoryInfo);
	SMTG_PRINT_TYPE_SIZE_ALIGN (PClassInfo);
	SMTG_PRINT_TYPE_SIZE_ALIGN (PClassInfo2);
	SMTG_PRINT_TYPE_SIZE_ALIGN (PClassInfoW);

	// ivstaudioprocessor.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (ProcessSetup);
	SMTG_PRINT_TYPE_SIZE_ALIGN (AudioBusBuffers);
	SMTG_PRINT_TYPE_SIZE_ALIGN (ProcessData);

	// ivstcomponent.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (BusInfo);
	SMTG_PRINT_TYPE_SIZE_ALIGN (RoutingInfo);

	// ivstcontextmenu.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (IContextMenuItem);

	// ivsteditcontroller.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (ParameterInfo);

	// ivstevents.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteOnEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteOffEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (DataEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (PolyPressureEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (ChordEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (ScaleEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (LegacyMIDICCOutEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (Event);

	// ivstnoteexpression.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteExpressionValueDescription);
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteExpressionValueEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteExpressionTextEvent);
	SMTG_PRINT_TYPE_SIZE_ALIGN (NoteExpressionTypeInfo);
	SMTG_PRINT_TYPE_SIZE_ALIGN (KeyswitchInfo);

	// ivstprocesscontext.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (FrameRate);
	SMTG_PRINT_TYPE_SIZE_ALIGN (Chord);
	SMTG_PRINT_TYPE_SIZE_ALIGN (ProcessContext);

	// ivstrepresentation.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (RepresentationInfo);

	// ivstunits.h
	SMTG_PRINT_TYPE_SIZE_ALIGN (UnitInfo);
	SMTG_PRINT_TYPE_SIZE_ALIGN (ProgramListInfo);
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Steinberg
