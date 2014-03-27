#define STRICT
#define ORBITER_MODULE

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <orbitersdk.h>
#include "CruiseMFD.h"
#include <oicominit.h>

int mode;      // identifier for new MFD mode
double setpoint=10.; // m/s setpoint for cruise 
bool active = false, init=false;

OICOM_PID pid=0, cruisepid; // our plugin ID

DLLCLBK void InitModule (HINSTANCE hDLL)
{
	// OICOM initialization
	int ret = OICOM::GetOrbiterIntercom();
	// this MFD is done very quickly for demonstration purposes
	// I didn't want fluff to get in the way of demonstrating how
	// the library is supposed to work. But for 
	// implementations of the MFD that take in account
	// mulitple vessel instances (ie. storing mfd internal data, giving each vessel its own MFD)
	// you can register the plugin with any naming scheme, like vesselname_oicomCruiseMFD etc etc
	pid=OICOM::addPlugin("OicomCruiseMFD");

	static char *name = "OICOM Cruise MFD";
	MFDMODESPECEX spec;
	spec.name    = name;
	spec.key     = OAPI_KEY_C;
	spec.context = NULL;
	spec.msgproc = CruiseMFD::MsgProc;
	mode = oapiRegisterMFDMode (spec);
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	oapiUnregisterMFDMode (mode);
}

// get the id for the cruise module
// that module provides the actual "autopilot" code
// this is just the interface
// this should really be done in opcOpenRenderViewport
// but I can't remember the parameter list right now :P
// since the initialization for modules occurs at InitModule, 
// this way we are sure the module we are targetting will be initialized
DLLCLBK void opcPreStep (double simt, double simdt, double mjd)
{
	if ( !init )
	{
		// get the plug-in ID for future communication
		cruisepid = OICOM::getPlugin("oicomcruisemodule");
		// link up state variables
		OICOM::sendMessage(cruisepid, "link_state", &active, sizeof(bool));
		OICOM::sendMessage(cruisepid, "link_setpoint", &setpoint, sizeof(double*));
		init=true;
	}
}

CruiseMFD::CruiseMFD (DWORD w, DWORD h, VESSEL *vessel)
: MFD (w, h, vessel)
{
	width=w/35;
	height=h/24;
}

CruiseMFD::~CruiseMFD ()
{
}


// message parser
int CruiseMFD::MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENEDEX: {
		MFDMODEOPENSPEC *ospec = (MFDMODEOPENSPEC*)wparam;
		return (int)(new CruiseMFD (ospec->w, ospec->h, (VESSEL*)lparam));
		}
	}
	return 0;
}

bool getSetpoint (void *id, char *str, void *data)
{
	if ( strlen(str) == 0 ) 
		return false;
	setpoint=atof(str);
	return true;
}

bool CruiseMFD::ConsumeKeyBuffered (DWORD key)
{
	switch (key) 
	{
	case OAPI_KEY_S:
		oapiOpenInputBox ("Enter setpoint m/s:", getSetpoint, 0, 20, 0);
		return true;
	case OAPI_KEY_T:
		{
			active=!active;
			return true;
		}
		
	}
	return false;
}

bool CruiseMFD::ConsumeButton (int bt, int event)
{
	if (!(event & PANEL_MOUSE_LBDOWN)) return false;
	static const DWORD btkey[2] = { OAPI_KEY_S, OAPI_KEY_T };
	if (bt < 2) return ConsumeKeyBuffered (btkey[bt]);
	else return false;
}

char *CruiseMFD::ButtonLabel (int bt)
{
	char *label[2] = {"SP", "TGL"};
	return (bt < 2 ? label[bt] : 0);
}

int CruiseMFD::ButtonMenu (const MFDBUTTONMENU **menu) const
{
	static const MFDBUTTONMENU mnu[2] = {
		{"set the setpoint", 0, 'S'},
		{"Toggle cruise", 0, 'T'}
	};
	if (menu) *menu = mnu;
	return 2;
}

void CruiseMFD::Update (HDC hDC)
{
	Title (hDC, "OICOM Cruise MFD");

	int len;
	char buffer[256];
	int y = height*2;

	len = sprintf(buffer, "state: %s", (active?"active":"deactive"));
	TextOut(hDC, width, y, buffer, len);
	y+=height*1;
	len = sprintf(buffer, "sp   : %.2f m/s", setpoint);
	TextOut(hDC, width, y, buffer, len);
}
