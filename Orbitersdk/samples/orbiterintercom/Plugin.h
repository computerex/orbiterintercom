#ifndef _H_ORBITER_INTERCOM_PLUGIN_H_
#define _H_ORBITER_INTERCOM_PLUGIN_H_

// maximum number of characters for the unique
// plug-in name.

#define PLUGIN_NAME_MAXCHAR      256
typedef int OICOM_PID; // plug-in ID

#include <Windows.h>
#include <vector>
#include "Message.h"

/*
	The plug-ins register themselves with a unique string identifier
	they also have a unique numerical identifer for convinience.
	The plug-ins will free any memory allocated for the messages
	Do not free the memory allocated for the messages! The data passed through
	the message however is not dellocated by the plug-in, do that yourself
*/

// do not create any instances of this class
class OrbiterPlugin
{
public:
							OrbiterPlugin(char * pluginName, OICOM_PID id);
							~OrbiterPlugin(); // dellocates the memory allocated for the messages

	OICOM_PID				getPluginID() { return id; }
	char *					getPluginName() { return pluginName; }
	void					addMessage(OrbiterPluginMessage * msg);
	void					deleteLastMessage();
	OrbiterPluginMessage*   getLastMessage();
	DWORD					getMessageCount() { return messages.size(); }
private:
	char pluginName[PLUGIN_NAME_MAXCHAR];					// unique name
	OICOM_PID id;											// unique numerical ID
	std::vector<OrbiterPluginMessage*> messages;		    // messages for the plug-in
};

#endif