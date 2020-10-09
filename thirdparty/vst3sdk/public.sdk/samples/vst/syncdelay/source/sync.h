//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : public.sdk/samples/vst/syncdelay/source/sync.h
// Created by  : Steinberg, 01/2020
// Description :
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

#include "pluginterfaces/vst/vsttypes.h"
#include <array>
#include <cassert>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
struct SyncEntry
{
	double value;
	const TChar* title;
};

//------------------------------------------------------------------------
static constexpr std::array<SyncEntry, 18> Synced = {{
    {4.000, STR16 ("1/1")},
    {2.000, STR16 ("1/2")},
    {1.000, STR16 ("1/4")},
    {0.500, STR16 ("1/8")},
    {0.250, STR16 ("1/16")},
    {0.125, STR16 ("1/32")},
    {8.000 / 3., STR16 ("1/1 T")},
    {4.000 / 3., STR16 ("1/2 T")},
    {2.000 / 3., STR16 ("1/4 T")},
    {1.000 / 3., STR16 ("1/8 T")},
    {0.500 / 3., STR16 ("1/16 T")},
    {0.250 / 3., STR16 ("1/32 T")},
    {6.000, STR16 ("1/1 D")},
    {3.000, STR16 ("1/2 D")},
    {1.500, STR16 ("1/4 D")},
    {0.750, STR16 ("1/8 D")},
    {0.375, STR16 ("1/16 D")},
    {0.1875, STR16 ("1/32 D")},
}};

//------------------------------------------------------------------------
} // Vst
} // Steinberg
