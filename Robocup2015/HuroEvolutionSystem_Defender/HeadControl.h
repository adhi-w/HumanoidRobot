// HeadControl.h: interface for the CHeadControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADCONTROL_H__A2370D06_96A6_4409_B2DB_8FEB7CD10EF7__INCLUDED_)
#define AFX_HEADCONTROL_H__A2370D06_96A6_4409_B2DB_8FEB7CD10EF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Serial.h"
#include "cv.h"
#include "highgui.h"
#include "General.h"

class CHeadControl  
{
public:

	short Rotate_speed;
	short Head_hor, Head_ver;
	short Head_state;
	bool Catch_start;

public:
	
	bool Time_Init;
	bool Getball;
	bool isBallGone;
	bool isZoomIn;

	CvPoint Pos_ball;
public:

	CSerial m_serial;

public:
	void V_ViewDown();
	void V_ViewRight();
	void V_ViewLeft();
	bool V_Scanball(bool capfinish);
	bool TimeDelay(int delay_time);
	void SendPos(int hor, int ver);
	CHeadControl(int port, int bundrate);
	virtual ~CHeadControl();
};

#endif // !defined(AFX_HEADCONTROL_H__A2370D06_96A6_4409_B2DB_8FEB7CD10EF7__INCLUDED_)
