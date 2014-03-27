#ifndef _H_ORBITER_INTERCOM_INTERFACE_BINDING_H_
#define _H_ORBITER_INTERCOM_INTERFACE_BINDING_H_

#include <Message.h>
#include <common.h>

// this is the static library that imports the OICOM interface from the core module
// it also exports an interface for 3rd party modules

// some typdefs for convinience 
typedef void(*ocstartOICOM)(void);
typedef OICOM_PID(*ocaddPlugin)(char * pluginName);
typedef int(*ocremovePluginByName)(char * pluginName);
typedef int(*ocremovePluginByID)(OICOM_PID plugin);
typedef OICOM_PID(*ocgetPluginByName)(char * pluginName);
typedef char*(*ocgetPluginNameByID)(OICOM_PID plugin);
typedef OICOM_PID(*ocgetPluginByIndex)(DWORD inx);
typedef DWORD(*ocgetPluginCount)(void);
typedef bool(*ocsendMessageByName)(char * pluginName, char * strMsg, void * data, unsigned int dataSize);
typedef bool(*ocsendMessageByID)(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize);
typedef int(*ocnextMessageByName)(char * pluginName, OrbiterPluginMessage ** opm);
typedef int(*ocnextMessageByID)(OICOM_PID pluginID, OrbiterPluginMessage ** opm);
typedef int(*ocremoveLastMessageByName)(char * pluginName);
typedef int(*ocremoveLastMessageByID)(OICOM_PID pluginID);
typedef bool(*ocisPluginByName)(char * pluginName);
typedef bool(*ocisPluginByID)(OICOM_PID pluginID);
typedef void(*octimeStep)(double simt, double simdt, double mjd);
typedef const char*(*ocgetVersion)(void);

namespace OICOM
{
	// C interface for the core
	// See OICOM.H for details about the following functions
	int GetOrbiterIntercom();
	// should be called on clean up - frees the orbiterintercom.dll
	void OrbiterIntercomCleanup();

	OICOM_PID addPlugin(char * pluginName);
	int       removePlugin(char * pluginName);
	int  	  removePlugin(OICOM_PID plugin);
	OICOM_PID getPlugin(char * pluginName);
	char *    getPluginName(OICOM_PID plugin);
	OICOM_PID getPluginByIndex(DWORD inx);
	DWORD     getPluginCount();
	bool      sendMessage(char * pluginName, char * strMsg, void * data, unsigned int dataSize);
	bool      sendMessage(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize);
	int		  nextMessage(char * pluginName, OrbiterPluginMessage ** opm);
	int		  nextMessage(OICOM_PID pluginID, OrbiterPluginMessage ** opm);
	int       removeLastMessage(char * pluginName);
	int       removeLastMessage(OICOM_PID pluginID);
	bool      isPlugin(char * pluginName);
	bool      isPlugin(OICOM_PID pluginID);
	void      timeStep(double simt, double simdt, double mjd); 
};
#endif