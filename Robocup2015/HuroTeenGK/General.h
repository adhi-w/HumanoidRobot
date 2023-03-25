/*
				*** header file ***
		
	  define all variables for all of class 1~6
	
	  and the parameters is seldom be changed

	//*************************************\\
*/
#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "cv.h"
#include "highgui.h"

//HEAD UART---------------------------
#define COM_PORT 4//2
#define BUNDRATE 1000000

//BODY UART---------------------------
#define B_COM_PORT 3//1
#define B_BUNDRATE 115200

//Camera info--------------------------
#define CAM_Hor_ID 104
#define CAM_Ver_ID 103
#define CAM_DEVICE 0

// Camera para-------------------------
#define P_VF 330
#define H_RO 73.5   //near 43
#define L_OF 5   //near 10

#define CAM_HOR_U_BOUDARY 665
#define CAM_HOR_L_BOUDARY 375
#define CAM_VER_U_BOUDARY 390//1869
#define CAM_VER_L_BOUDARY 480

#define CAM_WIDTH 320
#define CAM_HEIGHT 240
#define CAM_HOR_CENTER 530
#define CAM_VER_CENTER 470


#define Stop 201
#define Forward 202
#define TurnRight 203
#define TurnLeft 204
#define ShiftLeft 206
#define ShiftRight 207
#define Backward 208
#define StepIt 209


//general function----------------------------------
double Dist(double dx, double dy);
double Q_Dist(CvPoint pt1, CvPoint pt2);
void Q_Byte2Deg(double *hor, double *ver);
double Deg2Rad(double ang);
double Rad2Deg(double ang);
bool Estimate(CvPoint pt, double hor, double ver, int *rx, int *ry);
void One_Step_Filter(int input, double param, int &out);
#endif