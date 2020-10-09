//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/iwindow.h
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

#include "pluginterfaces/gui/iplugview.h"
#include <memory>
#include <string>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

using Coord = int32_t;

//------------------------------------------------------------------------
struct Point
{
	Coord x;
	Coord y;
};

//------------------------------------------------------------------------
struct Size
{
	Coord width;
	Coord height;
};

//------------------------------------------------------------------------
inline bool operator!= (const Size& lhs, const Size& rhs)
{
	return lhs.width != rhs.width || lhs.height != rhs.height;
}

//------------------------------------------------------------------------
inline bool operator== (const Size& lhs, const Size& rhs)
{
	return lhs.width == rhs.width && lhs.height == rhs.height;
}

//------------------------------------------------------------------------
struct Rect
{
	Point origin;
	Size size;
};

//------------------------------------------------------------------------
inline Rect ViewRectToRect (ViewRect r)
{
	Rect result {};
	result.origin.x = r.left;
	result.origin.y = r.top;
	result.size.width = r.right - r.left;
	result.size.height = r.bottom - r.top;
	return result;
}

//------------------------------------------------------------------------
struct NativePlatformWindow
{
	FIDString type;
	void* ptr;
};

class IWindow;

//------------------------------------------------------------------------
class IWindowController
{
public:
	virtual ~IWindowController () noexcept = default;

	virtual void onShow (IWindow& window) = 0;
	virtual void onClose (IWindow& window) = 0;
	virtual void onResize (IWindow& window, Size newSize) = 0;
	virtual void onContentScaleFactorChanged (IWindow& window, float newScaleFactor) = 0;
	virtual Size constrainSize (IWindow& window, Size requestedSize) = 0;
};

using WindowControllerPtr = std::shared_ptr<IWindowController>;

//------------------------------------------------------------------------
class IWindow
{
public:
	virtual ~IWindow () noexcept = default;

	virtual void show () = 0;
	virtual void close () = 0;
	virtual void resize (Size newSize) = 0;
	virtual Size getContentSize () = 0;

	virtual NativePlatformWindow getNativePlatformWindow () const = 0;

	virtual tresult queryInterface (const TUID iid, void** obj) = 0;
};

using WindowPtr = std::shared_ptr<IWindow>;

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
