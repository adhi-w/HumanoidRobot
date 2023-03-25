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
	unsigned short CRC;

	short Ver_state;

	bool Head_Init;
	bool Scan_Finish;

	bool Catch_start;

	int position_i;
	int position_d;
public:

	bool Getball;
	CvPoint Pos_ball;
public:

	CSerial m_serial;

public:
	void Set_hor_speed(int hor_vel);
	bool V_Init();
	bool V_Scanobst();
	unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
	void TorqueON();
	void HeadControl_ver(int motor_ver);
	void HeadControl_hor(double motor_hor);
	bool V_Scanball();
	bool V_Down();
	bool TimeDelay(int delay_time);
	bool V_Scan(bool capstart, bool capfinish);
	void HeadState(bool start, bool capfinish);
	void SendPos(int hor, int ver);
	CHeadControl(int port, int bundrate);
	virtual ~CHeadControl();
};

#endif // !defined(AFX_HEADCONTROL_H__A2370D06_96A6_4409_B2DB_8FEB7CD10EF7__INCLUDED_)
