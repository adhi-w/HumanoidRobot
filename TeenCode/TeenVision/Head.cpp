// Head.cpp: implementation of the Head class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TeenVision.h"
#include "Head.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Head::Head()
{	
	Head_state = 1;
	headtrack = false;
	jumping_check = false;

	dhor = 512;
	dver = 420;
	camSpeed= 90;
}

Head::~Head()
{
	
}

void Head::SendPos(int hor, int ver)
{
	char sum = 0x00;
	char array[18];

	array[0] = array[1] = (char)0xFF;
	array[2] = (char)0xFE;		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
	array[3] = ((4+1)*2 + 4);//0x0E;//		//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28, N: the number of RX-28s)
	array[4] = (char)0x83;		//SYNC WRITE
	array[5] = 0x1E;			//Parameter1 Start address to write Data
	array[6] = 0x04;			//Parameter2 Length of Data to write
	array[7] = panHead_ID;			//id
	array[8] = hor % 256;	
	array[9] = hor / 256;
	array[10] = camSpeed % 256;	
	array[11] = camSpeed / 256;
	array[12] = tiltHead_ID;
	array[13] = ver % 256;	
	array[14] = ver / 256;
	array[15] = camSpeed % 256;	
	array[16] = camSpeed / 256;
	
	for(int i = 2; i < 17; i++)
	{
		sum += array[i];
	}
	array[17] = ~sum;
	
	serial.SendData(array, sizeof(array));	
}

void Head::ReadPackage(int id)
{
	char array[8];
    array[0] = array[1] = (char)0xff;
   	array[2] = id;
   	array[3] = 0x04;
    array[4] = 0x02;
	array[5] = 0x24;
   	array[6] = 0x02;
	char sum = 0x00;
    for(int i = 2; i < 7; i++)
   	{
    	sum += array[i];
   	} 
  	array[7] = ~sum;
	serial.SendData(array, sizeof(array));	

}

bool Head::ReceiveData(int ID)
{
	ReadPackage(ID);

	int i,j;
	if(serial.ReadDataWaiting() > 0)	
	{
		static BYTE buffer[256] = {0};
		static BYTE ptr = {0};
		BYTE tempBuffer[30];
		int msg = serial.ReadDataWaiting();
		if(msg > 30)	msg = 30;

		if(msg > 0)
		{
			serial.ReadData(tempBuffer, serial.ReadDataWaiting());
			for(i = 0; i < msg; i++)
			{
				buffer[ptr+i] = tempBuffer[i];
			}
			ptr += msg;

			for(i = 0; i < 256; i++)
			{
				if(buffer[i] == 0xff && buffer[(i+1)%256] == 0xff)
				{
					BYTE sum = 0x00;
					for(j = 2; j < 7; j++)
					{
						sum += buffer[(i+j)%256];
					}
					sum = ~sum;
					if(buffer[(i+7)%256] == sum)
					{
						int return_id = buffer[(i+2)%256];
						int presentPos = buffer[(i+5)%256] + buffer[(i+6)%256] * 256;
						if(return_id == panHead_ID)
							PresentHorPos = presentPos;
						else if(return_id == tiltHead_ID)
							PresentVerPos = presentPos;
						
						for(j = 0; j < 256; j++)
							buffer[j] = 0;
//						HorPos = PresentHorPos;
						break;
					}
					else	
						continue;
				}
			}
		}
	}
	return true;
}

bool Head::TimeDelay(int delay_time)
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

//Not Used---------------------
void Head::Track_Ball(bool findBall, CvPoint ball, CvPoint CamPixel)
{	

	if(findBall)
	{	
				
		int cx = CamPixel.x/2; 
		int	cy = CamPixel.y/2; 

		double P_vf = 670.0;
		
		double P_ipx = (double)(ball.x - cx);
		double P_ipy = (double)(ball.y - cy);

		double Theta_ipx = atan2(P_ipx, P_vf) * 180 /3.14;
		double Theta_ipy = atan2(P_ipy, P_vf) * 180 /3.14;

		int command_ver = int(0.2 * Theta_ipy * 1024.0 / 300.0); //4096->MX-106

		Head_ver = Head_ver - command_ver;
		
		if(Head_ver < ver_L)	Head_ver = ver_L;
		if(Head_ver > ver_U)	Head_ver = ver_U;

		if(fabs(P_ipx) > 15)
		{
			int command_hor = int(0.8 * Theta_ipx * 1024.0 /300);
			Head_hor = Head_hor - command_hor;

			if(Head_hor > hor_U) Head_hor = hor_U;
			else if(Head_hor < hor_L) Head_hor = hor_L;
		}
	}

	SendPos(Head_hor, Head_ver);
}

