//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/linux/runloop.cpp
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
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "public.sdk/samples/vst-hosting/editorhost/source/platform/linux/runloop.h"
#include <algorithm>
#include <iostream>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

using LockGuard = std::lock_guard<std::recursive_mutex>;

//------------------------------------------------------------------------
RunLoop& RunLoop::instance ()
{
	static RunLoop gInstance;
	return gInstance;
}

//------------------------------------------------------------------------
void RunLoop::setDisplay (Display* display)
{
	this->display = display;
}

//------------------------------------------------------------------------
void RunLoop::registerWindow (XID window, const EventCallback& callback)
{
	map.emplace (window, callback);
}

//------------------------------------------------------------------------
void RunLoop::unregisterWindow (XID window)
{
	auto it = map.find (window);
	if (it == map.end ())
		return;
	map.erase (it);
}

//------------------------------------------------------------------------
void RunLoop::registerFileDescriptor (int fd, const FileDescriptorCallback& callback)
{
	fileDescriptors.emplace (fd, callback);
}

//------------------------------------------------------------------------
void RunLoop::unregisterFileDescriptor (int fd)
{
	auto it = fileDescriptors.find (fd);
	if (it == fileDescriptors.end ())
		return;
	fileDescriptors.erase (it);
}

//------------------------------------------------------------------------
void RunLoop::select (timeval* timeout)
{
	int nfds = 0;
	fd_set readFDs = {}, writeFDs = {}, exceptFDs = {};

	for (auto& e : fileDescriptors)
	{
		int fd = e.first;
		FD_SET (fd, &readFDs);
		FD_SET (fd, &writeFDs);
		FD_SET (fd, &exceptFDs);
		nfds = std::max (nfds, fd);
	}

	int result = ::select (nfds, &readFDs, &writeFDs, nullptr, timeout);

	if (result > 0)
	{
		for (auto& e : fileDescriptors)
		{
			if (FD_ISSET (e.first, &readFDs) || FD_ISSET (e.first, &writeFDs) ||
			    FD_ISSET (e.first, &exceptFDs))
				e.second (e.first);
		}
	}
}

//------------------------------------------------------------------------
bool RunLoop::handleEvents ()
{
	auto count = XPending (display);
	if (count == 0)
		return false;
	for (auto i = 0; i < count; ++i)
	{
		XEvent event {};
		XNextEvent (display, &event);
		auto it = map.find (event.xany.window);
		if (it != map.end ())
		{
			it->second (event);
			if (event.type == DestroyNotify)
			{
				map.erase (it);
			}
		}
		else
		{
			XPutBackEvent (display, &event);
			break;
		}
	}
	return true;
}

//------------------------------------------------------------------------
TimerID RunLoop::registerTimer (TimerInterval interval, const TimerCallback& callback)
{
	return timerProcessor.registerTimer (interval, callback);
}

//------------------------------------------------------------------------
void RunLoop::unregisterTimer (TimerID id)
{
	timerProcessor.unregisterTimer (id);
}

//------------------------------------------------------------------------
bool timeValEmpty (timeval& val)
{
	return val.tv_sec == 0 && val.tv_usec == 0;
}

//------------------------------------------------------------------------
void RunLoop::start ()
{
	using namespace std::chrono;
	using clock = high_resolution_clock;

	running = true;

	auto fd = XConnectionNumber (display);
	registerFileDescriptor (fd, [this] (int) { handleEvents (); });

	XSync (display, false);
	handleEvents ();
	timeval selectTimeout {};
	while (running && !map.empty ())
	{
		select (timeValEmpty (selectTimeout) ? nullptr : &selectTimeout);
		auto nextFireTime = timerProcessor.handleTimersAndReturnNextFireTimeInMs ();
		if (nextFireTime == TimerProcessor::noTimers)
		{
			selectTimeout = {};
		}
		else
		{
			selectTimeout.tv_sec = nextFireTime / 1000;
			selectTimeout.tv_usec = (nextFireTime - (selectTimeout.tv_sec * 1000)) * 1000;
		}
	}
}

//------------------------------------------------------------------------
void RunLoop::stop ()
{
	running = false;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
uint64_t TimerProcessor::handleTimersAndReturnNextFireTimeInMs ()
{
	using std::chrono::time_point_cast;

	if (timers.empty ())
		return noTimers;

	auto current = time_point_cast<Millisecond> (Clock::now ());

	std::vector<TimerID> timersToFire;
	for (auto& timer : timers)
	{
		if (timer.nextFireTime > current)
			break;
		timersToFire.push_back (timer.id);
		updateTimerNextFireTime (timer, current);
	}

	for (auto id : timersToFire)
	{
		for (auto& timer : timers)
		{
			if (timer.id == id)
			{
				timer.callback (timer.id);
				break;
			}
		}
	}
	if (timersToFire.empty ())
		return noTimers;

	sortTimers ();

	auto nextFireTime = timers.front ().nextFireTime;
	current = now ();
	if (nextFireTime < current)
		return 0;
	return (nextFireTime - current).count ();
}

//------------------------------------------------------------------------
void TimerProcessor::updateTimerNextFireTime (Timer& timer, TimePoint current)
{
	timer.nextFireTime = current + Millisecond (timer.interval);
}

//------------------------------------------------------------------------
void TimerProcessor::sortTimers ()
{
	std::sort (timers.begin (), timers.end (),
	           [] (const Timer& t1, const Timer& t2) { return t1.nextFireTime < t2.nextFireTime; });
}

//------------------------------------------------------------------------
auto TimerProcessor::now () -> TimePoint
{
	using std::chrono::time_point_cast;

	return time_point_cast<Millisecond> (Clock::now ());
}

//------------------------------------------------------------------------
auto TimerProcessor::registerTimer (TimerInterval interval, const TimerCallback& callback)
    -> TimerID
{
	auto timerId = ++timerIdCounter;
	Timer timer;
	timer.id = timerId;
	timer.callback = callback;
	timer.interval = interval;
	updateTimerNextFireTime (timer, now ());

	timers.emplace_back (std::move (timer));
	sortTimers ();

	return timerId;
}

//------------------------------------------------------------------------
void TimerProcessor::unregisterTimer (TimerID id)
{
	for (auto it = timers.begin (), end = timers.end (); it != end; ++it)
	{
		if (it->id == id)
		{
			timers.erase (it);
			break;
		}
	}
}

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
