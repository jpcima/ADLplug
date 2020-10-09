//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/again/source/againprocess.h
// Created by  : Steinberg, 11/2016
// Description : AGain Example for VST SDK 3.0
//               Simple gain plug-in with gain, bypass values and 1 midi input
//               and the same plug-in with sidechain 
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#pragma once

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
template <typename SampleType>
SampleType AGain::processAudio (SampleType** in, SampleType** out, int32 numChannels,
                                int32 sampleFrames, float gain)
{
	SampleType vuPPM = 0;

	// in real Plug-in it would be better to do dezippering to avoid jump (click) in gain value
	for (int32 i = 0; i < numChannels; i++)
	{
		int32 samples = sampleFrames;
		SampleType* ptrIn = (SampleType*)in[i];
		SampleType* ptrOut = (SampleType*)out[i];
		SampleType tmp;
		while (--samples >= 0)
		{
			// apply gain
			tmp = (*ptrIn++) * gain;
			(*ptrOut++) = tmp;

			// check only positive values
			if (tmp > vuPPM)
			{
				vuPPM = tmp;
			}
		}
	}
	return vuPPM;
}


//------------------------------------------------------------------------
template <typename SampleType>
SampleType AGain::processVuPPM (SampleType** in, int32 numChannels,	int32 sampleFrames)
{
	SampleType vuPPM = 0;

	for (int32 i = 0; i < numChannels; i++)
	{
		int32 samples = sampleFrames;
		SampleType* ptrIn = (SampleType*)in[i];
		SampleType tmp;
		while (--samples >= 0)
		{
			tmp = (*ptrIn++);
			
			// check only positive values
			if (tmp > vuPPM)
			{
				vuPPM = tmp;
			}
		}
	}
	return vuPPM;
}

} // Vst
} // Steinberg
