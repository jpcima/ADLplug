//-----------------------------------------------------------------------------
// Flags       : clang-format auto
// Project     : VST SDK
//
// Category    : EditorHost
// Filename    : public.sdk/samples/vst-hosting/editorhost/source/platform/linux/runloop.h
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

#pragma once

#include <X11/Xlib.h>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {
namespace EditorHost {

//------------------------------------------------------------------------
using TimerID = uint64_t;
using TimerInterval = uint64_t;
using TimerCallback = std::function<void (TimerID)>;

//------------------------------------------------------------------------
class TimerProcessor
{
public:
	TimerID registerTimer (TimerInterval interval, const TimerCallback& callback);
	void unregisterTimer (TimerID id);

	static constexpr uint64_t noTimers = std::numeric_limits<uint64_t>::max ();
	uint64_t handleTimersAndReturnNextFireTimeInMs ();

private:
	using Clock = std::chrono::steady_clock;
	using Millisecond = std::chrono::milliseconds;
	using TimePoint = std::chrono::time_point<Clock, Millisecond>;

	struct Timer
	{
		TimerID id;
		TimerInterval interval;
		TimerCallback callback;
		TimePoint nextFireTime;
	};
	using Timers = std::vector<Timer>;
	Timers timers;
	TimerID timerIdCounter {0};

	void updateTimerNextFireTime (Timer& timer, TimePoint current);
	void sortTimers ();
	TimePoint now ();
};

//------------------------------------------------------------------------
class RunLoop
{
public:
	using EventCallback = std::function<bool (const XEvent& event)>;
	using FileDescriptorCallback = std::function<void (int fd)>;

	static RunLoop& instance ();

	void setDisplay (Display* display);

	void registerWindow (XID window, const EventCallback& callback);
	void unregisterWindow (XID window);

	void registerFileDescriptor (int fd, const FileDescriptorCallback& callback);
	void unregisterFileDescriptor (int fd);

	TimerID registerTimer (TimerInterval interval, const TimerCallback& callback);
	void unregisterTimer (TimerID id);

	void start ();
	void stop ();

private:
	void select (timeval* timeout = nullptr);
	bool handleEvents ();

	using WindowMap = std::unordered_map<XID, EventCallback>;
	using FileDescriptorCallbacks = std::unordered_map<int, FileDescriptorCallback>;

	WindowMap map;
	FileDescriptorCallbacks fileDescriptors;
	TimerProcessor timerProcessor;

	Display* display {nullptr};
	bool running {false};
};

//------------------------------------------------------------------------
} // EditorHost
} // Vst
} // Steinberg
