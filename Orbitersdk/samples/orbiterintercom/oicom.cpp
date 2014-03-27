#include "OICOM.h"

OrbiterIntercom::OrbiterIntercom()
{
	mfdlog.insert("Orbiter Intercom version %s Initialized", OICOM_VERSION);
	counter=0;
}

OrbiterIntercom::~OrbiterIntercom()
{
	// release the resources allocated for the plugins
	while(!plugins.empty()){
		OrbiterPlugin * plugin = plugins.back();
		if ( plugin != 0 )
			delete plugin;
		plugins.pop_back();
	}
}
DWORD OrbiterIntercom::getPluginCount()
{
	return plugins.size(); 
}
OICOM_PID OrbiterIntercom::getPluginByName(char * name)
{
	DWORD len = strlen(name);
	if ( len == 0 ) {
		mfdlog.insert("Attempt to attain ID for plug-in name with empty string as argument");
		return PLUGIN_INVALID;
	}
	OICOM_PID pid;
	for ( DWORD i = 0; i < plugins.size(); i++ )
	{
		if ( !_strnicmp(name, plugins[i]->getPluginName(), len) ){
			pid=plugins[i]->getPluginID();
			mfdlog.insert("getPluginByName(): returning pid %d for name %s", pid, name);
			return pid;
		}
	}
	return PLUGIN_INVALID;
}
char* OrbiterIntercom::getPluginNameByID(OICOM_PID pid)
{
	for ( DWORD i = 0; i < plugins.size(); i++ )
		if ( plugins[i]->getPluginID() == pid ) {
			char * name=plugins[i]->getPluginName();
			mfdlog.insert("getPluginNameByID(): returning %s for ID %d", name, pid);
			return name;
		}
	mfdlog.insert("getPluginNameByID(): return NULL for pid %d, plug-in does not exist", pid);
	return NULL;
}
OICOM_PID OrbiterIntercom::getPluginByIndex(DWORD inx)
{
	DWORD size = plugins.size();
	if ( inx >= size ) {
		mfdlog.insert("getPluginByIndex(): index %d is out of range!", inx);
		return PLUGIN_INVALID;
	}
	OICOM_PID pid = plugins[inx]->getPluginID();
	mfdlog.insert("getPluginByIndex(): returning pid %d for index %d", pid, inx);
	return pid;
}
OICOM_PID OrbiterIntercom::addPlugin(char * pluginName)
{
	// name too short
	if ( strlen(pluginName) < 6 ) {
		mfdlog.insert("addPlugin(): Attempt to register plug-in with name %s failed, name too short", pluginName);
		return PLUGIN_NAME_TOO_SHORT;
	}
	// plugin already exists
	OICOM_PID pid = getPluginByName(pluginName);
	if ( pid != PLUGIN_INVALID ) {
		mfdlog.insert("addPlugin(): plug-in with name %s already exists! (pid %d)", pluginName, pid);
		return PLUGIN_ALREADY_EXISTS;
	}
	// counter is for ID assignment
	pid = counter;
	plugins.push_back( new OrbiterPlugin(pluginName, pid));
	counter++;
	mfdlog.insert("addPlugin(): plug-in %s registered with pid %d", pluginName, pid);
	return pid;
}
int OrbiterIntercom::removePlugin(char * pluginName)
{
	OICOM_PID pid = getPluginByName(pluginName);
	if ( pid == PLUGIN_INVALID ) {
		mfdlog.insert("removePlugin(): plug-in: %s does not exist!", pluginName);
		return pid;
	}
	DWORD inx = getPluginIndex(pid);
	removePluginByIndex(inx);
	mfdlog.insert("removePlugin(): plugin-in %s removed", pluginName);
	return PLUGIN_REMOVED;
}
int OrbiterIntercom::removePlugin(OICOM_PID pluginID)
{
	if ( !isPlugin(pluginID) ) {
		mfdlog.insert("removePlugin(): plug-in: %d does not exist!", pluginID);
		return PLUGIN_INVALID;
	}
	DWORD inx = getPluginIndex(pluginID);
	removePluginByIndex(inx);
	mfdlog.insert("removePlugin(): plugin-in %d removed", pluginID);
	return PLUGIN_REMOVED;
}

