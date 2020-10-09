//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/win32/window.cpp
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

#include "window.h"
#include "public.sdk/source/vst/utility/stringconvert.h"

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0
#endif

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

//------------------------------------------------------------------------
namespace {

static Window::WindowList gAllWindows;

//------------------------------------------------------------------------
static void addWindow (Window* window)
{
	gAllWindows.push_back (window);
}

//------------------------------------------------------------------------
static void removeWindow (Window* window)
{
	auto it = std::find (gAllWindows.begin (), gAllWindows.end (), window);
	if (it != gAllWindows.end ())
		gAllWindows.erase (it);
}

//------------------------------------------------------------------------
} // anonymous

//------------------------------------------------------------------------
static constexpr WCHAR* gWindowClassName = L"VSTSDK WindowClass";

//------------------------------------------------------------------------
WindowPtr Window::make (const std::string& name, Size size, bool resizeable,
                        const WindowControllerPtr& controller, HINSTANCE instance)
{
	auto window = std::make_shared<Window> ();
	if (window->init (name, size, resizeable, controller, instance))
		return window;
	return nullptr;
}

//------------------------------------------------------------------------
LRESULT CALLBACK Window::WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto* window = reinterpret_cast<Window*> ((LONG_PTR)GetWindowLongPtr (hWnd, GWLP_USERDATA));
	if (window)
		return window->proc (message, wParam, lParam);
	return DefWindowProc (hWnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
void Window::registerWindowClass (HINSTANCE instance)
{
	static bool once = true;
	if (!once)
		return;
	once = true;

	WNDCLASSEX wcex {};

	wcex.cbSize = sizeof (WNDCLASSEX);

	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = instance;
	wcex.hCursor = LoadCursor (instance, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszClassName = gWindowClassName;

	RegisterClassEx (&wcex);
}

//------------------------------------------------------------------------
auto Window::getWindows () -> WindowList
{
	return gAllWindows;
}

//------------------------------------------------------------------------
bool Window::init (const std::string& name, Size size, bool resizeable,
                   const WindowControllerPtr& controller, HINSTANCE instance)
{
	this->controller = controller;
	registerWindowClass (instance);
	DWORD exStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (resizeable)
		dwStyle |= WS_SIZEBOX | WS_MAXIMIZEBOX;
	auto windowTitle = VST3::StringConvert::convert (name);

	RECT rect {0, 0, size.width, size.height};
	AdjustWindowRectEx (&rect, dwStyle, false, exStyle);

	if (hwnd = CreateWindowEx (exStyle, gWindowClassName, (const TCHAR*)windowTitle.data (),
	                           dwStyle, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
	                           nullptr, nullptr, instance, nullptr))
	{
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (__int3264) (LONG_PTR)this);
		This = shared_from_this ();
		addWindow (this);
	}
	return hwnd != nullptr;
}

//------------------------------------------------------------------------
LRESULT Window::proc (UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_ERASEBKGND:
		{
			return TRUE; // do not draw background
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps {};
			BeginPaint (hwnd, &ps);
			EndPaint (hwnd, &ps);
			return FALSE;
		}
		case WM_SIZE:
		{
			controller->onResize (*this, getContentSize ());
			break;
		}
		case WM_SIZING:
		{
			auto* newSize = reinterpret_cast<RECT*> (lParam);
			RECT oldSize;
			GetWindowRect (hwnd, &oldSize);
			RECT clientSize;
			GetClientRect (hwnd, &clientSize);

			auto diffX = (newSize->right - newSize->left) - (oldSize.right - oldSize.left);
			auto diffY = (newSize->bottom - newSize->top) - (oldSize.bottom - oldSize.top);

			Size newClientSize = {(clientSize.right - clientSize.left),
			                      (clientSize.bottom - clientSize.top)};
			newClientSize.width += diffX;
			newClientSize.height += diffY;

			auto constraintSize = controller->constrainSize (*this, newClientSize);
			if (constraintSize != newClientSize)
			{
				auto diffX = (oldSize.right - oldSize.left) - (clientSize.right - clientSize.left);
				auto diffY = (oldSize.bottom - oldSize.top) - (clientSize.bottom - clientSize.top);
				newSize->right = newSize->left + static_cast<LONG> (constraintSize.width + diffX);
				newSize->bottom = newSize->top + static_cast<LONG> (constraintSize.height + diffY);
			}
			return TRUE;
		}
		case WM_CLOSE:
		{
			closeImmediately ();
			return TRUE;
		}
		case WM_DPICHANGED:
		{
			controller->onContentScaleFactorChanged (*this, getContentScaleFactor ());
			break;
		}
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

//------------------------------------------------------------------------
void Window::closeImmediately ()
{
	close ();
	removeWindow (this);
	controller->onClose (*this);
	SetWindowLongPtr (hwnd, GWLP_USERDATA, (__int3264) (LONG_PTR) nullptr);
	This = nullptr;
}

//------------------------------------------------------------------------
Size Window::getContentSize ()
{
	RECT r;
	GetClientRect (hwnd, &r);
	return {r.right - r.left, r.bottom - r.top};
}

//------------------------------------------------------------------------
float Window::getContentScaleFactor () const
{
	if (auto dpiForWindow = ShcoreLibrary::instance ().getDpiForWindow (hwnd))
	{
		return static_cast<float> (dpiForWindow->x) / 96.f;
	}
	else
	{
		return 1.f;
	}
}

//------------------------------------------------------------------------
void Window::show ()
{
	controller->onContentScaleFactorChanged (*this, getContentScaleFactor ());
	controller->onShow (*this);
	SetWindowPos (hwnd, HWND_TOP, 0, 0, 0, 0,
	              SWP_NOSIZE | SWP_NOMOVE | SWP_NOCOPYBITS | SWP_SHOWWINDOW);
}

//------------------------------------------------------------------------
void Window::close ()
{
	ShowWindow (hwnd, false);
}

//------------------------------------------------------------------------
void Window::resize (Size newSize)
{
	if (getContentSize () == newSize)
		return;
	WINDOWINFO windowInfo;
	GetWindowInfo (hwnd, &windowInfo);
	RECT clientRect {};
	clientRect.right = newSize.width;
	clientRect.bottom = newSize.height;
	AdjustWindowRectEx (&clientRect, windowInfo.dwStyle, false, windowInfo.dwExStyle);
	SetWindowPos (hwnd, HWND_TOP, 0, 0, clientRect.right - clientRect.left,
	              clientRect.bottom - clientRect.top, SWP_NOMOVE | SWP_NOCOPYBITS | SWP_NOACTIVATE);
}

//------------------------------------------------------------------------
NativePlatformWindow Window::getNativePlatformWindow () const
{
	return {kPlatformTypeHWND, hwnd};
}

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
