// HeadControl.cpp: implementation of the CHeadControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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

	Rotate_speed = 100;


	Head_hor = 2048;
	Head_ver = 2048;

	Head_state = 1;
	Ver_state = 1;

	Catch_start = false;
	Scan_Finish = false;

	position_i = 0;
	position_d = 0;

	CRC = 0;

	Getball = false;

	m_serial.Open(port, bundrate);

	if (m_serial.Open(port, bundrate) == 0)
	{
		AfxMessageBox("Head's serial connect fail!");
	}
	

	TorqueON();
	
	Set_hor_speed(150);
	SendPos(Head_hor, Head_ver);
}

CHeadControl::~CHeadControl()
{
	SendPos(2048, 2048);
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
			V_Down();
		}
	}
}

void CHeadControl::SendPos(int hor, int ver)
{
	HeadControl_hor(hor);
	HeadControl_ver(ver);
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

bool CHeadControl::V_Down()
{
	Catch_start = true;


	Head_hor = 2048;
	Head_ver = 2400;

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
/************************************************************
Hor
					 LF     FF     RF
					2560   2048   1536
						\    |    /
						 \   |   /
						  \  |  /
						   \ | /
				LL 2072 ----------- 1024 RR
						   /   \
 						  /     \
						 /       \
						/         \
					3584           512
					 LB            RB
************************************************************/
/************************************************************
Ver
								  2048		Top
					         |    /
					         |   /
					         |  /
							 | /
							 ------ 2250	Middle
						     | \
 						     |  \
						     |   \
						     |    \
					              2450		Bottom
	-------------------------------------------------------->
	Backward									      Forward
************************************************************/
// 	short hor_init = 2048;
// 	short ver_init = 2048;
// 	
// 	short hor_boundary_ltop   = 3584;
// 	short hor_boundary_lbot   = 2574;
// 	
// 	short hor_boundary_rtop   = 512;
// 	short hor_boundary_rbot   = 1353;
// 	
// 	short ver_boundary_top    = 2175;
// 	short ver_boundary_bot    = 2379;
// 	short ver_boundary_midbot = 2460;
//	
//	short s_delay = 150;
//	short l_delay = 1100;
	short Hor_RB = 512;
	short Hor_RR = 1024;
	short Hor_RF = 1536;
	short Hor_FF = 2048;
	short Hor_LF = 2560;
	short Hor_LL = 3072;
	short Hor_LB = 3584;

	short Ver_Top = 2048;
	short Ver_Mid = 2250;
	short Hor_Bot = 2450;

	short SetDelay = 800;
	//if not find ball
	if(!Getball)
	{
		if(Head_state == 1)
		{
			Head_hor = Hor_LB;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 2;
				Head_Init = true;
			}
		}else if(Head_state == 2)
		{
			Head_hor = Hor_LB;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay + SetDelay + SetDelay))
			{
				Head_state = 3;
				Head_Init = true;
			}
		}
		else if(Head_state == 3)
		{
			Head_hor = Hor_LL;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 4;
				Head_Init = true;
			}
		}
		else if(Head_state == 4)
		{
			Head_hor = Hor_LF;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 5;
				Head_Init = true;
			}
		}
		else if(Head_state == 5)
		{
			Head_hor = Hor_FF;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 6;
				Head_Init = true;
			}
		}
		else if(Head_state == 6)
		{
			Head_hor = Hor_RF;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 7;
				Head_Init = true;
			}
		}
		else if(Head_state == 7)
		{
			Head_hor = Hor_RR;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 8;
				Head_Init = true;
			}
		}
		else if(Head_state == 8)
		{
			Head_hor = Hor_RB;
			if(Ver_state == 1)
				Head_ver = Ver_Top;
			else if(Ver_state == 2)
				Head_ver = Ver_Mid;
			else if(Ver_state == 3)
				Head_ver = Hor_Bot;
			
			if(TimeDelay(SetDelay))
			{
				Head_state = 1;
				Head_Init = true;

				if (Ver_state == 1)			Ver_state = 2;
 				else if (Ver_state == 2)	Ver_state = 3;
				else						Ver_state = 1;
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
		if(Head_ver > 2450)
			Head_ver = 2450;
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

void CHeadControl::HeadControl_hor(double motor_hor)
{
	position_i = Round((motor_hor - 2048) * 360.0 / 4096.0); //你輸入是角度就不用進行換算
	position_d = Round((position_i * 2048.0) / 180.0);
	position_i = Round(position_d);
	
	char sum = 0x00;
	char array[16];
	array[0] = array[1] = (char)0xFF;	//
	array[2] = (char)0xFD;		// Dynamixel pro
	array[3] = (char)0x00;		// Dynamixel pro
	array[4] = HEAD_ID_H;		// ID
	array[5] = 0x09;		// Num of SendData	
	array[6] = 0x00;		// Num of SendData
	array[7] = 0x03;		
	array[8] = 0x54;		//Position no	(596 => 0254)_ Low	
	array[9] = 0x02;		//Position no	(596 => 0254)_ High	
	if(position_i >= 0)	// positive 
	{
		array[10] = (unsigned char)(position_i%256);	//Position()_ Low
		array[11] = (unsigned char)(position_i/256);		//Position()_ High	
		array[12] = 0x00;			//Position()_ Low
		array[13] = 0x00;			//Position()_ High	round
		
	}	
	else		// negative  
	{
		position_i = (~abs(position_i)) + 1;
		array[10] = (unsigned char)(position_i & 0x00FF);	//Position()_ Low
		array[11] = (unsigned char)((position_i>>8) & 0x00FF);	//Position()_ High	
		array[12] = 0xff;			//Position()_ Low
		array[13] = 0xff;			//Position()_ High	
	}
	CRC = update_crc(0, (unsigned char*)(array),14);	
	array[14] = (CRC & 0x00FF);		//16bit CRC : CRC-16 values to verify reliability of data._ Low
	array[15] = (CRC>>8) & 0x00FF;	//16bit CRC : CRC-16 values to verify reliability of data._ High
	
	m_serial.SendData(array, sizeof(array));
}

void CHeadControl::HeadControl_ver(int motor_ver)
{
	char sum = 0x00;
	unsigned char vpl = motor_ver % 256;
	unsigned char vph = motor_ver / 256;
	unsigned char vsl = Rotate_speed % 256;
    unsigned char vsh = Rotate_speed / 256;
	
	char array[13];
	array[0] = array[1] = (char)0xFF;		//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
   	array[2] = (char)0xFE;				//ID.Broadcasting ID.all linked RX-28s execute command of Instruction Packet, and Status Packet is not returned.
   	array[3] = (char)0x09;				//LENGTH.(L+1) X N + 4 (L: Data Length per RX-28 = 4, N: the number of RX-28s = 1)
    array[4] = (char)0x83;				//SYNC WRITE
	array[5] = (char)0x1E;				//Parameter1 Start address to write Data
   	array[6] = (char)0x04;				//Parameter2 Length of Data to write
	array[7] = HEAD_ID_V;
   	array[8] = vpl;
   	array[9] = vph;
   	array[10] = vsl;
   	array[11] = vsh;  
	for(int i = 2; i < 12; i++)
	{
		sum += array[i];
	}
	array[12] = ~sum;
	
	m_serial.SendData(array, sizeof(array));
}

void CHeadControl::TorqueON()
{
	char sum2 = 0x00;
	
	char array2[13];// 
	
	array2[0] = array2[1] = (char)0xFF;	//
	array2[2] = (char)0xFD;	// Dynamixel pro
	array2[3] = (char)0x00;		// Dynamixel pro
	array2[4] = HEAD_ID_H;	// ID
	array2[5] = 0x06;		// Num of SendData	
	array2[6] = 0x00;		// Num of SendData
	array2[7] = 0x03;		// Wirte
	array2[8] = 0x32;		//Torque no	(562 => 0232)_ Low	
	array2[9] = 0x02;		//Torque no	(562 => 0232)_ High	
	array2[10] = 0x01;		//Torque ON
	
	CRC = update_crc(0, (unsigned char*)(array2),11);
	
	array2[11] = (CRC & 0x00FF);	//16bit CRC : CRC-16 values to verify reliability of data._ Low
	array2[12] = (CRC>>8) & 0x00FF;	//16bit CRC : CRC-16 values to verify reliability of data._ High
	
	m_serial.SendData(array2, sizeof(array2));
}

unsigned short CHeadControl::update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
	unsigned short i, j;
    unsigned short crc_table[256] = {
        0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
			0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
			0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
			0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
			0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
			0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
			0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
			0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
			0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
			0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
			0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
			0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
			0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
			0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
			0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
			0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
			0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
			0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
			0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
			0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
			0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
			0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
			0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
			0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
			0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
			0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
			0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
			0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
			0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
			0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
			0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
			0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
    };	
    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }
	
    return crc_accum;
}

