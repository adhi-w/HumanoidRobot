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

	short Ver_state;

	bool Head_Init;
	bool Scan_Finish;

	bool Catch_start;

public:

	bool Getball;
	CvPoint Pos_ball;
public:

	CSerial m_serial;

public:
	bool V_Scanball();
	bool V_Lock();
	bool TimeDelay(int delay_time);
	bool V_Scan(bool capstart, bool capfinish);
	void HeadState(bool start, bool capfinish);
	void SendPos(int hor, int ver);
	CHeadControl(int port, int bundrate);
	virtual ~CHeadControl();
};

#endif // !defined(AFX_HEADCONTROL_H__A2370D06_96A6_4409_B2DB_8FEB7CD10EF7__INCLUDED_)
