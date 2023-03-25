// General.cpp: implementation of the General class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "General.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double Q_Dist(CvPoint pt1, CvPoint pt2)
{
	return pow((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y), 0.5);
}

double Distance(CPoint pt1, CPoint pt2)
{
	return pow((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y), 0.5);
}

double Dist(double dx, double dy)
{
	return sqrt(dx * dx + dy * dy);
}

double Deg2Rad(double ang)
{
	return ang * 3.14 / 180.0;
}

double Rad2Deg(double ang)
{
	return ang * 180.0 / CV_PI;
}

void Q_Byte2Deg(double *hor, double *ver)
{
	(*hor) = 360.0 * double(CAM_HOR_CENTER - (*hor)) / 4096.0;
	(*ver) = -30 + 90 + 360.0 * double(CAM_VER_CENTER - (*ver)) / 4096.0;
}

bool Estimate(int x, int y, double Head_hor, double Head_ver, int *rx, int *ry)
{
	double Robot_Head_Hight = 117.0;
	int cx = x - Round(320 * 0.5);
	int cy = Round(240 * 0.5) - (y);//744 * zoom_ratio
	
	double ang_hor = Head_hor*180.0/2048.0;		//
	double ang_ver = (360.0 - (Head_ver*360.0/4096.0))- 90.0;	//
	double PI = 3.14159265;
	double N1 = 7.138;//cm //71.38mm
	double N2 = 6.053;//cm //60.53mm
	double Theta_n = 30.0 * PI / 180.0;
	double Theta_ca, Theta_mp, Theta_ipy, Theta_ripy, Theta_ipx, L_ripx, L_ripy, L_ca, H_ca, S_ip;	
	Theta_mp = PI * ang_hor / 180.0;
	//pis
	double p_ipx = (double)cx;
	double p_ipy = (double)cy;
	Theta_ca = ang_ver * PI/180.0;	
	double P_vf = 391.242 * (320.0 / 320.0);	//需做實驗
	double H_tmp; 
	double Theta_all = Theta_ca - Theta_n;
	L_ca = N1* cos(Theta_ca) + N2*cos(Theta_ca-Theta_n);
	H_tmp = N1* sin(Theta_ca) + N2*sin(Theta_ca-Theta_n);
	Theta_all = Theta_all *180.0/PI;
	
	H_ca = Robot_Head_Hight + H_tmp;		//Robot_Head_Hight需做更改,機器人脖子高度
	
	Theta_ipy = atan2(p_ipy, P_vf);
	Theta_ripy = Theta_ipy + Theta_ca-Theta_n;
	L_ripy = L_ca + H_ca * tan(Theta_ripy);
	
	double P_vfx = 373.889 * (240.0 / 240.0);	//需做實驗
	Theta_ipx = atan2(p_ipx, P_vfx);
	S_ip = H_ca / cos(Theta_ripy);
	
	double offset = 0;//0;//-10	//robot offset
	L_ripx = S_ip * tan(Theta_ipx) + offset;	//S_ip
	
	(*rx) = -(L_ripx * cos(Theta_mp) - L_ripy * sin(Theta_mp)); //
	(*ry) = -(L_ripx * sin(Theta_mp) + L_ripy * cos(Theta_mp)) ; //
	
	return true;
}

void One_Step_Filter(int input, double param, int &out)
{
	static int pre_input;
	pre_input = int(pre_input + (param * (input - pre_input)));
	out = pre_input;
}

int Round(double Get_Value)
{
	BOOL isNa = FALSE;
	
	if (Get_Value < 0)
	{
		Get_Value = fabs(Get_Value);
		isNa = TRUE;
	}
	if (((int)(Get_Value * 10)) % 10 >= 5)
	{
		if (isNa)	return (-(((int)Get_Value) + 1));
		else		return (((int)Get_Value) + 1);
	} 
	else
	{
		if (isNa)	return (-((int)Get_Value));
		else		return ((int)Get_Value);
	}
}

double CosineAngle(double l1, double l2, double l3)
{
	return acos((l1 * l1 + l2 * l2 - l3 * l3) / (2 * l1 * l2));
}
