// RobotBehavior.cpp: implementation of the CRobotBehavior class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HuroEvolutionSystem.h"
#include "RobotBehavior.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRobotBehavior::CRobotBehavior(int port, int bundrate)
{
	m_Serial.Open(port, bundrate);
//	m_Serial_Compass.Open(COMPASS_COM_PORT, COMPASS_BUNDRATE);

	//----------------Robot Information--------------------
	m_Robot.flag_stop = false;
	m_Robot.flag_clear = false;

	m_Robot.tx = 0;
	m_Robot.ty = 0;
	m_Robot.ttheta = 0;
	m_Robot.high_byte_compass = 0;
	m_Robot.low_byte_compass = 0;
}

CRobotBehavior::~CRobotBehavior()
{
	m_Serial.Close();
//	m_Serial_Compass.Close();
}

void CRobotBehavior::ReveiveData()
{
	unsigned char buffer[1024] = {0};
	int msg = m_Serial.ReadDataWaiting();
	
	if(msg >= 6)
	{
		if(msg > 1023)	
			msg = 1023;
		
		m_Serial.ReadData(buffer, msg);
		
		for(int i = (msg-1); i >= 0; i--)
		{
			if(buffer[i - 10] == 254 && buffer[i] == 255)
			{
				if(buffer[i - 2] == 1)
				{
					//for button start
					//flag_start = true;
				}
				m_Robot.x     = (int)buffer[i - 9] - 128;
				m_Robot.y     = (int)buffer[i - 8] - 128;
				m_Robot.theta = (int)buffer[i - 7] - 128;
				m_Robot.low_byte_compass = (int)buffer[i - 4];
				m_Robot.high_byte_compass = (int)buffer[i - 5];
				

				if(m_Robot.x > 0)
					m_Robot.tx         += int(0.743178 * m_Robot.x);
				else
					m_Robot.tx         += int(0.32953 * m_Robot.x);
				//odemetry y-axis 
				if(m_Robot.y > 0)
					m_Robot.ty     += int(0.782534 * m_Robot.y);
				else
					m_Robot.ty     += int(0.616667 * m_Robot.y);
				//odemetry rotate
				if(m_Robot.theta >= 0)
					m_Robot.ttheta += int(0.276578 * m_Robot.theta);//0.590617
				else
					m_Robot.ttheta += int(0.26223 * m_Robot.theta);//0.25702
					
				
				
				if(buffer[i - 6] != 100)
					m_Robot.flag_stop = true;
				else
					m_Robot.flag_stop = false;
			}
		}
	}
}

void CRobotBehavior::SendData(unsigned char array[], int index)
{
	m_Serial.WriteCommByte(array[index]);
}

