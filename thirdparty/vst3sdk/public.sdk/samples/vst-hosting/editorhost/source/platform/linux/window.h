//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/linux/window.h
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

// Use X11 instead of GTK
// #define EDITORHOST_GTK

#include "public.sdk/samples/vst-hosting/editorhost/source/platform/iwindow.h"
#include <functional>

struct _XDisplay;
typedef struct _XDisplay Display;

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

//------------------------------------------------------------------------
class X11Window : public IWindow
{
public:
	using Ptr = std::shared_ptr<X11Window>;
	using WindowClosedFunc = std::function<void (X11Window*)>;

	static Ptr make (const std::string& name, Size size, bool resizeable,
	                 const WindowControllerPtr& controller, Display* display,
	                 const WindowClosedFunc& windowClosedFunc);

	X11Window ();
	~X11Window () override;

	bool init (const std::string& name, Size size, bool resizeable,
	           const WindowControllerPtr& controller, Display* display,
	           const WindowClosedFunc& windowClosedFunc);

	void show () override;
	void close () override;
	void resize (Size newSize) override;
	Size getContentSize () override;

	NativePlatformWindow getNativePlatformWindow () const override;
	tresult queryInterface (const TUID iid, void** obj) override;

	void onIdle ();

private:
	Size getSize () const;

	struct Impl;
	std::unique_ptr<Impl> impl;
};

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
