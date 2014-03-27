#define STRICT
#define ORBITER_MODULE

#include <orbitersdk.h>
#include <oicominit.h>

OICOM_PID pid=0; // plug-in ID for this plugin. 
bool *active=0;
double *setpoint=0;

DLLCLBK void opcPreStep(double simt, double simdt, double mjd)
{
	OrbiterPluginMessage * opm; // holds our msg and msg data
	char * msg;
	int len;
	// nextMessage() returns MESSAGE_OK if a message is succesfully polled
	while(OICOM::nextMessage(pid, &opm)==MESSAGE_OK)
	{
		msg=opm->strMsg;
		len=strlen(msg);
		// we don't care about any text only events for this module
		if ( opm->data == 0 ) 
		{
			// remove this message from the message stack
			OICOM::removeLastMessage(pid);
			continue;
		}
		// perma-link! hook up this module's state variables with any module providing input
		// this is powerful
		if ( !_strnicmp(msg, "link_state", len) )
			active=(bool*)opm->data;
		else if (!_strnicmp(msg, "link_setpoint", len) )
			setpoint=(double*)opm->data;
		OICOM::removeLastMessage(pid); // this is important otherwise we'll end up with an infinite loop!
									   // I am thinking about automating it.. calling this at nextMessage
	}
	// if not linked to any module for input, no need to proceed
	if ( active == 0 || setpoint == 0 ) return;

	if ( !*active ) return;
	// crappy "autopilot" code follows, IGNORE IT xD
	VESSEL * vessel = oapiGetFocusInterface();
	DWORD inx = vessel->GetGroupThrusterCount(THGROUP_MAIN);
	VECTOR3 dv;
	vessel->GetDragVector(dv);
	THRUSTER_HANDLE th;
	double e = *setpoint-vessel->GetAirspeed();
	double lvl = 0;
	for (DWORD i = 0; i < inx; i++)
	{
		th = vessel->GetGroupThruster(THGROUP_MAIN, i);
		lvl = (-dv.z+e*vessel->GetMass()*70)/vessel->GetThrusterMax0(th);
		if (lvl<0)
			lvl=0;
		else if (lvl>1)
			lvl=1;
		vessel->SetThrusterLevel(th, lvl);
	}
}

DLLCLBK void InitModule(HINSTANCE dll)
{
	// initializaiton of the plugin and OICOM
	int ret = OICOM::GetOrbiterIntercom();
	pid=OICOM::addPlugin("oicomcruisemodule");
}
