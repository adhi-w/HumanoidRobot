// BodyControl.h: interface for the CBodyControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BODYCONTROL_H__15C6A02A_DF13_4E85_AE13_EB78ECBBF610__INCLUDED_)
#define AFX_BODYCONTROL_H__15C6A02A_DF13_4E85_AE13_EB78ECBBF610__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"
#define Arduino_Port 4
#define Arduino_Baud_rate 115200

class CBodyControl  
{
public:
	void odemeter_measure();
	CPoint Odemeter();
	void KickBall(int type, int angle, int power);
	void RightRotate(int value);
	void LeftRotate(int value);
	void LeftShift(int value);
	void RightShift(int value);
	void Backward(int value);
	void Forward(int value);
	void Stop();
	void MultMove(int dx, int dy, int dz);
	CBodyControl();
	virtual ~CBodyControl();

	CSerial m_Serial_Body;
	int direction;
	int compass_bias;
	int robot_x;
	int robot_y;

	double odemter_x;
	double odemter_y;
	double odemter_x_unit;
	double odemter_y_unit;

	double compass_show;
};

#endif // !defined(AFX_BODYCONTROL_H__15C6A02A_DF13_4E85_AE13_EB78ECBBF610__INCLUDED_)
