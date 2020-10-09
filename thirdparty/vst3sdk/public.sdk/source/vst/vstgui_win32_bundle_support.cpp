//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : public.sdk/source/vst/vstgui_win32_bundle_support.cpp
// Created by  : Steinberg, 10/2018
// Description : VSTGUI Win32 Bundle Support
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2019, Steinberg Media Technologies GmbH, All Rights Reserved
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

#include "vstgui_win32_bundle_support.h"
#include "vstgui/lib/platform/platform_win32.h"
#include "vstgui/lib/platform/win32/win32support.h"
#include "vstgui/lib/optional.h"
#include <string>

//-----------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
static VSTGUI::Optional<std::string> ascend (std::string& path, char delimiter = '\\')
{
	auto index = path.find_last_of (delimiter);
	if (index == std::string::npos)
		return {};
	path.erase (index);
	return VSTGUI::Optional<std::string> (std::move (path));
}

//-----------------------------------------------------------------------------
void setupVSTGUIBundleSupport (void* hInstance)
{
	using namespace VSTGUI;
	WCHAR path[MAX_PATH];
	if (SUCCEEDED (GetModuleFileNameW (static_cast<HMODULE> (hInstance), path, MAX_PATH)))
	{
		UTF8StringHelper helper (path);
		auto utf8Path = std::string (helper.getUTF8String ());
		if (auto p = ascend (utf8Path))
		{
			if (p = ascend (*p))
			{
				*p += "\\Resources";
				IWin32PlatformFrame::setResourceBasePath (UTF8String (*p));
			}
		}
	}

}

//-----------------------------------------------------------------------------
} // Vst
} // Steinberg
