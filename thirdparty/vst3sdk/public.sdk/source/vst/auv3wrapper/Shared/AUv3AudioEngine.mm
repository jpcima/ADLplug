//-----------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Helpers
// Filename    : AUv3AudioEngine.mm
// Created by  : Steinberg, 07/2017.
// Description : VST 3 AUv3Wrapper
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

#import "AUv3AudioEngine.h"
#import <CoreMIDI/CoreMIDI.h>
#import <functional>
#import <vector>
#import <utility>

//------------------------------------------------------------------------
namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
class MidiIO
{
public:
	using ReadCallback = std::function<void (const MIDIPacketList* pktlist)>;

	MidiIO (ReadCallback&& callback) : readCallback (std::move (callback)) { activate (); }
	~MidiIO () = default;

private:
	bool activate ();
	bool deactivate ();

	void onSourceAdded (MIDIObjectRef source);
	void onSetupChanged ();
	void disconnectSources ();

	void onInput (const MIDIPacketList* pktlist);

	static void readProc (const MIDIPacketList* pktlist, void* readProcRefCon, void* srcConnRefCon);
	static void notifyProc (const MIDINotification* message, void* refCon);

	MIDIClientRef client {0};
	MIDIPortRef inputPort {0};
	MIDIEndpointRef destPort {0};

	ReadCallback readCallback;

	using ConnectionList = std::vector<MIDIEndpointRef>;
	ConnectionList connectedSources;
};

//------------------------------------------------------------------------
bool MidiIO::activate ()
{
	if (client)
		return true;

	OSStatus err;
	NSString* name = [[NSBundle mainBundle] bundleIdentifier];
	if ((err = MIDIClientCreate ((__bridge CFStringRef)name, notifyProc, this, &client) != noErr))
		return false;

	if ((err = MIDIInputPortCreate (client, CFSTR ("Input"), readProc, this, &inputPort) != noErr))
	{
		MIDIClientDispose (client);
		client = 0;
		return false;
	}
	name = [[[NSBundle mainBundle] infoDictionary] valueForKey:@"CFBundleName"];
	if ((err = MIDIDestinationCreate (client, (__bridge CFStringRef)name, readProc, this,
	                                  &destPort) != noErr))
	{
		MIDIPortDispose (inputPort);
		inputPort = 0;
		MIDIClientDispose (client);
		client = 0;
		return false;
	}
	onSetupChanged ();
	return true;
}

//------------------------------------------------------------------------
bool MidiIO::deactivate ()
{
	if (client == 0)
		return true;
	disconnectSources ();
	auto status = MIDIEndpointDispose (destPort);
	destPort = 0;
	status |= MIDIPortDispose (inputPort);
	inputPort = 0;
	status |= MIDIClientDispose (client);
	client = 0;
	return status == noErr;
}

//------------------------------------------------------------------------
void MidiIO::onSourceAdded (MIDIObjectRef source)
{
	connectedSources.push_back ((MIDIEndpointRef)source);
	MIDIPortConnectSource (inputPort, (MIDIEndpointRef)source, NULL);
}

//------------------------------------------------------------------------
void MidiIO::onSetupChanged ()
{
	disconnectSources ();
	ItemCount numSources = MIDIGetNumberOfSources ();
	for (ItemCount i = 0; i < numSources; i++)
	{
		onSourceAdded (MIDIGetSource (i));
	}
}

//------------------------------------------------------------------------
void MidiIO::disconnectSources ()
{
	for (auto source : connectedSources)
		MIDIPortDisconnectSource (inputPort, source);
	connectedSources.clear ();
}

//------------------------------------------------------------------------
void MidiIO::onInput (const MIDIPacketList* pktlist)
{
	if (readCallback)
		readCallback (pktlist);
}

//------------------------------------------------------------------------
void MidiIO::readProc (const MIDIPacketList* pktlist, void* readProcRefCon, void* srcConnRefCon)
{
	MidiIO* io = static_cast<MidiIO*> (readProcRefCon);
	io->onInput (pktlist);
}

