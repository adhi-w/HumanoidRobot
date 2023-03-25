// JoyStick.cpp: implementation of the CJoyStick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JoyControlRobot.h"
#include "JoyStick.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJoyStick::CJoyStick()
{
	m_Joy.dwButtons = NULL;
	DX = DY = DZ = 0;
	button_status = 0;
}

CJoyStick::~CJoyStick()
{

}

void CJoyStick::GetPos()
{
	joyGetPosEx(JOYSTICKID1, &m_Joy);
	DX = m_Joy.dwXpos;
	DY = m_Joy.dwYpos;
	DZ = m_Joy.dwRpos;
	button_status=m_Joy.dwButtons;


}

void CJoyStick::DrawJoyStick(CDC &mendc, int cx, int cy, double l, double ang)
{
	CRect rect = CRect(-60, -60, 60, 60);
	mendc.Rectangle(rect + CPoint(cx, cy));
//	if(l > 50.0) l = 50.0;
	int x = Round(cx + l * cos(ang));
	int y = Round(cy + l * sin(ang));
	DrawCross(mendc, x, y);
}

void CJoyStick::DrawCross(CDC &mendc, int x, int y)
{
	int l = 5;
	CPoint pt[4];
	pt[0] = CPoint(-l, 0);
	pt[1] = CPoint( l, 0);
	pt[2] = CPoint( 0, l);
	pt[3] = CPoint( 0, -l);
	for(int i = 0; i < 4; i++)
	{
		pt[i].x += x;
		pt[i].y += y;
	}
	mendc.MoveTo(pt[0]);
	mendc.LineTo(pt[1]);
	mendc.MoveTo(pt[2]);
	mendc.LineTo(pt[3]);
}

int CJoyStick::Round(double n)
{
	if(n > 0)
	{
		if(fabs(n - (int)n) > 0.45)
			return (int)n + 1;
		else return (int)n;
	}else
	{
		if(fabs(n - (int)n) > 0.45)
			return (int)n - 1;
		else return (int)n;
	}
}
