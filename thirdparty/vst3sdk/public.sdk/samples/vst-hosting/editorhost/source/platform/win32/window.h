//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/win32/window.h
// Created by  : Steinberg 09.2016
// Description : Example of opening a plug-in editor
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

#include "public.sdk/samples/vst-hosting/editorhost/source/platform/iwindow.h"
#include "public.sdk/source/vst/utility/optional.h"
#include <Windows.h>
#include <vector>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

//------------------------------------------------------------------------
class Window : public IWindow, public std::enable_shared_from_this<Window>
{
public:
	static WindowPtr make (const std::string& name, Size size, bool resizeable,
	                       const WindowControllerPtr& controller, HINSTANCE instance);

	bool init (const std::string& name, Size size, bool resizeable,
	           const WindowControllerPtr& controller, HINSTANCE instance);

	void show () override;
	void close () override;
	void resize (Size newSize) override;
	Size getContentSize () override;

	NativePlatformWindow getNativePlatformWindow () const override;

	tresult queryInterface (const TUID iid, void** obj) override { return kNoInterface; }

	void closeImmediately ();

	using WindowList = std::vector<Window*>;
	static WindowList getWindows ();

private:
	LRESULT CALLBACK proc (UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	float getContentScaleFactor () const;
	void registerWindowClass (HINSTANCE instance);

	WindowPtr This;
	WindowControllerPtr controller {nullptr};
	HWND hwnd {nullptr};
};

//------------------------------------------------------------------------
struct DynamicLibrary
{
	DynamicLibrary (const char* name) { module = LoadLibraryA (name); }

	~DynamicLibrary () { FreeLibrary (module); }

	template <typename T>
	T getProcAddress (const char* name)
	{
		return module ? reinterpret_cast<T> (GetProcAddress (module, name)) : nullptr;
	}

private:
	HMODULE module {nullptr};
};

struct ShcoreLibrary : DynamicLibrary
{
	static ShcoreLibrary& instance ()
	{
		static ShcoreLibrary gInstance;
		return gInstance;
	}

	struct DPI
	{
		UINT x;
		UINT y;
	};

	VST3::Optional<DPI> getDpiForWindow (HWND window) const
	{
		if (!getDpiForMonitorProc)
			return {};
		auto monitor = MonitorFromWindow (window, MONITOR_DEFAULTTONEAREST);
		UINT x, y;
		getDpiForMonitorProc (monitor, MDT_EFFECTIVE_DPI, &x, &y);
		return DPI {x, y};
	}

	HRESULT setProcessDpiAwareness (bool perMonitor = true)
	{
		if (!setProcessDpiAwarenessProc)
			return S_FALSE;
		return setProcessDpiAwarenessProc (perMonitor ? PROCESS_PER_MONITOR_DPI_AWARE :
		                                                PROCESS_SYSTEM_DPI_AWARE);
	}

private:
	enum PROCESS_DPI_AWARENESS
	{
		PROCESS_DPI_UNAWARE = 0,
		PROCESS_SYSTEM_DPI_AWARE = 1,
		PROCESS_PER_MONITOR_DPI_AWARE = 2
	};

	enum MONITOR_DPI_TYPE
	{
		MDT_EFFECTIVE_DPI = 0,
		MDT_ANGULAR_DPI = 1,
		MDT_RAW_DPI = 2,
		MDT_DEFAULT = MDT_EFFECTIVE_DPI
	};

	using GetDpiForMonitorProc = HRESULT (WINAPI*) (HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);
	using SetProcessDpiAwarenessProc = HRESULT (WINAPI*) (PROCESS_DPI_AWARENESS);

	ShcoreLibrary () : DynamicLibrary ("Shcore.dll")
	{
		getDpiForMonitorProc = getProcAddress<GetDpiForMonitorProc> ("GetDpiForMonitor");
		setProcessDpiAwarenessProc =
		    getProcAddress<SetProcessDpiAwarenessProc> ("SetProcessDpiAwareness");
	}

	GetDpiForMonitorProc getDpiForMonitorProc {nullptr};
	SetProcessDpiAwarenessProc setProcessDpiAwarenessProc {nullptr};
};

#ifndef DPI_ENUMS_DECLARED

#define DPI_ENUMS_DECLARED
#endif // (DPI_ENUMS_DECLARED)

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