//------------------------------------------------------------------------
void MidiIO::notifyProc (const MIDINotification* message, void* refCon)
{
	if (message->messageID == kMIDIMsgSetupChanged)
	{
		MidiIO* mio = (MidiIO*)refCon;
		mio->onSetupChanged ();
	}
}

using MidiIOPtr = std::unique_ptr<MidiIO>;

//------------------------------------------------------------------------
} // Vst
} // Steinberg

//------------------------------------------------------------------------
@implementation AUv3AudioEngine
{
	AVAudioEngine* audioEngine;
	AVAudioFile* audioFile;
	AVAudioPlayerNode* playerNode;
	AVAudioUnit* avAudioUnit;
	Steinberg::Vst::MidiIOPtr midi;
	UInt32 componentType;
	BOOL playing;
	BOOL isDone;
}

//------------------------------------------------------------------------
- (instancetype)initWithComponentType:(uint32_t)unitComponentType
{
	self = [super init];

	isDone = false;

	if (self)
	{
		audioEngine = [[AVAudioEngine alloc] init];
		componentType = unitComponentType;
	}

#if TARGET_OS_IPHONE
	NSError* error = nil;
	BOOL success =
	    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback error:&error];
	if (NO == success)
	{
		NSLog (@"Error setting category: %@", [error localizedDescription]);
	}
#endif

	playing = false;

	return self;
}

//------------------------------------------------------------------------
- (void)shutdown
{
	if (playing)
		[self stopPlaying];
	audioEngine = nil;
	midi.reset ();
}

//------------------------------------------------------------------------
- (void)onAudioUnitInstantiated:(AVAudioUnit* __nullable)audioUnit
                          error:(NSError* __nullable)error
                     completion:(void (^) (void))completionBlock
{
	if (audioUnit == nil)
		return;

	avAudioUnit = audioUnit;

	_currentAudioUnit = avAudioUnit.AUAudioUnit;
	[audioEngine attachNode:avAudioUnit];
	[audioEngine connect:avAudioUnit to:audioEngine.outputNode format:audioFile.processingFormat];

	completionBlock ();
}

//------------------------------------------------------------------------
- (void)loadAudioUnitWithComponentDescription:(AudioComponentDescription)desc
                                   completion:(void (^) (void))completionBlock
{
	[AVAudioUnit instantiateWithComponentDescription:desc
	                                         options:0
	                               completionHandler:^(AVAudioUnit* __nullable audioUnit,
	                                                   NSError* __nullable error) {
		                             [self onAudioUnitInstantiated:audioUnit
		                                                     error:error
		                                                completion:completionBlock];
	                               }];

	if (componentType == kAudioUnitType_MusicDevice)
	{
		midi = Steinberg::Vst::MidiIOPtr (
		    new Steinberg::Vst::MidiIO ([=] (const MIDIPacketList* pktlist) {
		    [self scheduleMIDIPackets:pktlist];
		}));
	}
}

//------------------------------------------------------------------------
- (NSError*)loadAudioFile:(NSURL*)url
{
	BOOL isPlaying = playing;
	if (isPlaying)
		[self startStop];
	if (playerNode)
	{
		[playerNode stop];
		[audioEngine detachNode:playerNode];
	}

	NSError* error = nil;
	audioFile = [[AVAudioFile alloc] initForReading:url error:&error];
	if (error)
		return error;

	[audioEngine detachNode:avAudioUnit];
	[audioEngine attachNode:avAudioUnit];
	[audioEngine connect:avAudioUnit to:audioEngine.outputNode format:audioFile.processingFormat];

	playerNode = [[AVAudioPlayerNode alloc] init];
	[audioEngine attachNode:playerNode];
	[audioEngine connect:playerNode to:avAudioUnit format:audioFile.processingFormat];

	if (isPlaying)
		[self startStop];

	return nil;
}

