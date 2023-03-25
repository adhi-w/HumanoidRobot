// JoyStick.h: interface for the CJoyStick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOYSTICK_H__A3AF4DE1_8FA8_4D0B_A0BD_450D7E160FCB__INCLUDED_)
#define AFX_JOYSTICK_H__A3AF4DE1_8FA8_4D0B_A0BD_450D7E160FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "math.h"
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

class CJoyStick  
{
public:
	int Round(double n);
	void DrawCross(CDC &mendc, int x, int y);
	void DrawJoyStick(CDC &mendc, int cx, int cy, double l, double ang);
	void GetPos();
	CJoyStick();
	virtual ~CJoyStick();

	JOYINFOEX m_Joy;
	int DX, DY, DZ;
	int button_status;
};

#endif // !defined(AFX_JOYSTICK_H__A3AF4DE1_8FA8_4D0B_A0BD_450D7E160FCB__INCLUDED_)