void Head::CamPos(bool findBall, CvPoint ball, CvPoint CamPixel)
{	
	
	//short hor_init = 512;		
	//short ver_init = 420;		
	
	short hor_left   = 785;	
	short hor_mleft   = 636;

	short hor_right   = 243;	
	short hor_mright   = 427;
	
	
	short ver_top    = 384;
	short ver_mtop    = 400;

	short ver_bot    = 524;	
	short ver_mbot    = 453;
	
	short l_delay = 300;
	short s_delay = 150;

	int cnt =0;

	int px=CamPixel.x/2;		
	int py=CamPixel.y/2;

	int dx=ball.x-px;	int dy=ball.y-py;

	static int lock_dhor, lock_dver;
	
 	if(headtrack)
 	{
	if(findBall)
	{	camSpeed = 80;

		double P_vf = 670.0;		//P_ipy, 
		//p
		double P_ipx = (double)(ball.x - px);
		double P_ipy = (double)(py - ball.y);
		//c
		double Theta_ipx = atan2(P_ipx, P_vf) * 180.0 / 3.14;
		double Theta_ipy = atan2(P_ipy, P_vf) * 180.0 / 3.14;
		
		int command_ver = int(0.1 * Theta_ipy * 1024.0 / 300.0);
			dver = dver - command_ver;
			
		if((fabs(P_ipx) > 30))
		{
			int command_hor;
			command_hor = int(0.1 * Theta_ipx * 1024.0 / 300.0);
			dhor = dhor - command_hor;				
 		}
		lock_dhor=dhor;
		lock_dver=dver;
		cnt = 0;

// 		if(ball.x>=px+15 || ball.x<=px-15){ ReceiveData(panHead_ID); dhor=PresentHorPos-(dx/4); lock_dhor=dhor; }
// 		else dhor=PresentHorPos;
// 
// 		if(ball.y>=py+15 || ball.y<=py-15){ ReceiveData(tiltHead_ID); dver=PresentVerPos+(dy/4); lock_dver=dver; }
// 		else dver=PresentVerPos;
		Head_Init = true;
		jumping_check = false;

	}
	

	else if(!findBall && !jumping_check)
	{
		if(TimeDelay(l_delay+700))
		{
			jumping_check = true;
			Head_Init = true;
		}
	}

	else if(!findBall && jumping_check)
	{	
		camSpeed = 180;
		
		if(Head_state == 1)
		{	dhor = hor_mleft;		dver = ver_mbot;
		
			if(TimeDelay(l_delay+l_delay))
			{
				Head_state = 2;
				Head_Init = true;
			}
		}

		else if(Head_state == 2)
		{	dhor = hor_mright;		dver = ver_mbot;
		
			if(TimeDelay(l_delay+l_delay+s_delay))
			{
				Head_state = 3;
				Head_Init = true;
			}
		}

		else if(Head_state == 3)
		{	dhor = hor_mright;		dver = ver_mtop;
		
			if(TimeDelay(l_delay+s_delay))
			{
				Head_state = 4;
				Head_Init = true;
			}
		}

		else if(Head_state == 4)
		{	dhor = hor_mleft;		dver = ver_mbot;
		
			if(TimeDelay(l_delay+l_delay))
			{
				Head_state = 5;
				Head_Init = true;
			}
		}

		else if(Head_state == 5)
		{	dhor = hor_left;		dver = ver_top;

			if(TimeDelay(l_delay+l_delay+s_delay))
			{
				Head_state = 6;
				Head_Init = true;
			}
		}
		
		else if(Head_state == 6)
		{	dhor = hor_left;	dver = ver_bot;

			if(TimeDelay(l_delay+l_delay+s_delay))
			{
				Head_state = 7;
				Head_Init = true;
			}
		}

		else if(Head_state == 7)
		{	dhor = hor_right;	dver = ver_bot;
		
			if(TimeDelay(l_delay+l_delay+s_delay))
			{
				Head_state = 8;
				Head_Init = true;
			}
		}

		else if(Head_state == 8)
		{	dhor = hor_right;	dver = ver_top;
		
			if(TimeDelay(l_delay+l_delay+s_delay))
			{
				Head_state = 9;
				Head_Init = true;
			}
		}

		else if(Head_state == 9)
		{	dhor = hor_init;	dver = ver_init;
		
			if(TimeDelay(l_delay+l_delay))
			{
				cnt++;
				if (cnt>=2){ dhor=lock_dhor; dver=lock_dver; }
				else { Head_state = 1;}
				Head_Init = true;				
			}
		}		 
	}
	} 

	if(dhor>=hor_U) dhor=hor_U;
	else if(dhor<=hor_L) dhor=hor_L;

	if(dver>=ver_U) dver=ver_U;
	else if(dver<=ver_L) dver=ver_L;
	
	SendPos(dhor,dver);
	
	
}