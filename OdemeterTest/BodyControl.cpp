// BodyControl.cpp: implementation of the CBodyControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BodyControl.h"
#include <math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBodyControl::CBodyControl()
{
	m_Serial_Body.Open(9, Arduino_Baud_rate);
	
	compass_bias = 0;
	direction = 0;
	robot_x = 0;
	robot_y = 0;

	odemter_x = 0.0;
	odemter_y = 0.0;

	odemter_x_unit = 0.0;
	odemter_y_unit = 0.0;
}

CBodyControl::~CBodyControl()
{
	Stop();
	Sleep(100);
	m_Serial_Body.Close();
}

void CBodyControl::MultMove(int dx, int dy, int dz) // R&L, F&B
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = dx + 128;
	array[4] = dy + 128;
	array[5] = dz + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::Stop()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 48;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::Forward(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = value + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::Backward(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = -value + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::RightShift(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = value + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::LeftShift(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = -value + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::LeftRotate(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = -value + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::RightRotate(int value)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = value + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

void CBodyControl::KickBall(int type, int angle, int power)
{
	//===============================================================
	if(angle > 30)
	{	
		angle = 30;
	}
	else if(angle < 0)
	{
		angle = 0;
	}
	//===============================================================

	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = type;
	array[2] = angle;
	array[3] = power;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial_Body.WriteCommByte(array[i]);
	}
}

CPoint CBodyControl::Odemeter()
{
	int ode_x = 0;
	int ode_y = 0;

	static CPoint robot = CPoint(0, 0);

	BYTE buffer[1024] = {0};
	int msg = m_Serial_Body.ReadDataWaiting();

	if(msg >= 6)
	{
		if(msg > 1024)	
			msg = 1024;

		m_Serial_Body.ReadData(buffer, msg);

		for(int i = (msg-1); i >= 0; i--)
		{
			if(buffer[i - 10] == 254 && buffer[i] == 255)
			{
				ode_x = (int)buffer[i - 9] - 128;
				ode_y = (int)buffer[i - 8] - 128;


				if(ode_x > 0)
				{
					robot_x = robot_x + 1.30751773*ode_x;
				}
				else
				{
					robot_x = robot_x + 1.145909581*ode_x;
				}
				if(ode_y > 0)
				{
					robot_y = robot_y + 0.732770801*ode_y;
				}
				else
				{
					robot_y = robot_y + 1.345069737*ode_y;
				}

				//if(buffer[i - 2] == 1) start_serial = true;

				//////////////////////direction/////////////////////
				int Compass_1 = buffer[i-5];
				int Compass_2 = buffer[i-4];
				int Compass = -( Compass_1*256 + Compass_2 );
				
				direction = Compass - compass_bias;
				if(direction < -180){
					direction = (direction + 720) % 360;
				}
				else if(direction > 180)
					direction = direction - 360;
				
				/*if(direction < 0)
					direction = Round(direction*(90.0/65.0));
				else
					direction = Round(direction*(90.0/108.0));*/

				if(direction > 180)
				{
					direction = direction - 360;
				}
			}
		}
	}

	robot.x = robot_x;
	robot.y = robot_y;

	return robot;
}

void CBodyControl::odemeter_measure()
{
	int ode_x = 0;
	int ode_y = 0;

//	odemter_x_unit = 0.0;
//	odemter_y_unit = 0.0;

	double unit_x = 0.0;
	double unit_y = 0.0;
	double ratio_x = 0.0;
	double ratio_y = 0.0;


	static CPoint robot = CPoint(0, 0);

	BYTE buffer[1024] = {0};
	int msg = m_Serial_Body.ReadDataWaiting();

	if(msg >= 6)
	{
		if(msg > 1024)	
			msg = 1024;

		m_Serial_Body.ReadData(buffer, msg);

		for(int i = (msg-1); i >= 0; i--)
		{
			if(buffer[i - 10] == 254 && buffer[i] == 255)
			{
				ode_x = (int)buffer[i - 9] - 128;
				ode_y = (int)buffer[i - 8] - 128;


				odemter_x_unit = ode_x;
				odemter_y_unit = ode_y;
//				=================measure==================
				if(ode_x > 0)
				{
//					odemter_x = odemter_x + 1.0*ode_x;

					odemter_x = odemter_x + 0.817369087*ode_x;
					

				}
				else
				{
					odemter_x = odemter_x + 1.0*ode_x;
				}
				if(ode_y > 0)
				{
					odemter_y = odemter_y + 1.0*ode_y;
				}
				else
				{
					odemter_y = odemter_y + 1.0*ode_y;
				}

//				=====================test===================				
// 
// 				
// 				if(ode_x > 0)
// 				{
// 					//	robot_x = robot_x + 1.30751773*ode_x;
// 					
// 					unit_x = abs(ode_x);
// 					
// 					double a1 = 0.00003;
// 					double b1 = -0.0015;
// 					double c1 = 0.0216;
// 					double d1 = -0.0745;
// 					double e1 = 1.0408;
// 					
// 					ratio_x = a1*ode_x*unit_x*unit_x*unit_x + b1*unit_x*unit_x*unit_x + c1*unit_x*unit_x
// 						+ d1*unit_x + e1;
// 					odemter_x = odemter_x + ratio_x*ode_x;
// 				}
// 				else
// 				{
// 					//robot_x = robot_x + 1.145909581*ode_x;
// 					
// 					
// 					unit_x = abs(ode_x);
// 					
// 					double a_1 = 0.1078;
// 					double b_1 = 12.41;
// 					double c_1 = 2.132;
// 					double a_2 = 0.9608;
// 					double b_2 = 19.92;
// 					double c_2 = 20.23;
// 					double a_3 = 0.5629;
// 					double b_3 = -1.368;
// 					double c_3 = 9.027;
// 					
// 					ratio_x = a_1*exp(-((unit_x-b_1)/c_1)*((unit_x-b_1)/c_1))
// 						+ a_2*exp(-((unit_x-b_2)/c_2)*((unit_x-b_2)/c_2))
// 						+ a_3*exp(-((unit_x-b_3)/c_3)*((unit_x-b_3)/c_3));
// 					
// 					odemter_x = odemter_x + ratio_x*ode_x;
// 				}
// 				if(ode_y > 0)
// 				{
// 					//	robot_y = robot_y + 0.732770801*ode_y;
// 					
// 					unit_y = abs(ode_y);
// 					double a3 = 0.00004;
// 					double b3 = -0.0016;
// 					double c3 = 0.0197;
// 					double d3 = -0.0448;
// 					double e3 = 0.5609;
// 					
// 					ratio_y = a3*unit_y*unit_y*unit_y*unit_y + b3*unit_y*unit_y*unit_y
// 						+ c3*unit_y*unit_y + d3*unit_y + e3;
// 					
// 					odemter_y = odemter_y + ratio_y*ode_y;
// 					
// 				}
// 				else
// 				{
// 					//	robot_y = robot_y + 1.345069737*ode_y;
// 					
// 					unit_y = abs(ode_y);
// 					double a4 = 0.000002;
// 					double b4 = -0.0000005;
// 					double c4 = -0.0008;
// 					double d4 = 0.0062;
// 					double e4 = -0.012;
// 					double f4 = 1.3334;
// 					
// 					ratio_y = a4*unit_y*unit_y*unit_y*unit_y*unit_y + b4*unit_y*unit_y*unit_y*unit_y
// 						+ c4*unit_y*unit_y*unit_y + d4*unit_y*unit_y + e4*unit_y + f4;
// 					
// 					odemter_y = odemter_y + ratio_y*ode_y;
// 				}
// 				
//  				odemter_x_unit = ratio_x*ode_x;
//  				odemter_y_unit = ratio_y*ode_y;

// 				if(ode_x > 0)
// 				{
// 					odemter_x = odemter_x + 1.054030007*ode_x;
// 				}
// 				else
// 				{
// 					odemter_x = odemter_x + 0.693929668*ode_x;
// 				}
// 				if(ode_y > 0)
// 				{
// 					odemter_y = odemter_y + 0.694147154*ode_y;
// 				}
// 				else
// 				{
// 					odemter_y = odemter_y + 1.195722022*ode_y;
// 				}


				//if(buffer[i - 2] == 1) start_serial = true;

				//////////////////////direction/////////////////////
				int Compass_1 = buffer[i-5];
				int Compass_2 = buffer[i-4];
				int Compass = -( Compass_1*256 + Compass_2 );
				
				compass_show = Compass;

				direction = Compass - compass_bias;
				if(direction < -180){
					direction = (direction + 720) % 360;
				}
				else if(direction > 180)
					direction = direction - 360;
				
				/*if(direction < 0)
					direction = Round(direction*(90.0/65.0));
				else
					direction = Round(direction*(90.0/108.0));*/

				if(direction > 180)
				{
					direction = direction - 360;
				}
			}
		}
	}
}
