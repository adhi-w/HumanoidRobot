// HeadControl.cpp: implementation of the CHeadControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HuroEvolutionSystem.h"
#include "HeadControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeadControl::CHeadControl(int port, int bundrate)
{

	Rotate_speed = 200;
	isBallGone = false;
	Getball = false;
	Head_state = 1;
	Time_Init = true;

	Catch_start = false;

	m_serial.Open(port, bundrate);
	
	Head_hor = CAM_HOR_CENTER;
	Head_ver = CAM_VER_U_BOUDARY+10;
	SendPos(Head_hor, Head_ver);
}

CHeadControl::~CHeadControl()
{
	m_serial.Close();
}

void CHeadControl::SendPos(int hor, int ver)
{
	char sum = 0x00;
	hor = hor + 65;
	ver = ver + 0;
	//	int speed = 70;
	unsigned char vpl = ver%256;
	unsigned char vph = ver/256;
	unsigned char vsl = Rotate_speed%256;
    unsigned char vsh = Rotate_speed/256;
	
	char array[18];
	array[0] = array[1] = (char)0xFF;		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
   	array[2] = (char)0xFE;				//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
   	array[3] = (char)0x0E;				//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
    array[4] = (char)0x83;				//SYNC WRITE
	array[5] = (char)0x1E;				//Parameter1 Start address to write Data
   	array[6] = (char)0x04;				//Parameter2 Length of Data to write
	array[7] = CAM_Ver_ID;
   	array[8] = vpl;
   	array[9] = vph;
   	array[10] = vsl;
   	array[11] = vsh;  
	unsigned char hpl = hor%256;
   	unsigned char hph = hor/256;
   	unsigned char hsl = Rotate_speed%256;
   	unsigned char hsh = Rotate_speed/256;
    array[12] = CAM_Hor_ID;
   	array[13] = hpl;
    array[14] = hph;
    array[15] = hsl;
    array[16] = hsh;
	for(int i = 2; i < 17; i++)
	{
		sum += array[i];
	}
	array[17] = ~sum;
	
	m_serial.SendData(array, sizeof(array));	
	
}
bool CHeadControl::TimeDelay(int delay_time)
{
	static double lastTime = 0;
	if(Time_Init)
	{
		lastTime = (double)cvGetTickCount();
		Time_Init = false;
	}
	else
	{
		double t = (double)cvGetTickCount();
		double pTime = ((double)cvGetTickCount() - lastTime) / ((double)cvGetTickFrequency()*1000);
		if(pTime > delay_time)
		{
			lastTime = t;
			return true;
		}
		else
		{
			return false;		
		}
	}return false;
}

bool CHeadControl::V_Scanball(bool capfinish)
{
	static int cnt = 0;
	short hor_init = CAM_HOR_CENTER;
	short ver_init = CAM_VER_CENTER;
	
	short hor_boundary_ltop   = CAM_HOR_L_BOUDARY;
	short hor_boundary_lbot   = CAM_HOR_L_BOUDARY;
	
	short hor_boundary_rtop   = CAM_HOR_U_BOUDARY;
	short hor_boundary_rbot   = CAM_HOR_U_BOUDARY;
	
	short ver_boundary_top    = CAM_VER_U_BOUDARY;
	short ver_boundary_bot    = CAM_VER_L_BOUDARY;
	short ver_boundary_midbot = CAM_VER_CENTER;
	
	short l_delay;
//	if(isZoomIn)
		l_delay = 500;
// 	else
// 		l_delay = 800;
	//if not find ball
 	if(!Getball)
 	{
		cnt = 0;
		Rotate_speed = 300;
		if(Head_state == 1)
		{
			Head_hor = hor_boundary_rbot;
			Head_ver = ver_boundary_bot;
			
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 2;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 2)
		{
			Head_hor = hor_init;
			Head_ver = ver_boundary_bot;
				
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 3;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 3)
		{
			Head_hor = hor_boundary_lbot;
			Head_ver = ver_boundary_bot;
		
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 4;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 4)
		{
			Head_hor = hor_boundary_ltop;
			Head_ver = ver_boundary_top;
			
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 5;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 5)
		{
			Head_hor = hor_init;
			Head_ver = ver_boundary_top;
		
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 6;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 6)
		{
			Head_hor = hor_boundary_rtop;
			Head_ver = ver_boundary_top;
			
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 7;
					Catch_start = false;
					Time_Init = true;
				}
			}
		}
		else if(Head_state == 7)
		{
			Head_hor = hor_init;
			Head_ver = ver_boundary_bot - 50;
			
			if(TimeDelay(l_delay))
			{
				Catch_start = true;
				if(capfinish)
				{
					Head_state = 1;
					Catch_start = false;
				Time_Init = true;
				}
			}
		}
 	}
	//if find ball
	else
	{
// 		cnt += 1;
// 		if(cnt == 1)
// 			Time_Init = true;
// 		if(cnt > 100)
// 			cnt = 100;
		int width = CAM_WIDTH;
		int height = CAM_HEIGHT;
		
		Catch_start = true;
		Rotate_speed = 250;
		int cx, cy;
		
		cx = width/2;
		cy = height/2;
		
		double P_vf = 670.0;		//P_ipy, 
		//p
		double P_ipx = (double)(Pos_ball.x - cx);
		double P_ipy = (double)(cy - Pos_ball.y);
		//c
		double Theta_ipx = atan2(P_ipx, P_vf) * 180.0 / 3.14;
		double Theta_ipy = atan2(P_ipy, P_vf) * 180.0 / 3.14;
		
		int command_ver = int(0.05 * Theta_ipy * 1024.0 / 300.0);
		Head_ver = Head_ver - command_ver;
		
		if(Head_ver < CAM_VER_U_BOUDARY)
			Head_ver = CAM_VER_U_BOUDARY;
		if(Head_ver > CAM_VER_L_BOUDARY)
			Head_ver = CAM_VER_L_BOUDARY;
		if((fabs(P_ipx) > 15))
		{
			int command_hor;
			
			//command_hor = int(0.4 * Theta_ipx * 1024.0 / 300.0);
			command_hor = int(0.1 * Theta_ipx * 1024.0 / 300.0);
			Head_hor = Head_hor - command_hor;
			if(Head_hor > CAM_HOR_U_BOUDARY)	Head_hor = CAM_HOR_U_BOUDARY;
			else if(Head_hor < CAM_HOR_L_BOUDARY)	Head_hor = CAM_HOR_L_BOUDARY;
		}
// 		if(Head_hor < CAM_HOR_L_CHANGE || Head_hor > CAM_HOR_U_CHANGE)
// 		Head_ver = CAM_VER_CHANGE;
	}


	SendPos(Head_hor, Head_ver);
	return false;
}

void CHeadControl::V_ViewLeft()
{
	Head_hor = CAM_HOR_CENTER+70;
	Head_ver = CAM_VER_L_BOUDARY-10;
	SendPos(Head_hor, Head_ver);
}

void CHeadControl::V_ViewRight()
{
	Head_hor = CAM_HOR_CENTER-60;
	Head_ver = CAM_VER_L_BOUDARY-10;
	SendPos(Head_hor, Head_ver);
}

void CHeadControl::V_ViewDown()
{
	Catch_start = true;
	Head_hor = CAM_HOR_CENTER;
	Head_ver = CAM_VER_U_BOUDARY;
	SendPos(Head_hor, Head_ver);
}

void CHeadControl::V_KickView()
{
	Catch_start = true;
	Head_hor = CAM_HOR_CENTER;
	Head_ver = CAM_VER_U_BOUDARY+10;
	SendPos(Head_hor, Head_ver);
}
