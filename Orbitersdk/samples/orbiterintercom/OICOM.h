#ifndef _H_ORBITER_INTERCOM_H_
#define _H_ORBITER_INTERCOM_H_

#include <windows.h>
#include "Plugin.h"
#include "hudscroll.h"
#include <message.h>
#include <common.h>

#define DLLCLBK extern "C" __declspec(dllexport)

// this is the main class that handles all the message forwarding/plug-in registration/etc
// do not create any instances of this class! This is a purely internal class
// all operations should be done with the exported C interface below this class
class OrbiterIntercom
{
public:
	OrbiterIntercom();
	~OrbiterIntercom(); // dellocates the memory allocated for the plug-ins

	// PLUGIN NAMES ARE CASE INSENSITIVE AND MUST BE AT LEAST 6 CHARACTERS LONG!!

	// register a plugin
	// if a plugin with the identifier pluginName already exists, PLUGIN_ALREADY_EXISTS is returned
	// otherwise if everything went well the plugin numerical id is returned. PLUGIN_INVALID is returned for unknown errors
	// PLUGIN_NAME_TOO_SHORT is returned if pluginName is less than 6 characters
	OICOM_PID addPlugin(char * pluginName);

	// removes the specified plug-in
	// returns PLUGIN_INVALID if no plugin with name pluignName or id "plugin" exists
	// returns PLUGIN_REMOVED if everything went ok
	int       removePlugin(char * pluginName);
	int  	  removePlugin(OICOM_PID plugin);

	// returns the plug-in ID given the name
	// returns PLUGIN_INVALID if no plugin with name pluginName exists
	OICOM_PID getPluginByName(char * pluginName);

	// returns NULL if no plugin with id "plugin" exists
	char *    getPluginNameByID(OICOM_PID plugin);

	// returns the plugin corresponding with the specified index
	// each plug-in has a numerical index from 0..n-1 where n is the number of plug-ins registered
	// do not rely on the index not changing to keep track of specific plug-ins! use the unique plug-in ID
	// index can and will change in the future
	// returns PLUGIN_INVALID if index is out of range
	OICOM_PID getPluginByIndex(DWORD inx);

	// returns the number of plugins registered
	DWORD     getPluginCount();

	// sends message to the specified plug-in
	// if a plugin with name pluginName or id pluginID doesn't exist function returns false
	bool      sendMessage(char * pluginName, OrbiterPluginMessage * msg);
	bool      sendMessage(char * pluginName, char * strMsg, void * data, unsigned int dataSize);
	bool      sendMessage(OICOM_PID pluginID, OrbiterPluginMessage * msg);
	bool      sendMessage(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize);

	// sets opm to point to the next message on the message stack and returns MESSAGE_OK if successful
	// returns MESSAGE_NONE_EXISTS if no messages left on stack 
	// if the pluginName or pluginID are invalid return PLUGIN_INVALID
	int		  nextMessage(char * pluginName, OrbiterPluginMessage ** opm);
	int		  nextMessage(OICOM_PID pluginID, OrbiterPluginMessage ** opm);

	// delete last message on the stack. 
	// returns MESSAGE_DELETE_OK if everything was fine
	// returns PLUGIN_INVALID if no plug-in exists with name pluginName or ID pluginID
	// returns MESSAGE_NONE_EXISTS if no messages on the stack
	int       removeLastMessage(char * pluginName);
	int       removeLastMessage(OICOM_PID pluginID);

	// returns true if plugin exists and is valid false if it doesn't
	bool      isPlugin(char * pluginName);
	bool      isPlugin(OICOM_PID pluginID);

	// must be called every timestep!
	void      timeStep(double simt, double simdt, double mjd); 
	HUDSCROLL  mfdlog;                    // stores up to 10 events and prints them in the MFD display 
										  // a more through and useful logger will follow for debugging
private:
	// plug-in must exist 
	// only meant to be used internally
	DWORD     getPluginIndex(OICOM_PID pluginID);
	void      removePluginByIndex(DWORD inx);
//	void	  forwardMessages();
private:
	std::vector<OrbiterPlugin*> plugins; // plug-in list
	DWORD counter;                       // used for the purposes of assigning numerical plugin ids
};

// DO NOT CALL THIS
OrbiterIntercom* __GetOrbiterIntercom__(); 

// this is the C interface used to interact with the module
// only the following functions should be used, do not ever
// try to instantiate the above class. Use the oicominterface libary.
DLLCLBK const char* oicom_getVersion();
DLLCLBK void      oicom_startOICOM();
DLLCLBK OICOM_PID oicom_addPlugin(char * pluginName);
DLLCLBK int       oicom_removePluginByName(char * pluginName);
DLLCLBK int  	  oicom_removePluginByID(OICOM_PID plugin);
DLLCLBK OICOM_PID oicom_getPluginByName(char * pluginName);
DLLCLBK char *    oicom_getPluginNameByID(OICOM_PID plugin);
DLLCLBK OICOM_PID oicom_getPluginByIndex(DWORD inx);
DLLCLBK DWORD     oicom_getPluginCount();
DLLCLBK bool      oicom_sendMessageByName(char * pluginName, char * strMsg, void * data, unsigned int dataSize);
DLLCLBK bool      oicom_sendMessageByID(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize);
DLLCLBK int		  oicom_nextMessageByName(char * pluginName, OrbiterPluginMessage ** opm);
DLLCLBK int		  oicom_nextMessageByID(OICOM_PID pluginID, OrbiterPluginMessage ** opm);
DLLCLBK int       oicom_removeLastMessageByName(char * pluginName);
DLLCLBK int       oicom_removeLastMessageByID(OICOM_PID pluginID);
DLLCLBK bool      oicom_isPluginByName(char * pluginName);
DLLCLBK bool      oicom_isPluginByID(OICOM_PID pluginID);
DLLCLBK void      oicom_timeStep(double simt, double simdt, double mjd); 

#endif