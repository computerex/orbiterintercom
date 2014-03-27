#ifndef _OICOM_MFD_H_
#define _OICOM_MFD_H_

#include <Orbitersdk.h>

class OICOMMFD: public MFD2 {
public:
	OICOMMFD (DWORD w, DWORD h, VESSEL *vessel);
	~OICOMMFD ();
	char *ButtonLabel (int bt);
	int ButtonMenu (const MFDBUTTONMENU **menu) const;
	bool ConsumeKeyBuffered(DWORD key);
	bool ConsumeButton (int bt, int event);
	bool Update (oapi::Sketchpad *skp);
	static int MsgProc (UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam);

protected:
	oapi::Font *font;
	DWORD width, height;
};

bool sendMsgWithID(void *id, char *str, void *data);
#endif