bool CHeadControl::V_Scanobst()
{
	short hor_init = 2048;
	short ver_init = 2048;
	
	short hor_boundary_ll   = 3072; 
	short hor_boundary_l   = 2560; 
	short hor_boundary_r   = 1536;
	short hor_boundary_rr   = 1024;
	
	short ver_boundary    = 2048;
	
	short s_delay = 150;
	short l_delay = 800;
	
	if(Head_state == 1)
	{
		Head_hor = hor_boundary_ll;
		Head_ver = ver_boundary;
		
		if(TimeDelay(l_delay+l_delay))
		{
			Head_state = 2;
			Head_Init = true;
		}
	}
	else if(Head_state == 2)
	{
		Head_hor = hor_boundary_l;
		Head_ver = ver_boundary;
		
		if(TimeDelay(l_delay))
		{
			Head_state = 3;
			Head_Init = true;
		}
	}
	else if(Head_state == 3)
	{
		Head_hor = hor_init;
		Head_ver = ver_boundary;
		
		if(TimeDelay(l_delay))
		{
			Head_state = 4;
			Head_Init = true;
		}
	}
	else if(Head_state == 4)
	{
		Head_hor = hor_boundary_r;
		Head_ver = ver_boundary;
		
		if(TimeDelay(l_delay))
		{
			Head_state = 5;
			Head_Init = true;
		}
	}
	else if(Head_state == 5)
	{
		Head_hor = hor_boundary_rr;
		Head_ver = ver_boundary;
		
		if(TimeDelay(l_delay))
		{
			Head_state = 1;
			Head_Init = true;
		}
	}

	SendPos(Head_hor, Head_ver);
	return false;
}

