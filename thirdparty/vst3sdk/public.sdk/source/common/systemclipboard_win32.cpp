//------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
//
// Project     : Steinberg Plug-In SDK
// Filename    : public.sdk/source/common/systemclipboard_win32.h
// Created by  : Steinberg 04.2020
// Description : Simple helper allowing to copy/retrieve text to/from the system clipboard
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// This Software Development Kit may not be distributed in parts or its entirety
// without prior written agreement by Steinberg Media Technologies GmbH.
// This SDK must not be used to re-engineer or manipulate any technology used
// in any Steinberg or Third-party application or software module,
// unless permitted by law.
// Neither the name of the Steinberg Media Technologies nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// THIS SDK IS PROVIDED BY STEINBERG MEDIA TECHNOLOGIES GMBH "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL STEINBERG MEDIA TECHNOLOGIES GMBH BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "systemclipboard.h"

#if SMTG_OS_WINDOWS

#include <windows.h>

namespace Steinberg {
namespace SystemClipboard {

//-----------------------------------------------------------------------------
bool copyTextToClipboard (const String& text)
{
	// Try opening the clipboard
	if (!OpenClipboard (nullptr))
		return false;

	bool result = false;

	int32 length = text.length ();
	if (length > 0)
	{
		if (auto memory =
		        GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, (length + 1) * sizeof (WCHAR)))
		{
			if (auto* data = static_cast<WCHAR*> (GlobalLock (memory)))
			{
				text.copyTo16 (data);
				GlobalUnlock (memory);

				SetClipboardData (CF_UNICODETEXT, memory);
				result = true;
			}
		}
	}
	// Release the clipboard
	CloseClipboard ();

	return result;
}

//-----------------------------------------------------------------------------
bool getTextFromClipboard (String& text)
{
	// Try opening the clipboard
	if (!OpenClipboard (nullptr))
		return false;

	// check if clipboard is empty
	if (EmptyClipboard ())
	{
		CloseClipboard ();
		return false;
	}

	bool result = false;

	// Get handle of clipboard object for unicode text
	if (auto hData = GetClipboardData (CF_UNICODETEXT))
	{
		// Lock the handle to get the actual text pointer
		if (auto* data = (const WCHAR*)GlobalLock (hData))
		{
			text.append (data, static_cast<int32> (GlobalSize (hData) / sizeof (WCHAR)));

			// Release the lock
			GlobalUnlock (hData);

			result = true;
		}
	}

	// Release the clipboard
	CloseClipboard ();

	return result;
}
} // namespace SystemClipboard
} // namespace Steinberg

#endif
