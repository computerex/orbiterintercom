#define STRICT
#define ORBITER_MODULE

#include <orbitersdk.h>
#include "OICOM.h"
#include "OICOMMFD.h"

OrbiterIntercom * oicom=0;
int g_MFDmode; // identifier for new MFD mode

OrbiterIntercom* __GetOrbiterIntercom__()
{
	if ( oicom!=0)return oicom;
	oicom=new OrbiterIntercom();
	return oicom;
}

DLLCLBK void opcPreStep(double simt, double simdt, double mjd)
{
}

DLLCLBK void InitModule(HINSTANCE module)
{
	// THIS IS THE ONLY PLACE THIS METHOD CAN BE CALLED
	// DO NOT CALL IT FROM ANYWHERE ELSE!
	__GetOrbiterIntercom__(); 
	static char *name = "OICOM MFD";   // MFD mode name
	MFDMODESPECEX spec;
	spec.name = name;
	spec.key = OAPI_KEY_O;                // MFD mode selection key
	spec.context = NULL;
	spec.msgproc = OICOMMFD::MsgProc;  // MFD mode callback function

	// Register the new MFD mode with Orbiter
	g_MFDmode = oapiRegisterMFDMode (spec);
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	// Unregister the custom MFD mode when the module is unloaded
	oapiUnregisterMFDMode (g_MFDmode);
}

// MFD message parser
int OICOMMFD::MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam)
{
	switch (msg) {
	case OAPI_MSG_MFD_OPENED:
		// Our new MFD mode has been selected, so we create the MFD and
		// return a pointer to it.
		return (int)(new OICOMMFD (LOWORD(wparam), HIWORD(wparam), (VESSEL*)lparam));
	}
	return 0;
}

// Repaint the MFD
bool OICOMMFD::Update (oapi::Sketchpad *skp)
{
	Title (skp, "OICOM MFD");
	// Draws the MFD title

	skp->SetFont(font);

	int y = height*2;
	const char * msg;
	for ( unsigned int i = 0; i < oicom->mfdlog.m.size(); i++ ){
		msg = oicom->mfdlog.m[i].c_str();
		skp->Text (width, y,msg, strlen(msg));
		y+=height;
	}
	return true;
}

bool sendMsgWithID(void *id, char *str, void *data)
{
	// sorry, but minimal input checking
	unsigned int len = strlen(str);
	if ( len == 0 ) return false;
	char * pch = strtok(str, " ");
	if ( pch == 0 ) return false;
	len = strlen(pch);
	for ( unsigned int i = 0; i < len; i++ )
		if ( isalpha(pch[i]) ) // expecting first token to be pid, and found alpha character
			return false;
	OICOM_PID pid = atoi(pch);
	pch = strtok(NULL, ""); // get the rest of the message
	oicom->sendMessage(pid, pch, 0, 0);
	return true;
}

// HERE IS THE IMPLEMENTATION OF THE C INTERFACE!!

DLLCLBK void oicom_startOICOM()
{
	// initialize oicom
	// this method will be called by
	// all modules using this library
	// to ensure OICOM is active before any 
	// addPlugin/removePlugin calls are sent
	__GetOrbiterIntercom__();
}
DLLCLBK OICOM_PID oicom_addPlugin(char * pluginName)
{
	return oicom->addPlugin(pluginName);
}

DLLCLBK int oicom_removePluginByName(char * pluginName)
{
	return oicom->removePlugin(pluginName);
}

DLLCLBK int oicom_removePluginByID(OICOM_PID plugin)
{
	return oicom->removePlugin(plugin);
}

DLLCLBK OICOM_PID oicom_getPluginByName(char * pluginName)
{
	return oicom->getPluginByName(pluginName);
}

DLLCLBK char * oicom_getPluginNameByID(OICOM_PID plugin)
{
	return oicom->getPluginNameByID(plugin);
}

DLLCLBK OICOM_PID oicom_getPluginByIndex(DWORD inx)
{
	return oicom->getPluginByIndex(inx);
}

DLLCLBK DWORD oicom_getPluginCount()
{
	return oicom->getPluginCount();
}

DLLCLBK bool oicom_sendMessageByName(char * pluginName, char * strMsg, void * data, unsigned int dataSize)
{
	return oicom->sendMessage(pluginName, strMsg, data, dataSize);
}

DLLCLBK bool oicom_sendMessageByID(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize)
{
	return oicom->sendMessage(pluginID, strMsg, data, dataSize);
}

DLLCLBK int oicom_nextMessageByName(char * pluginName, OrbiterPluginMessage ** opm)
{
	return oicom->nextMessage(pluginName, opm);
}

DLLCLBK int oicom_nextMessageByID(OICOM_PID pluginID, OrbiterPluginMessage ** opm)
{
	return oicom->nextMessage(pluginID, opm);
}

DLLCLBK int oicom_removeLastMessageByName(char * pluginName)
{
	return oicom->removeLastMessage(pluginName);
}

DLLCLBK int oicom_removeLastMessageByID(OICOM_PID pluginID)
{
	return oicom->removeLastMessage(pluginID);
}

DLLCLBK bool oicom_isPluginByName(char * pluginName)
{
	return oicom->isPlugin(pluginName);
}

DLLCLBK bool oicom_isPluginByID(OICOM_PID pluginID)
{
	return oicom->isPlugin(pluginID);
}

DLLCLBK void oicom_timeStep(double simt, double simdt, double mjd)
{
	oicom->timeStep(simt, simdt, mjd);
}
DLLCLBK const char* oicom_getVersion()
{
	static char version[] = OICOM_VERSION;
	return version;
}