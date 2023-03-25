// RobotBehavior.h: interface for the CRobotBehavior class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTBEHAVIOR_H__E1445694_99B9_43E2_A0E5_CD39F8C23499__INCLUDED_)
#define AFX_ROBOTBEHAVIOR_H__E1445694_99B9_43E2_A0E5_CD39F8C23499__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "General.h"
#include "Serial.h"

struct RobotInfo
{
	int x, tx;
	int y, ty;
	int theta, ttheta;
	int high_byte_compass, low_byte_compass;
	bool flag_stop;
	bool flag_clear;
};

class CRobotBehavior  
{
public:
	RobotInfo m_Robot;
	CSerial m_Serial;

public:
	void SendData(unsigned char array[], int index);
	void ReveiveData();
	CRobotBehavior(int port, int bundrate);
	virtual ~CRobotBehavior();

};

#endif // !defined(AFX_ROBOTBEHAVIOR_H__E1445694_99B9_43E2_A0E5_CD39F8C23499__INCLUDED_)
