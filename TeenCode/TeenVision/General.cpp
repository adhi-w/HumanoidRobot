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
	(*hor) = 300.0 * double(hor_init - (*hor)) / 1024.0;
	(*ver) = 30 + 300.0 * double(512 - (*ver)) / 1024.0;
}

bool Estimate(CvPoint pt, double hor, double ver, int *rx, int *ry)
{
	double ang_hor = hor;
	double ang_ver = ver;
	Q_Byte2Deg(&ang_hor, &ang_ver);
	double Theta_ca, Theta_mt, Theta_mp, Theta_ipy, Theta_ripy, 
		Theta_ipx, L_ripx, L_ripy, L_ca, H_ca, S_ip;
	
	double H_ro = H_RO;
	double P_ipx, P_ipy, P_vf = P_VF;
	double D_ov = 1.11;
	
	Theta_mt = 3.14 * ang_ver / 180.0;
	Theta_mp = 3.14 * ang_hor / 180.0;
	P_ipy = (double)((camHeight/2) - pt.y);
	P_ipx = (double)(pt.x - (camWidth/2));
	//c
	double Theta_of = asin(D_ov / L_OF);
	Theta_ca = Theta_mt - Theta_of;
	L_ca = L_OF * cos(Theta_of) * cos(Theta_ca);
	H_ca = H_ro + L_OF * cos(Theta_of) * sin(Theta_ca);
	Theta_ipy = atan2(P_ipy, P_vf);
	Theta_ripy = Theta_ipy + Theta_ca;
	L_ripy = L_ca + H_ca * tan(Theta_ripy);
	Theta_ipx = atan2(P_ipx, P_vf);
	S_ip = H_ca / cos(Theta_ripy);
	L_ripx = S_ip * tan(Theta_ipx);
	
	double offset = 0.0;
	//RX, RY
	(*rx) =  int(L_ripx * cos(Theta_mp) + L_ripy * sin(Theta_mp) + offset);
	(*ry) =  int(-L_ripx * sin(Theta_mp) + L_ripy * cos(Theta_mp));
	return TRUE; 
}

void One_Step_Filter(int input, double param, int &out)
{
	static int pre_input;
	pre_input = int(pre_input + (param * (input - pre_input)));
	out = pre_input;
}