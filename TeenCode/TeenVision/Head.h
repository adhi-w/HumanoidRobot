// Head.h: interface for the Head class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEAD_H__D9A06E71_E0A5_419C_A8D8_707D6987A719__INCLUDED_)
#define AFX_HEAD_H__D9A06E71_E0A5_419C_A8D8_707D6987A719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cv.h>
#include <math.h>
#include "Serial.h"
#include "General.h"

class Head  
{
public:
	Serial serial;

public:
	long int dhor, dver;
	int PresentHorPos, PresentVerPos;
	static int hor, ver;
	int camSpeed;

	bool Head_Init;
	bool headtrack;
	bool jumping_check;
	
	short Head_state;
	short Head_hor, Head_ver;

public:
	void SendPos(int hor, int ver);
	void ReadPackage(int id);

	bool ReceiveData(int ID);
	bool TimeDelay(int delay_time);

	void Track_Ball(bool findBall, CvPoint ball, CvPoint CamPixel);

	void CamPos(bool findBall, CvPoint ball, CvPoint CamPixel);

	Head();
	virtual ~Head();

};

#endif // !defined(AFX_HEAD_H__D9A06E71_E0A5_419C_A8D8_707D6987A719__INCLUDED_)
