//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/linux/window.cpp
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

#ifdef EDITORHOST_GTK
#include <gtkmm.h>
#endif

#include "public.sdk/source/vst/utility/stringconvert.h"
#include <X11/Xutil.h>
#include <cassert>
#include <iostream>
#include <unordered_map>

#ifdef EDITORHOST_GTK
#include <gdk/gdkx.h>
#include <gtkmm/socket.h>
#else
#include "public.sdk/samples/vst-hosting/editorhost/source/platform/linux/runloop.h"
#include <X11/Xlib.h>
#endif

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

#ifdef EDITORHOST_GTK

struct ContentView : Gtk::Fixed
{
	void on_size_allocate (Gtk::Allocation& allocation) override
	{
		Gtk::Widget::on_size_allocate (allocation);
		for (auto child : get_children ())
		{
			child->size_allocate (allocation);
		}
	}
};

struct WindowGtk : Gtk::Window
{
	WindowGtk ()
	{
		add (container);
		container.put (label, 0, 0);
		container.add (socket);
		show_all_children ();
	}

	void on_show () override
	{
		Gtk::Window::on_show ();
		if (controller)
			controller->onShow (*window);
	}

	void on_unmap () override
	{
		Gtk::Window::on_unmap ();
		if (controller)
			controller->onClose (*window);
		if (windowClosedFunc)
			windowClosedFunc (window);
	}

	bool on_configure_event (GdkEventConfigure* event) override
	{
		// TODO: does not work yet
		Gtk::Window::on_configure_event (event);
		return false;
		if (controller)
		{
			if (currentSize.width != event->width || currentSize.height != event->height)
			{
				Size newSize {event->width, event->height};
				auto size = controller->constrainSize (*window, newSize);
				if (size != newSize)
				{
					resize (size.width, size.height);
				}
			}
		}
		return true;
	}

	void on_size_allocate (Gtk::Allocation& allocation) override
	{
		auto size = Size {allocation.get_width (), allocation.get_height ()};
		if (size != currentSize)
		{
			if (controller)
			{
				size = controller->constrainSize (*window, size);
				controller->onResize (*window, size);
			}
			currentSize = size;
			allocation.set_width (currentSize.width);
			allocation.set_height (currentSize.height);
			resize (currentSize.width, currentSize.height);
		}
		Gtk::Window::on_size_allocate (allocation);
	}

	void checkSize ()
	{
		int width, height;
		get_size (width, height);
		if (currentSize.width != width || currentSize.height != height)
		{
			resize (currentSize.width, currentSize.height);
		}
	}

	::Window getId ()
	{
		if (!socket.is_visible ())
			return 0;
		std::cout << "Socket ID: " << socket.get_id () << "\n";
		std::cout.flush ();
		return socket.get_id ();
	}

	X11Window::WindowClosedFunc windowClosedFunc;
	WindowControllerPtr controller {nullptr};
	X11Window* window {nullptr};
	Size currentSize;
	ContentView container;
	Gtk::Label label {"HostLabel"};
	Gtk::Socket socket;
};
#else

#endif

//------------------------------------------------------------------------
struct X11Window::Impl : public Linux::IRunLoop
{
	Impl (X11Window* x11Window);
	bool init (const std::string& name, Size size, bool resizeable,
	           const WindowControllerPtr& controller, Display* display,
	           const WindowClosedFunc& windowClosedFunc);

	void show ();
	void close ();
	Size getSize () const;
	void resize (Size newSize, bool force);

#ifdef EDITORHOST_GTK
	WindowGtk window;

//------------------------------------------------------------------------
	struct EventHandler
	{
		IPtr<Linux::IEventHandler> handler;
		GIOChannel* ioChannel;
	};

//------------------------------------------------------------------------
	using TimerHandler = IPtr<Linux::ITimerHandler>;
	using TimerID = guint;
#else
//------------------------------------------------------------------------
	struct XEmbedInfo
	{
		uint32_t version;
		uint32_t flags;
	};

