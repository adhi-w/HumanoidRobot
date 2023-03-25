#include "stdafx.h"
#include <math.h> 
#include <stdlib.h>
#include "Serial.h"

#define hor_U	954 //280 degree //RX-28 ---- //MX-106 //3584 //315 degree
#define hor_L	70 //20 degree //512  //45 degree
#define ver_U	512 //2048 //180 degree
#define ver_L	308 //1024 //90 degree
#define center_PanHead	2048

#define panHead_ID 4//21
#define tiltHead_ID 3

Serial serial;

int camSpeed=70;
bool findBall=false, findGoal;
long int dhor, dver,hh;
int PresentHorPos, PresentVerPos;
static int hor, ver;

bool Head_Init=false;

short Head_state=1;
short Head_hor, Head_ver;

void SendPos(int hor, int ver)
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

void ReadPackage(int id)
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

BOOL ReceiveData(int ID)
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

bool TimeDelay(int delay_time)
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

bool CamSet(bool start)
{
	if(start){	SendPos(512, 390);	}
	return false;
}

bool V_Scan(bool capstart, bool capfinish)
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
//#define hor_U	954 //280 degree //RX-28 ---- //MX-106 //3584 //315 degree
//#define hor_L	70 //20 degree //512  //45 degree
//#define ver_U	512 //2048 //180 degree
//#define ver_L	308 //1024 //90 degree

	short hor_init = 512;		//2048;
	short ver_init = 320;		//2048;

	short hor_boundary_ltop   = 954;	//2620;
	short hor_boundary_lbot   = 940;	//2574;
	
	short hor_boundary_rtop   = 70;		//1365;
	short hor_boundary_rbot   = 64;		//1353;

	short ver_boundary_top    = 512;//2175;
	short ver_boundary_bot    = 308;//2379;
	short ver_boundary_midbot = 380;//2460;

	short s_delay = 150;
	short l_delay = 300;

	if(Head_state == 1)
	{
		Head_hor = hor_boundary_rbot;
		Head_ver = ver_boundary_bot;
		
		if(TimeDelay(l_delay+l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 2;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 2)
	{
		Head_hor = hor_init;
		Head_ver = ver_boundary_midbot;
			
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 3;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 3)
	{
		Head_hor = hor_boundary_lbot;
		Head_ver = ver_boundary_bot;
	
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 4;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 4)
	{
		Head_hor = hor_boundary_ltop;
		Head_ver = ver_boundary_top;
		
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 5;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 5)
	{
		Head_hor = hor_init;
		Head_ver = ver_boundary_top;
	
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 6;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 6)
	{
		Head_hor = hor_boundary_rtop;
		
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 7;
				Head_Init = true;
			//	Catch_start = false;
			}
		}
	}
	else if(Head_state == 7)
	{
		Head_hor = hor_init;
		Head_ver = ver_init;
		
		if(TimeDelay(l_delay))
		{
			//Catch_start = true;
			if(capfinish)
			{
				Head_state = 1;
				Head_Init = true;
			//	Catch_start = false;
			//	Scan_Finish = true;
			}
		}
	}
	SendPos(Head_hor, Head_ver);
	return false;
}

void cam_pos(int x, int y, int cam_w, int cam_y)
{	
	
	int px=cam_w/2;		
	int py=cam_y/2;

	//----------------------
	static int pos_x, pos_y;
	int last_x=pos_x;  int last_y=pos_y;
	pos_x=x; pos_y=y;
	//---------------------

	int dx=x-px;	int dy=y-py;
	
	if(findBall)
	{
		if(x>=px+10||x<=px-10){ ReceiveData(panHead_ID); dhor=PresentHorPos+(dx/3); }
		else dhor=PresentHorPos;

		if(y>=py+10 || y<=py-10){ ReceiveData(tiltHead_ID); dver=PresentVerPos-(dy/4); }
		else dver=PresentVerPos;
	}
	else 
	{	//V_Scan(1,1);
	}
		
	//dhor=hor*4096/360;	//convert to dynamixel data
	//dver=ver*4096/360;

	if(dhor>=hor_U) dhor=hor_U;
	else if(dhor<=hor_L) dhor=hor_L;

	if(dver>=ver_U) dver=ver_U;
	else if(dver<=ver_L) dver=ver_L;
	
	SendPos(dhor,dver);
}