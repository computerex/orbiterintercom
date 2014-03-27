#ifndef _H_OICOM_CRUISE_MFD_H_
#define _H_OICOM_CRUISE_MFD_H_

#include <orbitersdk.h>

class CruiseMFD: public MFD {
public:
	CruiseMFD (DWORD w, DWORD h, VESSEL *vessel);
	~CruiseMFD ();
	bool ConsumeKeyBuffered (DWORD key);
	bool ConsumeButton (int bt, int event);
	char *ButtonLabel (int bt);
	int  ButtonMenu (const MFDBUTTONMENU **menu) const;
	void Update (HDC hDC);
	static int MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);
private:
	DWORD width, height;
};

#endif