#include <windows.h>
#include <string.h>
#include <orbitersdk.h>
#include <string>
#include "OICOM.h"
#include "OICOMMFD.h"

// ==============================================================
// MFD class implementation

// Constructor
OICOMMFD::OICOMMFD (DWORD w, DWORD h, VESSEL *vessel)
: MFD2 (w, h, vessel)
{
	font = oapiCreateFont (w/25, true, "Arial", FONT_NORMAL);
	width=w/35;
	height=h/24;
	// Add MFD initialisation here
}

// Destructor
OICOMMFD::~OICOMMFD ()
{
	oapiReleaseFont (font);
	// Add MFD cleanup code here
}

// Return button labels
char *OICOMMFD::ButtonLabel (int bt)
{
	// The labels for the two buttons used by our MFD mode
	static char *label[1] = {"SND"};
	return (bt < 1 ? label[bt] : 0);
}
bool OICOMMFD::ConsumeButton (int bt, int event)
{
	if (!(event & PANEL_MOUSE_LBDOWN)) return false;
	static const DWORD btkey[1] = { OAPI_KEY_S };
	if (bt < 1) return ConsumeKeyBuffered (btkey[bt]);
	else return false;
}

// Return button menus
int OICOMMFD::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	// The menu descriptions for the two buttons
	static const MFDBUTTONMENU mnu[1] = {
		{"Send a plug-in msg with PID", 0, 'S'}
	};
	if (menu) *menu = mnu;
	return 1; // return the number of buttons used
}
bool OICOMMFD::ConsumeKeyBuffered(DWORD key)
{
	switch(key)
	{
	case OAPI_KEY_S:
		oapiOpenInputBox("Enter msg in form [pid msg]: ", sendMsgWithID, 0, 20, 0);
		break;
	}
	return true;
}
