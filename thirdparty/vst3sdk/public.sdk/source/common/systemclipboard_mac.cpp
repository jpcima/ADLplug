//------------------------------------------------------------------------
// Flags       : clang-format SMTGSequencer
//
// Project     : Steinberg Plug-In SDK
// Filename    : public.sdk/source/common/systemclipboard_mac.mm
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

#if SMTG_OS_MACOS
#include <array>
#include <cstdio>
#include <string>

namespace Steinberg {
namespace SystemClipboard {

//-----------------------------------------------------------------------------
bool copyTextToClipboard (const String& text)
{
	/* NSPasteboard* pasteBoard = [NSPasteboard generalPasteboard];

	[pasteBoard declareTypes: [NSArray arrayWithObject: NSPasteboardTypeString] owner: nil];

// TODO text to NSString
//	string
//	[pasteBoard setString: string forType: NSPasteboardTypeString];
*/
	bool result = false;
	if (text.length () > 0)
	{
		String command;
		command.printf ("echo %s | cbcopy", text.text8 ());

		system (command);
		result = true;
	}
	return result;
}

//-----------------------------------------------------------------------------
bool getTextFromClipboard (String& text)
{
	//[[NSPasteboard generalPasteboard] stringForType: NSPasteboardTypeString];

	auto pipe = ::popen ("cbpaste", "r");
	if (!pipe)
		return false;

	std::array<char, 128> buffer;
	std::string result;

	while (::fgets (buffer.data (), 128, pipe) != nullptr)
		result += buffer.data ();

	::pclose (pipe);

	text = result.data ();
	return !result.empty ();
}

} // namespace SystemClipboard
} // namespace Steinberg

#endif
