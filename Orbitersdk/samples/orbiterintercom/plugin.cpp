#include "Plugin.h"

OrbiterPlugin::OrbiterPlugin(char * pluginName, OICOM_PID pluginID)
{
	strcpy_s(this->pluginName, pluginName);
	this->id=pluginID;
}
OrbiterPlugin::~OrbiterPlugin()
{
	while ( !messages.empty() ){
		deleteLastMessage();
	}
}
void OrbiterPlugin::addMessage(OrbiterPluginMessage * opm)
{
	messages.push_back(opm);
}
void OrbiterPlugin::deleteLastMessage()
{
	OrbiterPluginMessage * msg = messages.back();
	if ( msg != 0 ) delete msg;
	messages.pop_back();
}
OrbiterPluginMessage* OrbiterPlugin::getLastMessage()
{
	if ( messages.size() <= 0 ) return 0;
	return messages.back();
}
