//------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
//
// Category    : Helpers
// Project     : Steinberg Plug-In SDK
// Filename    : public.sdk/source/common/openurl.cpp
// Created by  : Steinberg 04.2020
// Description : Simple helper allowing to open a URL in the default associated application
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

#include "openurl.h"
#include "pluginterfaces/base/ftypes.h"

#if SMTG_OS_WINDOWS
#include <windows.h>
#else
#include <cstdlib>
#endif

namespace Steinberg {

bool openURLInDefaultApplication (const String& address)
{
	bool res = false;

#if SMTG_OS_WINDOWS
	auto r = ShellExecuteA (nullptr, "open", address.text8 (), nullptr, nullptr, SW_SHOWNORMAL);
	res = (r != nullptr);
#elif SMTG_OS_OSX
	String cmd;
	cmd += "open \"";
	cmd += address.text8 ();
	cmd += "\"";
	res = system (cmd) != 0;
#elif SMTG_OS_LINUX
	String cmd;
	cmd += "xdg-open \"";
	cmd += address.text8 ();
	cmd += "\"";
	res = system (cmd) != 0;
#endif
	return res;
}
}
