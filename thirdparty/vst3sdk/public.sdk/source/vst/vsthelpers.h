//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vsthelpers.h
// Created by  : Steinberg, 11/2018
// Description : common defines
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

#pragma once
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstattributes.h"
#include "pluginterfaces/vst/vstpresetkeys.h"

#include <cstring>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace Helpers {
//------------------------------------------------------------------------
/** Helpers */
//------------------------------------------------------------------------

//------------------------------------------------------------------------
/** Retrieve from a IBStream the state type, here the StateType::kProject
	return kResultTrue if the state is coming from a project,
	return kResultFalse if the state is coming from a preset,
	return kNotImplemented if the host does not implement such feature
*/
tresult isProjectState (IBStream* state)
{
	if (!state)
		return kInvalidArgument;

	FUnknownPtr<IStreamAttributes> stream (state);
	if (!stream)
		return kNotImplemented;

	if (IAttributeList* list = stream->getAttributes ())
	{
		// get the current type (project/Default..) of this state
		String128 string = {0};
		if (list->getString (PresetAttributes::kStateType, string, 128 * sizeof (TChar)) ==
		    kResultTrue)
		{
			UString128 tmp (string);
			char ascii[128];
			tmp.toAscii (ascii, 128);
			if (!strncmp (ascii, StateType::kProject, strlen (StateType::kProject)))
			{
				return kResultTrue;
			}
			return kResultFalse;
		}
	}
	return kNotImplemented;
}
/*@}*/

//------------------------------------------------------------------------
} // namespace Helpers
} // namespace Vst
} // namespace Steinberg