	~Impl ();
	void onClose ();
	bool handleMainWindowEvent (const XEvent& event);
	bool handlePlugEvent (const XEvent& event);
	XEmbedInfo* getXEmbedInfo ();
	void checkSize ();
	void callPlugEventHandlers ();

	WindowControllerPtr controller {nullptr};
	WindowClosedFunc windowClosedFunc;
	Display* xDisplay {nullptr};
	XEmbedInfo* xembedInfo {nullptr};
	Window xWindow {};
	Window plugParentWindow {};
	Window plugWindow {};
	GC xGraphicContext {};
	Atom xEmbedInfoAtom {None};
	Atom xEmbedAtom {None};
	bool isMapped {false};

	using EventHandler = IPtr<Linux::IEventHandler>;
	using TimerHandler = IPtr<Linux::ITimerHandler>;
#endif
	Size mCurrentSize {};
	X11Window* x11Window {nullptr};

	using EventHandlers = std::unordered_map<Linux::FileDescriptor, EventHandler>;
	EventHandlers eventHandlers;

	using TimerHandlers = std::unordered_map<TimerID, TimerHandler>;
	TimerHandlers timerHandlers;

	tresult PLUGIN_API registerEventHandler (Linux::IEventHandler* handler,
	                                         Linux::FileDescriptor fd) override;
	tresult PLUGIN_API unregisterEventHandler (Linux::IEventHandler* handler) override;
	tresult PLUGIN_API registerTimer (Linux::ITimerHandler* handler,
	                                  Linux::TimerInterval milliseconds) override;
	tresult PLUGIN_API unregisterTimer (Linux::ITimerHandler* handler) override;

	uint32 PLUGIN_API addRef () override { return 1000; }
	uint32 PLUGIN_API release () override { return 1000; }
	tresult PLUGIN_API queryInterface (const TUID, void**) override { return kNoInterface; }
};

//------------------------------------------------------------------------
auto X11Window::make (const std::string& name, Size size, bool resizeable,
                      const WindowControllerPtr& controller, Display* display,
                      const WindowClosedFunc& windowClosedFunc) -> Ptr
{
	auto window = std::make_shared<X11Window> ();
	if (window->init (name, size, resizeable, controller, display, windowClosedFunc))
	{
		return window;
	}
	return nullptr;
}

//------------------------------------------------------------------------
X11Window::X11Window ()
{
	impl = std::unique_ptr<Impl> (new Impl (this));
}

//------------------------------------------------------------------------
X11Window::~X11Window ()
{
}

//------------------------------------------------------------------------
bool X11Window::init (const std::string& name, Size size, bool resizeable,
                      const WindowControllerPtr& controller, Display* display,
                      const WindowClosedFunc& windowClosedFunc)
{
	return impl->init (name, size, resizeable, controller, display, windowClosedFunc);
}

//------------------------------------------------------------------------
Size X11Window::getSize () const
{
	return impl->getSize ();
}

//------------------------------------------------------------------------
void X11Window::show ()
{
	impl->show ();
}

//------------------------------------------------------------------------
void X11Window::close ()
{
	impl->close ();
}

//------------------------------------------------------------------------
void X11Window::resize (Size newSize)
{
	impl->resize (newSize, false);
}

//------------------------------------------------------------------------
Size X11Window::getContentSize ()
{
	return {};
}

//------------------------------------------------------------------------
NativePlatformWindow X11Window::getNativePlatformWindow () const
{
#ifdef EDITORHOST_GTK
	return {kPlatformTypeX11EmbedWindowID, reinterpret_cast<void*> (impl->window.getId ())};
#else
	return {kPlatformTypeX11EmbedWindowID, reinterpret_cast<void*> (impl->plugParentWindow)};
#endif
}

//------------------------------------------------------------------------
tresult X11Window::queryInterface (const TUID iid, void** obj)
{
	if (FUnknownPrivate::iidEqual (iid, Linux::IRunLoop::iid))
	{
		*obj = impl.get ();
		return kResultTrue;
	}
	return kNoInterface;
}