bool CHeadControl::V_Init()
{
	Head_hor = 2048;
	Head_ver = 2048;
	
	SendPos(Head_hor, Head_ver);
	return false;
}

void CHeadControl::Set_hor_speed(int hor_vel)
{
	char sum = 0x00;
	char array[16];
	
	array[0] = array[1] = (char)0xFF;	//
	array[2] = (char)0xFD;		// Dynamixel pro
	array[3] = (char)0x00;		// Dynamixel pro
	array[4] = HEAD_ID_H;		// ID
	array[5] = 0x09;		// Num of SendData	
	array[6] = 0x00;		// Num of SendData
	array[7] = 0x03;		
	array[8] = 0x58;		//Position no	(596 => 0254)_ Low	
	array[9] = 0x02;		//Position no	(596 => 0254)_ High	
	
	array[10] = (unsigned char)(hor_vel%256);		//Goal Velocity Byte1
	array[11] = (unsigned char)(hor_vel/256);		//Goal Velocity Byte2
	array[12] = 0x00;								//Goal Velocity Byte3
	array[13] = 0x00;								//Goal Velocity Byte4
	
	CRC = update_crc(0, (unsigned char*)(array),14);	
	array[14] = (CRC & 0x00FF);		//16bit CRC : CRC-16 values to verify reliability of data._ Low
	array[15] = (CRC>>8) & 0x00FF;	//16bit CRC : CRC-16 values to verify reliability of data._ High
	
	m_serial.SendData(array, sizeof(array));
}
