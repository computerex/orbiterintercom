#include <Windows.h>
#include <stdio.h>
#include <oicominit.h>

ocstartOICOM                 startoicom;
ocgetVersion                 getversion;
ocaddPlugin                  addplugin;
ocremovePluginByName         removepluginbyname;
ocremovePluginByID           removepluginbyid;
ocgetPluginByName            getpluginbyname;
ocgetPluginNameByID          getpluginnamebyid;
ocgetPluginByIndex           getpluginbyindex;
ocgetPluginCount             getplugincount;
ocsendMessageByName          sendmessagebyname;
ocsendMessageByID            sendmessagebyid;
ocnextMessageByName          nextmessagebyname;
ocnextMessageByID            nextmessagebyid;
ocremoveLastMessageByName    removelastmessagebyname;
ocremoveLastMessageByID      removelastmessagebyid;
ocisPluginByName             ispluginbyname;
ocisPluginByID               ispluginbyid;
octimeStep                   timestep;

HMODULE dll = 0;

int OICOM::GetOrbiterIntercom()
{
	dll = LoadLibrary("orbiterintercom.dll");
	if ( dll == 0 ){
		MessageBox(HWND_DESKTOP, "Orbiter Intercom Library not installed! Install the library or turn off the module", "Initialization Error", MB_OK);
		::exit(0);
		return 1;
	}
	
	// late bindings
	getversion                = (ocgetVersion)GetProcAddress(dll, "oicom_getVersion");
	// startOICOM is NOT exported to be used by 3rd party modules
	// it is imported and used here, and should not be used anywhere else
	startoicom                = (ocstartOICOM)GetProcAddress(dll, "oicom_startOICOM");
	addplugin                 = (ocaddPlugin)GetProcAddress(dll, "oicom_addPlugin");
	removepluginbyname        = (ocremovePluginByName)GetProcAddress(dll, "oicom_removePluginByName");
	removepluginbyid		  = (ocremovePluginByID)GetProcAddress(dll, "oicom_removePluginByID");
	getpluginbyname           = (ocgetPluginByName)GetProcAddress(dll, "oicom_getPluginByName");
	getpluginnamebyid		  = (ocgetPluginNameByID)GetProcAddress(dll, "oicom_getPluginNameByID");
	getpluginbyindex          = (ocgetPluginByIndex)GetProcAddress(dll, "oicom_getPluginByIndex");
	getplugincount            = (ocgetPluginCount)GetProcAddress(dll, "oicom_getPluginCount");
	sendmessagebyname         = (ocsendMessageByName)GetProcAddress(dll, "oicom_sendMessageByName");
	sendmessagebyid           = (ocsendMessageByID)GetProcAddress(dll, "oicom_sendMessageByID");
	nextmessagebyname         = (ocnextMessageByName)GetProcAddress(dll, "oicom_nextMessageByName");
	nextmessagebyid           = (ocnextMessageByID)GetProcAddress(dll, "oicom_nextMessageByID");
	removelastmessagebyname   = (ocremoveLastMessageByName)GetProcAddress(dll, "oicom_removeLastMessageByName");
	removelastmessagebyid     = (ocremoveLastMessageByID)GetProcAddress(dll, "oicom_removeLastMessageByID");
	ispluginbyname            = (ocisPluginByName)GetProcAddress(dll, "oicom_isPluginByName");
	ispluginbyid              = (ocisPluginByID)GetProcAddress(dll, "oicom_isPluginByID");
	timestep                  = (octimeStep)GetProcAddress(dll, "oicom_timeStep");

	// check the version
	if ( strcmp(getversion(), OICOM_VERSION) ){
		char buffer[256];
		sprintf_s(buffer, "OICOM Version discrepency! Library installed version: %s version used by this plugin: %s",
						getversion(), OICOM_VERSION);
		MessageBox(HWND_DESKTOP, buffer, "Version Error", MB_OK);
	}
	// the call here is to make absolute sure that OICOM is initialized
	startoicom();
	return 0;
}
void OICOM::OrbiterIntercomCleanup()
{
	if ( dll )
		FreeLibrary(dll);
}
OICOM_PID OICOM::addPlugin(char * pluginName)
{
	return addplugin(pluginName);
}
int OICOM::removePlugin(char * pluginName)
{
	return removepluginbyname(pluginName);
}
int OICOM::removePlugin(OICOM_PID plugin)
{
	return removepluginbyid(plugin);
}
OICOM_PID OICOM::getPlugin(char * pluginName)
{
	return getpluginbyname(pluginName);
}
char* OICOM::getPluginName(OICOM_PID plugin)
{
	return getpluginnamebyid(plugin);
}
OICOM_PID OICOM::getPluginByIndex(DWORD inx)
{
	return getpluginbyindex(inx);
}
DWORD OICOM::getPluginCount()
{
	return getplugincount();
}
bool OICOM::sendMessage(char * pluginName, char * strMsg, void * data, unsigned int dataSize)
{
	return sendmessagebyname(pluginName, strMsg, data, dataSize);
}
bool OICOM::sendMessage(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize)
{
	return sendmessagebyid(pluginID, strMsg, data, dataSize);
}
int OICOM::nextMessage(char * pluginName, OrbiterPluginMessage ** opm)
{
	return nextmessagebyname(pluginName, opm);
}
int OICOM::nextMessage(OICOM_PID pluginID, OrbiterPluginMessage ** opm)
{
	return nextmessagebyid(pluginID, opm);
}
int OICOM::removeLastMessage(char * pluginName)
{
	return removelastmessagebyname(pluginName);
}
int OICOM::removeLastMessage(OICOM_PID pluginID)
{
	return removelastmessagebyid(pluginID);
}
bool OICOM::isPlugin(char * pluginName)
{
	return ispluginbyname(pluginName);
}
bool OICOM::isPlugin(OICOM_PID pluginID)
{
	return ispluginbyid(pluginID);
}
void OICOM::timeStep(double simt, double simdt, double mjd)
{
	timestep(simt, simdt, mjd);
}