//------------------------------------------------------------------------
void X11Window::onIdle ()
{
#ifdef EDITORHOST_GTK
	impl->window.checkSize ();
#endif
}

#ifdef EDITORHOST_GTK
//------------------------------------------------------------------------
X11Window::Impl::Impl (X11Window* x11Window) : x11Window (x11Window)
{
}

//------------------------------------------------------------------------
bool X11Window::Impl::init (const std::string& name, Size size, bool resizeable,
                            const WindowControllerPtr& controller, Display* display,
                            const WindowClosedFunc& windowClosedFunc)
{
	window.set_title (name.data ());
	window.set_default_size (size.width, size.height);
	window.set_resizable (resizeable);
	window.show ();
	window.set_visible (false);

	window.window = x11Window;
	window.controller = controller;
	window.windowClosedFunc = windowClosedFunc;

	return true;
}

//------------------------------------------------------------------------
void X11Window::Impl::show ()
{
	window.set_visible (true);
	window.show ();
}

//------------------------------------------------------------------------
void X11Window::Impl::close ()
{
	window.close ();
}

//------------------------------------------------------------------------
Size X11Window::Impl::getSize () const
{
	int width, height;
	window.get_size (width, height);
	return {width, height};
}

//------------------------------------------------------------------------
void X11Window::Impl::resize (Size newSize, bool force)
{
	if (!force && mCurrentSize == newSize)
		return;
	window.resize (newSize.width, newSize.height);
	mCurrentSize = newSize;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::registerEventHandler (Linux::IEventHandler* handler,
                                                          Linux::FileDescriptor fd)
{
	if (!handler)
		return kInvalidArgument;

	if (eventHandlers.find (fd) != eventHandlers.end ())
		return kInvalidArgument;

	auto display = gdk_window_get_display (window.get_window ()->gobj ());
	auto xDisplay = gdk_x11_display_get_xdisplay (display);
	if (XConnectionNumber (xDisplay) == fd)
	{
		// the plug-in is using GTK as well, so no need in registering it as event handler
		return kResultTrue;
	}

	auto ioChannel = g_io_channel_unix_new (fd);
	g_io_add_watch (ioChannel, (GIOCondition) (G_IO_IN | G_IO_OUT | G_IO_ERR | G_IO_HUP),
	                [] (auto* source, auto condition, auto data) {
		                (void)condition;
		                auto handler = reinterpret_cast<Linux::IEventHandler*> (data);
		                handler->onFDIsSet (g_io_channel_unix_get_fd (source));
		                return static_cast<int> (0);
	                },
	                handler);

	eventHandlers[fd] = {handler, ioChannel};

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::unregisterEventHandler (Linux::IEventHandler* handler)
{
	if (!handler)
		return kInvalidArgument;

	for (auto it = eventHandlers.begin (), end = eventHandlers.end (); it != end; ++it)
	{
		if (it->second.handler == handler)
		{
			g_io_channel_unref (it->second.ioChannel);
			eventHandlers.erase (it);
			return kResultTrue;
		}
	}

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::registerTimer (Linux::ITimerHandler* handler,
                                                   Linux::TimerInterval milliseconds)
{
	if (!handler || milliseconds == 0)
		return kInvalidArgument;

	auto id = g_timeout_add (milliseconds,
	                         [] (auto data) {
		                         auto handler = reinterpret_cast<Linux::ITimerHandler*> (data);
		                         handler->onTimer ();
		                         return static_cast<gboolean> (true);
	                         },
	                         handler);
	timerHandlers.emplace (id, handler);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::unregisterTimer (Linux::ITimerHandler* handler)
{
	if (!handler)
		return kInvalidArgument;

	for (auto it = timerHandlers.begin (), end = timerHandlers.end (); it != end; ++it)
	{
		if (it->second == handler)
		{
			if (auto source = g_main_context_find_source_by_id (nullptr, it->first))
			{
				g_source_destroy (source);
			}
			timerHandlers.erase (it);
			return kResultTrue;
		}
	}

	return kNotImplemented;
}

#else

/* XEMBED messages */
#define XEMBED_EMBEDDED_NOTIFY 0
#define XEMBED_WINDOW_ACTIVATE 1
#define XEMBED_WINDOW_DEACTIVATE 2
#define XEMBED_REQUEST_FOCUS 3
#define XEMBED_FOCUS_IN 4
#define XEMBED_FOCUS_OUT 5
#define XEMBED_FOCUS_NEXT 6
#define XEMBED_FOCUS_PREV 7
/* 8-9 were used for XEMBED_GRAB_KEY/XEMBED_UNGRAB_KEY */
#define XEMBED_MODALITY_ON 10
#define XEMBED_MODALITY_OFF 11
#define XEMBED_REGISTER_ACCELERATOR 12
#define XEMBED_UNREGISTER_ACCELERATOR 13
#define XEMBED_ACTIVATE_ACCELERATOR 14

void send_xembed_message (Display* dpy, /* display */
                          Window w, /* receiver */
                          Atom messageType, long message, /* message opcode */
                          long detail, /* message detail */
                          long data1, /* message data 1 */
                          long data2 /* message data 2 */
                          )
{
	XEvent ev;
	memset (&ev, 0, sizeof (ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = w;
	ev.xclient.message_type = messageType;
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = CurrentTime;
	ev.xclient.data.l[1] = message;
	ev.xclient.data.l[2] = detail;
	ev.xclient.data.l[3] = data1;
	ev.xclient.data.l[4] = data2;
	XSendEvent (dpy, w, False, NoEventMask, &ev);
	XSync (dpy, False);
}

#define XEMBED_MAPPED (1 << 0)

//------------------------------------------------------------------------
X11Window::Impl::Impl (X11Window* x11Window) : x11Window (x11Window)
{
}

//------------------------------------------------------------------------
X11Window::Impl::~Impl ()
{
}

//------------------------------------------------------------------------
bool X11Window::Impl::init (const std::string& name, Size size, bool resizeable,
                            const WindowControllerPtr& controller, Display* display,
                            const WindowClosedFunc& windowClosedFunc)
{
	this->windowClosedFunc = windowClosedFunc;
	this->controller = controller;
	xDisplay = display;

	xEmbedInfoAtom = XInternAtom (xDisplay, "_XEMBED_INFO", true);
	if (xEmbedInfoAtom == None)
	{
		std::cerr << "_XEMBED_INFO does not exist" << std::endl;
		return false;
	}

	// Get screen size from display
	auto screen_num = DefaultScreen (xDisplay);
	auto displayWidth = DisplayWidth (xDisplay, screen_num);
	auto displayHeight = DisplayHeight (xDisplay, screen_num);
	unsigned int border_width = 1;

	XVisualInfo vInfo;
	if (!XMatchVisualInfo (xDisplay, screen_num, 24, TrueColor, &vInfo))
	{
		exit (-1);
	}

	XSetWindowAttributes winAttr {};
	winAttr.border_pixel = BlackPixel (xDisplay, screen_num);
	winAttr.background_pixel = WhitePixel (xDisplay, screen_num);
	winAttr.colormap =
	    XCreateColormap (xDisplay, XDefaultRootWindow (xDisplay), vInfo.visual, AllocNone);
	uint32_t winAttrMask = CWBackPixel | CWColormap | CWBorderPixel;
	xWindow = XCreateWindow (xDisplay, RootWindow (xDisplay, screen_num), 0, 0, displayWidth,
	                         displayHeight, border_width, vInfo.depth, InputOutput, vInfo.visual,
	                         winAttrMask, &winAttr);
	XFlush (xDisplay);

	resize (size, true);

	XSelectInput (xDisplay, xWindow, /*  KeyPressMask | ButtonPressMask |*/
	              ExposureMask | /*ResizeRedirectMask |*/ StructureNotifyMask |
	                  SubstructureNotifyMask | FocusChangeMask);

	auto sizeHints = XAllocSizeHints ();
	sizeHints->flags = PMinSize;
	if (!resizeable)
	{
		sizeHints->flags |= PMaxSize;
		sizeHints->min_width = sizeHints->max_width = size.width;
		sizeHints->min_height = sizeHints->max_height = size.height;
	}
	else
	{
		sizeHints->min_width = sizeHints->min_height = 80;
	}
	XSetWMNormalHints (xDisplay, xWindow, sizeHints);
	XFree (sizeHints);

	// set a title
	XStoreName (xDisplay, xWindow, name.data ());

	XTextProperty iconName;
	auto icon_name = const_cast<char*> (name.data ());
	XStringListToTextProperty (&icon_name, 1, &iconName);
	XSetWMIconName (xDisplay, xWindow, &iconName);

	Atom wm_delete_window;
	wm_delete_window = XInternAtom (xDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols (xDisplay, xWindow, &wm_delete_window, 1);

	xGraphicContext = XCreateGC (xDisplay, xWindow, 0, 0);
	XSetForeground (xDisplay, xGraphicContext, WhitePixel (xDisplay, screen_num));
	XSetBackground (xDisplay, xGraphicContext, BlackPixel (xDisplay, screen_num));

	winAttr = {};
	winAttr.override_redirect = true;
	winAttr.event_mask =
	    ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
	plugParentWindow =
	    XCreateWindow (xDisplay, xWindow, 0, 0, size.width, size.height, border_width, vInfo.depth,
	                   InputOutput, CopyFromParent, winAttrMask, &winAttr);

	XSelectInput (xDisplay, plugParentWindow, SubstructureNotifyMask | PropertyChangeMask);

	XMapWindow (xDisplay, plugParentWindow);

	RunLoop::instance ().registerWindow (plugParentWindow,
	                                     [this] (const XEvent& e) { return handlePlugEvent (e); });

	RunLoop::instance ().registerWindow (
	    xWindow, [this] (const XEvent& e) { return handleMainWindowEvent (e); });

	return true;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::registerEventHandler (Linux::IEventHandler* handler,
                                                          Linux::FileDescriptor fd)
{
	if (!handler || eventHandlers.find (fd) != eventHandlers.end ())
		return kInvalidArgument;

	RunLoop::instance ().registerFileDescriptor (fd,
	                                             [handler] (int fd) { handler->onFDIsSet (fd); });
	eventHandlers.emplace (fd, handler);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::unregisterEventHandler (Linux::IEventHandler* handler)
{
	if (!handler)
		return kInvalidArgument;

	for (auto it = eventHandlers.begin (), end = eventHandlers.end (); it != end; ++it)
	{
		if (it->second == handler)
		{
			RunLoop::instance ().unregisterFileDescriptor (it->first);
			eventHandlers.erase (it);
			return kResultTrue;
		}
	}

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::registerTimer (Linux::ITimerHandler* handler,
                                                   Linux::TimerInterval milliseconds)
{
	if (!handler || milliseconds == 0)
		return kInvalidArgument;

	auto id = RunLoop::instance ().registerTimer (milliseconds,
	                                              [handler] (auto) { handler->onTimer (); });
	timerHandlers.emplace (id, handler);
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API X11Window::Impl::unregisterTimer (Linux::ITimerHandler* handler)
{
	if (!handler)
		return kInvalidArgument;

	for (auto it = timerHandlers.begin (), end = timerHandlers.end (); it != end; ++it)
	{
		if (it->second == handler)
		{
			RunLoop::instance ().unregisterTimer (it->first);
			timerHandlers.erase (it);
			return kResultTrue;
		}
	}

	return kNotImplemented;
}

//------------------------------------------------------------------------
void X11Window::Impl::callPlugEventHandlers ()
{
	for (auto& e : eventHandlers)
	{
		e.second->onFDIsSet (e.first);
	}
}

//------------------------------------------------------------------------
void X11Window::Impl::show ()
{
	XMapWindow (xDisplay, xWindow);
}

//------------------------------------------------------------------------
void X11Window::Impl::close ()
{
	XUnmapWindow (xDisplay, xWindow);
}

//------------------------------------------------------------------------
void X11Window::Impl::onClose ()
{
	XFreeGC (xDisplay, xGraphicContext);
	XDestroyWindow (xDisplay, xWindow);

	xDisplay = nullptr;
	xWindow = 0;

	isMapped = false;
	if (windowClosedFunc)
		windowClosedFunc (x11Window);
}

//------------------------------------------------------------------------
void X11Window::Impl::resize (Size newSize, bool force)
{
	if (!force && mCurrentSize == newSize)
		return;
	if (xWindow)
		XResizeWindow (xDisplay, xWindow, newSize.width, newSize.height);
	if (plugParentWindow)
		XResizeWindow (xDisplay, plugParentWindow, newSize.width, newSize.height);
	mCurrentSize = newSize;
}

//------------------------------------------------------------------------
Size X11Window::Impl::getSize () const
{
	::Window root;
	int x, y;
	unsigned int width, height;
	unsigned int border_width;
	unsigned int depth;

	XGetGeometry (xDisplay, xWindow, &root, &x, &y, &width, &height, &border_width, &depth);

	return {static_cast<int> (width), static_cast<int> (height)};
}

//------------------------------------------------------------------------
void X11Window::Impl::checkSize ()
{
	if (getSize () != mCurrentSize)
	{
		resize (mCurrentSize, true);
	}
}

//------------------------------------------------------------------------
bool X11Window::Impl::handleMainWindowEvent (const XEvent& event)
{
#if LOG_EVENTS
	std::cout << "event " << event.type << "\n";
#endif
	bool res = false;

	switch (event.type)
	{
		case Expose:
			if (event.xexpose.count == 0)
			{
				XClearWindow (xDisplay, xWindow);
				XFillRectangle (xDisplay, xWindow, xGraphicContext, 0, 0, mCurrentSize.width,
				                mCurrentSize.height);
			}
			res = true;
			break;

		//--- StructureNotifyMask ------------------------------
		// Window has been resized
		case ConfigureNotify:
		{
			if (event.xconfigure.window != xWindow)
				break;

			auto width = event.xconfigure.width;
			auto height = event.xconfigure.height;

			Size size {width, height};
			if (mCurrentSize != size)
			{
				auto constraintSize = controller->constrainSize (*x11Window, size);
				if (constraintSize != mCurrentSize)
				{
					mCurrentSize = size;
					controller->onResize (*x11Window, size);
				}
				if (constraintSize != size)
					resize (constraintSize, true);
				else
				{
					if (plugParentWindow)
						XResizeWindow (xDisplay, plugParentWindow, size.width, size.height);
				}

#if LOG_EVENTS
				std::cout << "new size " << width << " x " << height << "\n";
#endif
			}
			res = true;
		}
		break;

		// Window has been map to the screen
		case MapNotify:
		{
			if (event.xany.window == xWindow && !isMapped)
			{
				controller->onShow (*x11Window);
				isMapped = true;
				res = true;
			}
		}
		break;

		case UnmapNotify:
		{
			if (event.xunmap.window == xWindow)
			{
				controller->onClose (*x11Window);
				onClose ();
				res = true;
			}
			break;
		}
		case DestroyNotify: break;

		case ClientMessage:
		{
			if (event.xany.window == xWindow)
			{
				controller->onClose (*x11Window);
				onClose ();
				res = true;
			}
			break;
		}

		case FocusIn:
		{
			if (xembedInfo)
				send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_WINDOW_ACTIVATE, 0,
				                     plugParentWindow, xembedInfo->version);
			break;
		}
		case FocusOut:
		{
			if (xembedInfo)
				send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_WINDOW_DEACTIVATE, 0,
				                     plugParentWindow, xembedInfo->version);
			break;
		}

		//--- ResizeRedirectMask --------------------------------
		case ResizeRequest:
		{
			if (event.xany.window == xWindow)
			{
				auto width = event.xresizerequest.width;
				auto height = event.xresizerequest.height;
				Size request {width, height};
				if (mCurrentSize != request)
				{
#if LOG_EVENTS
					std::cout << "requested Size " << width << " x " << height << "\n";
#endif

					auto constraintSize = controller->constrainSize (*x11Window, request);
					if (constraintSize != request)
					{
#if LOG_EVENTS
						std::cout << "constraint Size " << constraintSize.width << " x "
						          << constraintSize.height << "\n";
#endif
					}
					resize (constraintSize, true);
				}
				res = true;
			}
		}
		break;
	}
	return res;
}

//------------------------------------------------------------------------
auto X11Window::Impl::getXEmbedInfo () -> XEmbedInfo*
{
	int actualFormat;
	unsigned long itemsReturned;
	unsigned long bytesAfterReturn;
	Atom actualType;
	XEmbedInfo* xembedInfo = NULL;
	if (xEmbedInfoAtom == None)
		xEmbedInfoAtom = XInternAtom (xDisplay, "_XEMBED_INFO", true);
	auto err =
	    XGetWindowProperty (xDisplay, plugWindow, xEmbedInfoAtom, 0, sizeof (xembedInfo), false,
	                        xEmbedInfoAtom, &actualType, &actualFormat, &itemsReturned,
	                        &bytesAfterReturn, reinterpret_cast<unsigned char**> (&xembedInfo));
	if (err != Success)
		return nullptr;
	return xembedInfo;
}

//------------------------------------------------------------------------
bool X11Window::Impl::handlePlugEvent (const XEvent& event)
{
	bool res = false;

	switch (event.type)
	{
		// XEMBED specific
		case ClientMessage:
		{
			auto name = XGetAtomName (xDisplay, event.xclient.message_type);
			std::cout << name << std::endl;
			if (event.xclient.message_type == xEmbedAtom)
			{
				switch (event.xclient.data.l[1])
				{
					case XEMBED_REQUEST_FOCUS:
					{
						send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_FOCUS_IN, 0,
						                     plugParentWindow, xembedInfo->version);
						break;
					}
				}
			}
			break;
		}
		case PropertyNotify:
		{
			auto name = XGetAtomName (xDisplay, event.xproperty.atom);
			std::cout << name << std::endl;
			if (event.xany.window == plugWindow)
			{
				if (event.xproperty.atom == xEmbedInfoAtom)
				{
					if (auto embedInfo = getXEmbedInfo ())
					{
					}
				}
				else
				{
				}
			}
			break;
		}
		case CreateNotify:
		{
			if (event.xcreatewindow.parent != plugParentWindow)
			{
				res = true;
				break;
			}

			plugWindow = event.xcreatewindow.window;

			xembedInfo = getXEmbedInfo ();
			if (!xembedInfo)
			{
				std::cerr << "XGetWindowProperty for _XEMBED_INFO failed" << std::endl;
				exit (-1);
			}
			if (xembedInfo->flags & XEMBED_MAPPED)
			{
				std::cerr << "Window already mapped error" << std::endl;
				exit (-1);
			}
			RunLoop::instance ().registerWindow (
			    plugWindow, [this] (const XEvent& e) { return handlePlugEvent (e); });

			// XSelectInput (xDisplay, plugWindow, PropertyChangeMask);

			if (xEmbedAtom == None)
				xEmbedAtom = XInternAtom (xDisplay, "_XEMBED", true);
			assert (xEmbedAtom != None);

			send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_EMBEDDED_NOTIFY, 0,
			                     plugParentWindow, xembedInfo->version);
			XMapWindow (xDisplay, plugWindow);
			XResizeWindow (xDisplay, plugWindow, mCurrentSize.width, mCurrentSize.height);
			// XSetInputFocus (xDisplay, plugWindow, RevertToParent, CurrentTime);
			send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_WINDOW_ACTIVATE, 0,
			                     plugParentWindow, xembedInfo->version);
			send_xembed_message (xDisplay, plugWindow, xEmbedAtom, XEMBED_FOCUS_IN, 0,
			                     plugParentWindow, xembedInfo->version);
			XSync (xDisplay, False);
			res = true;
			break;
		}
	}

	return res;
}
#endif

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