bool OrbiterIntercom::isPlugin(OICOM_PID pid)
{
	if ( pid < 0 ) {
		mfdlog.insert("isPlugin(): plugin-in %d is not a valid plug-in", pid);
		return false;
	}
	unsigned len = plugins.size();
	for ( DWORD i = 0; i < plugins.size(); i++ )
		if ( pid==plugins[i]->getPluginID() ){
			return true;
		}
	mfdlog.insert("isPlugin(): plugin-in %d is not a valid plug-in", pid);
	return false;
}
bool OrbiterIntercom::isPlugin(char * pluginName)
{
	bool ret = (getPluginByName(pluginName)!=PLUGIN_INVALID?true:false);
	if ( !ret )
		mfdlog.insert("isPlugin(): plugin-in %s is not a valid plug-in", pluginName);
	return ret;
}

bool OrbiterIntercom::sendMessage(char * pluginName, OrbiterPluginMessage * msg)
{
	if ( msg == 0 ) {
		mfdlog.insert("sendMessage(): msg to %s is NULL!", pluginName);
		return false;
	}
	char * strmsg = msg->strMsg;
	if ( strmsg == 0 ) {
		mfdlog.insert("sendMessage(): strMsg to %s is NULL", pluginName);
		return false;
	}
	OICOM_PID pid = getPluginByName(pluginName);
	if ( pid == PLUGIN_INVALID ){
		mfdlog.insert("sendMessage(): destination plug-in %s does not exist", pluginName);
		return false;
	}
	DWORD inx = getPluginIndex(pid);
	plugins[inx]->addMessage(msg);
	mfdlog.insert("sendMessage(): \"%s\" sent to %s", strmsg, pluginName);
	return true;
}
bool OrbiterIntercom::sendMessage(char * pluginName, char * strMsg, void * data, unsigned int dataSize)
{
	if ( strMsg == 0 ) {
		mfdlog.insert("sendMessage(): strMsg to %s is NULL", pluginName);
		return false;
	}
	OICOM_PID pid = getPluginByName(pluginName);
	if ( pid== PLUGIN_INVALID ) {
		mfdlog.insert("sendMessage(): destination plug-in %s does not exist", pluginName);
		return false;
	}
	DWORD inx = getPluginIndex(pid);
	OrbiterPluginMessage * opm = new OrbiterPluginMessage();
	strcpy_s(opm->strMsg, strMsg);
	opm->data=data;
	opm->dataSize=dataSize;
	plugins[inx]->addMessage(opm);
	mfdlog.insert("sendMessage(): \"%s\" sent to %s", strMsg, pluginName);
	return true;
}
bool OrbiterIntercom::sendMessage(OICOM_PID pluginID, OrbiterPluginMessage * msg)
{
	if ( msg == 0 ) {
		mfdlog.insert("sendMessage(): msg to %d is NULL!", pluginID);
		return false;
	}
	char * strmsg = msg->strMsg;
	if ( strmsg == 0 ) {
		mfdlog.insert("sendMessage(): strMsg to %d is NULL", pluginID);
		return false;
	}
	if ( !isPlugin(pluginID) ) {
		mfdlog.insert("sendMessage(): %d is not a valid plug-in!", pluginID);
		return false;
	}
	DWORD inx = getPluginIndex(pluginID);
	plugins[inx]->addMessage(msg);
	mfdlog.insert("sendMessage(): sent msg \"%s\" to %d", strmsg, pluginID);
	return true;
}
bool OrbiterIntercom::sendMessage(OICOM_PID pluginID, char * strMsg, void * data, unsigned int dataSize)
{
	if ( strMsg == 0 ) {
		mfdlog.insert("sendMessage(): strMsg to %d is NULL!", pluginID);
		return false;
	}
	if ( !isPlugin(pluginID) ) {
		mfdlog.insert("sendMessage(): plug-in %d does not exist", pluginID);
		return false;
	}
	DWORD inx = getPluginIndex(pluginID);
	OrbiterPluginMessage * opm=new OrbiterPluginMessage();
	strcpy_s(opm->strMsg, strMsg);
	opm->dataSize=dataSize;
	opm->data=data;
	plugins[inx]->addMessage(opm );
	mfdlog.insert("sendMessage(): msg \"%s\" sent to %d", strMsg, pluginID);
	return true;
}
int OrbiterIntercom::nextMessage(char * pluginName, OrbiterPluginMessage ** opm)
{
	OICOM_PID pid = getPluginByName(pluginName);
	if (pid == PLUGIN_INVALID ) {
		mfdlog.insert("nextMessage(): plug-in %s does not exist", pluginName);
		return false;
	}
	DWORD inx = getPluginIndex(pid);
	OrbiterPlugin * p = plugins[inx];
	if ( p->getMessageCount() == 0 ) {
		return MESSAGE_NONE_EXISTS;
	}
	*opm = p->getLastMessage();
	mfdlog.insert("nextMessage(): plugin: %s msg returned (\"%s\")", pluginName, (*opm)->strMsg);
	return MESSAGE_OK;
}
int OrbiterIntercom::nextMessage(OICOM_PID pluginID, OrbiterPluginMessage ** opm)
{
	if ( !isPlugin(pluginID) ){
		mfdlog.insert("nextMessage(): plug-in %d does not exist", pluginID);
		return PLUGIN_INVALID;
	}
	DWORD inx = getPluginIndex(pluginID);
	OrbiterPlugin * p = plugins[inx];
	if ( p->getMessageCount() == 0 ) return MESSAGE_NONE_EXISTS;
	*opm = p->getLastMessage();
	mfdlog.insert("nextMessage(): plug-in %d msg returned (\"%s\")", pluginID, (*opm)->strMsg);
	return MESSAGE_OK;
}
int OrbiterIntercom::removeLastMessage(char * pluginName)
{
	OICOM_PID pid = getPluginByName(pluginName);
	if ( pid == PLUGIN_INVALID) {
		mfdlog.insert("removeLastMessage(): plug-in %s does not exist", pluginName);
		return pid;
	}
	DWORD inx = getPluginIndex(pid);
	OrbiterPlugin * p = plugins[inx];
	if ( p->getMessageCount() == 0 ) {
		mfdlog.insert("removeLastMessage(): plug-in %s attempted to delete msg when none exists", pluginName);
		return MESSAGE_NONE_EXISTS;
	}
	p->deleteLastMessage();
	mfdlog.insert("removeLastMessage(): plug-in %s removed last msg", pluginName);
	return MESSAGE_DELETE_OK;
}
int OrbiterIntercom::removeLastMessage(OICOM_PID pluginID)
{
	if ( !isPlugin(pluginID) ) {
		mfdlog.insert("removeLastMessage(): plug-in %d does not exist", pluginID);
		return PLUGIN_INVALID;
	}
	DWORD inx = getPluginIndex(pluginID);
	OrbiterPlugin * p = plugins[inx];
	if ( p->getMessageCount() == 0 ) {
		mfdlog.insert("removeLastMessage(): plug-in %d attempted to delete msg when none exists", pluginID);
		return MESSAGE_NONE_EXISTS;
	}
	p->deleteLastMessage();
	mfdlog.insert("removeLastMessage(): plug-in %d removed last msg", pluginID);
	return MESSAGE_DELETE_OK;
}

void OrbiterIntercom::timeStep(double simt, double simdt, double mjd)
{
}

// meant to be used internally
void OrbiterIntercom::removePluginByIndex(DWORD inx)
{
	// get the itt to the plugin. erase calls the object's destructor
	// but the vector holds pointers to OrbiterPlugin so in this case
	// it'll call the generic destructor and do nothing
	std::vector<OrbiterPlugin*>::iterator it = plugins.begin()+inx;
	OrbiterPlugin * plugin = plugins[inx];
	if ( plugin!=0 )
		delete plugin;
	plugins.erase(it);
}
DWORD OrbiterIntercom::getPluginIndex(OICOM_PID pluginID)
{
	for ( DWORD i = 0; i < plugins.size(); i++ )
	{
		if ( plugins[i]->getPluginID() == pluginID )
			return i;
	}
	return 0;
}