//------------------------------------------------------------------------
- (BOOL)startStop
{
	playing = !playing;

	playing ? ([self startPlaying]) : ([self stopPlaying]);

	return playing;
}

//------------------------------------------------------------------------
- (void)startPlaying
{
	[self activateSession:true];
	NSError* error = nil;

	if (![audioEngine startAndReturnError:&error])
	{
		NSLog (@"engine failed to start: %@", error);
		return;
	}

	if (playerNode)
	{
		[self loopAudioFile];
		[playerNode play];
	}
}

//------------------------------------------------------------------------
- (void)stopPlaying
{
	if (playerNode)
		[playerNode stop];
	[audioEngine stop];
	[self activateSession:false];
}

//------------------------------------------------------------------------
- (void)loopAudioFile
{
	if (playerNode)
	{
		[playerNode scheduleFile:audioFile
		                  atTime:nil
		       completionHandler:^{
			     if (playerNode.playing)
				     [self loopAudioFile];
		       }];
	}
}

//------------------------------------------------------------------------
- (void)scheduleMIDIPackets:(const MIDIPacketList*) pktlist
{
	if (!_currentAudioUnit || _currentAudioUnit.scheduleMIDIEventBlock == nil)
		return;

	auto packet = &pktlist->packet[0];
	for (auto i = 0u; i < pktlist->numPackets; i++)
	{
		_currentAudioUnit.scheduleMIDIEventBlock (AUEventSampleTimeImmediate, 0, packet->length,
		                                          packet->data);
		packet = MIDIPacketNext (packet);
	}

}

//------------------------------------------------------------------------
- (void)loopMIDIsequence
{
	UInt8 cbytes[3], *cbytesPtr;

	cbytesPtr = cbytes;

	dispatch_async (dispatch_get_global_queue (DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
	  cbytesPtr[0] = 0xB0;
	  cbytesPtr[1] = 123;
	  cbytesPtr[2] = 0;

	  if (_currentAudioUnit.scheduleMIDIEventBlock == nil)
		  return;

	  _currentAudioUnit.scheduleMIDIEventBlock (AUEventSampleTimeImmediate, 0, 3, cbytesPtr);
	  usleep (useconds_t (0.1 * 1e6));

	  float releaseTime = 0.05;

	  usleep (useconds_t (0.1 * 1e6));

	  int i = 0;

	  @synchronized (self)
	  {
		  while (playing)
		  {
			  if (releaseTime < 10.0)
				  releaseTime = (releaseTime * 1.05) > 10.0 ? (releaseTime * 1.05) : 10.0;

			  cbytesPtr[0] = 0x90;
			  cbytesPtr[1] = UInt8 (60 + i);
			  cbytesPtr[2] = UInt8 (64); // note on
			  _currentAudioUnit.scheduleMIDIEventBlock (AUEventSampleTimeImmediate, 0, 3,
				                                        cbytesPtr);

			  usleep (useconds_t (0.2 * 1e6));

			  cbytesPtr[0] = 0x80;
			  cbytesPtr[1] = UInt8 (60 + i);
			  cbytesPtr[2] = UInt8 (0); // note off
			  _currentAudioUnit.scheduleMIDIEventBlock (AUEventSampleTimeImmediate, 0, 3,
				                                        cbytesPtr);

			  i += 2;
			  if (i >= 24)
			  {
				  i = -12;
			  }
		  }

		  cbytesPtr[0] = 0xB0;
		  cbytesPtr[1] = 123;
		  cbytesPtr[2] = 0;
		  _currentAudioUnit.scheduleMIDIEventBlock (AUEventSampleTimeImmediate, 0, 3, cbytesPtr);

		  isDone = true;
	  }
	});
}

//------------------------------------------------------------------------
- (void)activateSession:(BOOL)active
{
#if TARGET_OS_IPHONE
	NSError* error = nil;
	BOOL success = [[AVAudioSession sharedInstance] setActive:active error:nil];
	if (NO == success)
	{
		NSLog (@"Error setting category: %@", [error localizedDescription]);
	}
#endif
}

@end
