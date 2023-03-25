// HeadControl.cpp: implementation of the CHeadControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FIRA_PK proc.h"
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
	Head_Init = false;

	Rotate_speed = 200;


	Head_hor = 2048;
	Head_ver = 2048;

	Head_state = 1;
	Ver_state = 0;

	Catch_start = false;
	Scan_Finish = false;


	Getball = false;

	m_serial.Open(port, bundrate);

	SendPos(Head_hor, Head_ver);
}

CHeadControl::~CHeadControl()
{
	m_serial.Close();
}

void CHeadControl::HeadState(bool start, bool capfinish)
{
	if(start)
	{
		if(!Scan_Finish)
		{
			Rotate_speed = 200;
			V_Scan(1, capfinish);
		}
		else
		{
			Rotate_speed = 200;
			V_Lock();
		}
	}
}

void CHeadControl::SendPos(int hor, int ver)
{
	char sum = 0x00;
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

bool CHeadControl::V_Scan(bool capstart, bool capfinish)
{	
	/*
	**
	**				
	**				
	**				4  -> 5	->  6
    **              |         /
    **              |      7
	**				|	      \	
	**				3 <-  2 <-  1
	**
	*/

	short hor_init = 2048;
	short ver_init = 2048;

	short hor_boundary_ltop   = 2620;
	short hor_boundary_lbot   = 2574;
	
	short hor_boundary_rtop   = 1365;
	short hor_boundary_rbot   = 1353;

	short ver_boundary_top    = 2175;
	short ver_boundary_bot    = 2379;
	short ver_boundary_midbot = 2460;

	short s_delay = 150;
	short l_delay = 300;

	if(Head_state == 1)
	{
		Head_hor = hor_boundary_rbot;
		Head_ver = ver_boundary_bot;
		
		if(TimeDelay(l_delay+l_delay))
		{
			Catch_start = true;
			if(capfinish)
			{
				Head_state = 2;
				Head_Init = true;
				Catch_start = false;
			}
		}
	}
	else if(Head_state == 2)
	{
		Head_hor = hor_init;
		Head_ver = ver_boundary_midbot;
			
		if(TimeDelay(l_delay))
		{
			Catch_start = true;
			if(capfinish)
			{
				Head_state = 3;
				Head_Init = true;
				Catch_start = false;
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
				Head_Init = true;
				Catch_start = false;
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
				Head_Init = true;
				Catch_start = false;
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
				Head_Init = true;
				Catch_start = false;
			}
		}
	}
	else if(Head_state == 6)
	{
		Head_hor = hor_boundary_rtop;
		
		if(TimeDelay(l_delay))
		{
			Catch_start = true;
			if(capfinish)
			{
				Head_state = 7;
				Head_Init = true;
				Catch_start = false;
			}
		}
	}
	else if(Head_state == 7)
	{
		Head_hor = hor_init;
		Head_ver = ver_init;
		
		if(TimeDelay(l_delay))
		{
			Catch_start = true;
			if(capfinish)
			{
				Head_state = 1;
				Head_Init = true;
				Catch_start = false;
				Scan_Finish = true;
			}
		}
	}
	SendPos(Head_hor, Head_ver);
	return false;
}

bool CHeadControl::V_Lock()
{
	Catch_start = true;


	Head_hor = 2048;
	Head_ver = 2450;

	SendPos(Head_hor, Head_ver);
	return false;
}

bool CHeadControl::TimeDelay(int delay_time)
{
	static double lastTime = 0;
	if(Head_Init)
	{
		lastTime = (double)cvGetTickCount();
		Head_Init = false;
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

bool CHeadControl::V_Scanball()
{
	short hor_init = 2048;
	short ver_init = 2048;
	
	short hor_boundary_ltop   = 2620;
	short hor_boundary_lbot   = 2574;
	
	short hor_boundary_rtop   = 1365;
	short hor_boundary_rbot   = 1353;
	
	short ver_boundary_top    = 2175;
	short ver_boundary_bot    = 2379;
	short ver_boundary_midbot = 2460;
	
	short s_delay = 150;
	short l_delay = 300;
	

	//if not find ball
	if(!Getball)
	{
		if(Head_state == 1)
		{
			Head_hor = hor_boundary_ltop;
			if(Ver_state == 1)
				Head_ver = ver_boundary_top;
			else if(Ver_state == 2)
				Head_ver = ver_boundary_midbot;
			
			if(TimeDelay(l_delay+l_delay))
			{
				Head_state = 2;
				Head_Init = true;
			}
		}
		else if(Head_state == 2)
		{
			Head_hor = hor_init;
			if(Ver_state == 1)
				Head_ver = ver_boundary_top;
			else if(Ver_state == 2)
				Head_ver = ver_boundary_midbot;
			
			if(TimeDelay(l_delay))
			{
				Head_state = 3;
				Head_Init = true;
			}
		}
		else if(Head_state == 3)
		{
			Head_hor = hor_boundary_rtop;
			if(Ver_state == 1)
				Head_ver = ver_boundary_top;
			else if(Ver_state == 2)
				Head_ver = ver_boundary_midbot;
			
			if(TimeDelay(l_delay))
			{
				Head_state = 1;
				Head_Init = true;
			}
		}
	}
	//if find ball
	else
	{
		int cx, cy;

		cx = CAM_WIDTH/2;
		cy = CAM_HEIGHT/2;
		
		double P_vf = 670.0;		//P_ipy, 
		//p
		double P_ipx = (double)(Pos_ball.x - cx);
		double P_ipy = (double)(cy - Pos_ball.y);
		//c
		double Theta_ipx = atan2(P_ipx, P_vf) * 180.0 / 3.14;
		double Theta_ipy = atan2(P_ipy, P_vf) * 180.0 / 3.14;
		
		int command_ver = int(0.2 * Theta_ipy * 4096.0 / 360.0);
		Head_ver = Head_ver - command_ver;
		
		if(Head_ver < 2048)
			Head_ver = 2048;
		if(Head_ver > 2800)
			Head_ver = 2800;
		if((fabs(P_ipx) > 15))
		{
			int command_hor;
				command_hor = int(0.8 * Theta_ipx * 4096.0 / 360.0);
			Head_hor = Head_hor - command_hor;
			if(Head_hor > CAM_HOR_U_BOUDARY)	Head_hor = CAM_HOR_U_BOUDARY;
			else if(Head_hor < CAM_HOR_L_BOUDARY)	Head_hor = CAM_HOR_L_BOUDARY;
 		}
	}
	SendPos(Head_hor, Head_ver);
	return false;
}
